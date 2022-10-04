void reverse_instructions(struct instructions* instrs){
    struct instruction* prev = NULL;
    struct instruction* current = instrs->first;
    struct instruction* next;

    while (current != NULL){
        next=current->next;
        current->next=prev;
        prev=current;
        current=next;
    }

    instrs->first=prev;
}

struct instructions* new_instructions(){
    struct instructions* instr=(struct instructions*)fpl_malloc(sizeof(struct instructions));

    instr->count=0;
    instr->first=NULL;

    return instr;
}

void add_instruction(struct instructions* instructions, enum opcode opcode, uint32_t arg, Position* start, Position* end){
    struct instruction* instr=(struct instruction*)fpl_malloc(sizeof(struct instruction));
    instr->arg=arg;
    instr->opcode=opcode;
    instr->next=instructions->first;
    instr->start=start;
    instr->end=end;

    instructions->first=instr;
    instructions->count++;
}

struct compiler* new_compiler(){
    struct compiler* compiler=(struct compiler*)fpl_malloc(sizeof(struct compiler));
    compiler->consts=new_list();
    compiler->instructions=new_instructions();
    compiler->names=new_list();
    compiler->scope=SCOPE_GLOBAL;
    compiler->keep_return=false;
    
    return compiler;
}

void compiler_del(struct compiler* compiler){
    DECREF(compiler->consts);
    DECREF(compiler->names);
    struct instruction* i=compiler->instructions->first;
    while (i){
        struct instruction* i_=i->next;
        free(i);
        i=i_;
    }
}

