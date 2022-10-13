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
        bool multi=true;
        bool noassign=false;
        bool inloop=false;
        bool inclass=false;

        Parser(){}

        Parser(vector<Token> t, string filedata){
            this->tokens=t;
            this->current_tok=this->tokens[0];
            this->tok_idx=0;
            this->filedata=filedata;
        }

        void add_parsing_error(parse_ret* ret, const char *_format, ...) {
            if (ret->errornum>0){
                return;
            }
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

            char *msg = (char*)fpl_malloc(sizeof(char)*length);

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
        
        bool next_next_tok_is(token_type type){
            if (this->tokens.size()<this->tok_idx+2){
                return false;
            }
            if (this->tokens[this->tok_idx+2].type==type){
                return true;
            }
            return false;
        }
        
        bool isname(nodetype type){
            if (type==N_IDENT || type==N_GLBL_IDENT || type==N_NONLOCAL || type==N_MULTIIDENT || type==N_DOT || type==N_CALL || type==N_SUBSCR){
                return true;
            }
            return false;
        }

        bool isname_literal(nodetype type){
            if (type==N_IDENT || type==N_GLBL_IDENT || type==N_NONLOCAL  || type==N_MULTIIDENT || type==N_DOT || type==N_SUBSCR){
                return true;
            }
            return false;
        }
        
        bool isname_tok(token_type type){
            if (type==T_IDENTIFIER || type==T_DOTIDENT){
                return true;
            }
            return false;
        }

        parse_ret parse(){
            return this->statements();            
        }

        parse_ret statements(){
            bool noassign=this->noassign;
            this->noassign=false;
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
                if (!this->current_tok_is(T_NEWLINE) && !this->current_tok_is(T_EOF) && !this->current_tok_is(T_RCURLY)){
                    this->add_parsing_error(&ret, "SyntaxError: Invalid syntax");
                }
                if (ret.errornum>0){
                    goto statements_return;
                }
                if (n!=NULL){
                    ret.nodes.push_back(n);
                }
                skip_newline;
            }

            statements_return:
            this->noassign=noassign;
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
            Node* n=(Node*)fpl_malloc(sizeof(Node));
            n->type=type;
            return n;
        }

        Node* make_int_literal(){
            Node* node=make_node(N_INT);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
            IntLiteral* i=(IntLiteral*)fpl_malloc(sizeof(IntLiteral));
            i->literal=new string(this->current_tok.data);
            node->node=i;
            return node;
        }

        Node* make_float_literal(){
            Node* node=make_node(N_FLOAT);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
            FloatLiteral* f=(FloatLiteral*)fpl_malloc(sizeof(FloatLiteral));
            f->literal=new string(this->current_tok.data);
            node->node=f;
            return node;
        }
        

        Node* make_string_literal(){
            Node* node=make_node(N_STR);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
            StringLiteral* s=(StringLiteral*)fpl_malloc(sizeof(StringLiteral));
            s->literal=new string(this->current_tok.data);
            node->node=s;
            
            return node;
        }

        Node* make_fstring(parse_ret* ret){
            this->advance();
            Node* node=make_string_literal();
            node->type=N_FSTRING;
            
            return node;
        }

        Node* make_identifier(parse_ret* ret){
            if (this->current_tok.data=="f" && this->get_next().type==T_STR){
                return make_fstring(ret);
            }
            Node* node=make_node(N_IDENT);
            
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);

            if (this->next_next_tok_is(T_IDENTIFIER) && this->next_tok_is(T_COMMA) && this->multi){
                vector<string*>* names=new vector<string*>;
                names->clear();
                
                names->push_back(new string(this->current_tok.data));
                while (this->next_tok_is(T_COMMA)){
                    this->advance();
                    this->advance();
                    names->push_back(new string(this->current_tok.data));
                }
                
                MultiIdentifier* i=(MultiIdentifier*)fpl_malloc(sizeof(MultiIdentifier));
                i->name=names;
                node->node=i;
                node->type=N_MULTIIDENT;
                return node;
            }

            Identifier* i=(Identifier*)fpl_malloc(sizeof(Identifier));
            i->name=new string(this->current_tok.data);
            node->node=i;
            
            if (this->next_tok_is(T_COLON)){
                string _name=this->current_tok.data;
                this->advance();
                this->advance();
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                Node* tp=this->expr(ret, LOWEST);
                this->noassign=noassign;
                this->multi=b;
                this->backadvance();

                free(node->node);
                delete i->name;

                AnnotatedIdentifier* i=(AnnotatedIdentifier*)fpl_malloc(sizeof(AnnotatedIdentifier));
                i->name=new string(_name);
                i->tp=tp;
                node->node=i;   
                node->type=N_ANONIDENT;    
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
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* node=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
            
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
            if (!current_tok_is(T_NOT) && (current_tok_is(T_IADD) || current_tok_is(T_IMUL) || current_tok_is(T_ISUB) || current_tok_is(T_IDIV)\
                || current_tok_is(T_IPOW) ||current_tok_is(T_IMOD) || current_tok_is(T_IAMP) || current_tok_is(T_IVBAR)\
                || current_tok_is(T_ILSH) || current_tok_is(T_IRSH) || current_tok_is(T_IFLDIV)) && !isname(left->type)){
                this->add_parsing_error(ret, "SyntaxError: Invalid syntax");
                this->advance();
                return NULL;
            }

            //Make new binop, with minimal setup
            Node* node=make_node(N_BINOP);
            node->start=left->start;

            BinOp* binop=(BinOp*)fpl_malloc(sizeof(BinOp));
            binop->left=left;
            binop->opr=opr;
            int precedence=get_precedence(this->current_tok);
            this->advance();
            if (this->current_tok_is(T_IN) && this->get_prev().type==T_NOT){
                precedence=get_precedence(this->current_tok);
                this->advance();
            }
            else if (this->get_prev().type==T_IS && this->current_tok_is(T_NOT)){
                precedence=get_precedence(this->current_tok);
                binop->opr=T_ISNOT;
                this->advance();
            }
            
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            binop->right=this->expr(ret, precedence);
            this->noassign=noassign;
            this->multi=b;

            if (ret->errornum>0){
                return NULL;
            }

            node->node=binop;

            node->end=binop->right->end;
            return node;
        }

        Node* make_assignment(parse_ret* ret, Node* left){
            if (!isname_literal(left->type) && left->type!=N_ASSIGN && left->type!=N_ANONIDENT\
             && left->type!=N_ANONGLBL_IDENT && left->type!=N_ANONDOT && left->type!=N_ANONNONLOCAL){
                this->add_parsing_error(ret, "SyntaxError: Cannot assign to literal, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            
            if (left->type==N_DOT){
                Node* node=make_node(N_DOTASSIGN);
                node->start=left->start;
                node->end=left->end;

                DotAssign* assign=(DotAssign*)fpl_malloc(sizeof(DotAssign));
                assign->dot=left;

                this->advance();
                if (!is_atomic()){
                    this->add_parsing_error(ret, "SyntaxError: Invalid syntax.");
                    this->advance();
                    return NULL;
                }                
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                Node* right=this->expr(ret, LOWEST);
                this->noassign=noassign;
                this->multi=b;
                if (right==NULL){
                    return NULL;
                }
                assign->right=right;

                node->node=assign;
                return node;
            }

            if (left->type==N_ANONIDENT || left->type==N_ANONGLBL_IDENT || left->type==N_ANONNONLOCAL || left->type==N_ANONDOT){
                Node* node=make_node(N_ANONASSIGN);
                node->start=left->start;
                node->end=left->end;

                Assign* assign=(Assign*)fpl_malloc(sizeof(Assign));
                assign->name=left;
                this->advance();
                if (!is_atomic()){
                    this->add_parsing_error(ret, "SyntaxError: Invalid syntax.");
                    this->advance();
                    return NULL;
                }
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                Node* right=this->expr(ret, LOWEST);
                this->noassign=noassign;
                this->multi=b;
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

            Assign* assign=(Assign*)fpl_malloc(sizeof(Assign));
            assign->name=left;

            this->advance();
            if (!is_atomic()){
                this->add_parsing_error(ret, "SyntaxError: Invalid syntax.");
                this->advance();
                return NULL;
            }
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* right=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
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
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* right=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
            this->backadvance();

            node->end=right->end;

            UnaryOp* unary=(UnaryOp*)fpl_malloc(sizeof(UnaryOp));
            unary->right=right;
            unary->opr=type;

            node->node=unary;
            return node;
        }

        Node* make_tilde(parse_ret* ret){
            Node* node=make_node(N_UNARY);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            enum token_type type=this->current_tok.type;
            this->advance();
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* right=this->expr(ret, BITWISE_NOT_PREC);
            this->noassign=noassign;
            this->multi=b;
            this->backadvance();

            node->end=right->end;

            UnaryOp* unary=(UnaryOp*)fpl_malloc(sizeof(UnaryOp));
            unary->right=right;
            unary->opr=type;

            node->node=unary;
            return node;
        }

        Node* make_not(parse_ret* ret){
            if (this->next_tok_is(T_IN)){
                this->backadvance();
                return (Node*)0x1;
            }
            Node* node=make_node(N_UNARY);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            enum token_type type=this->current_tok.type;
            this->advance();
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* right=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;

            node->end=right->end;

            UnaryOp* unary=(UnaryOp*)fpl_malloc(sizeof(UnaryOp));
            unary->right=right;
            unary->opr=type;

            node->node=unary;
            this->backadvance();
            return node;
        }

        Node* make_list(parse_ret* ret){
            this->advance();
            vector<Node*>* list=new vector<Node*>;

            if (this->current_tok_is(T_RSQUARE)){
                Node* node=make_node(N_LIST);
                node->start=new Position(this->get_prev().start.infile, this->get_prev().start.index, this->get_prev().start.col, this->get_prev().start.line);
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                List* l=(List*)fpl_malloc(sizeof(List));
                l->list=list;
                
                node->node=l;
                return node;
            }
            
            
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            if (expr==NULL){
                delete list;
                return NULL;
            }
            this->multi=b;

            if (ret->errornum>0){
                delete list;
                return NULL;
            }
            list->push_back(expr);

            while(this->current_tok_is(T_COMMA)){
                this->advance();
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                Node* expr=this->expr(ret, LOWEST);
                this->noassign=noassign;
                if (expr==NULL){
                    delete list;
                    return NULL;
                }
                this->multi=b;
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
            
            List* l=(List*)fpl_malloc(sizeof(List));
            l->list=list;
            
            node->node=l;
            return node;
        }

        Node* make_tuple(parse_ret* ret, Node* base){
            this->advance();
            vector<Node*>* list=new vector<Node*>;
            list->push_back(base);

            if (this->current_tok_is(T_RPAREN)){
                Node* node=make_node(N_TUPLE);
                node->start=new Position(this->get_prev().start.infile, this->get_prev().start.index, this->get_prev().start.col, this->get_prev().start.line);
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                List* l=(List*)fpl_malloc(sizeof(List));
                l->list=list;
                
                node->node=l;
                return node;
            }
            
            
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            if (expr==NULL){
                delete list;
                return NULL;
            }
            this->multi=b;
            if (ret->errornum>0){
                delete list;
                return NULL;
            }
            list->push_back(expr);

            while(this->current_tok_is(T_COMMA)){
                this->advance();
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                Node* expr=this->expr(ret, LOWEST);
                this->noassign=noassign;
                if (expr==NULL){
                    delete list;
                    return NULL;
                }
                this->multi=b;
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
            
            List* l=(List*)fpl_malloc(sizeof(List));
            l->list=list;
            
            node->node=l;
            return node;
        }

        Node* make_set(parse_ret* ret, Node* base){
            this->advance();
            vector<Node*>* list=new vector<Node*>;
            list->push_back(base);

            if (this->current_tok_is(T_RCURLY)){
                Node* node=make_node(N_SET);
                node->start=new Position(this->get_prev().start.infile, this->get_prev().start.index, this->get_prev().start.col, this->get_prev().start.line);
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                List* l=(List*)fpl_malloc(sizeof(List));
                l->list=list;
                
                node->node=l;
                return node;
            }
            
            
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            if (expr==NULL){
                delete list;
                return NULL;
            }
            this->multi=b;
            if (ret->errornum>0){
                delete list;
                return NULL;
            }
            list->push_back(expr);

            while(this->current_tok_is(T_COMMA)){
                this->advance();
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                Node* expr=this->expr(ret, LOWEST);
                this->noassign=noassign;
                if (expr==NULL){
                    delete list;
                    return NULL;
                }
                this->multi=b;
                if (ret->errornum>0){
                    delete list;
                    return NULL;
                }
                list->push_back(expr);
                if (this->current_tok_is(T_RCURLY)){
                    break;
                }
            }

            Node* node=make_node(N_SET);
            node->start=new Position(this->get_prev().start.infile, this->get_prev().start.index, this->get_prev().start.col, this->get_prev().start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            List* l=(List*)fpl_malloc(sizeof(List));
            l->list=list;
            
            node->node=l;
            return node;
        }

        Node* make_dict(parse_ret* ret){
            this->advance();
            vector<Node*>* keys=new vector<Node*>;
            vector<Node*>* vals=new vector<Node*>;

            if (this->current_tok_is(T_RCURLY)){
                Node* node=make_node(N_DICT);
                node->start=new Position(this->get_prev().start.infile, this->get_prev().start.index, this->get_prev().start.col, this->get_prev().start.line);
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                Dict* d=(Dict*)fpl_malloc(sizeof(Dict));
                d->keys=keys;
                d->vals=vals;
                
                node->node=d;
                return node;
            }

            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* key=this->expr(ret, LOWEST);
            this->noassign=noassign;
            if (key==NULL){
                delete keys;
                delete vals;
                return NULL;
            }
            this->multi=b;
            if (ret->errornum>0){
                delete vals;
                delete keys;
                return NULL;
            }
            if (!this->current_tok_is(T_COLON)){
                if (this->current_tok_is(T_COMMA)){
                    delete keys;
                    delete vals;
                    return make_set(ret, key);
                }
                this->add_parsing_error(ret, "SyntaxError: Expected :");
                delete vals;
                delete keys;
                return NULL;
            }
            this->advance();
            b=this->multi;
            noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* value=this->expr(ret, LOWEST);
            this->noassign=noassign;
            if (value==NULL){
                delete keys;
                delete vals;
                return NULL;
            }
            this->multi=b;
            if (ret->errornum>0){
                delete vals;
                delete keys;
                return NULL;
            }
            keys->push_back(key);
            vals->push_back(value);

            while(this->current_tok_is(T_COMMA)){
                this->advance();
                b=this->multi;
                noassign=this->noassign;
                this->noassign=false;
                this->multi=false;
                Node* key=this->expr(ret, LOWEST);
                this->noassign=noassign;
                if (key==NULL){
                    delete keys;
                    delete vals;
                    return NULL;
                }
                this->multi=b;
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
                b=this->multi;
                noassign=this->noassign;
                this->noassign=false;
                this->multi=false;
                Node* value=this->expr(ret, LOWEST);
                this->noassign=noassign;
                if (value==NULL){
                    delete keys;
                    delete vals;
                    return NULL;
                }
                this->multi=b;
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
            
            Dict* d=(Dict*)fpl_malloc(sizeof(Dict));
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
            vector<int>* unpackkwargs=new vector<int>;
            unpackkwargs->clear();
            vector<int>* unpackargs=new vector<int>;
            unpackargs->clear();

            if (this->current_tok_is(T_RPAREN)){
                Node* node;
                if (left->type==N_DOT){
                    node=make_node(N_DOTCALL);
                    node->start=left->start;
                    node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                    
                    DotCall* c=(DotCall*)fpl_malloc(sizeof(DotCall));
                    c->args=args;
                    c->kwargs=kwargs;
                    c->dot=left;
                    c->stargs=unpackargs;
                    c->stkwargs=unpackkwargs;
                    
                    node->node=c;
                }
                else{
                    node=make_node(N_CALL);
                    node->start=left->start;
                    node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                    
                    Call* c=(Call*)fpl_malloc(sizeof(Call));
                    c->args=args;
                    c->kwargs=kwargs;
                    c->object=left;
                    c->stargs=unpackargs;
                    c->stkwargs=unpackkwargs;
                    
                    node->node=c;
                }

                this->advance();

                return node;
            }
            


            int idx=0;

            if (this->current_tok_is(T_MUL)){
                this->advance();
                unpackargs->push_back(idx);
            }
            else if (this->current_tok_is(T_POW)){
                this->advance();
                unpackkwargs->push_back(idx);
            }
            
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            if (expr==NULL){
                return NULL;
            }
            this->noassign=noassign;
            this->multi=b;

            if (ret->errornum>0){
                delete unpackargs;
                delete unpackkwargs;
                return NULL;
            }
            if (expr->type==N_ASSIGN){
                kwargs->push_back(expr);
            }
            else {
                args->push_back(expr);
            }
            idx++;
            

            while(this->current_tok_is(T_COMMA)){
                this->advance();

                if (this->current_tok_is(T_MUL)){
                    this->advance();
                    unpackargs->push_back(idx);
                }
                else if (this->current_tok_is(T_POW)){
                    this->advance();
                    unpackkwargs->push_back(idx);
                }

                
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=false;
                this->multi=false;
                Node* expr=this->expr(ret, LOWEST);
                this->noassign=noassign;
                if (expr==NULL){
                    delete unpackargs;
                    delete unpackkwargs;
                    return NULL;
                }
                this->multi=b;

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
                        delete unpackargs;
                        delete unpackkwargs;
                        return NULL;
                    }
                    args->push_back(expr);
                }
                if (ret->errornum>0){
                    delete unpackargs;
                    delete unpackkwargs;
                    return NULL;
                }
                if (!this->current_tok_is(T_COMMA) && !this->current_tok_is(T_RPAREN) && !this->current_tok_is(T_EOF)){
                    this->add_parsing_error(ret, "SyntaxError: Invalid syntax.");
                    this->advance();
                    delete unpackargs;
                    delete unpackkwargs;
                    return NULL;
                }
                idx++;
            }
            
            if (!this->current_tok_is(T_RPAREN)){
                this->add_parsing_error(ret, "SyntaxError: Invalid syntax.");
                this->advance();
                delete unpackargs;
                delete unpackkwargs;
                return NULL;
            }
            

            Node* node;
            if (left->type==N_DOT){
                node=make_node(N_DOTCALL);
                node->start=left->start;
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                DotCall* c=(DotCall*)fpl_malloc(sizeof(DotCall));
                c->args=args;
                c->kwargs=kwargs;
                c->dot=left;
                c->stargs=unpackargs;
                c->stkwargs=unpackkwargs;
                
                node->node=c;
            }
            else{
                node=make_node(N_CALL);
                node->start=left->start;
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                Call* c=(Call*)fpl_malloc(sizeof(Call));
                c->args=args;
                c->kwargs=kwargs;
                c->object=left;
                c->stargs=unpackargs;
                c->stkwargs=unpackkwargs;
                
                node->node=c;
            }

            this->advance();

            return node;
        }

        Node* make_store_subscr(parse_ret* ret, Node* left){
            this->advance();
            
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
            
            Node* node=make_node(N_STORE_SUBSCR);
            node->start=left->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            StoreSubscript* s=(StoreSubscript*)fpl_malloc(sizeof(StoreSubscript));
            s->left=left;
            s->expr=expr;
            
            node->node=s;
            return node;
        }

        Node* make_store_slice(parse_ret* ret, Node* left){
            this->advance();
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
            
            Node* node=make_node(N_STORE_SLICE);
            node->start=left->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            StoreSlice* s=(StoreSlice*)fpl_malloc(sizeof(StoreSlice));
            s->left=left;
            s->expr=expr;
            
            node->node=s;
            return node;
        }

        Node* make_slice(parse_ret* ret, Node* left){
            this->advance();
            if (this->current_tok_is(T_RSQUARE)){
                Node* node=make_node(N_SLICE);
                node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line); //No guarrantee
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

                Slice* s=(Slice*)fpl_malloc(sizeof(Slice));
                s->left=left;
                s->right=NULL;
                
                node->node=s;
                
                this->advance();

                if (this->current_tok_is(T_EQ)){
                    return this->make_store_slice(ret, node);
                }
                return node;
            }
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
            
            Node* node=make_node(N_SLICE);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line); //No guarrantee
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            Slice* s=(Slice*)fpl_malloc(sizeof(Slice));
            s->left=left; //If null, same passthrough
            s->right=expr;
            
            node->node=s;
            
            if (!this->current_tok_is(T_RSQUARE)){
                return NULL;
            }

            this->advance();
            if (this->current_tok_is(T_EQ)){
                return this->make_store_slice(ret, node);
            }
            return node;
        }

        Node* make_subscr(parse_ret* ret, Node* left){
            this->advance();
            if (this->current_tok_is(T_COLON)){
                Node* node=make_node(N_SUBSCR);
                node->start=left->start;
                node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                
                Subscript* s=(Subscript*)fpl_malloc(sizeof(Subscript));
                s->left=left;
                s->expr=NULL;
                
                node->node=s;

                return this->make_slice(ret, node);
            }
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
            
            Node* node=make_node(N_SUBSCR);
            node->start=left->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            Subscript* s=(Subscript*)fpl_malloc(sizeof(Subscript));
            s->left=left;
            s->expr=expr;
            
            node->node=s;

            if (this->current_tok_is(T_COLON)){
                return this->make_slice(ret, node);
            }

            if (!this->current_tok_is(T_RSQUARE)){
                this->add_parsing_error(ret, "SyntaxError: Expected ], got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            
            this->advance();

            if (this->current_tok_is(T_EQ)){
                return this->make_store_subscr(ret, node);
            }
            return node;
        }

        Node* make_nonlocal(parse_ret* ret){
            if (!this->current_tok_is(T_IDENTIFIER)){
                this->add_parsing_error(ret, "SyntaxError: Invalid syntax");
                this->advance();
                return NULL;
            }
            Node* name=this->atom(ret);
            name->type=N_NONLOCAL;
            
            if (this->next_tok_is(T_COLON)){
                this->advance();
                string _name=this->current_tok.data;
                if (!this->current_tok_is(T_IDENTIFIER)){
                    this->add_parsing_error(ret, "SyntaxError: Expected identifier, got '%s'", token_type_to_str(this->current_tok.type).c_str());
                    this->advance();
                    return NULL;
                }
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                Node* tp=this->expr(ret, LOWEST);
                this->noassign=noassign;
                this->backadvance();

                free(name->node);

                AnnotatedIdentifier* i=(AnnotatedIdentifier*)fpl_malloc(sizeof(AnnotatedIdentifier));
                i->name=new string(_name);
                i->tp=tp;
                name->node=i;   
                name->type=N_ANONNONLOCAL;  
                return name;           
            }
            return name;
        }

        Node* make_colon(parse_ret* ret){
            this->advance();
            if (this->current_tok_is(T_COLON)){
                this->advance();
                return make_nonlocal(ret);
            }
            if (!this->current_tok_is(T_IDENTIFIER)){
                this->backadvance();
                this->add_parsing_error(ret, "SyntaxError: Invalid syntax");
                this->advance();
                return NULL;
            }
            Node* name=this->atom(ret);
            
            Node* node=make_node(N_GLBL_IDENT);
            node->start=name->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            GlblIdent* g=(GlblIdent*)fpl_malloc(sizeof(GlblIdent));
            g->name=name;
            
            node->node=g;
            
            this->backadvance();
            if (this->current_tok_is(T_COLON)){
                string _name=this->get_prev().data;
                this->advance();
                if (!this->current_tok_is(T_IDENTIFIER)){
                    this->add_parsing_error(ret, "SyntaxError: Expected identifier, got '%s'", token_type_to_str(this->current_tok.type).c_str());
                    this->advance();
                    return NULL;
                }
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                Node* tp=this->expr(ret, LOWEST);
                this->noassign=noassign;
                this->backadvance();

                free(node->node);
                free(g->name);

                AnnotatedIdentifier* i=(AnnotatedIdentifier*)fpl_malloc(sizeof(AnnotatedIdentifier));
                i->name=new string(_name);
                i->tp=tp;
                node->node=i;   
                node->type=N_ANONGLBL_IDENT;  
                return node;           
            }
            this->advance();
            return node;
        }

        Node* make_lambda(parse_ret* ret){
            this->advance();
            vector<Node*>* args=new vector<Node*>;
            args->clear();
            vector<Node*>* kwargs=new vector<Node*>;
            kwargs->clear();
            bool starargs=false;
            bool starkwargs=false;
            Node* stargs=NULL;
            Node* stkwargs=NULL;
            Node* rettp=NULL;

            Node* name=make_node(N_IDENT);
            
            name->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            name->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);

            Identifier* i=(Identifier*)fpl_malloc(sizeof(Identifier));
            i->name=new string("<lambda>");
            name->node=i;

            //Parse arguments
            while (!this->current_tok_is(T_LCURLY)){
                if (this->current_tok_is(T_MUL)){
                    this->advance();
                    if (!this->current_tok_is(T_COMMA) && !this->current_tok_is(T_IDENTIFIER) || starargs){
                            this->add_parsing_error(ret, "SyntaxError: Invalid syntax");
                            this->advance();
                            delete args;
                            delete kwargs;
                            return NULL;
                    }
                    stargs=this->atom(ret);
                    if (stargs->type!=N_IDENT){
                        this->add_parsing_error(ret, "SyntaxError: Expected identifier");
                        this->advance();
                        delete args;
                        delete kwargs;
                        return NULL;
                    }
                    starargs=true;  
                    this->advance();
                    continue;                
                }
                else if (this->current_tok_is(T_POW)){
                    this->advance();
                    if (!this->current_tok_is(T_COMMA) && !this->current_tok_is(T_IDENTIFIER) || starkwargs){
                            this->add_parsing_error(ret, "SyntaxError: Invalid syntax");
                            this->advance();
                            delete args;
                            delete kwargs;
                            return NULL;
                    }
                    stkwargs=this->atom(ret);
                    if (stkwargs->type!=N_IDENT){
                        this->add_parsing_error(ret, "SyntaxError: Expected identifier");
                        this->advance();
                        delete args;
                        delete kwargs;
                        return NULL;
                    }
                    starkwargs=true; 
                    this->advance();
                    continue;
                }
                
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=false;
                this->multi=false;
                Node* expr=this->expr(ret, LOWEST);
                this->noassign=noassign;
                if (expr==NULL){
                    return NULL;
                }
                this->multi=b;
                if (expr->type==N_ASSIGN || expr->type==N_ANONASSIGN){
                    kwargs->push_back(expr);
                    if (this->current_tok_is(T_LCURLY)){
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
                    
                    args->push_back(expr);
                    if (this->current_tok_is(T_LCURLY)){
                        break;
                    }
                    this->advance();
                }
            }
            //
            
            if (!this->current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                delete args;
                delete kwargs;
                return NULL;
            }

            this->advance();
            bool inloop=this->inloop;
            this->inloop=false;
            skip_newline;
            parse_ret code;
            if (!this->current_tok_is(T_RCURLY)){
                code=this->statements();
            }
            else{
                code.errornum=0;
                code.nodes.clear();
            }
            if (code.errornum>0){
                (*ret)=code;
            }
            this->inloop=inloop;
            
            if (!this->current_tok_is(T_RCURLY)){
                this->backadvance();
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                delete args;
                delete kwargs;
                return NULL;
            }
            if (this->next_tok_is(T_COLON)){
                this->advance();
                this->advance();
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                Node* rettp=this->expr(ret, LOWEST);
                this->noassign=noassign;
                this->multi=b;
                // this->advance(); // BACKADVANCE would cancel out
            }

            Node* node=make_node(N_FUNC);
            node->start=name->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Func* f=(Func*)fpl_malloc(sizeof(Func));
            f->name=name;
            f->code=new vector<Node*>;
            f->code->clear();
            for (Node* n: code.nodes){
                f->code->push_back(n);
            }
            f->args=args;
            f->kwargs=kwargs;
            f->type=FUNCTION_LAMBDA;
            f->stargs=stargs;
            f->stkwargs=stkwargs;
            f->starargs=starargs;
            f->starkwargs=starkwargs;
            f->rettp=rettp;

            node->node=f;
            return node;
        }

        Node* make_ternary(parse_ret* ret, Node* left){
            this->advance();
            //Make new binop, with minimal setup
            Node* node=make_node(N_TERNARY);
            node->start=left->start;

            Ternary* tern=(Ternary*)fpl_malloc(sizeof(Ternary));
            tern->left=left;
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            tern->expr1=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
            
            if (!this->current_tok_is(T_COLON)){
                this->add_parsing_error(ret, "SyntaxError: Expected :, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            this->advance();
            
            b=this->multi;
            noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            tern->expr2=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;

            node->node=tern;

            node->end=tern->expr2->end;
            return node;
        }

        Node* make_decorator(parse_ret* ret){
            this->advance();
            
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* name=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
            if (!isname(name->type)){
                this->add_parsing_error(ret, "SyntaxError: Invalid syntax");
                this->advance();
                return NULL;
            }
            Node* function=NULL;
            Node* decorator=NULL;

            this->advance();
            skip_newline;
            if (this->current_tok_is(T_KWD) && this->current_tok.data=="func"){
                function=make_function(ret, FUNCTION_NORMAL);
            }
            else{
                //We must have another decorator.. othwerise.. ERROR!
                if (!this->current_tok_is(T_AT)){
                    this->add_parsing_error(ret, "SyntaxError: Invalid syntax");
                    this->advance();
                    return NULL;
                }
                
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                decorator=this->expr(ret, LOWEST);
                this->noassign=noassign;
                this->multi=b;
            }

            
            //Make new binop, with minimal setup
            Node* node=make_node(N_DECORATOR);
            node->start=name->start;

            Decorator* dec=(Decorator*)fpl_malloc(sizeof(Decorator));
            dec->function=function;
            dec->decorator=decorator;
            dec->name=name;

            node->node=dec;

            node->end=name->end;
            this->backadvance();
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
                    left=make_identifier(ret);
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
                case T_TILDE:
                    left=make_tilde(ret);
                    break;
                    
                case T_NOT:
                    left=make_not(ret);
                    break;

                case T_COLON:
                    left=make_colon(ret);
                    break;

                case T_AT:
                    left=make_decorator(ret);
                    break;

                case T_KWD:
                    if (this->current_tok.data=="lambda"){
                        left=make_lambda(ret);
                    }
                    break;
                    
                    

                default:
                    return NULL;

            }
            return left;
        }

        bool is_atomic(){
            switch (this->current_tok.type){
                case T_INT:
                case T_FLOAT:
                case T_STR:
                case T_IDENTIFIER:
                case T_LPAREN:
                case T_LSQUARE:
                case T_LCURLY:
                case T_TRUE:
                case T_FALSE:
                case T_NONE:
                case T_PLUS:
                case T_MINUS:
                case T_TILDE:
                case T_NOT:
                case T_COLON:
                    return true;
                case T_KWD:
                    if (this->current_tok.data=="lambda"){
                        return true;
                    }
            }
            return false;
        }

        Node* expr(parse_ret* ret, int prec){
            Node* left=this->atom(ret);
            if (left==NULL){
                if (this->current_tok.type==T_EOF){
                    this->add_parsing_error(ret, "SyntaxError: Unexpected EOF.");
                    this->advance();
                    return left;
                }
                if (this->current_tok.type==T_ERROR_EOF){
                    this->add_parsing_error(ret, "SyntaxError: Unexpected EOF.");
                    this->advance();
                    return left;
                }
                if (this->current_tok.type==T_ERR){
                    this->add_parsing_error(ret, "SyntaxError: Invalid syntax.");
                    this->backadvance();
                    return left;
                }
                if (this->current_tok.type==T_UNKNOWN){
                    this->add_parsing_error(ret, "SyntaxError: Invalid syntax.");
                    this->backadvance();
                    return left;
                }
                this->add_parsing_error(ret, "SyntaxError: Invalid syntax.");//Expected expression, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return left;
            }
            
            
            this->advance();
            while (!(this->current_tok_is(T_EOF)) && prec<get_precedence(this->current_tok) || this->current_tok_is(T_DOTIDENT)){
                switch (this->current_tok.type){
                    case T_EQ:
                        if (this->noassign){
                            return left;
                        }
                        left=make_assignment(ret, left);
                        break;

                    case T_PLUS:
                    case T_MINUS:
                    case T_MUL:
                    case T_DIV:
                    case T_IADD:
                    case T_ISUB:
                    case T_IMUL:
                    case T_IDIV:
                    case T_NE:
                    case T_EE:
                    case T_GT:
                    case T_GTE:
                    case T_LT:
                    case T_LTE:
                    case T_PERCENT:
                    case T_IS:
                    case T_POW:
                    case T_IPOW:
                    case T_IMOD:
                    case T_AND:
                    case T_OR:
                    case T_AMPERSAND:
                    case T_VBAR:
                    case T_LSHIFT:
                    case T_RSHIFT:
                    case T_IAMP:
                    case T_IVBAR:
                    case T_ILSH:
                    case T_IRSH:
                    case T_NOT:
                    case T_IN:
                    case T_FLDIV:
                    case T_IFLDIV:
                        left=make_binop(ret, left, this->current_tok.type);
                        break;
                    
                    case T_LPAREN:
                        left=make_call(ret, left);
                        break;

                    case T_LSQUARE:
                        left=make_subscr(ret, left);
                        break;

                    case T_DOTIDENT: {
                        if (!isname(left->type) && this->next_tok_is(T_DOTIDENT)){
                            this->add_parsing_error(ret, "SyntaxError: Invalid syntax");
                            this->advance();
                            return NULL;
                        }
                        Node* newnode=make_node(N_DOT);
                        newnode->start=left->start;
                        newnode->end=left->end;
                        Dot* d=(Dot*)fpl_malloc(sizeof(Dot));
                        d->names=new vector<Node*>;
                        d->names->clear();
                        d->names->push_back(left);
                        
                        while (this->current_tok_is(T_DOTIDENT)){
                            Node* n=make_node(N_IDENT);
                            n->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
                            n->end=new Position(this->current_tok.end.infile, this->current_tok.end.index, this->current_tok.end.col, this->current_tok.end.line);
                            Identifier* i=(Identifier*)fpl_malloc(sizeof(Identifier));
                            i->name=new string(this->current_tok.data);
                            
                            n->node=i;

                            d->names->push_back(n);
                            this->advance();
                        }

                        
                        newnode->node=d;

                        if (this->current_tok_is(T_COLON)){
                            this->advance();
                            if (!this->current_tok_is(T_IDENTIFIER)){
                                this->add_parsing_error(ret, "SyntaxError: Expected identifier, got '%s'", token_type_to_str(this->current_tok.type).c_str());
                                this->advance();
                                return NULL;
                            }
                            bool b=this->multi;
                            bool noassign=this->noassign;
                            this->noassign=true;
                            this->multi=false;
                            Node* tp=this->expr(ret, LOWEST);
                            this->noassign=noassign;
                            this->multi=b;
                            this->backadvance();

                            AnnotatedDot* i=(AnnotatedDot*)fpl_malloc(sizeof(AnnotatedDot));
                            i->names=d->names;
                            i->tp=tp;
                            newnode->node=i;   
                            newnode->type=N_ANONDOT; 
                            this->advance(); 
                                     
                        }

                        left=newnode;
                        break;
                    }

                    case T_QMARK:
                        left=make_ternary(ret, left);
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
            bool noassign=this->noassign;
            this->noassign=true;
            Node* n;
            if (this->current_tok.data=="func"){
                n=make_function(ret, FUNCTION_NORMAL);
            }
            else if (this->current_tok.data=="class"){
                n= make_class(ret);
            }
            else if (this->current_tok.data=="return"){
                n= make_return(ret);
            }
            else if (this->current_tok.data=="if"){
                n= make_if(ret);
            }   
            else if (this->current_tok.data=="raise"){
                n= make_raise(ret);
            }
            else if (this->current_tok.data=="try"){
                n= make_try(ret);
            }               
            else if (this->current_tok.data=="for"){
                n= make_for(ret);
            }
            else if (this->current_tok.data=="break"){
                n= make_break(ret);
            }
            else if (this->current_tok.data=="continue"){
                n= make_continue(ret);
            }
            else if (this->current_tok.data=="while"){
                n= make_while(ret);
            }
            else if (this->current_tok.data=="import"){
                n= make_import(ret);
            }
            else if (this->current_tok.data=="from"){
                n= make_from(ret);
            }
            else if (this->current_tok.data=="del"){
                n= make_del(ret);
            }
            else if (this->current_tok.data=="assert"){
                n= make_assert(ret);
            }
            else if (this->current_tok.data=="lambda"){
                n=this->expr(ret,LOWEST);
            }
            else if (this->current_tok.data=="yield"){
                n= make_yield(ret);
            }
            else{
                this->add_parsing_error(ret, "SyntaxError: Unexpected keyword '%s'",this->current_tok.data.c_str());
                this->advance();
                return NULL;
            }
            this->noassign=noassign;
            return n;
        }

        Node* make_function(parse_ret* ret, int type){
            vector<Node*>* args=new vector<Node*>;
            args->clear();
            vector<Node*>* kwargs=new vector<Node*>;
            kwargs->clear();
            bool starargs=false;
            bool starkwargs=false;
            Node* stargs=NULL;
            Node* stkwargs=NULL;
            Node* rettp=NULL;

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
                this->add_parsing_error(ret, "SyntaxError: Expected (, got '%s'",token_type_to_str(errtok.type).c_str());
                this->advance();
                delete args;
                delete kwargs;
                return NULL;
            }
            //Parse arguments
            this->advance();
            while (!this->current_tok_is(T_RPAREN)){
                if (this->current_tok_is(T_MUL)){
                    this->advance();
                    if (!this->current_tok_is(T_COMMA) && !this->current_tok_is(T_IDENTIFIER) || starargs){
                            this->add_parsing_error(ret, "SyntaxError: Invalid syntax");
                            this->advance();
                            delete args;
                            delete kwargs;
                            return NULL;
                    }
                    stargs=this->atom(ret);
                    if (stargs->type!=N_IDENT){
                        this->add_parsing_error(ret, "SyntaxError: Expected identifier");
                        this->advance();
                        delete args;
                        delete kwargs;
                        return NULL;
                    }
                    starargs=true;  
                    this->advance();
                    continue;                
                }
                else if (this->current_tok_is(T_POW)){
                    this->advance();
                    if (!this->current_tok_is(T_COMMA) && !this->current_tok_is(T_IDENTIFIER) || starkwargs){
                            this->add_parsing_error(ret, "SyntaxError: Invalid syntax");
                            this->advance();
                            delete args;
                            delete kwargs;
                            return NULL;
                    }
                    stkwargs=this->atom(ret);
                    if (stkwargs->type!=N_IDENT){
                        this->add_parsing_error(ret, "SyntaxError: Expected identifier");
                        this->advance();
                        delete args;
                        delete kwargs;
                        return NULL;
                    }
                    starkwargs=true; 
                    this->advance();
                    continue;
                }
                
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=false;
                this->multi=false;
                Node* expr=this->expr(ret, LOWEST);
                this->noassign=noassign;
                if (expr==NULL){
                    return NULL;
                }
                this->multi=b;
                if (expr->type==N_ASSIGN || expr->type==N_ANONASSIGN){
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
                    
                    args->push_back(expr);
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
            if (this->current_tok_is(T_COLON)){
                this->advance();
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                rettp=this->expr(ret, LOWEST);
                this->noassign=noassign;
                this->multi=b;
                // this->advance(); // BACKADVANCE would cancel out
            }
            if (!this->current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                delete args;
                delete kwargs;
                return NULL;
            }

            this->advance();
            bool inloop=this->inloop;
            this->inloop=false;
            skip_newline;
            parse_ret code;
            if (!this->current_tok_is(T_RCURLY)){
                code=this->statements();
            }
            else{
                code.errornum=0;
                code.nodes.clear();
            }
            if (code.errornum>0){
                (*ret)=code;
            }
            this->inloop=inloop;
            if (!this->current_tok_is(T_RCURLY)){
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

            Func* f=(Func*)fpl_malloc(sizeof(Func));
            f->name=name;
            f->code=new vector<Node*>;
            f->code->clear();
            for (Node* n: code.nodes){
                f->code->push_back(n);
            }
            f->args=args;
            f->kwargs=kwargs;
            f->type=type;
            f->stargs=stargs;
            f->stkwargs=stkwargs;
            f->starargs=starargs;
            f->starkwargs=starkwargs;
            f->rettp=rettp;

            node->node=f;
            this->advance();
            return node;
        }

        Node* make_class(parse_ret* ret){
            this->advance();
            Node* name=this->atom(ret);
            if (name==NULL || name->type!=N_IDENT){
                this->add_parsing_error(ret, "SyntaxError: Expected identifier, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            this->advance();
            vector<Node*>* bases=new vector<Node*>;
            bases->clear();
            if (this->current_tok_is(T_LPAREN)){
                this->advance();

                if (!this->current_tok_is(T_RPAREN)){
                    bool b=this->multi;
                    bool noassign=this->noassign;
                    this->noassign=true;
                    this->multi=false;
                    Node* expr=this->expr(ret, LOWEST);
                    if (expr==NULL){
                        return NULL;
                    }
                    this->noassign=noassign;
                    this->multi=b;

                    bases->push_back(expr);
                }
                while (this->current_tok_is(T_COMMA)){
                    this->advance();

                    bool b=this->multi;
                    bool noassign=this->noassign;
                    this->noassign=true;
                    this->multi=false;
                    Node* expr=this->expr(ret, LOWEST);
                    if (expr==NULL){
                        return NULL;
                    }
                    this->noassign=noassign;
                    this->multi=b;
                    
                    bases->push_back(expr); 
                }
                if (!this->current_tok_is(T_RPAREN)){
                    this->add_parsing_error(ret, "SyntaxError: Expected ), got '%s'",token_type_to_str(this->current_tok.type).c_str());
                    this->advance();
                    return NULL;
                }
                this->advance();
            }
            if (!this->current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            this->advance();

            bool inloop=this->inloop;
            bool inclass=this->inclass;
            this->inloop=false;
            this->inclass=true;
            skip_newline;
            parse_ret code;
            if (!this->current_tok_is(T_RCURLY)){
                code=this->statements();
            }
            else{
                code.errornum=0;
                code.nodes.clear();
            }
            if (code.errornum>0){
                (*ret)=code;
            }
            this->inloop=inloop;
            this->inclass=inclass;

            if (!this->current_tok_is(T_RCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }

            Node* node=make_node(N_CLASS);
            node->start=name->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Class* c=(Class*)fpl_malloc(sizeof(Class));
            c->name=name;
            c->code=new vector<Node*>;
            c->code->clear();
            for (Node* n: code.nodes){
                c->code->push_back(n);
            }
            c->bases=bases;

            node->node=c;
            
            this->advance();
            return node;
        }

        Node* make_return(parse_ret* ret){
            Node* node=make_node(N_RETURN);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            this->advance();
            Node* n;
            if (!this->current_tok_is(T_NEWLINE) && !this->current_tok_is(T_RCURLY)){
                bool b=this->multi;
                bool noassign=this->noassign;
                this->noassign=true;
                this->multi=false;
                n=this->expr(ret, LOWEST);
                this->noassign=noassign;
                this->multi=b;
            }
            else{
                n=make_none();
                this->backadvance();
            }

            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Return* r=(Return*)fpl_malloc(sizeof(Return));
            r->node=n;

            node->node=r;
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
                code.errornum=0;
                code.nodes.clear();
            }
            if (code.errornum>0){
                (*ret)=code;
            }

            if (!this->current_tok_is(T_RCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }         

            Node* node=make_node(N_ELSE);
            node->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Else* e=(Else*)fpl_malloc(sizeof(Else));
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

            Control* ei=(Control*)fpl_malloc(sizeof(Control));
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
                code.errornum=0;
                code.nodes.clear();
            }
            if (code.errornum>0){
                (*ret)=code;
            }

            if (!this->current_tok_is(T_RCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }         

            Node* node=make_node(N_ELSE);
            node->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Else* e=(Else*)fpl_malloc(sizeof(Else));
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

            Control* ei=(Control*)fpl_malloc(sizeof(Control));
            ei->bases=bases;

            node->node=ei;
            
            return node;
        }

        Node* make_elif_(parse_ret* ret){
            this->advance();
            
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
            
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
                code.errornum=0;
                code.nodes.clear();
            }
            if (code.errornum>0){
                (*ret)=code;
            }

            if (!this->current_tok_is(T_RCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }         

            Node* node=make_node(N_IF);
            node->start=expr->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            If* i=(If*)fpl_malloc(sizeof(If));
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

            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
            
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
                code.errornum=0;
                code.nodes.clear();
            }
            if (code.errornum>0){
                (*ret)=code;
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

            If* i=(If*)fpl_malloc(sizeof(If));
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
            else if (this->current_tok_is(T_KWD) && this->current_tok.data=="elif"){
                return this->make_elif(ret, node);
            }
            else if (!this->current_tok_is(T_EOF)){
                reverse_non_newline;
            }

            Node* n=make_node(N_CONTROL);
            n->start=bases->front()->start;
            n->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Control* ei=(Control*)fpl_malloc(sizeof(Control));
            ei->bases=bases;

            n->node=ei;
            return n;
        }

        Node* make_raise(parse_ret* ret){
            this->advance();
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;
            this->advance();

            Node* n=make_node(N_RAISE);
            n->start=expr->start;
            n->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

            Raise* r=(Raise*)fpl_malloc(sizeof(Raise));
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

            TryExceptFinally* f=(TryExceptFinally*)fpl_malloc(sizeof(TryExceptFinally));
            
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
                try_code.errornum=0;
                try_code.nodes.clear();
            }
            if (try_code.errornum>0){
                (*ret)=try_code;
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

            Try* i=(Try*)fpl_malloc(sizeof(Try));
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
                    except_code.errornum=0;
                    except_code.nodes.clear();
                }
                if (except_code.errornum>0){
                    (*ret)=except_code;
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

                Except* e=(Except*)fpl_malloc(sizeof(Except));
                e->code=new vector<Node*>;
                e->code->clear();
                for (Node* n: except_code.nodes){
                    e->code->push_back(n);
                }
                e->type=type;
                e->name=name;

                node->node=e;

                bases->push_back(node);

                this->advance();
            }
            
            skip_newline;

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
                    finally_code.errornum=0;
                    finally_code.nodes.clear();
                }
                if (finally_code.errornum>0){
                    (*ret)=finally_code;
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

                Finally* i=(Finally*)fpl_malloc(sizeof(Finally));
                i->code=new vector<Node*>;
                i->code->clear();
                for (Node* n: finally_code.nodes){
                    i->code->push_back(n);
                }

                node->node=i;

                bases->push_back(node);
            }
            else if (!this->current_tok_is(T_EOF)){
                reverse_non_newline;
            }


            f->bases=bases;

            n->node=f;

            return n;
        }

        Node* make_for(parse_ret* ret){
            this->advance();
            
            if (!current_tok_is(T_IDENTIFIER)){
                this->add_parsing_error(ret, "SyntaxError: Expected identifier, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            Node* ident=this->atom(ret);
            this->advance();
            
            if (!current_tok_is(T_IN)){
                this->add_parsing_error(ret, "SyntaxError: Expected 'in', got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            this->advance();
            
            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;

            if (!current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            this->advance();
            
            bool inloop=this->inloop;
            this->inloop=true;
            skip_newline;
            parse_ret code;
            if (!this->current_tok_is(T_RCURLY)){
                code=this->statements();
            }
            else{
                code.errornum=0;
                code.nodes.clear();
            }
            if (code.errornum>0){
                (*ret)=code;
            }
            this->inloop=inloop;

            if (!this->current_tok_is(T_RCURLY) && !this->get_prev().type!=T_RCURLY){
                this->backadvance();
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }

            this->advance();

            Node* elsen=NULL;

            skip_newline;
            if (this->current_tok_is(T_KWD) && this->current_tok.data=="else"){                
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
                    code.errornum=0;
                    code.nodes.clear();
                }
                if (code.errornum>0){
                    (*ret)=code;
                }

                if (!this->current_tok_is(T_RCURLY)){
                    this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                    this->advance();
                    return NULL;
                }         

                elsen=make_node(N_ELSE);
                elsen->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
                elsen->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);

                Else* e=(Else*)fpl_malloc(sizeof(Else));
                e->base=NULL;
                e->code=new vector<Node*>;
                e->code->clear();
                for (Node* n: code.nodes){
                    e->code->push_back(n);
                }

                elsen->node=e;
                
                this->advance();
            }
            else if (!this->current_tok_is(T_EOF)){
                reverse_non_newline;
            }

            Node* node=make_node(N_FOR);
            node->start=ident->start;
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            For* f=(For*)fpl_malloc(sizeof(For));
            f->ident=ident;
            f->expr=expr;
            f->code=new vector<Node*>;
            f->code->clear();
            for (Node* n: code.nodes){
                f->code->push_back(n);
            }
            f->elsen=elsen;

            node->node=f;

            return node;
        }

        Node* make_break(parse_ret* ret){
            if (!this->inloop){
                this->add_parsing_error(ret, "Break outside loop");
                this->advance();
                return NULL;
            }
            Node* node=make_node(N_BREAK);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            this->advance();
            return node;
        }

        Node* make_continue(parse_ret* ret){
            if (!this->inloop){
                this->add_parsing_error(ret, "Continue outside loop");
                this->advance();
                return NULL;
            }
            Node* node=make_node(N_CONTINUE);
            node->start=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            this->advance();
            return node;
        }

        Node* make_while(parse_ret* ret){
            this->advance();

            Token t=this->current_tok;

            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;

            if (!current_tok_is(T_LCURLY)){
                this->add_parsing_error(ret, "SyntaxError: Expected {, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }
            this->advance();
            
            bool inloop=this->inloop;
            this->inloop=true;
            skip_newline;
            parse_ret code;
            if (!this->current_tok_is(T_RCURLY)){
                code=this->statements();
            }
            else{
                code.errornum=0;
                code.nodes.clear();
            }
            if (code.errornum>0){
                (*ret)=code;
            }
            this->inloop=inloop;

            if (!this->current_tok_is(T_RCURLY) && !this->get_prev().type!=T_RCURLY){
                this->backadvance();
                this->add_parsing_error(ret, "SyntaxError: Expected }, got '%s'",token_type_to_str(this->current_tok.type).c_str());
                this->advance();
                return NULL;
            }

            this->advance();

            Node* node=make_node(N_WHILE);
            node->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            While* w=(While*)fpl_malloc(sizeof(While));
            w->expr=expr;
            w->code=new vector<Node*>;
            w->code->clear();
            for (Node* n: code.nodes){
                w->code->push_back(n);
            }

            node->node=w;

            return node;
        }

        Node* make_import(parse_ret* ret){
            this->advance();

            Token t=this->current_tok;

            vector<Node*>* libnames=new vector<Node*>;
            libnames->clear();
            vector<Node*>* names=new vector<Node*>;
            names->clear();
            
            bool m=this->multi;
            this->multi=false;
            if (!this->current_tok_is(T_IDENTIFIER)){
                this->add_parsing_error(ret, "Expected identifier, got '%s'", token_type_to_str(this->current_tok.type).c_str());
                return NULL;
            }
            Node* libname=this->atom(ret);
            this->multi=m;

            this->advance();
            Node* name=NULL;
            if (this->current_tok_is(T_IDENTIFIER)){
                bool m=this->multi;
                this->multi=false;
                name=this->atom(ret);
                this->multi=m;
                this->advance();
            }
            
            libnames->push_back(libname);
            names->push_back(name);

            while (this->current_tok_is(T_COMMA)){
                this->advance();
                bool m=this->multi;
                this->multi=false;
                if (!this->current_tok_is(T_IDENTIFIER)){
                    this->add_parsing_error(ret, "Expected identifier, got '%s'", token_type_to_str(this->current_tok.type).c_str());
                    return NULL;
                }
                Node* libname=this->atom(ret);
                this->multi=m;
                
                this->advance();
                Node* name=NULL;
                if (this->current_tok_is(T_IDENTIFIER)){
                    bool m=this->multi;
                    this->multi=false;
                    name=this->atom(ret);
                    this->multi=m;
                    this->advance();
                }
                
                libnames->push_back(libname);
                names->push_back(name);
            }
            
            Node* node=make_node(N_IMPORT);
            node->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            Import* i=(Import*)fpl_malloc(sizeof(Import));
            i->libnames=libnames;
            i->names=names;

            node->node=i;

            return node;
        }

        Node* make_from(parse_ret* ret){
            this->advance();

            Token t=this->current_tok;

            bool m=this->multi;
            this->multi=false;
            if (!this->current_tok_is(T_IDENTIFIER)){
                this->add_parsing_error(ret, "Expected identifier, got '%s'", token_type_to_str(this->current_tok.type).c_str());
                return NULL;
            }
            Node* lib=this->atom(ret);
            this->multi=m;
            this->advance();

            if (!(this->current_tok_is(T_KWD) && this->current_tok.data=="import")){
                this->add_parsing_error(ret, "Expected 'import', got '%s'", token_type_to_str(this->current_tok.type).c_str());
                return NULL;
            }
            
            vector<Node*>* names=new vector<Node*>;
            names->clear();

            this->advance();

            if (!this->current_tok_is(T_IDENTIFIER) && !this->current_tok_is(T_MUL)){
                this->add_parsing_error(ret, "Expected identifier, got '%s'", token_type_to_str(this->current_tok.type).c_str());
                return NULL;
            }
            
            m=this->multi;
            this->multi=false;
            Node* name=NULL;
            if (!this->current_tok_is(T_MUL)){
                name=this->atom(ret);
            }
            this->multi=m;

            this->advance();

            names->push_back(name);

            while (this->current_tok_is(T_COMMA)){
                this->advance();
                bool m=this->multi;
                this->multi=false;
                if (!this->current_tok_is(T_IDENTIFIER)){
                    this->add_parsing_error(ret, "Expected identifier, got '%s'", token_type_to_str(this->current_tok.type).c_str());
                    return NULL;
                }
                Node* name=this->atom(ret);
                this->multi=m;

                names->push_back(name);
            }
            
            Node* node=make_node(N_FROM);
            node->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            From* i=(From*)fpl_malloc(sizeof(From));
            i->names=names;
            i->name=lib;

            node->node=i;

            return node;
        }

        Node* make_del(parse_ret* ret){
            this->advance();

            Token t=this->current_tok;
            if (!this->current_tok_is(T_IDENTIFIER) && !this->current_tok_is(T_COLON)){
                this->add_parsing_error(ret, "Expected identifier, got '%s'", token_type_to_str(this->current_tok.type).c_str());
                return NULL;
            }

            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;

            if (!this->isname(expr->type)){
                this->add_parsing_error(ret, "Expected name, got non-name");
                return NULL;
            }
            
            Node* node=make_node(N_DEL);
            node->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            Del* d=(Del*)fpl_malloc(sizeof(Del));
            d->expr=expr;

            node->node=d;

            return node;
        }

        Node* make_assert(parse_ret* ret){
            this->advance();

            Token t=this->current_tok;

            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;

            Node* node=make_node(N_ASSERT);
            node->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            Assert* a=(Assert*)fpl_malloc(sizeof(Assert));
            a->expr=expr;

            node->node=a;

            return node;
        }

        Node* make_yield(parse_ret* ret){
            this->advance();

            Token t=this->current_tok;

            bool b=this->multi;
            bool noassign=this->noassign;
            this->noassign=true;
            this->multi=false;
            Node* expr=this->expr(ret, LOWEST);
            this->noassign=noassign;
            this->multi=b;

            Node* node=make_node(N_YIELD);
            node->start=new Position(t.start.infile, t.start.index, t.start.col, t.start.line);
            node->end=new Position(this->current_tok.start.infile, this->current_tok.start.index, this->current_tok.start.col, this->current_tok.start.line);
            
            Yield* y=(Yield*)fpl_malloc(sizeof(Yield));
            y->expr=expr;

            node->node=y;

            return node;
        }
};