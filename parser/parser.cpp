typedef struct parse_ret{
    vector<Node*> nodes;
    string* error;
    string header;
    string snippet;
    string arrows;
    int errornum;
}parse_ret;

class Parser{
    public:
        vector<Token> tokens;
        Token current_tok;
        int tok_idx;
        string filedata;

        Parser(){}

        Parser(vector<Token> t, string filedata){
            this->tokens=t;
            this->current_tok=this->tokens[0];
            this->tok_idx=0;
            this->filedata=filedata;
        }

        void add_parsing_error(parse_ret* ret, const char *_format, ...) {
            va_list args;
            const int length=256;
            char format[length];
            sprintf(format, "%s", _format);

            ret->header="In file \""+this->current_tok.start.infile+"\", line "+to_string(this->current_tok.start.line+1)+":";

            int line=0;
            int target=this->current_tok.start.line;
            int startidx=0;
            int endidx=0;
            int idx=0;
            bool entered=false;
            while (true){
                if (line==target && !entered){
                    startidx=idx;
                    entered=true;
                }
                if (entered && (this->filedata[idx]=='\n' || this->filedata[idx]=='\0')){
                    endidx=idx;
                    break;
                }
                else if (this->filedata[idx]=='\n'){
                    line++;
                }
                idx++;
            }

            string snippet="";
            string arrows="";
            for (int i=startidx; i<endidx; i++){
                snippet+=this->filedata[i];
                if (i>this->current_tok.start.index-1 && i<this->current_tok.end.index+1 && this->current_tok.type!=T_EOF\
                && this->current_tok.type==T_STR && this->current_tok.type!=T_ERROR_EOF){
                    arrows+="^";
                }
                else if (i>this->current_tok.start.index-1 && i<this->current_tok.end.index && this->current_tok.type!=T_EOF){
                    arrows+="^";
                }
                else if (this->current_tok.type==T_EOF && i==endidx-1){
                    arrows+=" ^";
                }
                else{
                    arrows+=" ";
                }
            }

            char *msg = (char*)malloc(sizeof(char)*length);

            va_start(args, _format);
            vsnprintf(msg, length, format, args);
            va_end(args);

            ret->error=(string*)msg;
            ret->snippet=snippet;
            ret->arrows=arrows;
            ret->errornum++;
        }

        Token advance(){
            if (this->tokens.size()<=this->tok_idx+1){
                return Token("", T_EOF, Position(), Position());
            }
            this->current_tok=this->tokens[++this->tok_idx];
            return this->current_tok;
        }

        Token backadvance(){
            if (0>this->tok_idx-1){
                return Token("", T_EOF, Position(), Position());
            }
            this->current_tok=this->tokens[--this->tok_idx];
            return this->current_tok;
        }

        Token get_next(){
            if (this->tokens.size()<this->tok_idx+1){
                return Token("", T_EOF, Position(), Position());
            }
            return this->tokens[this->tok_idx+1];
        }

        Token get_prev(){
            if (0>this->tok_idx-1){
                return Token("", T_EOF, Position(), Position());
            }
            return this->tokens[this->tok_idx-1];
        }

        bool current_tok_is(token_type type){
            if (this->current_tok.type==type){
                return true;
            }
            return false;
        }
        
        bool next_tok_is(token_type type){
            if (this->tokens.size()<this->tok_idx+1){
                return false;
            }
            if (this->tokens[this->tok_idx+1].type==type){
                return true;
            }
            return false;
        }
        
        bool isname(nodetype type){
            if (type==N_IDENT || type==N_ASSIGN || type==N_DOT){
                return true;
            }
            return false;
        }

        parse_ret parse(){
            return this->statements();            
        }

        parse_ret statements(){
            parse_ret ret;
            ret.errornum=0;
            ret.nodes.clear();
            ret.arrows="";
            ret.snippet="";

            if (current_tok_is(T_EOF)){
                goto statements_return;
            }

            skip_newline;
            while (!current_tok_is(T_EOF) && !current_tok_is(T_RCURLY)){
                skip_newline;
                Node* n=this->statement(&ret);
                if (ret.errornum>0){
                    goto statements_return;
                }
                if (n!=NULL){
                    ret.nodes.push_back(n);
                }
                skip_newline;
            }

            statements_return:
            return ret;
        }