int compile_expr(struct compiler* compiler, Node* expr){
    nodetype type=expr->type;
    switch (expr->type){
        case N_ASSIGN: {
            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            int cmpexpr=compile_expr(compiler, ASSIGN(expr->node)->right); //Push data
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (ASSIGN(expr->node)->name->type==N_MULTIIDENT){
                add_instruction(compiler->instructions,UNPACK_SEQ,  MULTIIDENT(ASSIGN(expr->node)->name->node)->name->size(), expr->start, expr->end);
            }
            if (!ret){
                compiler->keep_return=false;
            }

            uint32_t idx;
            if (ASSIGN(expr->node)->name->type==N_IDENT){
                if (!_list_contains(compiler->names, IDENTI(ASSIGN(expr->node)->name->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(ASSIGN(expr->node)->name->node)->name));
                    idx=NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*IDENTI(ASSIGN(expr->node)->name->node)->name));
                }

                switch (compiler->scope){
                    case SCOPE_GLOBAL:
                        add_instruction(compiler->instructions,STORE_GLOBAL, idx, expr->start, expr->end);
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler->instructions,STORE_NAME, idx, expr->start, expr->end);
                        break;
                }
                if (!compiler->keep_return){
                    add_instruction(compiler->instructions,POP_TOS, 0, expr->start, expr->end);
                }
            }
            else if (ASSIGN(expr->node)->name->type==N_MULTIIDENT){
                int cmpexpr=compile_expr(compiler, ASSIGN(expr->node)->name);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                add_instruction(compiler->instructions,POP_TOS, 0, expr->start, expr->end);
            }
            else if (ASSIGN(expr->node)->name->type==N_NONLOCAL){
                if (!_list_contains(compiler->names, IDENTI(ASSIGN(expr->node)->name->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(ASSIGN(expr->node)->name->node)->name));
                    idx=NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*IDENTI(ASSIGN(expr->node)->name->node)->name));
                }
                add_instruction(compiler->instructions,STORE_NONLOCAL, idx, expr->start, expr->end);
            }
            else if (ASSIGN(expr->node)->name->type==N_GLBL_IDENT){
                if (!_list_contains(compiler->names, IDENTI(GLBLIDENT(ASSIGN(expr->node)->name->node)->name->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(GLBLIDENT(ASSIGN(expr->node)->name->node)->name->node)->name));
                    idx=NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*IDENTI(GLBLIDENT(ASSIGN(expr->node)->name->node)->name->node)->name));
                }
                add_instruction(compiler->instructions,STORE_GLOBAL, idx, expr->start, expr->end);
            }
            break;
        }

        case N_MULTIIDENT: {
            uint32_t idx;
            for (string* s: (*MULTIIDENT(expr->node)->name) ){
                if (!_list_contains(compiler->names, s)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*s));
                    idx=NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*s));
                }

                switch (compiler->scope){
                    case SCOPE_GLOBAL:
                        add_instruction(compiler->instructions,STORE_GLOBAL, idx, expr->start, expr->end);
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler->instructions,STORE_NAME, idx, expr->start, expr->end);
                        break;
                }
                
                add_instruction(compiler->instructions,POP_TOS, 0, expr->start, expr->end);
            }
            break;
        }

        case N_BINOP: {
            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            int cmpexpr=compile_expr(compiler, BINOP(expr->node)->left); //Push data
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            cmpexpr=compile_expr(compiler, BINOP(expr->node)->right); //Push data
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (!ret){
                compiler->keep_return=false;
            }
            
            switch (BINOP(expr->node)->opr){
                case T_PLUS:
                    add_instruction(compiler->instructions,BINOP_ADD,0, expr->start, expr->end);
                    break;
                case T_MINUS:
                    add_instruction(compiler->instructions,BINOP_SUB,0, expr->start, expr->end);
                    break;
                case T_MUL:
                    add_instruction(compiler->instructions,BINOP_MUL,0, expr->start, expr->end);
                    break;
                case T_DIV:
                    add_instruction(compiler->instructions,BINOP_DIV,0, expr->start, expr->end);
                    break;
                case T_IS:
                    add_instruction(compiler->instructions,BINOP_IS,0, expr->start, expr->end);
                    break;
                case T_EE:
                    add_instruction(compiler->instructions,BINOP_EE,0, expr->start, expr->end);
                    break;
                case T_NE:
                    add_instruction(compiler->instructions,BINOP_NE,0, expr->start, expr->end);
                    break;
                case T_GT:
                    add_instruction(compiler->instructions,BINOP_GT,0, expr->start, expr->end);
                    break;
                case T_GTE:
                    add_instruction(compiler->instructions,BINOP_GTE,0, expr->start, expr->end);
                    break;
                case T_LT:
                    add_instruction(compiler->instructions,BINOP_LT,0, expr->start, expr->end);
                    break;
                case T_LTE:
                    add_instruction(compiler->instructions,BINOP_LTE,0, expr->start, expr->end);
                    break;
                case T_PERCENT:
                    add_instruction(compiler->instructions,BINOP_MOD,0, expr->start, expr->end);
                    break;
                case T_POW:
                    add_instruction(compiler->instructions,BINOP_POW,0, expr->start, expr->end);
                    break;
                case T_AND:
                    add_instruction(compiler->instructions,BINOP_AND,0, expr->start, expr->end);
                    break;
                case T_OR:
                    add_instruction(compiler->instructions,BINOP_OR,0, expr->start, expr->end);
                    break;
                case T_AMPERSAND:
                    add_instruction(compiler->instructions,BITWISE_AND,0, expr->start, expr->end);
                    break;
                case T_VBAR:
                    add_instruction(compiler->instructions,BITWISE_OR,0, expr->start, expr->end);
                    break;
                case T_LSHIFT:
                    add_instruction(compiler->instructions,BITWISE_LSHIFT,0, expr->start, expr->end);
                    break;
                case T_RSHIFT:
                    add_instruction(compiler->instructions,BITWISE_RSHIFT,0, expr->start, expr->end);
                    break;
                case T_NOT:
                    add_instruction(compiler->instructions,BINOP_NOTIN,0, expr->start, expr->end);
                    break;
                case T_IN:
                    add_instruction(compiler->instructions,BINOP_IN,0, expr->start, expr->end);
                    break;
                case T_ISNOT:
                    add_instruction(compiler->instructions,BINOP_ISNOT,0, expr->start, expr->end);
                    break;
                case T_IADD: {
                    uint32_t idx;
                    if (BINOP(expr->node)->left->type!=N_IDENT){
                        parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected identifier for left expression");
                        return 0x100;
                    }
                    if (!_list_contains(compiler->names, IDENTI(BINOP(expr->node)->left->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                    }
                    add_instruction(compiler->instructions,BINOP_IADD,idx, expr->start, expr->end);
                    break;
                }
                case T_ISUB: {
                    uint32_t idx;
                    if (BINOP(expr->node)->left->type!=N_IDENT){
                        parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected identifier for left expression");
                        return 0x100;
                    }
                    if (!_list_contains(compiler->names, IDENTI(BINOP(expr->node)->left->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                    }
                    add_instruction(compiler->instructions,BINOP_ISUB,0, expr->start, expr->end);
                    break;
                }
                case T_IMUL: {
                    uint32_t idx;
                    if (BINOP(expr->node)->left->type!=N_IDENT){
                        parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected identifier for left expression");
                        return 0x100;
                    }
                    if (!_list_contains(compiler->names, IDENTI(BINOP(expr->node)->left->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                    }
                    add_instruction(compiler->instructions,BINOP_IMUL,0, expr->start, expr->end);
                    break;
                }
                case T_IDIV: {
                    uint32_t idx;
                    if (BINOP(expr->node)->left->type!=N_IDENT){
                        parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected identifier for left expression");
                        return 0x100;
                    }
                    if (!_list_contains(compiler->names, IDENTI(BINOP(expr->node)->left->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                    }
                    add_instruction(compiler->instructions,BINOP_IDIV,idx, expr->start, expr->end);
                    break;
                }
                case T_IPOW: {
                    uint32_t idx;
                    if (BINOP(expr->node)->left->type!=N_IDENT){
                        parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected identifier for left expression");
                        return 0x100;
                    }
                    if (!_list_contains(compiler->names, IDENTI(BINOP(expr->node)->left->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                    }
                    add_instruction(compiler->instructions,BINOP_IPOW,idx, expr->start, expr->end);
                    break;
                }
                case T_IMOD: {
                    uint32_t idx;
                    if (BINOP(expr->node)->left->type!=N_IDENT){
                        parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected identifier for left expression");
                        return 0x100;
                    }
                    if (!_list_contains(compiler->names, IDENTI(BINOP(expr->node)->left->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                    }
                    add_instruction(compiler->instructions,BINOP_IMOD,idx, expr->start, expr->end);
                    break;
                }

                
                case T_IAMP: {
                    uint32_t idx;
                    if (BINOP(expr->node)->left->type!=N_IDENT){
                        parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected identifier for left expression");
                        return 0x100;
                    }
                    if (!_list_contains(compiler->names, IDENTI(BINOP(expr->node)->left->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                    }
                    add_instruction(compiler->instructions,BINOP_IAND,idx, expr->start, expr->end);
                    break;
                }
                case T_IVBAR: {
                    uint32_t idx;
                    if (BINOP(expr->node)->left->type!=N_IDENT){
                        parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected identifier for left expression");
                        return 0x100;
                    }
                    if (!_list_contains(compiler->names, IDENTI(BINOP(expr->node)->left->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                    }
                    add_instruction(compiler->instructions,BINOP_IOR,idx, expr->start, expr->end);
                    break;
                }
                case T_ILSH: {
                    uint32_t idx;
                    if (BINOP(expr->node)->left->type!=N_IDENT){
                        parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected identifier for left expression");
                        return 0x100;
                    }
                    if (!_list_contains(compiler->names, IDENTI(BINOP(expr->node)->left->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                    }
                    add_instruction(compiler->instructions,BINOP_ILSH,idx, expr->start, expr->end);
                    break;
                }
                case T_IRSH: {
                    uint32_t idx;
                    if (BINOP(expr->node)->left->type!=N_IDENT){
                        parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected identifier for left expression");
                        return 0x100;
                    }
                    if (!_list_contains(compiler->names, IDENTI(BINOP(expr->node)->left->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(BINOP(expr->node)->left->node)->name));
                    }
                    add_instruction(compiler->instructions,BINOP_IRSH,idx, expr->start, expr->end);
                    break;
                }
            }
            break;
        }

        case N_UNARY: {
            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            int cmpexpr=compile_expr(compiler, UNARYOP(expr->node)->right); //Push data
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (!ret){
                compiler->keep_return=false;
            }
            
            switch (UNARYOP(expr->node)->opr){
                case T_MINUS:
                    add_instruction(compiler->instructions,UNARY_NEG,0, expr->start, expr->end);
                    break; 
                case T_NOT:
                    add_instruction(compiler->instructions,UNARY_NOT,0, expr->start, expr->end);
                    break;
                case T_TILDE:
                    add_instruction(compiler->instructions,BITWISE_NOT,0, expr->start, expr->end);
                    break;
                case T_PLUS:
                    break;
            }
            break;
        }

        case N_INT: {
            uint32_t idx;
            if (!_list_contains(compiler->consts, INTLIT(expr->node)->literal)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, new_int_fromstr(INTLIT(expr->node)->literal));
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, new_int_fromstr(INTLIT(expr->node)->literal));
            }
            
            add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
            break;
        }

        case N_FLOAT: {
            uint32_t idx;
            if (!_list_contains(compiler->consts, FLOATLIT(expr->node)->literal)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, new_float_fromstr(FLOATLIT(expr->node)->literal));
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, new_float_fromstr(FLOATLIT(expr->node)->literal));
            }
            
            add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
            break;
        }

        case N_STR: {
            uint32_t idx;
            if (!_list_contains(compiler->consts, STRLIT(expr->node)->literal)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*STRLIT(expr->node)->literal));
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, str_new_fromstr(*STRLIT(expr->node)->literal));
            }
            add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);
            break;     
        }
        
        case N_IDENT: {
            uint32_t idx;
            if (!_list_contains(compiler->names, IDENTI(expr->node)->name)){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(expr->node)->name));
                idx = NAMEIDX(compiler->names);
            }
            else{
                idx=object_find(compiler->names, str_new_fromstr(*IDENTI(expr->node)->name));
            }
            switch (compiler->scope){
                case SCOPE_GLOBAL:
                    add_instruction(compiler->instructions,LOAD_GLOBAL, idx, expr->start, expr->end);
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler->instructions,LOAD_NAME, idx, expr->start, expr->end);
                    break;
            }
            break;
        }
        
        case N_FUNC: {
            //Name
            uint32_t nameidx;
            
            if (!_list_contains(compiler->consts, IDENTI(FUNCT(expr->node)->name->node)->name )){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(expr->node)->name->node)->name));
                nameidx = NAMEIDX(compiler->consts);
            }
            else{
                nameidx=object_find(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(expr->node)->name->node)->name));
            }
            
            
            add_instruction(compiler->instructions,LOAD_CONST, nameidx, expr->start, expr->end);

            //Arguments
            size_t argc=0;

            object* args=new_tuple();
            object* kwargs=new_tuple();

            //Setup args
            for (Node* n: (*FUNCT(expr->node)->args)){
                argc++;
                args->type->slot_mappings->slot_append(args, str_new_fromstr(*IDENTI(n->node)->name));
            }
            
            //
            
            //Setup kwargs (code object)
            for (Node* n: (*FUNCT(expr->node)->kwargs)){
                argc++;
                args->type->slot_mappings->slot_append(args, str_new_fromstr(*IDENTI(ASSIGN(n->node)->name->node)->name));
                int cmpexpr=compile_expr(compiler, ASSIGN(n->node)->right);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            //

            add_instruction(compiler->instructions,BUILD_TUPLE, FUNCT(expr->node)->kwargs->size(), expr->start, expr->end);
            
            uint32_t idx;
            if (!object_find_bool(compiler->consts, args)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, args);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, args);
            }
            add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);
            

            //

            //Code
            parse_ret c;
            c.nodes=(*FUNCT(expr->node)->code);
            struct compiler* comp=new_compiler();
            comp->scope=SCOPE_LOCAL;
            struct compiler* compiler_=compiler;
            compiler=comp;
            object* code=compile(comp, c, expr->start->line);
            compiler=compiler_;
            
            object* lines=new_list();
            
            object* tuple=new_tuple();
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(0));
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(0));
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(expr->start->line));
            
            lines->type->slot_mappings->slot_append(lines, tuple);
            for (int i=0; i<CAST_LIST(CAST_CODE(code)->co_lines)->size; i++){
                lines->type->slot_mappings->slot_append(lines, list_index_int(CAST_CODE(code)->co_lines, i));
            }
            DECREF(CAST_CODE(code)->co_lines);
            CAST_CODE(code)->co_lines=lines; //No incref necessary

            DECREF(CAST_CODE(code)->co_file);
            CAST_CODE(code)->co_file=object_repr(str_new_fromstr(*IDENTI(FUNCT(expr->node)->name->node)->name));
            
            
            if (!object_find_bool(compiler->consts, code)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, code);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, code);
            }
            add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);

            //Create callable
            if (compiler->scope!=SCOPE_GLOBAL){
                if (FUNCT(expr->node)->type==FUNCTION_NORMAL || FUNCT(expr->node)->type==FUNCTION_LAMBDA){
                    add_instruction(compiler->instructions,MAKE_CLOSURE, argc, expr->start, expr->end);
                }
                if (FUNCT(expr->node)->type==FUNCTION_STATIC){
                    add_instruction(compiler->instructions,MAKE_STATICMETH, argc, expr->start, expr->end);
                }
                if (FUNCT(expr->node)->type==FUNCTION_CLASS){
                    add_instruction(compiler->instructions,MAKE_CLASSMETH, argc, expr->start, expr->end);
                }
            }
            else{
                add_instruction(compiler->instructions,MAKE_FUNCTION, argc, expr->start, expr->end);
            }

            if (FUNCT(expr->node)->type!=FUNCTION_LAMBDA){
                //Store function
                uint32_t nameidxstore;
                if (!_list_contains(compiler->names, IDENTI(FUNCT(expr->node)->name->node)->name )){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(FUNCT(expr->node)->name->node)->name));
                    nameidxstore = NAMEIDX(compiler->names);
                }
                else{
                    nameidxstore=object_find(compiler->names, str_new_fromstr(*IDENTI(FUNCT(expr->node)->name->node)->name));
                }
                switch (compiler->scope){
                    case SCOPE_GLOBAL:
                        add_instruction(compiler->instructions,STORE_GLOBAL, nameidxstore, expr->start, expr->end);
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler->instructions,STORE_NAME, nameidxstore, expr->start, expr->end);
                        break;
                }
            }
            break;
        }

        case N_CALL:  {
            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            //Args (iterate backwards)
            for (auto it =  (*CALL(expr->node)->args).rbegin(); it != (*CALL(expr->node)->args).rend(); ++it){
                int cmpexpr=compile_expr(compiler, *it);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            //Kwargs (iterate backwards)
            for (auto it =  (*CALL(expr->node)->kwargs).rbegin(); it != (*CALL(expr->node)->kwargs).rend(); ++it){
                uint32_t idx;
                if (!_list_contains(compiler->consts, IDENTI(ASSIGN((*it)->node)->name->node)->name)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(ASSIGN((*it)->node)->name->node)->name));
                    idx = NAMEIDX(compiler->consts);
                }
                else{
                    idx=object_find(compiler->consts, str_new_fromstr(*IDENTI(ASSIGN((*it)->node)->name->node)->name));
                }
                add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);
                int cmpexpr=compile_expr(compiler, ASSIGN((*it)->node)->right);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            if (!ret){
                compiler->keep_return=false;
            }
            
            //Num of pos args
            uint32_t idx;
            object* size=new_int_fromint(CALL(expr->node)->args->size());
            uint32_t argc=CALL(expr->node)->args->size()+CALL(expr->node)->kwargs->size();
            
            if (!object_find_bool(compiler->consts,size)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, size);
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, size);
            }
            
            add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
            //Object
            int cmpexpr=compile_expr(compiler, CALL(expr->node)->object);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            
            add_instruction(compiler->instructions,CALL_FUNCTION, argc, expr->start, expr->end);

            if (!compiler->keep_return){
                add_instruction(compiler->instructions,POP_TOS, 0, expr->start, expr->end);
            }
            break;
        }

        case N_TRUE: {
            uint32_t idx;
            if (!object_find_bool(compiler->consts,trueobj)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, new_bool_true());
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, trueobj);
            }
            
            add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
            break;
        }

        case N_FALSE: {
            uint32_t idx;
            if (!object_find_bool(compiler->consts,falseobj)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, new_bool_false());
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, new_bool_false());
            }
            
            add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
            break;
        }

        case N_NONE: {
            uint32_t idx;
            if (!object_find_bool(compiler->consts,noneobj)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, noneobj);
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, noneobj);
            }
            
            add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
            break;
        }

        case N_CLASS: {
            add_instruction(compiler->instructions,LOAD_BUILD_CLASS, 0, expr->start, expr->end);
            add_instruction(compiler->instructions,LOAD_REGISTER_POP, 0, expr->start, expr->end);

            //Name
            uint32_t nameidx;
            if (!_list_contains(compiler->consts, IDENTI(CLASS(expr->node)->name->node)->name )){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(CLASS(expr->node)->name->node)->name));
                nameidx = NAMEIDX(compiler->consts);
            }
            else{
                nameidx=object_find(compiler->consts, str_new_fromstr(*IDENTI(CLASS(expr->node)->name->node)->name));
            }
            add_instruction(compiler->instructions,LOAD_CONST, nameidx, expr->start, expr->end);

            object* args=new_tuple();
            
            uint32_t idx;
            if (!object_find_bool(compiler->consts, args)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, args);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, args);
            }
            add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end); //Faux args
            add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end); //Same but for kwargs

            //Code
            parse_ret c;
            c.nodes=(*CLASS(expr->node)->code);
            struct compiler* comp=new_compiler();
            comp->scope=SCOPE_LOCAL;
            struct compiler* compiler_=compiler;
            compiler=comp;
            object* code=compile(comp, c, expr->start->line);
            compiler=compiler_;

            DECREF(CAST_CODE(code)->co_file);
            CAST_CODE(code)->co_file=object_repr(str_new_fromstr(*IDENTI(FUNCT(expr->node)->name->node)->name));
            
            if (!object_find_bool(compiler->consts, code)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, code);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, code);
            }
            add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);

            add_instruction(compiler->instructions,MAKE_FUNCTION, 0, expr->start, expr->end);            
        
            add_instruction(compiler->instructions,LOAD_CONST, nameidx, expr->start, expr->end);
            

            uint32_t nbases=CLASS(expr->node)->bases->size();
            for (Node* n: *CLASS(expr->node)->bases){
                int cmpexpr=compile_expr(compiler, n);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            add_instruction(compiler->instructions,BUILD_LIST, nbases, expr->start, expr->end);


            object* i=new_int_fromint(3);
            if (!object_find_bool(compiler->consts, i)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, i);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, i);
            }
            add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);

            add_instruction(compiler->instructions,READ_REGISTER_PUSH, 0, expr->start, expr->end);
            
            add_instruction(compiler->instructions,CALL_FUNCTION, 3, expr->start, expr->end);

            //Store class
            uint32_t nameidxstore;
            if (!_list_contains(compiler->names, IDENTI(FUNCT(expr->node)->name->node)->name )){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(FUNCT(expr->node)->name->node)->name));
                nameidxstore = NAMEIDX(compiler->names);
            }
            else{
                nameidxstore=object_find(compiler->names, str_new_fromstr(*IDENTI(FUNCT(expr->node)->name->node)->name));
            }
            switch (compiler->scope){
                case SCOPE_GLOBAL:
                    add_instruction(compiler->instructions,STORE_GLOBAL, nameidxstore, expr->start, expr->end);
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler->instructions,STORE_NAME, nameidxstore, expr->start, expr->end);
                    break;
            }

            break;
        }

        case N_DOT: {
            vector<Node*>* names=DOT(expr->node)->names;
            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            compile_expr(compiler, names->at(0));
            compiler->keep_return=ret;

            for (size_t i=1; i<names->size(); i++){
                uint32_t idx;
                if (!_list_contains(compiler->names, IDENTI(names->at(i)->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                    idx = NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                }

                add_instruction(compiler->instructions,LOAD_ATTR, idx, expr->start, expr->end);
            }
            break;
        }

        case N_DOTASSIGN: {
            int cmpexpr=compile_expr(compiler, DOTASSIGN(expr->node)->right);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            vector<Node*>* names=DOT(DOTASSIGN(expr->node)->dot->node)->names;
            
            compile_expr(compiler, names->at(0));
            
            for (size_t i=1; i<names->size(); i++){
                uint32_t idx;
                if (!_list_contains(compiler->names, IDENTI(names->at(i)->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                    idx = NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                }
                
                if (i==names->size()-1){
                    add_instruction(compiler->instructions,STORE_ATTR, idx, expr->start, expr->end);
                    continue;
                }     
                add_instruction(compiler->instructions,LOAD_ATTR, idx, expr->start, expr->end);
            }
            if (!ret){
                compiler->keep_return=false;
            }
            break;
        }        

        case N_DOTCALL: {
            vector<Node*>* names=DOT(DOTCALL(expr->node)->dot->node)->names;
            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            compile_expr(compiler, names->at(0));
            compiler->keep_return=ret;
            
            for (size_t i=1; i<names->size(); i++){
                uint32_t idx;
                if (!_list_contains(compiler->names, IDENTI(names->at(i)->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                    idx = NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                }
                
                if (i==0){
                    switch (compiler->scope){
                        case SCOPE_GLOBAL:
                            add_instruction(compiler->instructions,LOAD_GLOBAL, idx, expr->start, expr->end);
                            break;

                        case SCOPE_LOCAL:
                            add_instruction(compiler->instructions,LOAD_NAME, idx, expr->start, expr->end);
                            break;
                    }
                    continue;
                }            
                if (i==names->size()-1){
                    add_instruction(compiler->instructions,LOAD_REGISTER_POP, 0, expr->start, expr->end);
                    bool ret=compiler->keep_return;
                    compiler->keep_return=true;
                    //Args (iterate backwards)
                    for (auto it =  (*DOTCALL(expr->node)->args).rbegin(); it != (*DOTCALL(expr->node)->args).rend(); ++it){
                        int cmpexpr=compile_expr(compiler, *it);
                        if (cmpexpr==0x100){
                            return cmpexpr;
                        }
                    }
                    //Kwargs (iterate backwards)
                    for (auto it =  (*DOTCALL(expr->node)->kwargs).rbegin(); it != (*DOTCALL(expr->node)->kwargs).rend(); ++it){
                        uint32_t idx;
                        if (!_list_contains(compiler->consts, IDENTI(ASSIGN((*it)->node)->name->node)->name)){
                            //Create object
                            compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(ASSIGN((*it)->node)->name->node)->name));
                            idx = NAMEIDX(compiler->consts);
                        }
                        else{
                            idx=object_find(compiler->consts, str_new_fromstr(*IDENTI(ASSIGN((*it)->node)->name->node)->name));
                        }
                        add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);
                        int cmpexpr=compile_expr(compiler, ASSIGN((*it)->node)->right);
                        if (cmpexpr==0x100){
                            return cmpexpr;
                        }
                    }
                    if (!ret){
                        compiler->keep_return=false;
                    }

                    //Num of pos args
                    uint32_t idx;
                    object* size=new_int_fromint(DOTCALL(expr->node)->args->size());
                    uint32_t argc=DOTCALL(expr->node)->args->size()+DOTCALL(expr->node)->kwargs->size();

                    if (!object_find_bool(compiler->consts,size)){
                        //Create object
                        compiler->consts->type->slot_mappings->slot_append(compiler->consts, size);
                        idx=NAMEIDX(compiler->consts);
                    }
                    else{
                        idx=object_find(compiler->consts, size);
                    }
                    
                    add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);

                    add_instruction(compiler->instructions,READ_REGISTER_PUSH, 0, expr->start, expr->end);
                    add_instruction(compiler->instructions,READ_REGISTER_PUSH, 0, expr->start, expr->end);
                    
                    //Object
                    if (!_list_contains(compiler->names, IDENTI(names->at(i)->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                    }


                    add_instruction(compiler->instructions,LOAD_ATTR, idx, expr->start, expr->end);

                    add_instruction(compiler->instructions,CALL_METHOD, argc, expr->start, expr->end);

                    if (!compiler->keep_return){
                        add_instruction(compiler->instructions,POP_TOS, 0, expr->start, expr->end);
                    }
                    continue;
                }     
                add_instruction(compiler->instructions,LOAD_ATTR, idx, expr->start, expr->end);
            }
            break;
        }        

        case N_RETURN: {
            if (compiler->scope==SCOPE_GLOBAL){
                parser.add_parsing_error(&parseretglbl, "SyntaxError: Return outside function");
                return 0x100;
            }

            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            int cmpexpr=compile_expr(compiler, RETURN(expr->node)->node);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            compiler->keep_return=ret;
            
            add_instruction(compiler->instructions, RETURN_VAL, 0, expr->start, expr->end);
            break;
        }

        case N_LIST: {
            for (size_t i=LIST(expr->node)->list->size(); i>0; i--){
                int cmpexpr=compile_expr(compiler, LIST(expr->node)->list->at(i-1));
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            add_instruction(compiler->instructions, BUILD_LIST, LIST(expr->node)->list->size(), new Position, new Position);
            break;
        }

        case N_TUPLE: {
            for (size_t i=LIST(expr->node)->list->size(); i>0; i--){
                int cmpexpr=compile_expr(compiler, LIST(expr->node)->list->at(i-1));
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            add_instruction(compiler->instructions, BUILD_TUPLE, LIST(expr->node)->list->size(), new Position, new Position);
            break;
        }

        case N_DICT: {
            for (size_t i=DICT(expr->node)->keys->size(); i>0; i--){
                int cmpexpr=compile_expr(compiler, DICT(expr->node)->vals->at(i-1)); //Value
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                cmpexpr=compile_expr(compiler, DICT(expr->node)->keys->at(i-1)); //Value
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            add_instruction(compiler->instructions, BUILD_DICT, DICT(expr->node)->keys->size(), new Position, new Position);
            break;
        }

        case N_IF: {
            int cmpexpr=compile_expr(compiler, IF(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }

            add_instruction(compiler->instructions,POP_JMP_TOS_FALSE, num_instructions(IF(expr->node)->code, compiler->scope)*2, expr->start, expr->end); 

            //Code
            for (Node* n: (*IF(expr->node)->code)){
                uint32_t start=compiler->instructions->count;
                long line=n->start->line;
                int i=compile_expr(compiler, n);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }
            }

            add_instruction(compiler->instructions,JUMP_DELTA,0, expr->start, expr->end);
            
            break;
        }

        case N_ELSE : {
            if (ELSE(expr->node)->base!=NULL){
                int cmpexpr=compile_expr(compiler, ELSE(expr->node)->base); //Compile first options
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }

            //Code
            for (Node* n: (*ELSE(expr->node)->code)){
                uint32_t start=compiler->instructions->count;
                long line=n->start->line;
                int i=compile_expr(compiler, n);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }
            }   
            break;
        }

        case N_CONTROL : {
            uint32_t target=num_instructions(CONTROL(expr->node)->bases, compiler->scope)*2;
            uint32_t instrs=0;
            for (Node* n: (*CONTROL(expr->node)->bases)){
                if (n->type==N_IF){
                    instrs+=num_instructions(IF(n->node)->code, compiler->scope)*2;
                    instrs+=num_instructions(IF(n->node)->expr, compiler->scope)*2;
                    
                    int cmpexpr=compile_expr(compiler, IF(n->node)->expr);
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    add_instruction(compiler->instructions,POP_JMP_TOS_FALSE, num_instructions(IF(n->node)->code, compiler->scope)*2+2, n->start, n->end);

                    //Code
                    for (Node* n: (*IF(n->node)->code)){
                        uint32_t start=compiler->instructions->count;
                        long line=n->start->line;
                        int i=compile_expr(compiler, n);
                        if (i==0x100){
                            return 0x100;
                        }
                        uint32_t end=compiler->instructions->count;
                        if (compiler->lines!=NULL && i!=0x200){
                            object* tuple=new_tuple();
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                            compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                        }
                    }
                    add_instruction(compiler->instructions,JUMP_DELTA,target-instrs, n->start, n->end);
                }
                if (n->type==N_ELSE){
                    instrs+=num_instructions(ELSE(n->node)->code, compiler->scope)*2;
                    
                    //Code
                    for (Node* n: (*ELSE(n->node)->code)){
                        uint32_t start=compiler->instructions->count;
                        long line=n->start->line;
                        int i=compile_expr(compiler, n);
                        if (i==0x100){
                            return 0x100;
                        }
                        uint32_t end=compiler->instructions->count;
                        if (compiler->lines!=NULL && i!=0x200){
                            object* tuple=new_tuple();
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                            compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                        }
                    }   
                }
            }
            break;
        }

        case N_SUBSCR: {
            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            if (!ret){
                compiler->keep_return=false;
            }
            compiler->keep_return=ret;
            int cmpexpr=compile_expr(compiler, SUBSCR(expr->node)->left);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            cmpexpr=compile_expr(compiler, SUBSCR(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            add_instruction(compiler->instructions,BINOP_SUBSCR, 0, expr->start, expr->end);
            

            if (!compiler->keep_return){
                add_instruction(compiler->instructions,POP_TOS, 0, expr->start, expr->end);
            }
            break;
        }

        case N_RAISE: {
            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            int cmpexpr=compile_expr(compiler, RAISE(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (!ret){
                compiler->keep_return=false;
            }
            compiler->keep_return=ret;
            add_instruction(compiler->instructions, RAISE_EXC, 0, expr->start, expr->end);
            break;
        }

        case N_STORE_SUBSCR: {
            int cmpexpr=compile_expr(compiler, SUBSCR(STSUBSCR(expr->node)->left->node)->left);
            if (cmpexpr==0x100){
                return cmpexpr;
            }

            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            cmpexpr=compile_expr(compiler, SUBSCR(STSUBSCR(expr->node)->left->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            cmpexpr=compile_expr(compiler, STSUBSCR(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (!ret){
                compiler->keep_return=false;
            }
            compiler->keep_return=ret;
            add_instruction(compiler->instructions,STORE_SUBSCR, 0, expr->start, expr->end);

            

            if (!compiler->keep_return){
                add_instruction(compiler->instructions,POP_TOS, 0, expr->start, expr->end);
            }
            break;
        }

        case N_TRY: {
            for (Node* n: (*TRY(expr->node)->code)){
                uint32_t start=compiler->instructions->count;
                long line=n->start->line;
                int i=compile_expr(compiler, n);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }
            }
            break;
        }

        case N_FINALLY: {
            for (Node* n: (*FINALLY(expr->node)->code)){
                uint32_t start=compiler->instructions->count;
                long line=n->start->line;        
                int i=compile_expr(compiler, n);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }
            }
            break;
        }

        case N_EXCEPT: {
            if (EXCEPT(expr->node)->name!=NULL){
                uint32_t idx;
                if (!_list_contains(compiler->names, STRLIT(EXCEPT(expr->node)->name->node)->literal)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*STRLIT(EXCEPT(expr->node)->name->node)->literal));
                    idx = NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*STRLIT(EXCEPT(expr->node)->name->node)->literal));
                }

                uint32_t start=compiler->instructions->count;
                
                switch (compiler->scope){
                    case SCOPE_GLOBAL:
                        add_instruction(compiler->instructions,STORE_GLOBAL, idx, expr->start, expr->end);
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler->instructions,STORE_NAME, idx, expr->start, expr->end);
                        break;
                }

                uint32_t end=compiler->instructions->count;
                if (compiler->lines!=NULL){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(EXCEPT(expr->node)->name->start->line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }
            }    

            if (EXCEPT(expr->node)->type!=NULL){
                    add_instruction(compiler->instructions,DUP_TOS,0, expr->start, expr->end); //For possible pop_jump

                    //Checks here
                    uint32_t idx;
                    if (!_list_contains(compiler->names, STRLIT(EXCEPT(expr->node)->type->node)->literal)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*STRLIT(EXCEPT(expr->node)->type->node)->literal));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*STRLIT(EXCEPT(expr->node)->type->node)->literal));
                    }
                    switch (compiler->scope){
                        case SCOPE_GLOBAL:
                            add_instruction(compiler->instructions,LOAD_GLOBAL, idx, expr->start, expr->end);
                            break;

                        case SCOPE_LOCAL:
                            add_instruction(compiler->instructions,LOAD_NAME, idx, expr->start, expr->end);
                            break;
                    }

                    add_instruction(compiler->instructions,BINOP_EE,0, expr->start, expr->end);                

                    add_instruction(compiler->instructions,POP_JMP_TOS_FALSE,0, expr->start, expr->end);

                    add_instruction(compiler->instructions,POP_TOS,0, expr->start, expr->end);
                }

            for (Node* n: (*EXCEPT(expr->node)->code)){
                uint32_t start=compiler->instructions->count;
                long line=n->start->line;        
                int i=compile_expr(compiler, n);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }
            }

            break;
        }

        case N_TRY_EXCEPT_FINALLY: {
            uint32_t val=compiler->instructions->count*2+num_instructions(TRY(TRYEXCEPTFINALLY(expr->node)->bases->at(0)->node)->code, compiler->scope)*2;
            add_instruction(compiler->instructions,SETUP_TRY,val, expr->start, expr->end);       
                 
            uint32_t target;
            if (TRYEXCEPTFINALLY(expr->node)->bases->back()->type==N_FINALLY){
                target=(num_instructions(TRYEXCEPTFINALLY(expr->node)->bases, compiler->scope)*2)-(num_instructions(TRYEXCEPTFINALLY(expr->node)->bases->back(), compiler->scope)*2);
            }
            else{
                target=num_instructions(TRYEXCEPTFINALLY(expr->node)->bases, compiler->scope)*2;
            }

            uint32_t instrs=0;
            for (uint32_t i=0; i<TRYEXCEPTFINALLY(expr->node)->bases->size(); i++){
                if (i==0){ //Add try block
                    Node* tryn=TRYEXCEPTFINALLY(expr->node)->bases->at(0);
                    instrs+=num_instructions(TRY(tryn->node)->code, compiler->scope)*2;
                    
                    for (Node* n: (*TRY(tryn->node)->code)){
                        uint32_t start=compiler->instructions->count;
                        long line=n->start->line;
                        int i=compile_expr(compiler, n);
                        if (i==0x100){
                            return 0x100;
                        }
                        uint32_t end=compiler->instructions->count;
                        if (compiler->lines!=NULL && i!=0x200){
                            object* tuple=new_tuple();
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                            compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                        }
                    }

                    add_instruction(compiler->instructions,JUMP_DELTA,target-instrs, tryn->start, tryn->end);
                    continue;
                }
                else if (i==TRYEXCEPTFINALLY(expr->node)->bases->size()-1 && TRYEXCEPTFINALLY(expr->node)->bases->at(i)->type==N_FINALLY){                    
                    add_instruction(compiler->instructions,RAISE_EXC,0, expr->start, expr->end); 
                    instrs+=2;
                    
                    Node* tryn=TRYEXCEPTFINALLY(expr->node)->bases->at(i);
                    instrs+=num_instructions(FINALLY(tryn->node)->code, compiler->scope)*2;
                    
                    for (Node* n: (*FINALLY(tryn->node)->code)){
                        uint32_t start=compiler->instructions->count;
                        long line=n->start->line;
                        int i=compile_expr(compiler, n);
                        if (i==0x100){
                            return 0x100;
                        }
                        uint32_t end=compiler->instructions->count;
                        if (compiler->lines!=NULL && i!=0x200){
                            object* tuple=new_tuple();
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                            compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                        }
                    }
                    continue;
                }
                Node* tryn=TRYEXCEPTFINALLY(expr->node)->bases->at(i);
                instrs+=num_instructions(EXCEPT(tryn->node)->code, compiler->scope)*2;
                if (EXCEPT(tryn->node)->name!=NULL){
                    uint32_t idx;
                    if (!_list_contains(compiler->names, STRLIT(EXCEPT(tryn->node)->name->node)->literal)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*STRLIT(EXCEPT(tryn->node)->name->node)->literal));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*STRLIT(EXCEPT(tryn->node)->name->node)->literal));
                    }

                    uint32_t start=compiler->instructions->count;
                    
                    switch (compiler->scope){
                        case SCOPE_GLOBAL:
                            add_instruction(compiler->instructions,STORE_GLOBAL, idx, expr->start, expr->end);
                            break;

                        case SCOPE_LOCAL:
                            add_instruction(compiler->instructions,STORE_NAME, idx, expr->start, expr->end);
                            break;
                    }
                    instrs+=2;

                    uint32_t end=compiler->instructions->count;
                    if (compiler->lines!=NULL){
                        object* tuple=new_tuple();
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(EXCEPT(tryn->node)->name->start->line));
                        compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                    }
                }    

                if (EXCEPT(tryn->node)->type!=NULL){
                    add_instruction(compiler->instructions,DUP_TOS,0, expr->start, expr->end); //For possible pop_jump
                    instrs+=2;

                    //Checks here
                    uint32_t idx;
                    if (!_list_contains(compiler->names, STRLIT(EXCEPT(tryn->node)->type->node)->literal)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*STRLIT(EXCEPT(tryn->node)->type->node)->literal));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*STRLIT(EXCEPT(tryn->node)->type->node)->literal));
                    }

                    uint32_t start=compiler->instructions->count;
                    
                    switch (compiler->scope){
                        case SCOPE_GLOBAL:
                            add_instruction(compiler->instructions,LOAD_GLOBAL, idx, expr->start, expr->end);
                            break;

                        case SCOPE_LOCAL:
                            add_instruction(compiler->instructions,LOAD_NAME, idx, expr->start, expr->end);
                            break;
                    }
                    instrs+=2;

                    uint32_t end=compiler->instructions->count;
                    if (compiler->lines!=NULL){
                        object* tuple=new_tuple();
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(EXCEPT(tryn->node)->type->start->line));
                        compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                    }

                    add_instruction(compiler->instructions,BINOP_EXC_CMP,0, expr->start, expr->end);         
                    instrs+=2;

                    add_instruction(compiler->instructions,POP_JMP_TOS_FALSE,num_instructions(EXCEPT(tryn->node)->code, compiler->scope)*2+4, expr->start, expr->end);
                    instrs+=2;

                    add_instruction(compiler->instructions,POP_TOS,0, expr->start, expr->end);
                    instrs+=2;
                }
                
                for (Node* n: (*EXCEPT(tryn->node)->code)){
                    uint32_t start=compiler->instructions->count;
                    long line=n->start->line;
                    int i=compile_expr(compiler, n);
                    if (i==0x100){
                        return 0x100;
                    }
                    uint32_t end=compiler->instructions->count;
                    if (compiler->lines!=NULL && i!=0x200){
                        object* tuple=new_tuple();
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                        compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                    }
                }

                add_instruction(compiler->instructions,JUMP_DELTA,target-instrs+4, tryn->start, tryn->end);

            }
            if (TRYEXCEPTFINALLY(expr->node)->bases->back()->type!=N_FINALLY){
                add_instruction(compiler->instructions,RAISE_EXC,0, expr->start, expr->end); 
            }
            add_instruction(compiler->instructions,FINISH_TRY,0, expr->start, expr->end);      
            
            break;
        }

        case N_FOR: { 
            bool ret=compiler->keep_return;
            compiler->keep_return=true;

            int cmpexpr=compile_expr(compiler, FOR(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            
            if (!ret){
                compiler->keep_return=false;
            }
            
            add_instruction(compiler->instructions,EXTRACT_ITER,0, expr->start, expr->end);

            uint32_t start=compiler->instructions->count*2;
            if (FOR(expr->node)->ident->type!=N_MULTIIDENT){
                add_instruction(compiler->instructions,FOR_TOS_ITER,compiler->instructions->count*2+num_instructions(FOR(expr->node)->code, compiler->scope)*2+num_instructions(FOR(expr->node)->ident, compiler->scope)*2+6, expr->start, expr->end); 
            }
            else{
                add_instruction(compiler->instructions,FOR_TOS_ITER,compiler->instructions->count*2+num_instructions(FOR(expr->node)->code, compiler->scope)*2+8+num_instructions(FOR(expr->node)->ident, compiler->scope)*2, expr->start, expr->end); 
            }

            uint32_t idx;
            if (FOR(expr->node)->ident->type==N_IDENT){
                if (!_list_contains(compiler->names, IDENTI(FOR(expr->node)->ident->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(FOR(expr->node)->ident->node)->name));
                    idx=NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*IDENTI(FOR(expr->node)->ident->node)->name));
                }

                switch (compiler->scope){
                    case SCOPE_GLOBAL:
                        add_instruction(compiler->instructions,STORE_GLOBAL, idx, expr->start, expr->end);
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler->instructions,STORE_NAME, idx, expr->start, expr->end);
                        break;
                }
                add_instruction(compiler->instructions,POP_TOS, 0, expr->start, expr->end);
            }
            else if (FOR(expr->node)->ident->type==N_MULTIIDENT){
                add_instruction(compiler->instructions,UNPACK_SEQ, MULTIIDENT(FOR(expr->node)->ident->node)->name->size(), expr->start, expr->end);
                int cmpexpr=compile_expr(compiler, FOR(expr->node)->ident);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                add_instruction(compiler->instructions,POP_TOS, 0, expr->start, expr->end);
            }
            
            //Code
            for (Node* n: (*FOR(expr->node)->code)){
                uint32_t start=compiler->instructions->count;
                long line=n->start->line;
                int i=compile_expr(compiler, n);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }            
            }

            add_instruction(compiler->instructions,JUMP_TO,start, expr->start, expr->end); 
            return 0x200;
        }

        case N_BREAK: {
            add_instruction(compiler->instructions,BREAK_LOOP,0, expr->start, expr->end); 
            break;
        }

        case N_CONTINUE: {
            add_instruction(compiler->instructions,CONTINUE_LOOP,0 , expr->start, expr->end); 
            break;
        }

        case N_WHILE: {
            //Checks here
            uint32_t start=compiler->instructions->count*2;
            uint32_t start_=start;
            int cmpexpr=compile_expr(compiler, WHILE(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            add_instruction(compiler->instructions,POP_JMP_TOS_FALSE,num_instructions(WHILE(expr->node)->code, compiler->scope)*2+2, expr->start, expr->end); 
            
            size_t idx=0;
            //Code
            for (Node* n: (*WHILE(expr->node)->code)){
                uint32_t start=compiler->instructions->count;
                long line=n->start->line;
                int i=0;
                
                if (n->type==N_BREAK){
                    vector<Node*>* n_=new vector<Node*>(WHILE(expr->node)->code->begin()+idx, WHILE(expr->node)->code->end());
                    add_instruction(compiler->instructions,JUMP_DELTA,num_instructions(n_, compiler->scope)*2, expr->start, expr->end);
                    delete n_;
                }
                else if (n->type==N_CONTINUE){
                    add_instruction(compiler->instructions,JUMP_TO,start_, expr->start, expr->end);
                }
                else{
                    i=compile_expr(compiler, n);
                }
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }     
                idx++;       
            }
            
            add_instruction(compiler->instructions,JUMP_TO,start, expr->start, expr->end);
            
            return 0x200;
        }

        case N_IMPORT: {
            for (uint32_t i=0; i<IMPORT(expr->node)->libnames->size(); i++){
                Node* libname=IMPORT(expr->node)->libnames->at(i);
                Node* name=IMPORT(expr->node)->names->at(i);
                
                uint32_t idx;
                if (!_list_contains(compiler->names, IDENTI(libname->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(libname->node)->name));
                    idx=NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*IDENTI(libname->node)->name));
                }
                
                add_instruction(compiler->instructions, IMPORT_NAME, idx, expr->start, expr->end);

                if (name!=NULL){
                    if (!_list_contains(compiler->names, IDENTI(name->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(name->node)->name));
                        idx=NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(name->node)->name));
                    }
                }

                switch (compiler->scope){
                    case SCOPE_GLOBAL:
                        add_instruction(compiler->instructions,STORE_GLOBAL, idx, expr->start, expr->end);
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler->instructions,STORE_NAME, idx, expr->start, expr->end);
                        break;
                }
            }
            break;
        }

        case N_FROM: {
            uint32_t idx;
            if (!_list_contains(compiler->names, IDENTI(FROM(expr->node)->name->node)->name)){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(FROM(expr->node)->name->node)->name));
                idx=NAMEIDX(compiler->names);
            }
            else{
                idx=object_find(compiler->names, str_new_fromstr(*IDENTI(FROM(expr->node)->name->node)->name));
            }

            add_instruction(compiler->instructions, IMPORT_NAME, idx, expr->start, expr->end);
            

            object* names=new_list();
            for (uint32_t i=0; i<FROM(expr->node)->names->size(); i++){
                Node* name=FROM(expr->node)->names->at(i);
                if (name==NULL){
                    break;
                }
                
                list_append(names, str_new_fromstr(*IDENTI(name->node)->name));
            }
            
            if (!object_find_bool(compiler->consts, names)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, names);
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, names);
            }

            add_instruction(compiler->instructions, LOAD_CONST, idx, expr->start, expr->end);

            add_instruction(compiler->instructions, IMPORT_FROM_MOD, 0, expr->start, expr->end);
            break;
        }
        
        case N_SLICE: {
            Node* base=SUBSCR(SLICE(expr->node)->left->node)->left;
            Node* left=SUBSCR(SLICE(expr->node)->left->node)->expr;
            Node* right=SLICE(expr->node)->right;

            uint32_t type=0;
            if (left==NULL && right!=NULL){
                type=1;
            }
            else if (left!=NULL && right==NULL){
                type=2;
            }
            else if (left==NULL && right==NULL){
                type=4;
            }
            else{
                type=3;
            }

            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            if (!ret){
                compiler->keep_return=false;
            }
            compiler->keep_return=ret;


            int cmpexpr=compile_expr(compiler, base);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (type==3){
                cmpexpr=compile_expr(compiler, left);
                cmpexpr=compile_expr(compiler, right);
                add_instruction(compiler->instructions, MAKE_SLICE, 0, expr->start, expr->end);
            }
            else if (type==2){
                int cmpexpr=compile_expr(compiler, left);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                uint32_t idx;
                if (!object_find_bool(compiler->consts,noneobj)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, noneobj);
                    idx=NAMEIDX(compiler->consts);
                }
                else{
                    idx=object_find(compiler->consts, noneobj);
                }
                
                add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
                add_instruction(compiler->instructions, MAKE_SLICE, 0, expr->start, expr->end);
            }
            else if (type==1) {
                uint32_t idx;
                if (!object_find_bool(compiler->consts,noneobj)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, noneobj);
                    idx=NAMEIDX(compiler->consts);
                }
                else{
                    idx=object_find(compiler->consts, noneobj);
                }
                
                add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
                int cmpexpr=compile_expr(compiler, right);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                add_instruction(compiler->instructions, MAKE_SLICE, 0, expr->start, expr->end);
            }
            else{
                //Type 4 needs nothing
                break;
            }

            add_instruction(compiler->instructions, BINOP_SUBSCR, 0, expr->start, expr->end);
            

            if (!compiler->keep_return){
                add_instruction(compiler->instructions,POP_TOS, 0, expr->start, expr->end);
            }
            break;
        }

        case N_STORE_SLICE: {

            Node* base=SUBSCR(SLICE(STSLICE(expr->node)->left->node)->left->node)->left;
            Node* left=SUBSCR(SLICE(STSLICE(expr->node)->left->node)->left->node)->expr;
            Node* right=SLICE(STSLICE(expr->node)->left->node)->right;

            uint32_t type=0;
            if (left==NULL && right!=NULL){
                type=1;
            }
            else if (left!=NULL && right==NULL){
                type=2;
            }
            else if (left==NULL && right==NULL){
                type=4;
            }
            else{
                type=3;
            }

            bool ret=compiler->keep_return;
            compiler->keep_return=true;
            if (!ret){
                compiler->keep_return=false;
            }
            compiler->keep_return=ret;


            int cmpexpr=compile_expr(compiler, base);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (type==3){
                cmpexpr=compile_expr(compiler, left);
                cmpexpr=compile_expr(compiler, right);
                add_instruction(compiler->instructions, MAKE_SLICE, 0, expr->start, expr->end);
            }
            else if (type==2){
                int cmpexpr=compile_expr(compiler, left);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                uint32_t idx;
                if (!object_find_bool(compiler->consts,noneobj)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, noneobj);
                    idx=NAMEIDX(compiler->consts);
                }
                else{
                    idx=object_find(compiler->consts, noneobj);
                }
                
                add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
                add_instruction(compiler->instructions, MAKE_SLICE, 0, expr->start, expr->end);
            }
            else if (type==1) {
                uint32_t idx;
                if (!object_find_bool(compiler->consts,noneobj)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, noneobj);
                    idx=NAMEIDX(compiler->consts);
                }
                else{
                    idx=object_find(compiler->consts, noneobj);
                }
                
                add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
                int cmpexpr=compile_expr(compiler, right);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                add_instruction(compiler->instructions, MAKE_SLICE, 0, expr->start, expr->end);
            }
            else{
                //Type 4 needs nothing
                break;
            }
            
            cmpexpr=compile_expr(compiler, STSLICE(expr->node)->expr);
            if (!ret){
                compiler->keep_return=false;
            }
            compiler->keep_return=ret;
            add_instruction(compiler->instructions,STORE_SUBSCR, 0, expr->start, expr->end);

            

            if (!compiler->keep_return){
                add_instruction(compiler->instructions,POP_TOS, 0, expr->start, expr->end);
            }
            break;
        }

        case N_DEL: {
            if (DEL(expr->node)->expr->type==N_SUBSCR){
                Node* subexpr=DEL(expr->node)->expr;
                
                int cmpexpr=compile_expr(compiler, SUBSCR(subexpr->node)->left);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                cmpexpr=compile_expr(compiler, SUBSCR(subexpr->node)->expr);
                add_instruction(compiler->instructions,DEL_SUBSCR, 0, expr->start, expr->end);
            }
            else if (DEL(expr->node)->expr->type==N_SLICE){
                Node* sliceexpr=DEL(expr->node)->expr;

                Node* base=SUBSCR(SLICE(sliceexpr->node)->left->node)->left;
                Node* left=SUBSCR(SLICE(sliceexpr->node)->left->node)->expr;
                Node* right=SLICE(sliceexpr->node)->right;

                uint32_t type=0;
                if (left==NULL && right!=NULL){
                    type=1;
                }
                else if (left!=NULL && right==NULL){
                    type=2;
                }
                else if (left==NULL && right==NULL){
                    type=4;
                }
                else{
                    type=3;
                }

                bool ret=compiler->keep_return;
                compiler->keep_return=true;
                if (!ret){
                    compiler->keep_return=false;
                }
                compiler->keep_return=ret;


                int cmpexpr=compile_expr(compiler, base);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                if (type==3){
                    cmpexpr=compile_expr(compiler, left);
                    cmpexpr=compile_expr(compiler, right);
                    add_instruction(compiler->instructions, MAKE_SLICE, 0, expr->start, expr->end);
                }
                else if (type==2){
                    int cmpexpr=compile_expr(compiler, left);
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx;
                    if (!object_find_bool(compiler->consts,noneobj)){
                        //Create object
                        compiler->consts->type->slot_mappings->slot_append(compiler->consts, noneobj);
                        idx=NAMEIDX(compiler->consts);
                    }
                    else{
                        idx=object_find(compiler->consts, noneobj);
                    }
                    
                    add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
                    add_instruction(compiler->instructions, MAKE_SLICE, 0, expr->start, expr->end);
                }
                else if (type==1) {
                    uint32_t idx;
                    if (!object_find_bool(compiler->consts,noneobj)){
                        //Create object
                        compiler->consts->type->slot_mappings->slot_append(compiler->consts, noneobj);
                        idx=NAMEIDX(compiler->consts);
                    }
                    else{
                        idx=object_find(compiler->consts, noneobj);
                    }
                    
                    add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
                    int cmpexpr=compile_expr(compiler, right);
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    add_instruction(compiler->instructions, MAKE_SLICE, 0, expr->start, expr->end);
                }
                else{
                    //Type 4 needs nothing
                    break;
                }

                add_instruction(compiler->instructions, DEL_SUBSCR, 0, expr->start, expr->end);
            } 
            else if (DEL(expr->node)->expr->type==N_GLBL_IDENT){
                uint32_t idx;
                if (!_list_contains(compiler->names, IDENTI(GLBLIDENT(DEL(expr->node)->expr->node)->name->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(GLBLIDENT(DEL(expr->node)->expr->node)->name->node)->name));
                    idx = NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*IDENTI(GLBLIDENT(DEL(expr->node)->expr->node)->name->node)->name));
                }
                add_instruction(compiler->instructions, DEL_GLBL, idx, expr->start, expr->end);
            }
            else if (DEL(expr->node)->expr->type==N_NONLOCAL){
                uint32_t idx;
                if (!_list_contains(compiler->names, IDENTI(DEL(expr->node)->expr->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(DEL(expr->node)->expr->node)->name));
                    idx = NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*IDENTI(DEL(expr->node)->expr->node)->name));
                }
                add_instruction(compiler->instructions, DEL_NONLOCAL, idx, expr->start, expr->end);
            }
            else if (DEL(expr->node)->expr->type==N_DOT){
                vector<Node*>* names=DOT(DEL(expr->node)->expr->node)->names;
                bool ret=compiler->keep_return;
                compiler->keep_return=true;
                compile_expr(compiler, names->at(0));
                compiler->keep_return=ret;

                for (size_t i=1; i<names->size(); i++){
                    uint32_t idx;
                    if (!_list_contains(compiler->names, IDENTI(names->at(i)->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                    }
                                
                    if (i==names->size()-1){
                        add_instruction(compiler->instructions,DEL_ATTR, idx, expr->start, expr->end);
                        continue;
                    }     

                    add_instruction(compiler->instructions,LOAD_ATTR, idx, expr->start, expr->end);
                }
            }
            else{
                uint32_t idx;
                if (!_list_contains(compiler->names, IDENTI(DEL(expr->node)->expr->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(DEL(expr->node)->expr->node)->name));
                    idx = NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*IDENTI(DEL(expr->node)->expr->node)->name));
                }
                add_instruction(compiler->instructions, DEL_NAME, idx, expr->start, expr->end);
            }
            break;
        }

        case N_GLBL_IDENT: {
            enum scope s=compiler->scope;
            compiler->scope=SCOPE_GLOBAL;
            int cmpexpr=compile_expr(compiler, GLBLIDENT(expr->node)->name);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            compiler->scope=s;
            break;
        }

        case N_FSTRING: {
            string data=*STRLIT(expr->node)->literal;
            
            int x=0;
            int i=0;
            while (i<data.size()){
                x++;
                if (data[i]=='{'){
                    string segment="";
                    bool repr=false;
                    i++;
                    while (data[i]!='}' && data[i]!='\0'){
                        segment+=data[i++];
                        if (data[i-1]=='!' && data[i]=='r'){
                            segment.pop_back();
                            repr=true;
                            i++;
                            continue;
                        }

                        if (data[i]=='\0'){
                            parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected '}' for format string");
                            return 0x100;
                        }

                        if (data[i]=='{'){
                            parser.add_parsing_error(&parseretglbl, "SyntaxError: Expected '}' for format string");
                            return 0x100;
                        }

                    }
                    if (data[i]!='\0'){
                        i++;
                    }

                    
                    Lexer lexer(segment,kwds);
                    lexer.pos=Position(program);

                    Position end=lexer.tokenize();
                    
                    Parser p=parser;
                    parser=Parser(lexer.tokens, segment);
                    parse_ret ast=parser.parse();
                    parser=p;

                    if (ast.errornum>0){
                        cout<<ast.header<<endl;
                        cout<<ast.snippet<<endl;
                        cout<<ast.arrows<<endl;
                        printf("%s\n",ast.error);
                        return 0x100;
                    }

                    int cmpexpr=compile_expr(compiler, ast.nodes.at(0));
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    uint32_t idx;

                    if (repr){
                        if (!object_find_bool(compiler->consts,trueobj)){
                            //Create object
                            compiler->consts->type->slot_mappings->slot_append(compiler->consts, new_bool_true());
                            idx=NAMEIDX(compiler->consts);
                        }
                        else{
                            idx=object_find(compiler->consts, trueobj);
                        }
                    }
                    else{
                        if (!object_find_bool(compiler->consts,falseobj)){
                            //Create object
                            compiler->consts->type->slot_mappings->slot_append(compiler->consts, new_bool_false());
                            idx=NAMEIDX(compiler->consts);
                        }
                        else{
                            idx=object_find(compiler->consts, falseobj);
                        }
                    }
                    add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);
                }
                else{
                    string segment="";
                    while (data[i]!='{'){
                        segment+=data[i++];
                    }

                    string* s=new string(segment);
                    uint32_t idx;
                    if (!_list_contains(compiler->consts, s)){
                        //Create object
                        compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(segment));
                        idx=NAMEIDX(compiler->consts);
                    }
                    else{
                        idx=object_find(compiler->consts, str_new_fromstr(segment));
                    }
                    delete s;
                    add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);

                    if (!object_find_bool(compiler->consts,falseobj)){
                        //Create object
                        compiler->consts->type->slot_mappings->slot_append(compiler->consts, new_bool_false());
                        idx=NAMEIDX(compiler->consts);
                    }
                    else{
                        idx=object_find(compiler->consts, falseobj);
                    }
                    add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);
                }  
            }
            add_instruction(compiler->instructions,BUILD_STRING, x, expr->start, expr->end);
            break;
        }

        case N_ASSERT: {
            compile_expr(compiler, ASSERT(expr->node)->expr);
            add_instruction(compiler->instructions,POP_JMP_TOS_TRUE, 2, expr->start, expr->end);
            add_instruction(compiler->instructions,RAISE_ASSERTIONERR, 0, expr->start, expr->end);
            break;            
        }

        case N_NONLOCAL: {
            uint32_t idx;
            if (!_list_contains(compiler->names, IDENTI(expr->node)->name)){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(expr->node)->name));
                idx = NAMEIDX(compiler->names);
            }
            else{
                idx=object_find(compiler->names, str_new_fromstr(*IDENTI(expr->node)->name));
            }
            
            add_instruction(compiler->instructions,LOAD_NONLOCAL, idx, expr->start, expr->end);
            break;
        }

    }
    
    if (!compiler_nofree){
        free(expr);
    }

    return 0;
}

