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
            if (type==N_IDENT || type==N_ASSIGN){
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
            return node;
        }

        Node* make_grouped_expr(parse_ret* ret){
            this->advance();
            Node* node=this->expr(ret, LOWEST);
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

        Node* make_call(parse_ret* ret, Node* left){
            this->advance();
            vector<Node*>* args=new vector<Node*>;
            args->clear();
            vector<Node*>* kwargs=new vector<Node*>;
            kwargs->clear();

            if (this->current_tok_is(T_RPAREN)){
                Node* node=make_node(N_CALL);
                node->start=left->start;
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                Call* c=(Call*)malloc(sizeof(Call));
                c->args=args;
                c->kwargs=kwargs;
                c->object=left;
                
                node->node=c;

                this->advance();

                return node;
            }
            

            Node* expr=this->expr(ret, LOWEST);
            if (expr->type==N_ASSIGN){
                kwargs->push_back(expr);
            }
            else {
                args->push_back(expr);
            }
            if (ret->errornum>0){
                return NULL;
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
            

            Node* node=make_node(N_CALL);
            node->start=left->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            Call* c=(Call*)malloc(sizeof(Call));
            c->args=args;
            c->kwargs=kwargs;
            c->object=left;
            
            node->node=c;

            this->advance();

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

                case T_PLUS:
                case T_MINUS:
                    left=make_unary(ret);
                    break;

                default:
                    return NULL;

            }
            /*
            if (this->get_next().type!=T_EOF){
                switch (this->get_next().type){
                    case T_EQ:
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
                    case T_NEWLINE:
                    case T_LPAREN:
                    case T_LCURLY:
                    case T_LSQUARE:
                    case T_COMMA:
                        break;
                    
                    default:
                        return left;
                
                }
            }*/
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
                        left=make_binop(ret, left, this->current_tok.type);
                        break;
                    
                    case T_LPAREN:
                        left=make_call(ret, left);
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
                return function(ret);
            }
            this->add_parsing_error(ret, "SyntaxError: Unknown keyword '%s'",this->current_tok.data.c_str());
            this->advance();
            return NULL;
        }

        Node* function(parse_ret* ret){
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

};