        Node* statement(parse_ret* ret){
            switch (this->current_tok.type){
                case T_KWD:
                    return this->keyword(ret);
                    break;
                default:
                    return this->expr(ret,LOWEST);
            }
        }
        
        Node* make_node(enum nodetype type){
            Node* n=(Node*)malloc(sizeof(Node));
            n->type=type;
            return n;
        }

        Node* make_int_literal(){
            Node* node=make_node(N_INT);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
            IntLiteral* i=(IntLiteral*)malloc(sizeof(IntLiteral));
            i->literal=new string(this->current_tok.data);
            node->node=i;
            return node;
        }

        Node* make_float_literal(){
            Node* node=make_node(N_FLOAT);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
            FloatLiteral* i=(FloatLiteral*)malloc(sizeof(FloatLiteral));
            i->literal=new string(this->current_tok.data);
            node->node=i;
            return node;
        }

        Node* make_string_literal(){
            Node* node=make_node(N_STR);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
            StringLiteral* s=(StringLiteral*)malloc(sizeof(StringLiteral));
            s->literal=new string(this->current_tok.data);
            node->node=s;
            return node;
        }

        Node* make_identifier(){
            Node* node=make_node(N_IDENT);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
            Identifier* i=(Identifier*)malloc(sizeof(Identifier));
            i->name=new string(this->current_tok.data);
            node->node=i;
            
            if (this->next_tok_is(T_DOTIDENT)){
                Node* newnode=make_node(N_DOT);
                newnode->start=node->start;
                newnode->end=node->end;
                Dot* d=(Dot*)malloc(sizeof(Dot));
                d->names=new vector<Node*>;
                d->names->clear();
                d->names->push_back(node);
                while (this->next_tok_is(T_DOTIDENT)){
                    this->advance();
                    Node* n=make_node(N_IDENT);
                    n->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                    n->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
                    Identifier* i=(Identifier*)malloc(sizeof(Identifier));
                    i->name=new string(this->current_tok.data);
                    n->node=i;

                    d->names->push_back(n);
                }
                
                newnode->node=d;
                return newnode;
            }
            return node;
        }

        Node* make_true(){
            Node* node=make_node(N_TRUE);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
            return node;
        }

        Node* make_false(){
            Node* node=make_node(N_FALSE);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
            return node;
        }

        Node* make_none(){
            Node* node=make_node(N_NONE);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
            return node;
        }