uint32_t num_instructions(vector<Node*>* nodes, scope s){
    bool nofree=compiler_nofree;
    compiler_nofree=true;

    struct compiler* comp=new_compiler();
    comp->lines=NULL;
    comp->scope=s;
    for (Node* n: (*nodes)){    
        int cmpexpr=compile_expr(comp, n);
        if (cmpexpr==0x100){
            return -1;
        }
    }

    compiler_nofree=nofree;
    return comp->instructions->count;
}

uint32_t num_instructions(Node* node, scope s){
    bool nofree=compiler_nofree;
    compiler_nofree=true;

    struct compiler* comp=new_compiler(); 
    comp->lines=NULL;   
    comp->scope=s;  
    int cmpexpr=compile_expr(comp, node);
    if (cmpexpr==0x100){
        return -1;
    }
        
    compiler_nofree=nofree;
    return comp->instructions->count;
}

struct object* compile(struct compiler* compiler, parse_ret ast, int fallback_line){
    compiler->lines=new_list();
    object* lines=compiler->lines;
    int i=0;
    for (Node* n: ast.nodes){
        uint32_t start=compiler->instructions->count;
        
        int line=n->start->line;
        
        int i=compile_expr(compiler, n);

        if (i==0x100){
            return NULL;
        }
        uint32_t end=compiler->instructions->count;
        if (i!=0x200){
            object* tuple=new_tuple();
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
            lines->type->slot_mappings->slot_append(lines, tuple);
        }
    }
        
    
    uint32_t idx;
    if (!object_find_bool(compiler->consts, noneobj)){
        //Create object
        compiler->consts->type->slot_mappings->slot_append(compiler->consts, new_none());
        idx=NAMEIDX(compiler->consts);
    }
    else{
        idx=object_find(compiler->consts, new_none());
    }
    
    add_instruction(compiler->instructions, LOAD_CONST, idx, new Position, new Position);
    add_instruction(compiler->instructions, RETURN_VAL, 0, new Position, new Position);


    if (ast.nodes.size()>0){
        object* tuple=new_tuple();
        object* lineno=object_sub(lines->type->slot_mappings->slot_len(lines), new_int_fromint(1));
        object* line=lines->type->slot_mappings->slot_get(lines, lineno);
        tuple->type->slot_mappings->slot_append(tuple, tuple_index_int(line, 0));
        tuple->type->slot_mappings->slot_append(tuple, object_add(tuple_index_int(line, 1), new_int_fromint(2)));
        tuple->type->slot_mappings->slot_append(tuple, tuple_index_int(line, 2));
        
        lines->type->slot_mappings->slot_set(lines, lineno, tuple);
    }
    else{
        object* tuple=new_tuple();
        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(0));
        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(2));
        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(fallback_line));
        lines->type->slot_mappings->slot_append(lines, tuple);
    }
    
    reverse_instructions(compiler->instructions);
    object* instructions=new_list();
    struct instruction* instruction=compiler->instructions->first;
    while (instruction){
        CAST_LIST(instructions)->type->slot_mappings->slot_append(instructions, new_int_fromint(instruction->opcode));
        CAST_LIST(instructions)->type->slot_mappings->slot_append(instructions, new_int_fromint(instruction->arg));
        instruction=instruction->next;
    }

    object* list=new_list();
    CAST_LIST(list)->type->slot_mappings->slot_append(list, compiler->names);
    CAST_LIST(list)->type->slot_mappings->slot_append(list, compiler->consts);
    CAST_LIST(list)->type->slot_mappings->slot_append(list, instructions);
    CAST_LIST(list)->type->slot_mappings->slot_append(list, object_repr(str_new_fromstr(program)));
    CAST_LIST(list)->type->slot_mappings->slot_append(list, lines);

    object* code=code_new_fromargs(list);
    CAST_CODE(code)->co_instructions=CAST_INT(instructions->type->slot_mappings->slot_len(instructions))->val->to_int();
    CAST_CODE(code)->filedata=glblfildata;
    return code;
}