        Node* make_grouped_expr(parse_ret* ret){
            this->advance();
            Node* node=this->expr(ret, LOWEST);
            if (this->current_tok_is(T_COMMA)){
                return make_tuple(ret, node);
            }
            if (node==NULL){
                return NULL;
            }
            if (!this->current_tok_is(T_RPAREN)){
                this->add_parsing_error(ret, "SyntaxError: Expected ')', got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return node;
            }
            return node;
        }

        Node* make_binop(parse_ret* ret, Node* left, enum token_type opr){
            //Make new binop, with minimal setup
            Node* node=make_node(N_BINOP);
            node->start=left->start;

            BinOp* binop=(BinOp*)malloc(sizeof(BinOp));
            binop->left=left;
            binop->opr=opr;
            int precedence=get_precedence(this->current_tok);
            this->advance();
            
            binop->right=this->expr(ret, precedence);
            if (ret->errornum>0){
                return NULL;
            }

            node->node=binop;

            node->end=binop->right->end;
            return node;
        }

        Node* make_assignment(parse_ret* ret, Node* left){
            if (!isname(left->type)){
                this->add_parsing_error(ret, "SyntaxError: Cannot assign to literal, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            
            if (left->type==N_DOT){
                Node* node=make_node(N_DOTASSIGN);
                node->start=left->start;
                node->end=left->end;

                DotAssign* assign=(DotAssign*)malloc(sizeof(DotAssign));
                assign->dot=left;

                this->advance();
                Node* right=this->expr(ret, LOWEST);
                if (right==NULL){
                    return NULL;
                }
                assign->right=right;

                node->node=assign;
                return node;
            }

            Node* node=make_node(N_ASSIGN);
            node->start=left->start;
            node->end=left->end;

            Assign* assign=(Assign*)malloc(sizeof(Assign));
            assign->name=left;

            this->advance();
            Node* right=this->expr(ret, LOWEST);
            if (right==NULL){
                return NULL;
            }
            assign->right=right;

            node->node=assign;
            return node;
        }

        Node* make_unary(parse_ret* ret){
            Node* node=make_node(N_UNARY);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            enum token_type type=this->current_tok.type;
            this->advance();
            Node* right=atom(ret);

            node->end=right->end;

            UnaryOp* unary=(UnaryOp*)malloc(sizeof(UnaryOp));
            unary->right=right;
            unary->opr=type;

            node->node=unary;
            return node;
        }

        Node* make_list(parse_ret* ret){
            this->advance();
            vector<Node*>* list=new vector<Node*>;

            if (this->current_tok_is(T_RSQUARE)){
                this->advance();
                Node* node=make_node(N_LIST);
                node->start=new Position(this->get_prev().start.infile, this->get_prev().start.index, this->get_prev().start.col, this->get_prev().start.line);
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                List* l=(List*)malloc(sizeof(List));
                l->list=list;
                
                node->node=l;
                return node;
            }
            
            Node* expr=this->expr(ret, LOWEST);
            if (ret->errornum>0){
                delete list;
                return NULL;
            }
            list->push_back(expr);

            while(this->current_tok_is(T_COMMA)){
                this->advance();
                Node* expr=this->expr(ret, LOWEST);
                if (ret->errornum>0){
                    delete list;
                    return NULL;
                }
                list->push_back(expr);
                if (this->current_tok_is(T_RSQUARE)){
                    break;
                }
            }

            Node* node=make_node(N_LIST);
            node->start=new Position(this->get_prev().start.infile, this->get_prev().start.index, this->get_prev().start.col, this->get_prev().start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            List* l=(List*)malloc(sizeof(List));
            l->list=list;
            
            node->node=l;
            return node;
        }

        Node* make_tuple(parse_ret* ret, Node* base){
            this->advance();
            vector<Node*>* list=new vector<Node*>;
            list->push_back(base);

            if (this->current_tok_is(T_RPAREN)){
                this->advance();
                Node* node=make_node(N_TUPLE);
                node->start=new Position(this->get_prev().start.infile, this->get_prev().start.index, this->get_prev().start.col, this->get_prev().start.line);
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                List* l=(List*)malloc(sizeof(List));
                l->list=list;
                
                node->node=l;
                return node;
            }
            
            Node* expr=this->expr(ret, LOWEST);
            if (ret->errornum>0){
                delete list;
                return NULL;
            }
            list->push_back(expr);

            while(this->current_tok_is(T_COMMA)){
                this->advance();
                Node* expr=this->expr(ret, LOWEST);
                if (ret->errornum>0){
                    delete list;
                    return NULL;
                }
                list->push_back(expr);
                if (this->current_tok_is(T_RPAREN)){
                    break;
                }
            }

            Node* node=make_node(N_TUPLE);
            node->start=new Position(this->get_prev().start.infile, this->get_prev().start.index, this->get_prev().start.col, this->get_prev().start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            List* l=(List*)malloc(sizeof(List));
            l->list=list;
            
            node->node=l;
            return node;
        }

        Node* make_dict(parse_ret* ret){
            this->advance();
            vector<Node*>* keys=new vector<Node*>;
            vector<Node*>* vals=new vector<Node*>;

            if (this->current_tok_is(T_RCURLY)){
                this->advance();
                Node* node=make_node(N_DICT);
                node->start=new Position(this->get_prev().start.infile, this->get_prev().start.index, this->get_prev().start.col, this->get_prev().start.line);
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                Dict* d=(Dict*)malloc(sizeof(Dict));
                d->keys=keys;
                d->vals=vals;
                
                node->node=d;
                return node;
            }
            
            Node* key=this->expr(ret, LOWEST);
            if (ret->errornum>0){
                delete vals;
                delete keys;
                return NULL;
            }
            if (!this->current_tok_is(T_COLON)){
                this->add_parsing_error(ret, "SyntaxError: Expected :");
                delete vals;
                delete keys;
                return NULL;
            }
            this->advance();
            Node* value=this->expr(ret, LOWEST);
            if (ret->errornum>0){
                delete vals;
                delete keys;
                return NULL;
            }
            keys->push_back(key);
            vals->push_back(value);

            while(this->current_tok_is(T_COMMA)){
                this->advance();
                Node* key=this->expr(ret, LOWEST);
                if (ret->errornum>0){
                    delete vals;
                    delete keys;
                    return NULL;
                }
                if (!this->current_tok_is(T_COLON)){
                    this->add_parsing_error(ret, "SyntaxError: Expected :");
                    delete vals;
                    delete keys;
                    return NULL;
                }
                this->advance();
                Node* value=this->expr(ret, LOWEST);
                if (ret->errornum>0){
                    delete vals;
                    delete keys;
                    return NULL;
                }
                keys->push_back(key);
                vals->push_back(value);

                if (this->current_tok_is(T_RCURLY)){
                    break;
                }
            }

            Node* node=make_node(N_DICT);
            node->start=new Position(this->get_prev().start.infile, this->get_prev().start.index, this->get_prev().start.col, this->get_prev().start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            Dict* d=(Dict*)malloc(sizeof(Dict));
            d->keys=keys;
            d->vals=vals;
            
            node->node=d;
            return node;
        }

        Node* make_call(parse_ret* ret, Node* left){
            this->advance();
            vector<Node*>* args=new vector<Node*>;
            args->clear();
            vector<Node*>* kwargs=new vector<Node*>;
            kwargs->clear();

            if (this->current_tok_is(T_RPAREN)){
                Node* node;
                if (left->type==N_DOT){
                    node=make_node(N_DOTCALL);
                    node->start=left->start;
                    node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                    
                    DotCall* c=(DotCall*)malloc(sizeof(DotCall));
                    c->args=args;
                    c->kwargs=kwargs;
                    c->dot=left;
                    
                    node->node=c;
                }
                else{
                    node=make_node(N_CALL);
                    node->start=left->start;
                    node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                    
                    Call* c=(Call*)malloc(sizeof(Call));
                    c->args=args;
                    c->kwargs=kwargs;
                    c->object=left;
                    
                    node->node=c;
                }

                this->advance();

                return node;
            }
            

            Node* expr=this->expr(ret, LOWEST);
            if (ret->errornum>0){
                return NULL;
            }
            if (expr->type==N_ASSIGN){
                kwargs->push_back(expr);
            }
            else {
                args->push_back(expr);
            }
            
            

            while(this->current_tok_is(T_COMMA)){
                this->advance();
                Node* expr=this->expr(ret, LOWEST);
                if (expr->type==N_ASSIGN){
                    kwargs->push_back(expr);
                }
                else {
                    if (kwargs->size()>0){
                        this->backadvance();
                        this->add_parsing_error(ret, "SyntaxError: Positional argument follows keyword argument");
                        this->advance();
                        delete args;
                        delete kwargs;
                        return NULL;
                    }
                    args->push_back(expr);
                }
                if (ret->errornum>0){
                    return NULL;
                }
                if (!this->current_tok_is(T_COMMA) && !this->current_tok_is(T_RPAREN)){
                    this->add_parsing_error(ret, "SyntaxError: Invalid syntax.");
                    this->advance();
                    return NULL;
                }
            }
            

            Node* node;
            if (left->type==N_DOT){
                node=make_node(N_DOTCALL);
                node->start=left->start;
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                DotCall* c=(DotCall*)malloc(sizeof(DotCall));
                c->args=args;
                c->kwargs=kwargs;
                c->dot=left;
                
                node->node=c;
            }
            else{
                node=make_node(N_CALL);
                node->start=left->start;
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                Call* c=(Call*)malloc(sizeof(Call));
                c->args=args;
                c->kwargs=kwargs;
                c->object=left;
                
                node->node=c;
            }

            this->advance();

            return node;
        }

        Node* make_store_subscr(parse_ret* ret, Node* left){
            this->advance();
            Node* expr=this->expr(ret, LOWEST);
            
            Node* node=make_node(N_STORE_SUBSCR);
            node->start=left->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            StoreSubscript* s=(StoreSubscript*)malloc(sizeof(StoreSubscript));
            s->left=left;
            s->expr=expr;
            
            node->node=s;
            
            this->advance();
            return node;
        }

        Node* make_subscr(parse_ret* ret, Node* left){
            this->advance();
            Node* expr=this->expr(ret, LOWEST);
            if (!this->current_tok_is(T_RSQUARE)){
                this->add_parsing_error(ret, "SyntaxError: Expected ], got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            
            this->advance();
            
            Node* node=make_node(N_SUBSCR);
            node->start=left->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            Subscript* s=(Subscript*)malloc(sizeof(Subscript));
            s->left=left;
            s->expr=expr;
            
            node->node=s;
            if (this->current_tok_is(T_EQ)){
                return this->make_store_subscr(ret, node);
            }
            return node;
        }

        Node* atom(parse_ret* ret){
            Node* left;
            switch (this->current_tok.type){
                case T_INT:
                    left=make_int_literal();
                    break;

                case T_FLOAT:
                    left=make_float_literal();
                    break;

                case T_STR:
                    left=make_string_literal();
                    break;
                
                case T_IDENTIFIER:
                    left=make_identifier();
                    break;

                case T_LPAREN:
                    left=make_grouped_expr(ret);
                    break;

                case T_LSQUARE:
                    left=make_list(ret);
                    break;

                case T_LCURLY:
                    left=make_dict(ret);
                    break;

                case T_TRUE:
                    left=make_true();
                    break;
                case T_FALSE:
                    left=make_false();
                    break;
                case T_NONE:
                    left=make_none();
                    break;

                case T_PLUS:
                case T_MINUS:
                    left=make_unary(ret);
                    break;

                default:
                    return NULL;

            }
            return left;
        }

        Node* expr(parse_ret* ret, int prec){
            Node* left=this->atom(ret);
            if (left==NULL){
                if (this->current_tok.type==T_EOF){
                    return left;
                }
                if (this->current_tok.type==T_ERROR_EOF){
                    this->add_parsing_error(ret, "SyntaxError: Unexpected EOF.");
                    this->advance();
                    return left;
                }
                this->add_parsing_error(ret, "SyntaxError: Invalid syntax.");//Expected expression, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return left;
            }


            
            this->advance();

            while (!(this->current_tok_is(T_EOF)) && prec<get_precedence(this->current_tok)){
                switch (this->current_tok.type){
                    case T_EQ:
                        left=make_assignment(ret, left);
                        break;

                    case T_PLUS:
                    case T_MINUS:
                    case T_MUL:
                    case T_DIV:
                    case T_NE:
                    case T_EE:
                    case T_GT:
                    case T_GTE:
                    case T_LT:
                    case T_LTE:
                    case T_PERCENT:
                    case T_IS:
                        left=make_binop(ret, left, this->current_tok.type);
                        break;
                    
                    case T_LPAREN:
                        left=make_call(ret, left);
                        break;

                    case T_LSQUARE:
                        left=make_subscr(ret, left);
                        break;

                    default:
                        return left;
                
                }

                if (left==NULL){
                    return left;
                }
            }

            return left;
        }

        Node* keyword(parse_ret* ret){
            if (this->current_tok.data=="func"){
                return make_function(ret);
            }
            if (this->current_tok.data=="class"){
                return make_class(ret);
            }
            if (this->current_tok.data=="return"){
                return make_return(ret);
            }
            if (this->current_tok.data=="if"){
                return make_if(ret);
            }   
            if (this->current_tok.data=="raise"){
                return make_raise(ret);
            }
            if (this->current_tok.data=="try"){
                return make_try(ret);
            }               
            this->add_parsing_error(ret, "SyntaxError: Unexpected keyword '%s'",this->current_tok.data.c_str());
            this->advance();
            return NULL;
        }

        Node* make_function(parse_ret* ret){
            vector<Node*>* args=new vector<Node*>;
            args->clear();
            vector<Node*>* kwargs=new vector<Node*>;
            kwargs->clear();

            this->advance();
            if (!this->current_tok_is(T_IDENTIFIER)){
                this->add_parsing_error(ret, "SyntaxError: Expected identifier, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                delete args;
                delete kwargs;
                return NULL;
            }
            Node* name=this->atom(ret);
            this->advance();
            if (!this->current_tok_is(T_LPAREN)){
                Token errtok=this->current_tok;
                this->backadvance();
                this->backadvance();
                this->add_parsing_error(ret, "SyntaxError: Expected (, got '%s'",token_type_to_str(errtok.type).c_str());
                this->advance();
                delete args;
                delete kwargs;
                return NULL;
            }
            //Parse arguments
            this->advance();
            while (!this->current_tok_is(T_RPAREN)){
                if (this->next_tok_is(T_EQ)){
                    Node* expr=this->expr(ret, LOWEST);
                    kwargs->push_back(expr);
                    if (this->current_tok_is(T_RPAREN)){
                        break;
                    }
                    this->advance();
                }
                else{
                    if (kwargs->size()>0){
                        this->add_parsing_error(ret, "SyntaxError: Positional argument follows keyword argument");
                        this->advance();
                        delete args;
                        delete kwargs;
                        return NULL;
                    }
                    Node* base=this->atom(ret);
                    if (base->type!=N_IDENT){
                        this->add_parsing_error(ret, "SyntaxError: Expected identifier");
                        this->advance();
                        delete args;
                        delete kwargs;
                        return NULL;
                    }
                    
                    this->advance();
                    args->push_back(base);
                    if (this->current_tok_is(T_RPAREN)){
                        break;
                    }
                    this->advance();
                }
            }
            //
            if (!this->current_tok_is(T_RPAREN)){
                this->add_parsing_error(ret, "SyntaxError: Expected ), got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                delete args;
                delete kwargs;
                return NULL;
            }

            this->advance();
            if (!this->current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                delete args;
                delete kwargs;
                return NULL;
            }

            this->advance();
            skip_newline;
            parse_ret code;
            if (!this->current_tok_is(T_RCURLY)){
                code=this->statements();
            }
            else{
                code.nodes.clear();
            }
            
            if (!this->current_tok_is(T_RCURLY) && !this->get_prev().type!=T_RCURLY){
                this->backadvance();
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                delete args;
                delete kwargs;
                return NULL;
            }

            Node* node=make_node(N_FUNC);
            node->start=name->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Func* f=(Func*)malloc(sizeof(Func));
            f->name=name;
            f->code=new vector<Node*>;
            f->code->clear();
            for (Node* n: code.nodes){
                f->code->push_back(n);
            }
            f->args=args;
            f->kwargs=kwargs;

            node->node=f;
            this->advance();
            return node;
        }

        Node* make_class(parse_ret* ret){
            this->advance();
            Node* name=this->atom(ret);
            if (name->type!=N_IDENT){
                this->add_parsing_error(ret, "SyntaxError: Expected identifier, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            this->advance();
            if (!this->current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            this->advance();
            skip_newline;
            parse_ret code;
            if (!this->current_tok_is(T_RCURLY)){
                code=this->statements();
            }
            else{
                code.nodes.clear();
            }
            if (!this->current_tok_is(T_RCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }

            Node* node=make_node(N_CLASS);
            node->start=name->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Class* c=(Class*)malloc(sizeof(Class));
            c->name=name;
            c->code=new vector<Node*>;
            c->code->clear();
            for (Node* n: code.nodes){
                c->code->push_back(n);
            }

            node->node=c;
            
            this->advance();
            return node;
        }

        Node* make_return(parse_ret* ret){
            Node* node=make_node(N_RETURN);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            this->advance();
            skip_newline;
            Node* n=this->expr(ret, LOWEST);

            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Return* r=(Return*)malloc(sizeof(Return));
            r->node=n;

            node->node=r;
            
            this->advance();
            return node;
        }

        Node* make_else(parse_ret* ret, Node* base){
            Token t=this->current_tok;
            this->advance(); 
            
            if (!this->current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            this->advance();
            skip_newline;
            parse_ret code;
            if (!this->current_tok_is(T_RCURLY)){
                code=this->statements();
            }
            else{
                code.nodes.clear();
            }
            if (!this->current_tok_is(T_RCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }         

            Node* node=make_node(N_ELSE);
            node->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Else* e=(Else*)malloc(sizeof(Else));
            e->base=base;
            e->code=new vector<Node*>;
            e->code->clear();
            for (Node* n: code.nodes){
                e->code->push_back(n);
            }

            node->node=e;

            
            vector<Node*>* bases=new vector<Node*>;

            bases->push_back(base);
            bases->push_back(node);

            Node* n=make_node(N_CONTROL);
            n->start=bases->front()->start;
            n->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Control* ei=(Control*)malloc(sizeof(Control));
            ei->bases=bases;

            n->node=ei;
            
            this->advance();
            return n;
        }

        Node* make_else_(parse_ret* ret, Node* base){
            Token t=this->current_tok;
            this->advance(); 
            
            if (!this->current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            this->advance();
            skip_newline;
            parse_ret code;
            if (!this->current_tok_is(T_RCURLY)){
                code=this->statements();
            }
            else{
                code.nodes.clear();
            }
            if (!this->current_tok_is(T_RCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }         

            Node* node=make_node(N_ELSE);
            node->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Else* e=(Else*)malloc(sizeof(Else));
            e->base=base;
            e->code=new vector<Node*>;
            e->code->clear();
            for (Node* n: code.nodes){
                e->code->push_back(n);
            }

            node->node=e;
            this->advance();
            return node;
        }

        Node* make_elif(parse_ret* ret, Node* base){
            vector<Node*>* bases=new vector<Node*>;
            bases->clear();

            bases->push_back(base);

            while (this->current_tok_is(T_KWD) &&\
            this->current_tok.data=="elif"){
                Node* n=this->make_elif_(ret);
                if (n==NULL){
                    delete bases;
                    return n;
                }
                bases->push_back(n);
                skip_newline;
            }

            if (this->current_tok_is(T_KWD) && this->current_tok.data=="else"){
                Node* n=make_else_(ret, NULL);
                if (n==NULL){
                    delete bases;
                    return n;
                }
                bases->push_back(n);
                this->advance();
            }

            Node* node=make_node(N_CONTROL);
            node->start=base->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Control* ei=(Control*)malloc(sizeof(Control));
            ei->bases=bases;

            node->node=ei;
            
            return node;
        }

        Node* make_elif_(parse_ret* ret){
            this->advance();

            Node* expr=this->expr(ret, LOWEST);   
            
            if (!this->current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            this->advance();
            skip_newline;
            parse_ret code;
            if (!this->current_tok_is(T_RCURLY)){
                code=this->statements();
            }
            else{
                code.nodes.clear();
            }
            if (!this->current_tok_is(T_RCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }         

            Node* node=make_node(N_IF);
            node->start=expr->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            If* i=(If*)malloc(sizeof(If));
            i->expr=expr;
            i->code=new vector<Node*>;
            i->code->clear();
            for (Node* n: code.nodes){
                i->code->push_back(n);
            }

            node->node=i;
            
            this->advance();
            return node;
        }

        Node* make_if(parse_ret* ret){
            vector<Node*>* bases=new vector<Node*>;
            this->advance();

            Node* expr=this->expr(ret, LOWEST);   
            
            if (!this->current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                delete bases;
                return NULL;
            }
            this->advance();
            skip_newline;
            parse_ret code;
            if (!this->current_tok_is(T_RCURLY)){
                code=this->statements();
            }
            else{
                code.nodes.clear();
            }
            if (!this->current_tok_is(T_RCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                delete bases;
                return NULL;
            }         

            Node* node=make_node(N_IF);
            node->start=expr->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            If* i=(If*)malloc(sizeof(If));
            i->expr=expr;
            i->code=new vector<Node*>;
            i->code->clear();
            for (Node* n: code.nodes){
                i->code->push_back(n);
            }

            node->node=i;

            bases->push_back(node);
            
            this->advance();

            skip_newline;

            if (this->current_tok_is(T_KWD) && this->current_tok.data=="else"){
                return this->make_else(ret, node);
            }
            if (this->current_tok_is(T_KWD) && this->current_tok.data=="elif"){
                return this->make_elif(ret, node);
            }

            Node* n=make_node(N_CONTROL);
            n->start=bases->front()->start;
            n->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Control* ei=(Control*)malloc(sizeof(Control));
            ei->bases=bases;

            n->node=ei;
            return n;
        }

        Node* make_raise(parse_ret* ret){
            this->advance();
            Node* expr=this->expr(ret, LOWEST);
            this->advance();

            Node* n=make_node(N_RAISE);
            n->start=expr->start;
            n->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Raise* r=(Raise*)malloc(sizeof(Raise));
            r->expr=expr;

            n->node=r;
            return n;
        }

        Node* make_try(parse_ret* ret){
            vector<Node*>* bases=new vector<Node*>;
            Token t=this->current_tok;
            this->advance();

            Node* n=make_node(N_TRY_EXCEPT_FINALLY);
            n->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
            n->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            TryExceptFinally* f=(TryExceptFinally*)malloc(sizeof(TryExceptFinally));
            
            //Parse try code
            if (!this->current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                free(n);
                free(f);
                return NULL;
            }
            this->advance();
            skip_newline;
            parse_ret try_code;
            if (!this->current_tok_is(T_RCURLY)){
                try_code=this->statements();
            }
            else{
                try_code.nodes.clear();
            }
            
            if (!this->current_tok_is(T_RCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                free(n);
                free(f);
                return NULL;
            }  
            
            Node* node=make_node(N_TRY);
            node->start=n->start;
            node->end=n->end;

            Try* i=(Try*)malloc(sizeof(Try));
            i->code=new vector<Node*>;
            i->code->clear();
            for (Node* n: try_code.nodes){
                i->code->push_back(n);
            }

            node->node=i;

            bases->push_back(node);
            
            this->advance();
            skip_newline;
            while(this->current_tok_is(T_KWD) && this->current_tok.data=="except"){
                this->advance();
                Node* type=NULL;
                if (this->current_tok_is(T_IDENTIFIER)){
                    type=this->atom(ret);
                    this->advance();
                }
                Node* name=NULL;
                if (this->current_tok_is(T_IDENTIFIER)){
                    name=this->atom(ret);
                    this->advance();
                }


                //Parse except code
                if (!this->current_tok_is(T_LCURLY)){
                    this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                    this->advance();
                    free(n);
                    free(f);
                    return NULL;
                }
                this->advance();
                skip_newline;
                parse_ret except_code;
                if (!this->current_tok_is(T_RCURLY)){
                    except_code=this->statements();
                }
                else{
                    except_code.nodes.clear();
                }
                if (!this->current_tok_is(T_RCURLY)){
                    this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                    this->advance();
                    free(n);
                    free(f);
                    return NULL;
                }  


                node=make_node(N_EXCEPT);
                node->start=n->start;
                node->end=n->end;

                Except* i=(Except*)malloc(sizeof(Except));
                i->code=new vector<Node*>;
                i->code->clear();
                for (Node* n: except_code.nodes){
                    i->code->push_back(n);
                }
                i->type=type;
                i->name=name;

                node->node=i;

                bases->push_back(node);

                this->advance();
                skip_newline;

            }

            if (this->current_tok_is(T_KWD) && this->current_tok.data=="finally"){
                this->advance();
                
                //Parse except code
                if (!this->current_tok_is(T_LCURLY)){
                    this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                    this->advance();
                    free(n);
                    free(f);
                    return NULL;
                }
                this->advance();
                skip_newline;
                parse_ret finally_code;
                if (!this->current_tok_is(T_RCURLY)){
                    finally_code=this->statements();
                }
                else{
                    finally_code.nodes.clear();
                }
                if (!this->current_tok_is(T_RCURLY)){
                    this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                    this->advance();
                    free(n);
                    free(f);
                    return NULL;
                }  

                node=make_node(N_FINALLY);
                node->start=n->start;
                node->end=n->end;

                Finally* i=(Finally*)malloc(sizeof(Finally));
                i->code=new vector<Node*>;
                i->code->clear();
                for (Node* n: finally_code.nodes){
                    i->code->push_back(n);
                }

                node->node=i;

                bases->push_back(node);
            }

            f->bases=bases;

            n->node=f;

            return n;
        }

};