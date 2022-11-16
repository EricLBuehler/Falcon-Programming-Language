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

void compile_expr_cleanup(int* compiler_ss, int* ss, int* compiler_bss, int* bss){
    if (*compiler_ss<*ss){
        *compiler_ss=*ss;
    }

    
    if (*compiler_bss<*bss){
        *compiler_bss=*bss;
    }
}


//If stack size grows after instruction is executed, return that number
//Otherwise, return 0.
int get_stack_size_inc(enum opcode opcode, uint32_t arg){
    switch (opcode){
        case LOAD_CONST: {
            return 1;
        }
        case STORE_NAME: {
            return 0;
        }
        case LOAD_NAME: {
            return 1;
        }
        case STORE_GLOBAL: {
            return 0;
        }
        case LOAD_GLOBAL: {
            return 1;
        }
        case BINOP_ADD: {
            return 0;
        }
        case BINOP_SUB: {
            return 0;
        }
        case UNARY_NEG: {
            return 0;
        }
        case BINOP_MUL: {
            return 0;
        }
        case BINOP_DIV: {
            return 0;
        }
        case MAKE_FUNCTION: {
            return 0;
        }
        case RETURN_VAL: {
            return 0;
        }
        case CALL_FUNCTION: {
            return 0;
        }
        case BUILD_TUPLE: {
            if (arg==0){
                return 1;
            }
            return 0;
        }
        case BUILD_DICT: {
            if (arg==0){
                return 1;
            }
            return 0;
        }
        case LOAD_BUILD_CLASS: {
            return 1;
        }
        case LOAD_ATTR: {
            return 0;
        }
        case STORE_ATTR: {
            return 0;
        }
        case BUILD_LIST: {
            if (arg==0){
                return 1;
            }
            return 0;
        }
        case BINOP_IS: {
            return 0;
        }
        case BINOP_EE: {
            return 0;
        }
        case POP_JMP_TOS_FALSE: {
            return 0;
        }
        case JUMP_DELTA: {
            return 0;
        }
        case BINOP_SUBSCR: {
            return 0;
        }
        case RAISE_EXC: {
            return 0;
        }
        case STORE_SUBSCR: {
            return 0;
        }
        case DUP_TOS: {
            return 1;
        }
        case POP_TOS: {
            return 0;
        }
        case SETUP_TRY: {
            return 0;
        }
        case FINISH_TRY: {
            return 0;
        }
        case BINOP_EXC_CMP: {
            return 0;
        }
        case BINOP_GT: {
            return 0;
        }
        case BINOP_GTE: {
            return 0;
        }
        case BINOP_LT: {
            return 0;
        }
        case BINOP_LTE: {
            return 0;
        }
        case FOR_TOS_ITER: {
            return 0;
        }
        case JUMP_TO: {
            return 0;
        }
        case EXTRACT_ITER: {
            return 0;
        }
        case BREAK_LOOP: {
            return 0;
        }
        case CONTINUE_LOOP: {
            return 0;
        }
        case UNPACK_SEQ: {
            return arg;
        }
        case IMPORT_NAME: {
            return 1;
        }
        case IMPORT_FROM_MOD: {
            return 0;
        }
        case MAKE_SLICE: {
            return 0;
        }
        case BINOP_NE: {
            return 0;
        }
        case DEL_SUBSCR: {
            return 0;
        }
        case DEL_NAME: {
            return 0;
        }
        case BINOP_MOD: {
            return 0;
        }
        case BINOP_POW: {
            return 0;
        }
        case BINOP_AND: {
            return 0;
        }
        case BINOP_OR: {
            return 0;
        }
        case UNARY_NOT: {
            return 0;
        }
        case BUILD_STRING: {
            if (arg==0){
                return 1;
            }
            return 0;
        }
        case POP_JMP_TOS_TRUE: {
            return 0;
        }
        case RAISE_ASSERTIONERR: {
            return 0;
        }
        case DEL_GLBL: {
            return 0;
        }
        case DEL_ATTR: {
            return 0;
        }
        case MAKE_CLOSURE: {
            return 0;
        }
        case LOAD_NONLOCAL: {
            return 1;
        }
        case STORE_NONLOCAL: {
            return 0;
        }
        case DEL_NONLOCAL: {
            return 0;
        }
        case BITWISE_NOT: {
            return 0;
        }
        case BITWISE_AND: {
            return 0;
        }
        case BITWISE_OR: {
            return 0;
        }
        case BITWISE_LSHIFT: {
            return 0;
        }
        case BITWISE_RSHIFT: {
            return 0;
        }
        case BINOP_NOTIN: {
            return 0;
        }
        case BINOP_IN: {
            return 0;
        }
        case BINOP_ISNOT: {
            return 0;
        }
        case BINOP_FLDIV: {
            return 0;
        }
        case TERNARY_TEST: {
            return 0;
        }
        case CALL_FUNCTION_BOTTOM: {
            return 0;
        }
        case ANNOTATE_GLOBAL: {
            return 0;
        }
        case ANNOTATE_NAME: {
            return 0;
        }
        case ANNOTATE_NONLOCAL: {
            return 0;
        }
        case STORE_ATTR_ANNOTATE: {
            return 0;
        }
        case YIELD_VALUE: {
            return 0;
        }
        case MAKE_GENERATOR: {
            return 0;
        }
        case MAKE_CLOSURE_GENERATOR: {
            return 0;
        }
        case BUILD_SET: {
            if (arg==0){
                return 1;
            }
            return 0;
        }
        case CLEAR_EXC: {
            return 0;
        }
        case ENTER_WHILE: {
            return 0;
        }
        case EXIT_WHILE: {
            return 0;
        }
        case ENTER_WITH: {
            return 0;
        }
        case EXIT_WITH: {
            return 0;
        }
        case SEQ_APPEND: {
            return 0;
        }
        case DICT_SET: {
            return 0;
        }
        case BITWISE_XOR: {
            return 0;
        }
        case BYTES_STRING: {
            return 0;
        }
        case CALL_FUNCTION_BOTTOM_KW: {
            return 0;
        }
        case CALL_FUNCTION_BOTTOM_U: {
            return 0;
        }
        case CALL_FUNCTION_BOTTOM_KW_U: {
            return 0;
        }
        case CALL_FUNCTION_KW: {
            return 0;
        }
        case CALL_FUNCTION_U: {
            return 0;
        }
        case CALL_FUNCTION_KW_U: {
            return 0;
        }
    }

    return 0;
}

//If stack size grows after instruction is executed, return that number
//Otherwise, return 0.
int get_blockstack_size_inc(enum opcode opcode, uint32_t arg){
    switch (opcode){
        case SETUP_TRY: {
            return 1;
        }
        case FOR_TOS_ITER: {
            return 1;
        }
        case ENTER_WHILE: {
            return 1;
        }
        case ENTER_WITH: {
            return 1;
        }
    }
    return 0;
}

void add_instruction(struct compiler* compiler, struct instructions* instructions, enum opcode opcode, uint32_t arg, int startcol, int endcol, int line){
    struct instruction* instr=(struct instruction*)fpl_malloc(sizeof(struct instruction));
    instr->arg=arg;
    instr->opcode=opcode;
    instr->next=instructions->first;
    
    object* tup=new_tuple();
    tuple_append(tup, new_int_fromint(startcol));
    tuple_append(tup, new_int_fromint(endcol));
    tuple_append(tup, new_int_fromint(line));
    dict_set_noret(compiler->lines_detailed, new_int_fromint(instructions->count*2), tup);
    FPLDECREF(tup);

    compiler->stack_size+=get_stack_size_inc(opcode, arg);
    compiler->blockstack_size+=get_blockstack_size_inc(opcode, arg);

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
    compiler->nofree=false;
    compiler->inclass=false;
    compiler->lines_detailed=new_dict();
    compiler->stack_size=0;
    compiler->blockstack_size=0;
    return compiler;
}

void compiler_del(struct compiler* compiler){
    FPLDECREF(compiler->consts);
    FPLDECREF(compiler->names);
    struct instruction* i=compiler->instructions->first;
    while (i){
        struct instruction* i_=i->next;
        fpl_free(i);
        i=i_;
    }
}

int compile_expr(struct compiler* compiler, Node* expr);

int compile_expr_keep(struct compiler* compiler, Node* expr){
    bool ret=compiler->keep_return;
    compiler->keep_return=true;
    int i=compile_expr(compiler, expr);
    compiler->keep_return=ret;
    return i;
}

#define GET_ANNO_N(node) (node->start->col), (node->end->col), (node->start->line)

uint32_t binop_inplace_setup(struct compiler* compiler, Node* left){
    switch (left->type){
        case N_IDENT: {
            if (!_list_contains(compiler->names, IDENTI(left->node)->name)){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(left->node)->name));
                return NAMEIDX(compiler->names);
            }
            else{
                return object_find(compiler->names, str_new_fromstr(*IDENTI(left->node)->name));
            }
            break;
        }
        case N_GLBL_IDENT: {
            if (!_list_contains(compiler->names, IDENTI(GLBLIDENT(left->node)->name->node)->name)){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(GLBLIDENT(left->node)->name->node)->name));
                return NAMEIDX(compiler->names);
            }
            else{
                return object_find(compiler->names, str_new_fromstr(*IDENTI(GLBLIDENT(left->node)->name->node)->name));
            }
            break;
        }
        case N_NONLOCAL: {
             if (!_list_contains(compiler->names, IDENTI(left->node)->name)){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(left->node)->name));
                return NAMEIDX(compiler->names);
            }
            else{
                return object_find(compiler->names, str_new_fromstr(*IDENTI(left->node)->name));
            }
            break;
        }
        case N_DOT: {
            vector<Node*>* names=DOT(left->node)->names;
            
            bool nofree=compiler->nofree;
            compiler->nofree=true;
            compile_expr_keep(compiler, names->at(0));
            compiler->nofree=nofree;
            uint32_t lastidx;
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
                lastidx=idx;
                add_instruction(compiler, compiler->instructions,LOAD_ATTR, idx, GET_ANNO_N(names->at(i)));
            }

            return lastidx;

            break;
        }
    }
    return -1;
}

void binop_inplace_finish(struct compiler* compiler, Node* left, uint32_t idx){
    switch (left->type){
        case N_IDENT: {
            switch (compiler->scope){
                case SCOPE_GLOBAL:
                    add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(left));
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(left));
                    break;
            }
            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(left));
            }
            break;
        }
        case N_GLBL_IDENT: {
            add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(left));
            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(left));
            }
            break;
        }
        case N_NONLOCAL: {
            add_instruction(compiler, compiler->instructions,STORE_NONLOCAL, idx, GET_ANNO_N(left));
            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(left));
            }
            break;
        }
        case N_DOT: {
            vector<Node*>* names=DOT(left->node)->names;
            
            compile_expr_keep(compiler, names->at(0));

            add_instruction(compiler, compiler->instructions,STORE_ATTR, idx, GET_ANNO_N(left));
            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(left));
            }
            break;
        }
    }
    
}

int compile_expr(struct compiler* compiler, Node* expr){
    nodetype type=expr->type;
    switch (expr->type){
        case N_ASSIGN: {
            int cmpexpr=compile_expr_keep(compiler, ASSIGN(expr->node)->right); //Push data
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (ASSIGN(expr->node)->name->type==N_MULTIIDENT){
                add_instruction(compiler, compiler->instructions,DUP_TOS,0, GET_ANNO_N(ASSIGN(expr->node)->name));
                add_instruction(compiler, compiler->instructions,UNPACK_SEQ,  MULTIIDENT(ASSIGN(expr->node)->name->node)->name->size(), GET_ANNO_N(ASSIGN(expr->node)->name));
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
                        add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
                        break;
                }
                if (!compiler->keep_return){
                    add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(ASSIGN(expr->node)->name));
                }
            }
            else if (ASSIGN(expr->node)->name->type==N_MULTIIDENT){
                int startcol=ASSIGN(expr->node)->name->start->col;
                int endcol=ASSIGN(expr->node)->name->end->col;
                int startln=ASSIGN(expr->node)->name->start->line;
                int cmpexpr=compile_expr(compiler, ASSIGN(expr->node)->name);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, startcol, endcol, startln);
                if (!compiler->keep_return){
                    add_instruction(compiler, compiler->instructions,POP_TOS, 0, startcol, endcol, startln);
                }
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
                add_instruction(compiler, compiler->instructions,STORE_NONLOCAL, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
                if (!compiler->keep_return){
                    add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(ASSIGN(expr->node)->name));
                }
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
                add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
                if (!compiler->keep_return){
                    add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(ASSIGN(expr->node)->name));
                }
            }
            break;
        }

        case N_MULTIIDENT: {
            uint32_t idx;
            for (int i=MULTIIDENT(expr->node)->name->size(); i>0; i--){
                string* s=MULTIIDENT(expr->node)->name->at(i-1);
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
                        add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(expr));
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(expr));
                        break;
                }
                
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
            }
            break;
        }

        case N_BINOP: {
            Node* left=BINOP(expr->node)->left;
            
            switch (BINOP(expr->node)->opr){
                case T_PLUS: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_ADD,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_MINUS: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_SUB,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_MUL: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_MUL,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_DIV: {                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_DIV,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_IS: {                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_IS,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_EE: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_EE,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_NE: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_NE,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_GT: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_GT,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_GTE: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_GTE,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_LT: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_LT,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_LTE: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_LTE,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_PERCENT: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_MOD,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_POW: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_POW,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_AND: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_AND,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_OR: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_OR,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_CARET: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BITWISE_XOR,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_AMPERSAND: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BITWISE_AND,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_VBAR: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BITWISE_OR,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_LSHIFT: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BITWISE_LSHIFT,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_RSHIFT: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BITWISE_RSHIFT,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_NOT: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_NOTIN,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_IN: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_IN,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_ISNOT: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_ISNOT,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_FLDIV: {                    
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->left); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_FLDIV,0, GET_ANNO_N(expr));
                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    break;
                }
                case T_IADD: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BINOP_ADD,idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
                case T_ISUB: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BINOP_SUB,idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
                case T_IMUL: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BINOP_MUL,idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
                case T_IDIV: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BINOP_DIV,idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
                case T_IPOW: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BINOP_POW,idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
                case T_IMOD: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BINOP_MOD,idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
                case T_IAMP: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BITWISE_AND, idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
                case T_IVBAR: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BITWISE_OR,idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
                case T_ILSH: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BITWISE_LSHIFT,idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
                case T_IRSH: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BITWISE_LSHIFT,idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
                case T_IFLDIV: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BINOP_FLDIV,idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
                case T_IXOR: {
                    int cmpexpr=compile_expr_keep(compiler, BINOP(expr->node)->right); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    uint32_t idx=binop_inplace_setup(compiler, left);
                    add_instruction(compiler, compiler->instructions,BITWISE_XOR,idx, GET_ANNO_N(expr));
                    binop_inplace_finish(compiler, left, idx);
                    break;
                }
            }
            
            break;
        }

        case N_UNARY: {
            int cmpexpr=compile_expr_keep(compiler, UNARYOP(expr->node)->right); //Push data
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            
            switch (UNARYOP(expr->node)->opr){
                case T_MINUS:
                    add_instruction(compiler, compiler->instructions,UNARY_NEG,0, GET_ANNO_N(expr));
                    break; 
                case T_NOT:
                    add_instruction(compiler, compiler->instructions,UNARY_NOT,0, GET_ANNO_N(expr));
                    break;
                case T_TILDE:
                    add_instruction(compiler, compiler->instructions,BITWISE_NOT,0, GET_ANNO_N(expr));
                    break;
                case T_PLUS:
                    break;
            }
            break;
        }

        case N_INT: {
            uint32_t idx;
            object* v=new_int_fromstr(INTLIT(expr->node)->literal);
            if (!object_find_bool(compiler->consts, v)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, v);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, v);
            }
            
            add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
            }
            break;
        }

        case N_FLOAT: {
            uint32_t idx;
            object* v=new_float_fromstr(FLOATLIT(expr->node)->literal);
            if (!object_find_bool(compiler->consts, v)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, v);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, v);
            }
            
            add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
            }
            break;
        }

        case N_STR: {
            uint32_t idx;
            object* v=str_new_fromstr(*STRLIT(expr->node)->literal);
            if (!object_find_bool(compiler->consts, v)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, v);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, v);
            }
            
            add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
            }
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
                    add_instruction(compiler, compiler->instructions,LOAD_GLOBAL, idx, GET_ANNO_N(expr));
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler, compiler->instructions,LOAD_NAME, idx, GET_ANNO_N(expr));
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
            
            add_instruction(compiler, compiler->instructions,LOAD_CONST, nameidx, GET_ANNO_N(FUNCT(expr->node)->name));

            //Arguments
            size_t argc=0;

            object* args=new_tuple();
            object* kwargs=new_tuple();

            //Setup args
            for (Node* n: (*FUNCT(expr->node)->args)){
                argc++;
                if (n->type==N_IDENT){
                    args->type->slot_mappings->slot_append(args, str_new_fromstr(*IDENTI(n->node)->name));
                }
                else{
                    args->type->slot_mappings->slot_append(args, str_new_fromstr(*ANONIDENT(n->node)->name));
                }
            }
            //
            
            //Setup kwargs (code object) backwards
            for (Node* n: (*FUNCT(expr->node)->kwargs)){
                if (n->type==N_ASSIGN){
                    args->type->slot_mappings->slot_append(args, str_new_fromstr(*IDENTI(ASSIGN(n->node)->name->node)->name));
                }
                else{
                    args->type->slot_mappings->slot_append(args, str_new_fromstr(*ANONIDENT(ASSIGN(n->node)->name->node)->name));
                }
            }
            
            for (auto n =  (*FUNCT(expr->node)->kwargs).rbegin(); n != (*FUNCT(expr->node)->kwargs).rend(); ++n){
                argc++;
                if ((*n)->type==N_ASSIGN){
                    int cmpexpr=compile_expr_keep(compiler, ASSIGN((*n)->node)->right);
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                }
                else{
                    int cmpexpr=compile_expr_keep(compiler, ASSIGN((*n)->node)->right);
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                }
            }
            //
            add_instruction(compiler, compiler->instructions,BUILD_TUPLE, FUNCT(expr->node)->kwargs->size(), GET_ANNO_N(FUNCT(expr->node)->name));
            
            uint32_t idx;
            if (!object_find_bool(compiler->consts, args)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, args);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, args);
            }
            add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(expr));
            

            //
            
            //Annotations
            uint32_t n_anno=0;
            if (FUNCT(expr->node)->rettp!=NULL){
                n_anno++;
                
                int startcol=FUNCT(expr->node)->rettp->start->col;
                int endcol=FUNCT(expr->node)->rettp->end->col;
                int startln=FUNCT(expr->node)->rettp->start->line;

                int cmpexpr=compile_expr_keep(compiler, FUNCT(expr->node)->rettp); //Push data
                if (cmpexpr==0x100){
                    return cmpexpr;
                }

                uint32_t idx;
                string* s=new string("return");
                if (!_list_contains(compiler->consts, s)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr("return"));
                    idx = NAMEIDX(compiler->consts);
                }
                else{
                    idx=object_find(compiler->consts, str_new_fromstr("return"));
                }
                delete s;
                add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, startcol, endcol, startln);
            }

            for (Node* n: (*FUNCT(expr->node)->args)){
                if (n->type!=N_IDENT){
                    n_anno++;
                    
                    int startcol=ANONIDENT(n->node)->tp->start->col;
                    int endcol=ANONIDENT(n->node)->tp->end->col;
                    int startln=ANONIDENT(n->node)->tp->start->line;

                    int cmpexpr=compile_expr_keep(compiler, ANONIDENT(n->node)->tp); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    uint32_t idx;
                    if (!_list_contains(compiler->consts, ANONIDENT(n->node)->name )){
                        //Create object
                        compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*ANONIDENT(n->node)->name));
                        idx = NAMEIDX(compiler->consts);
                    }
                    else{
                        idx=object_find(compiler->consts, str_new_fromstr(*ANONIDENT(n->node)->name));
                    }
                    add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, startcol, endcol, startln);
                }
            }
            for (Node* n: (*FUNCT(expr->node)->kwargs)){
                if (n->type!=N_ASSIGN){
                    n_anno++; 

                    int startcol=ANONIDENT(n->node)->tp->start->col;
                    int endcol=ANONIDENT(n->node)->tp->end->col;
                    int startln=ANONIDENT(n->node)->tp->start->line;

                    int cmpexpr=compile_expr_keep(compiler, ANONIDENT(ASSIGN(n->node)->name->node)->tp); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    uint32_t idx;
                    if (!_list_contains(compiler->consts, ANONIDENT(ASSIGN(n->node)->name->node)->name)){
                        //Create object
                        compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*ANONIDENT(ASSIGN(n->node)->name->node)->name));
                        idx = NAMEIDX(compiler->consts);
                    }
                    else{
                        idx=object_find(compiler->consts, str_new_fromstr(*ANONIDENT(ASSIGN(n->node)->name->node)->name));
                    }
                    add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, startcol, endcol, startln);
                }
            }
            
            add_instruction(compiler, compiler->instructions,BUILD_DICT, n_anno, GET_ANNO_N(expr));

            //


            //Code
            parse_ret c;
            c.nodes=(*FUNCT(expr->node)->code);
            struct compiler* comp=new_compiler();
            comp->scope=SCOPE_LOCAL;
            struct compiler* compiler_=compiler;
            compiler=comp;
            object* code=compile(comp, c, expr->start->line);
            if (code==NULL){
                return 0x100;
            }
            compiler=compiler_;
            bool isgen=false;
            compile_expr_cleanup(&compiler->stack_size, &comp->stack_size, &compiler->blockstack_size, &comp->blockstack_size);
            
            for (int i=0; i<CAST_LIST(CAST_CODE(code)->co_code)->size; i+=2){
                if (*CAST_INT(list_index_int(CAST_CODE(code)->co_code, i))->val==YIELD_VALUE){
                    isgen=true;
                    break;
                }
            }
            
            object* lines=new_list();
            
            object* tuple=new_tuple();
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(0));
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(0));
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(expr->start->line));
            
            lines->type->slot_mappings->slot_append(lines, tuple);
            for (int i=0; i<CAST_LIST(CAST_CODE(code)->co_lines)->size; i++){
                lines->type->slot_mappings->slot_append(lines, list_index_int(CAST_CODE(code)->co_lines, i));
            }
            FPLDECREF(CAST_CODE(code)->co_lines);
            CAST_CODE(code)->co_lines=lines; //No FPLINCREF necessary

            FPLDECREF(CAST_CODE(code)->co_file);
            CAST_CODE(code)->co_file=object_repr(str_new_fromstr(*IDENTI(FUNCT(expr->node)->name->node)->name));
            
            
            if (!object_find_bool(compiler->consts, code)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, code);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, code);
            }
            add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(expr));

            //Star args/kwargs
            int star=FUNC_STRICTARGS;
            uint32_t star_kwargs;
            uint32_t star_args;
            if (FUNCT(expr->node)->starargs && FUNCT(expr->node)->starkwargs){
                star=FUNC_STAR;
                if (!_list_contains(compiler->consts, IDENTI(FUNCT(expr->node)->stkwargs->node)->name)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(expr->node)->stkwargs->node)->name));
                    star_kwargs = NAMEIDX(compiler->consts);
                }
                else{
                    star_kwargs=object_find(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(expr->node)->stkwargs->node)->name));
                }
                
                if (!_list_contains(compiler->consts, IDENTI(FUNCT(expr->node)->stargs->node)->name)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(expr->node)->stargs->node)->name));
                    star_args = NAMEIDX(compiler->consts);
                }
                else{
                    star_args=object_find(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(expr->node)->stargs->node)->name));
                }
                add_instruction(compiler, compiler->instructions,LOAD_CONST, star_args, GET_ANNO_N(FUNCT(expr->node)->stargs));
                add_instruction(compiler, compiler->instructions,LOAD_CONST, star_kwargs, GET_ANNO_N(FUNCT(expr->node)->stargs));
            }
            else if(FUNCT(expr->node)->starargs){
                star=FUNC_STARARGS;
                if (!_list_contains(compiler->consts, IDENTI(FUNCT(expr->node)->stargs->node)->name)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(expr->node)->stargs->node)->name));
                    star_args = NAMEIDX(compiler->consts);
                }
                else{
                    star_args=object_find(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(expr->node)->stargs->node)->name));
                }
                add_instruction(compiler, compiler->instructions,LOAD_CONST, star_args, GET_ANNO_N(FUNCT(expr->node)->stargs));
            }
            else if(FUNCT(expr->node)->starkwargs){
                star=FUNC_STARKWARGS;
                if (!_list_contains(compiler->consts, IDENTI(FUNCT(expr->node)->stkwargs->node)->name)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(expr->node)->stkwargs->node)->name));
                    star_kwargs = NAMEIDX(compiler->consts);
                }
                else{
                    star_kwargs=object_find(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(expr->node)->stkwargs->node)->name));
                }
                add_instruction(compiler, compiler->instructions,LOAD_CONST, star_kwargs, GET_ANNO_N(FUNCT(expr->node)->stkwargs));
            }

            object* star_int=new_int_fromint(star);
            if (!object_find_bool(compiler->consts, star_int)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, star_int);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, star_int);
            }
            add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(expr));

            //Create callable
            if (compiler->scope!=SCOPE_GLOBAL && !compiler->inclass){
                if (!isgen){
                    add_instruction(compiler, compiler->instructions,MAKE_CLOSURE, argc, GET_ANNO_N(expr));
                }
                else{
                    add_instruction(compiler, compiler->instructions,MAKE_CLOSURE_GENERATOR, argc, GET_ANNO_N(expr));                    
                }
            }
            else{
                if (!isgen){
                    add_instruction(compiler, compiler->instructions,MAKE_FUNCTION, argc, GET_ANNO_N(expr));
                }
                else{
                    add_instruction(compiler, compiler->instructions,MAKE_GENERATOR, argc, GET_ANNO_N(expr));
                }
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
                        add_instruction(compiler, compiler->instructions,STORE_GLOBAL, nameidxstore, GET_ANNO_N(FUNCT(expr->node)->name));
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler, compiler->instructions,STORE_NAME, nameidxstore, GET_ANNO_N(FUNCT(expr->node)->name));
                        break;
                }
                
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(FUNCT(expr->node)->name));
            }
            else{
                if (!compiler->keep_return){
                    add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(FUNCT(expr->node)->name));
                }
            }
            break;
        }

        case N_CALL:  {            
            object* stargs=new_tuple();
            object* stkwargs=new_tuple();
            object* kwargs=new_tuple();

            int i_=0;
            size_t argsize=CALL(expr->node)->args->size();

            //Args (iterate backwards)
            for (auto it =  (*CALL(expr->node)->args).rbegin(); it != (*CALL(expr->node)->args).rend(); ++it){
                int cmpexpr=compile_expr_keep(compiler, *it);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                if (find(CALL(expr->node)->stargs->begin(), CALL(expr->node)->stargs->end(), i_)!=CALL(expr->node)->stargs->end()){
                    tuple_append(stargs, new_int_fromint(i_));
                }
                if (find(CALL(expr->node)->stkwargs->begin(), CALL(expr->node)->stkwargs->end(), i_+argsize-1)!=CALL(expr->node)->stkwargs->end()){
                    tuple_append(stkwargs, new_int_fromint(i_+argsize-1));
                }
                i_++;
            }
            //Kwargs (iterate backwards)
            for (auto it =  (*CALL(expr->node)->kwargs).rbegin(); it != (*CALL(expr->node)->kwargs).rend(); ++it){
                tuple_append(kwargs, str_new_fromstr(*IDENTI(ASSIGN((*it)->node)->name->node)->name));
                int cmpexpr=compile_expr_keep(compiler, ASSIGN((*it)->node)->right);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }

            
            uint32_t idx;

            if (CAST_TUPLE(kwargs)->size != 0){
                if (!object_find_bool(compiler->consts,kwargs)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, kwargs);
                    idx=NAMEIDX(compiler->consts);
                }
                else{
                    idx=object_find(compiler->consts, kwargs);
                }
                
                add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
            }

            if (CAST_TUPLE(stargs)->size != 0 || CAST_TUPLE(stkwargs)->size != 0){
                if (!object_find_bool(compiler->consts,stargs)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, stargs);
                    idx=NAMEIDX(compiler->consts);
                }
                else{
                    idx=object_find(compiler->consts, stargs);
                }
                
                add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));

                if (!object_find_bool(compiler->consts,stkwargs)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, stkwargs);
                    idx=NAMEIDX(compiler->consts);
                }
                else{
                    idx=object_find(compiler->consts, stkwargs);
                }
                
                add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
            }
            
            //Num of pos args
            uint32_t argc=CALL(expr->node)->args->size()+CALL(expr->node)->kwargs->size();
            
            //Object
            int cmpexpr=compile_expr_keep(compiler, CALL(expr->node)->object);
            if (cmpexpr==0x100){
                return cmpexpr;
            }

            if ((CAST_TUPLE(kwargs)->size != 0) && (CAST_TUPLE(stargs)->size != 0 || CAST_TUPLE(stkwargs)->size != 0)){
                add_instruction(compiler, compiler->instructions,CALL_FUNCTION_KW_U, argc, GET_ANNO_N(expr));
            }
            else if ((CAST_TUPLE(kwargs)->size != 0)){
                add_instruction(compiler, compiler->instructions,CALL_FUNCTION_KW, argc, GET_ANNO_N(expr));
            }
            else if ((CAST_TUPLE(stargs)->size != 0 || CAST_TUPLE(stkwargs)->size != 0)){
                add_instruction(compiler, compiler->instructions,CALL_FUNCTION_U, argc, GET_ANNO_N(expr));
            }
            else{
                add_instruction(compiler, compiler->instructions,CALL_FUNCTION, argc, GET_ANNO_N(expr));
            }

            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
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
            
            add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
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
            
            add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
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
            
            add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
            break;
        }

        case N_CLASS: {
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
            add_instruction(compiler, compiler->instructions,LOAD_CONST, nameidx, GET_ANNO_N(expr));

            object* args=new_tuple();
            object* anno=new_tuple();
            
            uint32_t idx;
            if (!object_find_bool(compiler->consts, args)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, args);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, args);
            }
            add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(expr)); //Faux args
            add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(expr)); //Same but for kwargs

            if (!object_find_bool(compiler->consts, anno)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, anno);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, anno);
            }
            
            add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(expr)); //Faux annotations

            //Code
            parse_ret c;
            c.nodes=(*CLASS(expr->node)->code);
            struct compiler* comp=new_compiler();
            comp->scope=SCOPE_LOCAL;
            comp->inclass=true;
            struct compiler* compiler_=compiler;
            compiler=comp;
            object* code=compile(comp, c, expr->start->line);
            if (code==NULL){
                return 0x100;
            }
            compiler=compiler_;
            compile_expr_cleanup(&compiler->stack_size, &comp->stack_size, &compiler->blockstack_size, &comp->blockstack_size);

            FPLDECREF(CAST_CODE(code)->co_file);
            CAST_CODE(code)->co_file=object_repr(str_new_fromstr(*IDENTI(FUNCT(expr->node)->name->node)->name));
            
            if (!object_find_bool(compiler->consts, code)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, code);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, code);
            }
            add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(expr));

            object* star_int=new_int_fromint(FUNC_STRICTARGS);
            if (!object_find_bool(compiler->consts, star_int)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, star_int);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, star_int);
            }
            add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(expr));



            add_instruction(compiler, compiler->instructions,MAKE_FUNCTION, 0, GET_ANNO_N(expr));            
        
            add_instruction(compiler, compiler->instructions,LOAD_CONST, nameidx, GET_ANNO_N(expr));
            

            uint32_t nbases=CLASS(expr->node)->bases->size();
            for (Node* n: *CLASS(expr->node)->bases){
                int cmpexpr=compile_expr_keep(compiler, n);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            add_instruction(compiler, compiler->instructions,BUILD_LIST, nbases, GET_ANNO_N(expr));



            add_instruction(compiler, compiler->instructions,LOAD_BUILD_CLASS, 0, GET_ANNO_N(expr));
            
            add_instruction(compiler, compiler->instructions,CALL_FUNCTION, 3, GET_ANNO_N(expr));

            //Store class
            uint32_t nameidxstore;
            if (!_list_contains(compiler->names, IDENTI(CLASS(expr->node)->name->node)->name )){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(CLASS(expr->node)->name->node)->name));
                nameidxstore = NAMEIDX(compiler->names);
            }
            else{
                nameidxstore=object_find(compiler->names, str_new_fromstr(*IDENTI(CLASS(expr->node)->name->node)->name));
            }
            switch (compiler->scope){
                case SCOPE_GLOBAL:
                    add_instruction(compiler, compiler->instructions,STORE_GLOBAL, nameidxstore, GET_ANNO_N(CLASS(expr->node)->name));
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler, compiler->instructions,STORE_NAME, nameidxstore, GET_ANNO_N(CLASS(expr->node)->name));
                    break;
            }
            
            add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(CLASS(expr->node)->name));

            break;
        }

        case N_DOT: {
            vector<Node*>* names=DOT(expr->node)->names;
            
            compile_expr_keep(compiler, names->at(0));

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

                add_instruction(compiler, compiler->instructions,LOAD_ATTR, idx, GET_ANNO_N(names->at(i)));
            }
            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
            }
            break;
        }

        case N_DOTASSIGN: {
            int cmpexpr=compile_expr_keep(compiler, DOTASSIGN(expr->node)->right);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            vector<Node*>* names=DOT(DOTASSIGN(expr->node)->dot->node)->names;
            
            compile_expr_keep(compiler, names->at(0));
            
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
                    add_instruction(compiler, compiler->instructions,STORE_ATTR, idx, GET_ANNO_N(names->at(i)));
                    continue;
                }     
                add_instruction(compiler, compiler->instructions,LOAD_ATTR, idx, GET_ANNO_N(names->at(i)));
            }

            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
            }
            break;
        }        

        case N_DOTCALL: {
            vector<Node*>* names=DOT(DOTCALL(expr->node)->dot->node)->names;
            
            compile_expr_keep(compiler, names->at(0));
            
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
                            add_instruction(compiler, compiler->instructions,LOAD_GLOBAL, idx, GET_ANNO_N(names->at(i)));
                            break;

                        case SCOPE_LOCAL:
                            add_instruction(compiler, compiler->instructions,LOAD_NAME, idx, GET_ANNO_N(names->at(i)));
                            break;
                    }
                    continue;
                }            
                if (i==names->size()-1){//Object
                    if (!_list_contains(compiler->names, IDENTI(names->at(i)->node)->name)){
                        //Create object
                        compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(*IDENTI(names->at(i)->node)->name));
                    }

                    add_instruction(compiler, compiler->instructions,LOAD_ATTR, idx, GET_ANNO_N(names->at(i)));

                    object* kwargs=new_tuple();

                    //Args (iterate backwards)
                    for (auto it =  (*DOTCALL(expr->node)->args).rbegin(); it != (*DOTCALL(expr->node)->args).rend(); ++it){
                        int cmpexpr=compile_expr_keep(compiler, *it);
                        if (cmpexpr==0x100){
                            return cmpexpr;
                        }
                    }
                    //Kwargs (iterate backwards)
                    for (auto it =  (*DOTCALL(expr->node)->kwargs).rbegin(); it != (*DOTCALL(expr->node)->kwargs).rend(); ++it){
                        tuple_append(kwargs, str_new_fromstr(*IDENTI(ASSIGN((*it)->node)->name->node)->name));
                        int cmpexpr=compile_expr_keep(compiler, ASSIGN((*it)->node)->right);
                        if (cmpexpr==0x100){
                            return cmpexpr;
                        }
                    }

            
                    uint32_t idx;
                    if (CAST_TUPLE(kwargs)->size != 0){
                        if (!object_find_bool(compiler->consts,kwargs)){
                            //Create object
                            compiler->consts->type->slot_mappings->slot_append(compiler->consts, kwargs);
                            idx=NAMEIDX(compiler->consts);
                        }
                        else{
                            idx=object_find(compiler->consts, kwargs);
                        }
                        
                        add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
                    }

                    object* stargs=new_tuple();
                    for (int i: *DOTCALL(expr->node)->stargs){
                        tuple_append(stargs, new_int_fromint(i));
                    }
                    object* stkwargs=new_tuple();
                    for (int i: *DOTCALL(expr->node)->stkwargs){
                        tuple_append(stkwargs, new_int_fromint(i));
                    }

                    if (CAST_TUPLE(stargs)->size != 0 || CAST_TUPLE(stkwargs)->size != 0){
                        if (!object_find_bool(compiler->consts,stargs)){
                            //Create object
                            compiler->consts->type->slot_mappings->slot_append(compiler->consts, stargs);
                            idx=NAMEIDX(compiler->consts);
                        }
                        else{
                            idx=object_find(compiler->consts, stargs);
                        }
                        
                        add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));

                        

                        if (!object_find_bool(compiler->consts,stkwargs)){
                            //Create object
                            compiler->consts->type->slot_mappings->slot_append(compiler->consts, stkwargs);
                            idx=NAMEIDX(compiler->consts);
                        }
                        else{
                            idx=object_find(compiler->consts, stkwargs);
                        }

                        add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
                    }                    

                    //Num of pos args
                    uint32_t argc=DOTCALL(expr->node)->args->size()+DOTCALL(expr->node)->kwargs->size();                   

                    if ((CAST_TUPLE(kwargs)->size != 0) && (CAST_TUPLE(stargs)->size != 0 || CAST_TUPLE(stkwargs)->size != 0)){
                        add_instruction(compiler, compiler->instructions,CALL_FUNCTION_BOTTOM_KW_U, argc, GET_ANNO_N(expr));
                    }
                    else if ((CAST_TUPLE(kwargs)->size != 0)){
                        add_instruction(compiler, compiler->instructions,CALL_FUNCTION_BOTTOM_KW, argc, GET_ANNO_N(expr));
                    }
                    else if ((CAST_TUPLE(stargs)->size != 0 || CAST_TUPLE(stkwargs)->size != 0)){
                        add_instruction(compiler, compiler->instructions,CALL_FUNCTION_BOTTOM_U, argc, GET_ANNO_N(expr));
                    }
                    else{
                        add_instruction(compiler, compiler->instructions,CALL_FUNCTION_BOTTOM, argc, GET_ANNO_N(expr));
                    }

                    if (!compiler->keep_return){
                        add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                    }
                    continue;
                }     
                add_instruction(compiler, compiler->instructions,LOAD_ATTR, idx, GET_ANNO_N(expr));
            }
            break;
        }        

        case N_RETURN: {
            int cmpexpr=compile_expr_keep(compiler, RETURN(expr->node)->node);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            
            add_instruction(compiler, compiler->instructions, RETURN_VAL, 0, GET_ANNO_N(expr));
            break;
        }

        case N_LIST: {
            for (size_t i=LIST(expr->node)->list->size(); i>0; i--){
                int cmpexpr=compile_expr_keep(compiler, LIST(expr->node)->list->at(i-1));
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            add_instruction(compiler, compiler->instructions, BUILD_LIST, LIST(expr->node)->list->size(), GET_ANNO_N(expr));
            break;
        }

        case N_TUPLE: {
            for (size_t i=LIST(expr->node)->list->size(); i>0; i--){
                int cmpexpr=compile_expr_keep(compiler, LIST(expr->node)->list->at(i-1));
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            add_instruction(compiler, compiler->instructions, BUILD_TUPLE, LIST(expr->node)->list->size(), GET_ANNO_N(expr));
            break;
        }

        case N_DICT: {
            for (size_t i=DICT(expr->node)->keys->size(); i>0; i--){
                int cmpexpr=compile_expr_keep(compiler, DICT(expr->node)->vals->at(i-1)); //Value
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                cmpexpr=compile_expr_keep(compiler, DICT(expr->node)->keys->at(i-1)); //Value
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            add_instruction(compiler, compiler->instructions, BUILD_DICT, DICT(expr->node)->keys->size(), GET_ANNO_N(expr));
            break;
        }

        case N_IF: {
            int startcol=IF(expr->node)->expr->start->col;
            int endcol=IF(expr->node)->expr->end->col;
            int startline=IF(expr->node)->expr->start->line;
            int cmpexpr=compile_expr_keep(compiler, IF(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }

            add_instruction(compiler, compiler->instructions,POP_JMP_TOS_FALSE, num_instructions(IF(expr->node)->code, compiler->scope)*2, startcol, endcol, startline); 

            //Code
            for (Node* n: (*IF(expr->node)->code)){
                uint32_t start=compiler->instructions->count*2;
                long line=n->start->line;
                int _stack_size=compiler->stack_size;
                compiler->stack_size=0;
                int _blockstack_size=compiler->blockstack_size;
                compiler->blockstack_size=0;
                int i=compile_expr(compiler, n);
                compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count*2;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }
            }

            add_instruction(compiler, compiler->instructions,JUMP_DELTA,0, GET_ANNO_N(expr));
            
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
                uint32_t start=compiler->instructions->count*2;
                long line=n->start->line;
                int _stack_size=compiler->stack_size;
                compiler->stack_size=0;
                int _blockstack_size=compiler->blockstack_size;
                compiler->blockstack_size=0;
                int i=compile_expr(compiler, n);
                compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count*2;
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
                    instrs+=num_instructions_keep(IF(n->node)->expr, compiler->scope)*2;
                    instrs+=4;
                    
                    int startcol=IF(n->node)->expr->start->col;
                    int endcol=IF(n->node)->expr->end->col;
                    int startline=IF(n->node)->expr->start->line;
                    int cmpexpr=compile_expr_keep(compiler, IF(n->node)->expr);
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    
                    add_instruction(compiler, compiler->instructions,POP_JMP_TOS_FALSE, num_instructions(IF(n->node)->code, compiler->scope)*2+2, startcol, endcol, startline);

                    //Code
                    for (Node* n: (*IF(n->node)->code)){
                        uint32_t start=compiler->instructions->count*2;
                        long line=n->start->line;
                        int _stack_size=compiler->stack_size;
                        compiler->stack_size=0;
                        int _blockstack_size=compiler->blockstack_size;
                        compiler->blockstack_size=0;
                        int i=compile_expr(compiler, n);
                        compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                        if (i==0x100){
                            return 0x100;
                        }
                        uint32_t end=compiler->instructions->count*2;
                        if (compiler->lines!=NULL && i!=0x200){
                            object* tuple=new_tuple();
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                            compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                        }
                    }
                    add_instruction(compiler, compiler->instructions,JUMP_DELTA,target-instrs, GET_ANNO_N(n));
                }
                if (n->type==N_ELSE){
                    instrs+=num_instructions(ELSE(n->node)->code, compiler->scope)*2;
                    
                    //Code
                    for (Node* n: (*ELSE(n->node)->code)){
                        uint32_t start=compiler->instructions->count*2;
                        long line=n->start->line;
                        int _stack_size=compiler->stack_size;
                        compiler->stack_size=0;
                        int _blockstack_size=compiler->blockstack_size;
                        compiler->blockstack_size=0;
                        int i=compile_expr(compiler, n);
                        compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                        if (i==0x100){
                            return 0x100;
                        }
                        uint32_t end=compiler->instructions->count*2;
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
            return 0x200;
        }

        case N_SUBSCR: {
            int cmpexpr=compile_expr_keep(compiler, SUBSCR(expr->node)->left);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            cmpexpr=compile_expr_keep(compiler, SUBSCR(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            add_instruction(compiler, compiler->instructions,BINOP_SUBSCR, 0, GET_ANNO_N(expr));
            

            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
            }
            break;
        }

        case N_RAISE: {
            int cmpexpr=compile_expr_keep(compiler, RAISE(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            add_instruction(compiler, compiler->instructions, RAISE_EXC, 0, GET_ANNO_N(expr));
            break;
        }

        case N_STORE_SUBSCR: {
            int cmpexpr=compile_expr_keep(compiler, SUBSCR(STSUBSCR(expr->node)->left->node)->left);
            if (cmpexpr==0x100){
                return cmpexpr;
            }

            cmpexpr=compile_expr_keep(compiler, SUBSCR(STSUBSCR(expr->node)->left->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            cmpexpr=compile_expr_keep(compiler, STSUBSCR(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            
            add_instruction(compiler, compiler->instructions,STORE_SUBSCR, 0, GET_ANNO_N(expr));
            break;
        }

        case N_TRY: {
            for (Node* n: (*TRY(expr->node)->code)){
                uint32_t start=compiler->instructions->count*2;
                long line=n->start->line;
                int _stack_size=compiler->stack_size;
                compiler->stack_size=0;
                int _blockstack_size=compiler->blockstack_size;
                compiler->blockstack_size=0;
                int i=compile_expr(compiler, n);
                compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count*2;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }
            }
            add_instruction(compiler, compiler->instructions,JUMP_DELTA,0, GET_ANNO_N(expr));
            break;
        }

        case N_FINALLY: {
            add_instruction(compiler, compiler->instructions,CLEAR_EXC,0, GET_ANNO_N(expr)); 
            
            for (Node* n: (*FINALLY(expr->node)->code)){
                uint32_t start=compiler->instructions->count*2;
                long line=n->start->line;        
                int _stack_size=compiler->stack_size;
                compiler->stack_size=0;
                int _blockstack_size=compiler->blockstack_size;
                compiler->blockstack_size=0;
                int i=compile_expr(compiler, n);
                compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count*2;
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

                uint32_t start=compiler->instructions->count*2;
                
                switch (compiler->scope){
                    case SCOPE_GLOBAL:
                        add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(EXCEPT(expr->node)->name));
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(EXCEPT(expr->node)->name));
                        break;
                }

                uint32_t end=compiler->instructions->count*2;
                if (compiler->lines!=NULL){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(EXCEPT(expr->node)->name->start->line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }
                
                    
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(EXCEPT(expr->node)->name));
            }    

            if (EXCEPT(expr->node)->type!=NULL){
                    add_instruction(compiler, compiler->instructions,DUP_TOS,0, GET_ANNO_N(EXCEPT(expr->node)->type)); //For possible pop_jump

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
                            add_instruction(compiler, compiler->instructions,LOAD_GLOBAL, idx, GET_ANNO_N(EXCEPT(expr->node)->type));
                            break;

                        case SCOPE_LOCAL:
                            add_instruction(compiler, compiler->instructions,LOAD_NAME, idx, GET_ANNO_N(EXCEPT(expr->node)->type));
                            break;
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_EE,0, GET_ANNO_N(EXCEPT(expr->node)->type));                

                    add_instruction(compiler, compiler->instructions,POP_JMP_TOS_FALSE,0, GET_ANNO_N(EXCEPT(expr->node)->type));
                }

            for (Node* n: (*EXCEPT(expr->node)->code)){
                uint32_t start=compiler->instructions->count*2;
                long line=n->start->line;        
                int _stack_size=compiler->stack_size;
                compiler->stack_size=0;
                int _blockstack_size=compiler->blockstack_size;
                compiler->blockstack_size=0;
                int i=compile_expr(compiler, n);
                compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count*2;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }
            }

            add_instruction(compiler, compiler->instructions,JUMP_DELTA,0, GET_ANNO_N(expr));

            break;
        }

        case N_TRY_EXCEPT_FINALLY: {
            uint32_t val=compiler->instructions->count*2+num_instructions(TRYEXCEPTFINALLY(expr->node)->bases->at(0), compiler->scope)*2;
            add_instruction(compiler, compiler->instructions,SETUP_TRY,val, GET_ANNO_N(expr));       
                 
            uint32_t target;
            if (TRYEXCEPTFINALLY(expr->node)->bases->back()->type==N_FINALLY){
                target=(num_instructions(TRYEXCEPTFINALLY(expr->node)->bases, compiler->scope)*2)-(num_instructions(TRYEXCEPTFINALLY(expr->node)->bases->back(), compiler->scope)*2);
            }
            else{
                target=num_instructions(TRYEXCEPTFINALLY(expr->node)->bases, compiler->scope)*2+2;
            }

            uint32_t instrs=0;
            for (uint32_t i=0; i<TRYEXCEPTFINALLY(expr->node)->bases->size(); i++){
                if (i==0){ //Add try block
                    Node* tryn=TRYEXCEPTFINALLY(expr->node)->bases->at(0);
                    instrs+=num_instructions(TRY(tryn->node)->code, compiler->scope)*2;
                    instrs+=2;
                    
                    for (Node* n: (*TRY(tryn->node)->code)){
                        uint32_t start=compiler->instructions->count*2;
                        long line=n->start->line;
                        int _stack_size=compiler->stack_size;
                        compiler->stack_size=0;
                        int _blockstack_size=compiler->blockstack_size;
                        compiler->blockstack_size=0;
                        int i=compile_expr(compiler, n);
                        compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                        if (i==0x100){
                            return 0x100;
                        }
                        uint32_t end=compiler->instructions->count*2;
                        if (compiler->lines!=NULL && i!=0x200){
                            object* tuple=new_tuple();
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                            compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                        }
                    }

                    add_instruction(compiler, compiler->instructions,JUMP_DELTA,target-instrs, GET_ANNO_N(tryn));
                    continue;
                }
                else if (i==TRYEXCEPTFINALLY(expr->node)->bases->size()-1 && TRYEXCEPTFINALLY(expr->node)->bases->at(i)->type==N_FINALLY){                    
                    add_instruction(compiler, compiler->instructions,CLEAR_EXC,0, GET_ANNO_N(expr)); 
                    instrs+=2;
                    
                    Node* tryn=TRYEXCEPTFINALLY(expr->node)->bases->at(i);
                    instrs+=num_instructions(FINALLY(tryn->node)->code, compiler->scope)*2;
                    
                    for (Node* n: (*FINALLY(tryn->node)->code)){
                        uint32_t start=compiler->instructions->count*2;
                        long line=n->start->line;
                        int _stack_size=compiler->stack_size;
                        compiler->stack_size=0;
                        int _blockstack_size=compiler->blockstack_size;
                        compiler->blockstack_size=0;
                        int i=compile_expr(compiler, n);
                        compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                        if (i==0x100){
                            return 0x100;
                        }
                        uint32_t end=compiler->instructions->count*2;
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

                if (EXCEPT(tryn->node)->type!=NULL){
                    add_instruction(compiler, compiler->instructions,DUP_TOS,0, GET_ANNO_N(EXCEPT(tryn->node)->type)); //For possible pop_jump
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

                    uint32_t start=compiler->instructions->count*2;
                    
                    switch (compiler->scope){
                        case SCOPE_GLOBAL:
                            add_instruction(compiler, compiler->instructions,LOAD_GLOBAL, idx, GET_ANNO_N(EXCEPT(tryn->node)->type));
                            break;

                        case SCOPE_LOCAL:
                            add_instruction(compiler, compiler->instructions,LOAD_NAME, idx, GET_ANNO_N(EXCEPT(tryn->node)->type));
                            break;
                    }
                    instrs+=2;

                    uint32_t end=compiler->instructions->count*2;
                    if (compiler->lines!=NULL && i!=0x200){
                        object* tuple=new_tuple();
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(EXCEPT(tryn->node)->type->start->line));
                        compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                    }

                    add_instruction(compiler, compiler->instructions,BINOP_EXC_CMP,0, GET_ANNO_N(EXCEPT(tryn->node)->type));         
                    instrs+=2;

                    add_instruction(compiler, compiler->instructions,POP_JMP_TOS_FALSE,num_instructions(EXCEPT(tryn->node)->code, compiler->scope)*2+4, GET_ANNO_N(EXCEPT(tryn->node)->type));
                    instrs+=2;
                }

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

                    uint32_t start=compiler->instructions->count*2;
                    
                    switch (compiler->scope){
                        case SCOPE_GLOBAL:
                            add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(EXCEPT(tryn->node)->name));
                            break;

                        case SCOPE_LOCAL:
                            add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(EXCEPT(tryn->node)->name));
                            break;
                    }
                    instrs+=2;

                    uint32_t end=compiler->instructions->count*2;
                    if (compiler->lines!=NULL && i!=0x200){
                        object* tuple=new_tuple();
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(EXCEPT(tryn->node)->name->start->line));
                        compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                    }
                    
                    add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(EXCEPT(tryn->node)->name));
                }    
                
                for (Node* n: (*EXCEPT(tryn->node)->code)){
                    uint32_t start=compiler->instructions->count*2;
                    long line=n->start->line;
                    int _stack_size=compiler->stack_size;
                    compiler->stack_size=0;
                    int _blockstack_size=compiler->blockstack_size;
                    compiler->blockstack_size=0;
                    int i=compile_expr(compiler, n);
                    compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                    if (i==0x100){
                        return 0x100;
                    }
                    uint32_t end=compiler->instructions->count*2;
                    if (compiler->lines!=NULL && i!=0x200){
                        object* tuple=new_tuple();
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                        compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                    }
                }            
                    
                instrs+=2;
                add_instruction(compiler, compiler->instructions,JUMP_DELTA,target-instrs, GET_ANNO_N(tryn));
            }
            if (TRYEXCEPTFINALLY(expr->node)->bases->back()->type!=N_FINALLY){
                //add_instruction(compiler, compiler->instructions,JUMP_DELTA,0, GET_ANNO_N(expr));
                add_instruction(compiler, compiler->instructions,RAISE_EXC,0, GET_ANNO_N(expr)); 
            }
            add_instruction(compiler, compiler->instructions,FINISH_TRY,0, GET_ANNO_N(expr));  
               
            
            
            return 0x200;
        }

        case N_FOR: {

            uint32_t start_=compiler->instructions->count*2;
            bool ret=compiler->keep_return;
            long line=FOR(expr->node)->expr->start->line;

            int startcol=FOR(expr->node)->expr->start->col;
            int endcol=FOR(expr->node)->expr->end->col;
            int startline=FOR(expr->node)->expr->start->line;

            int cmpexpr=compile_expr_keep(compiler, FOR(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            
            if (compiler->lines!=NULL && cmpexpr!=0x200){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            } 
            
            add_instruction(compiler, compiler->instructions,EXTRACT_ITER,0, startcol, endcol, startline);
            size_t target=0;
            
            uint32_t start=compiler->instructions->count*2;

            size_t n_elsen=0;
            if (FOR(expr->node)->elsen!=NULL){
                n_elsen=num_instructions(ELSE(FOR(expr->node)->elsen->node)->code, compiler->scope)*2;
                target+=n_elsen;
            }
            
            if (FOR(expr->node)->ident->type!=N_MULTIIDENT){
                target+=compiler->instructions->count*2+num_instructions(FOR(expr->node)->code, compiler->scope)*2+num_instructions(FOR(expr->node)->ident, compiler->scope)*2+6;
            }
            else{
                target+=compiler->instructions->count*2+num_instructions(FOR(expr->node)->code, compiler->scope)*2+8+num_instructions(FOR(expr->node)->ident, compiler->scope)*2;
            }
            
            add_instruction(compiler, compiler->instructions,FOR_TOS_ITER, target-n_elsen, GET_ANNO_N(expr));
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
                        add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(FOR(expr->node)->ident));
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(FOR(expr->node)->ident));
                        break;
                }
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(FOR(expr->node)->ident));
            }
            else if (FOR(expr->node)->ident->type==N_MULTIIDENT){
                add_instruction(compiler, compiler->instructions,UNPACK_SEQ, MULTIIDENT(FOR(expr->node)->ident->node)->name->size(), GET_ANNO_N(FOR(expr->node)->ident));
                int cmpexpr=compile_expr_keep(compiler, FOR(expr->node)->ident);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(FOR(expr->node)->ident));
            }
            
            //Code
            for (Node* n: (*FOR(expr->node)->code)){
                uint32_t start=compiler->instructions->count*2;
                long line=n->start->line;
                int _stack_size=compiler->stack_size;
                compiler->stack_size=0;
                int _blockstack_size=compiler->blockstack_size;
                compiler->blockstack_size=0;
                int i=compile_expr(compiler, n);
                compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count*2;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }            
            }

            add_instruction(compiler, compiler->instructions,JUMP_TO,start, GET_ANNO_N(expr)); 
            
            if (FOR(expr->node)->elsen!=NULL){
                //Code
                for (Node* n: (*ELSE(FOR(expr->node)->elsen->node)->code)){
                    uint32_t start=compiler->instructions->count*2;
                    long line=n->start->line;
                    int _stack_size=compiler->stack_size;
                    compiler->stack_size=0;
                    int _blockstack_size=compiler->blockstack_size;
                    compiler->blockstack_size=0;
                    int i=compile_expr(compiler, n);
                    compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                    if (i==0x100){
                        return 0x100;
                    }
                    uint32_t end=compiler->instructions->count*2;
                    if (compiler->lines!=NULL && i!=0x200){
                        object* tuple=new_tuple();
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                        compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                    }
                }   
            }

            
            
            return 0x200;
        }

        case N_BREAK: {
            add_instruction(compiler, compiler->instructions,BREAK_LOOP,0, GET_ANNO_N(expr)); 
            break;
        }

        case N_CONTINUE: {
            add_instruction(compiler, compiler->instructions,CONTINUE_LOOP,0 , GET_ANNO_N(expr)); 
            break;
        }

        case N_WHILE: {
            size_t target=0;
            
            size_t n_elsen=0;
            if (WHILE(expr->node)->elsen!=NULL){
                n_elsen=num_instructions(ELSE(WHILE(expr->node)->elsen->node)->code, compiler->scope)*2;
                target+=n_elsen;
            }
            target+=compiler->instructions->count*2+num_instructions(WHILE(expr->node)->code, compiler->scope)*2+8+num_instructions(WHILE(expr->node)->expr, compiler->scope)*2;
            
            add_instruction(compiler, compiler->instructions,ENTER_WHILE,target, GET_ANNO_N(expr));

            //Checks here
            uint32_t start=compiler->instructions->count*2;
            uint32_t start_=start;
            long line=WHILE(expr->node)->expr->start->line;

            int startcol=WHILE(expr->node)->expr->start->col;
            int endcol=WHILE(expr->node)->expr->end->col;
            int startline=WHILE(expr->node)->expr->start->line;

            int cmpexpr=compile_expr_keep(compiler, WHILE(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            } 
            
            add_instruction(compiler, compiler->instructions,POP_JMP_TOS_FALSE,num_instructions(WHILE(expr->node)->code, compiler->scope)*2+2, startcol, endcol, startline);
            
            if (compiler->lines!=NULL && cmpexpr!=0x200){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            } 

            
            //Code
            uint32_t idx=0;
            for (Node* n: (*WHILE(expr->node)->code)){
                uint32_t start=compiler->instructions->count*2;
                long line=n->start->line;
                int _stack_size=compiler->stack_size;
                compiler->stack_size=0;
                int _blockstack_size=compiler->blockstack_size;
                compiler->blockstack_size=0;
                int i=compile_expr(compiler, n);
                compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count*2;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }     
                idx++;       
            }
            
            add_instruction(compiler, compiler->instructions,JUMP_TO,start, GET_ANNO_N(expr));

            add_instruction(compiler, compiler->instructions,EXIT_WHILE,0, GET_ANNO_N(expr));
            
            if (WHILE(expr->node)->elsen!=NULL){
                //Code
                for (Node* n: (*ELSE(WHILE(expr->node)->elsen->node)->code)){
                    uint32_t start=compiler->instructions->count*2;
                    long line=n->start->line;
                    int _stack_size=compiler->stack_size;
                    compiler->stack_size=0;
                    int _blockstack_size=compiler->blockstack_size;
                    compiler->blockstack_size=0;
                    int i=compile_expr(compiler, n);
                    compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                    if (i==0x100){
                        return 0x100;
                    }
                    uint32_t end=compiler->instructions->count*2;
                    if (compiler->lines!=NULL && i!=0x200){
                        object* tuple=new_tuple();
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                        tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                        compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                    }
                }   
            }

            
            
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
                
                add_instruction(compiler, compiler->instructions, IMPORT_NAME, idx, GET_ANNO_N(libname));

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
                        add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(libname));
                        break;

                    case SCOPE_LOCAL:
                        add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(libname));
                        break;
                }
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(libname));
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

            add_instruction(compiler, compiler->instructions, IMPORT_NAME, idx, GET_ANNO_N(FROM(expr->node)->name));
            

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

            add_instruction(compiler, compiler->instructions, LOAD_CONST, idx, GET_ANNO_N(expr));

            add_instruction(compiler, compiler->instructions, IMPORT_FROM_MOD, 0, GET_ANNO_N(expr));
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


            int cmpexpr=compile_expr_keep(compiler, base);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (type==3){
                cmpexpr=compile_expr_keep(compiler, left);
                cmpexpr=compile_expr_keep(compiler, right);
                add_instruction(compiler, compiler->instructions, MAKE_SLICE, 0, GET_ANNO_N(expr));
            }
            else if (type==2){
                int cmpexpr=compile_expr_keep(compiler, left);
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
                
                add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
                add_instruction(compiler, compiler->instructions, MAKE_SLICE, 0, GET_ANNO_N(expr));
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
                
                add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
                int cmpexpr=compile_expr(compiler, right);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                add_instruction(compiler, compiler->instructions, MAKE_SLICE, 0, GET_ANNO_N(expr));
            }
            else{
                //Type 4 needs nothing
                break;
            }

            add_instruction(compiler, compiler->instructions, BINOP_SUBSCR, 0, GET_ANNO_N(expr));
            

            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
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


            int cmpexpr=compile_expr_keep(compiler, base);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (type==3){
                cmpexpr=compile_expr_keep(compiler, left);
                cmpexpr=compile_expr_keep(compiler, right);
                add_instruction(compiler, compiler->instructions, MAKE_SLICE, 0, GET_ANNO_N(expr));
            }
            else if (type==2){
                int cmpexpr=compile_expr_keep(compiler, left);
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
                
                add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
                add_instruction(compiler, compiler->instructions, MAKE_SLICE, 0, GET_ANNO_N(expr));
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
                
                add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
                int cmpexpr=compile_expr_keep(compiler, right);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                add_instruction(compiler, compiler->instructions, MAKE_SLICE, 0, GET_ANNO_N(expr));
            }
            else{
                //Type 4 needs nothing
                break;
            }
            
            cmpexpr=compile_expr_keep(compiler, STSLICE(expr->node)->expr);
            
            add_instruction(compiler, compiler->instructions,STORE_SUBSCR, 0, GET_ANNO_N(expr));

            

            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
            }
            break;
        }

        case N_DEL: {
            if (DEL(expr->node)->expr->type==N_SUBSCR){
                Node* subexpr=DEL(expr->node)->expr;
                
                int cmpexpr=compile_expr_keep(compiler, SUBSCR(subexpr->node)->left);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            
                cmpexpr=compile_expr_keep(compiler, SUBSCR(subexpr->node)->expr);
                
                add_instruction(compiler, compiler->instructions,DEL_SUBSCR, 0, GET_ANNO_N(expr));
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

                int cmpexpr=compile_expr_keep(compiler, base);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                if (type==3){
                    cmpexpr=compile_expr_keep(compiler, left);
                    cmpexpr=compile_expr_keep(compiler, right);
                    add_instruction(compiler, compiler->instructions, MAKE_SLICE, 0, GET_ANNO_N(expr));
                }
                else if (type==2){
                    int cmpexpr=compile_expr_keep(compiler, left);
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
                    
                    add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
                    add_instruction(compiler, compiler->instructions, MAKE_SLICE, 0, GET_ANNO_N(expr));
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
                    
                    add_instruction(compiler, compiler->instructions,LOAD_CONST,idx, GET_ANNO_N(expr));
                    int cmpexpr=compile_expr_keep(compiler, right);
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                    add_instruction(compiler, compiler->instructions, MAKE_SLICE, 0, GET_ANNO_N(expr));
                }
                else{
                    //Type 4 needs nothing
                    break;
                }

                add_instruction(compiler, compiler->instructions, DEL_SUBSCR, 0, GET_ANNO_N(expr));
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
                add_instruction(compiler, compiler->instructions, DEL_GLBL, idx, GET_ANNO_N(GLBLIDENT(DEL(expr->node)->expr->node)->name));
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
                add_instruction(compiler, compiler->instructions, DEL_NONLOCAL, idx, GET_ANNO_N(DEL(expr->node)->expr));
            }
            else if (DEL(expr->node)->expr->type==N_DOT){
                vector<Node*>* names=DOT(DEL(expr->node)->expr->node)->names;
                compile_expr_keep(compiler, names->at(0));

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
                        add_instruction(compiler, compiler->instructions,DEL_ATTR, idx, GET_ANNO_N(names->at(i)));
                        continue;
                    }     

                    add_instruction(compiler, compiler->instructions,LOAD_ATTR, idx, GET_ANNO_N(names->at(i)));
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
                add_instruction(compiler, compiler->instructions, DEL_NAME, idx, GET_ANNO_N((DEL(expr->node)->expr)));
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
                    int starti=i;
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

                    }
                    if (data[i]!='\0'){
                        i++;
                    }
                    
                    Lexer lexer(segment,kwds);
                    lexer.pos=Position(program, 0, starti+expr->start->col+2, expr->start->line);

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

                    int cmpexpr=compile_expr_keep(compiler, ast.nodes.at(0));
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
                    add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(expr));
                }
                else{
                    string segment="";
                    while (data[i]!='{' && data[i]!='\0'){
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
                    add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(expr));

                    if (!object_find_bool(compiler->consts,falseobj)){
                        //Create object
                        compiler->consts->type->slot_mappings->slot_append(compiler->consts, new_bool_false());
                        idx=NAMEIDX(compiler->consts);
                    }
                    else{
                        idx=object_find(compiler->consts, new_bool_false());
                    }
                    
                    add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(expr));
                }  
            }
            add_instruction(compiler, compiler->instructions,BUILD_STRING, x, GET_ANNO_N(expr));
            break;
        }

        case N_ASSERT: {
            compile_expr_keep(compiler, ASSERT(expr->node)->expr);
            add_instruction(compiler, compiler->instructions,POP_JMP_TOS_TRUE, 2, GET_ANNO_N(expr));
            add_instruction(compiler, compiler->instructions,RAISE_ASSERTIONERR, 0, GET_ANNO_N(expr));
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
            
            add_instruction(compiler, compiler->instructions,LOAD_NONLOCAL, idx, GET_ANNO_N(expr));
            break;
        }

        case N_TERNARY: {
            compile_expr_keep(compiler,TERNARY(expr->node)->left);
            compile_expr_keep(compiler,TERNARY(expr->node)->expr2);
            compile_expr_keep(compiler,TERNARY(expr->node)->expr1);
            
            add_instruction(compiler, compiler->instructions,TERNARY_TEST, 0, GET_ANNO_N(expr));
            break;
        }

        case N_DECORATOR: {
            vector<Decorator*> decorators;
            decorators.clear();
            
            decorators.push_back(DECORATOR(expr->node));

            if (DECORATOR(expr->node)->decorator!=NULL){
                Decorator* ptr=DECORATOR(DECORATOR(expr->node)->decorator->node); //Next
                while (ptr){
                    decorators.push_back(ptr);
                    ptr=DECORATOR(ptr->decorator);
                    if (ptr!=NULL){
                        ptr=DECORATOR(((Node*)ptr)->node);
                    }
                }
            }

            for (size_t i_=decorators.size(); i_>0; i_--){
                size_t i=i_-1;
                compile_expr_keep(compiler, DECORATOR(decorators.at(i))->name);
            }
            
            //Make a FUNCTION!
            
            //Name
            uint32_t nameidx;
            
            if (!_list_contains(compiler->consts, IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->name->node)->name )){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->name->node)->name));
                nameidx = NAMEIDX(compiler->consts);
            }
            else{
                nameidx=object_find(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->name->node)->name));
            }
            
            
            add_instruction(compiler, compiler->instructions,LOAD_CONST, nameidx, GET_ANNO_N(FUNCT(DECORATOR(decorators.back())->function->node)->name));

            //Arguments
            size_t argc=0;

            object* args=new_tuple();
            object* kwargs=new_tuple();

            //Setup args
            for (Node* n: (*FUNCT(DECORATOR(decorators.back())->function->node)->args)){
                argc++;
                if (n->type==N_IDENT){
                    args->type->slot_mappings->slot_append(args, str_new_fromstr(*IDENTI(n->node)->name));
                }
                else{
                    args->type->slot_mappings->slot_append(args, str_new_fromstr(*ANONIDENT(n->node)->name));
                }
            }
            //
            
            //Setup kwargs (code object) backwards
            for (Node* n: (*FUNCT(DECORATOR(decorators.back())->function->node)->kwargs)){
                if (n->type==N_ASSIGN){
                    args->type->slot_mappings->slot_append(args, str_new_fromstr(*IDENTI(ASSIGN(n->node)->name->node)->name));
                }
                else{
                    args->type->slot_mappings->slot_append(args, str_new_fromstr(*ANONIDENT(ASSIGN(n->node)->name->node)->name));
                }
            }

            for (auto n =  (*FUNCT(DECORATOR(decorators.back())->function->node)->kwargs).rbegin(); n != (*FUNCT(DECORATOR(decorators.back())->function->node)->kwargs).rend(); ++n){
                argc++;
                if ((*n)->type==N_ASSIGN){
                    int cmpexpr=compile_expr_keep(compiler, ASSIGN((*n)->node)->right);
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                }
                else{
                    int cmpexpr=compile_expr_keep(compiler, ASSIGN((*n)->node)->right);
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }
                }
            }
            
            //
            add_instruction(compiler, compiler->instructions,BUILD_TUPLE, FUNCT(DECORATOR(decorators.back())->function->node)->kwargs->size(), GET_ANNO_N(expr));
            
            uint32_t idx;
            if (!object_find_bool(compiler->consts, args)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, args);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, args);
            }
            add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(DECORATOR(decorators.back())->function));
            

            //


            

            //Annotations
            uint32_t n_anno=0;
            if (FUNCT(DECORATOR(decorators.back())->function->node)->rettp!=NULL){
                n_anno++;

                int startcol=FUNCT(DECORATOR(decorators.back())->function->node)->rettp->start->col;
                int endcol=FUNCT(DECORATOR(decorators.back())->function->node)->rettp->end->col;
                int startln=FUNCT(DECORATOR(decorators.back())->function->node)->rettp->start->line;

                int cmpexpr=compile_expr_keep(compiler, FUNCT(DECORATOR(decorators.back())->function->node)->rettp); //Push data
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                
                uint32_t idx;
                string* s=new string("return");
                if (!_list_contains(compiler->consts, s)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr("return"));
                    idx = NAMEIDX(compiler->consts);
                }
                else{
                    idx=object_find(compiler->consts, str_new_fromstr("return"));
                }
                delete s;
                add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, startcol, endcol, startln);
            }

            for (Node* n: (*FUNCT(DECORATOR(decorators.back())->function->node)->args)){
                if (n->type!=N_IDENT){
                    n_anno++;
                    
                    int startcol=FUNCT(DECORATOR(decorators.back())->function->node)->rettp->start->col;
                    int endcol=FUNCT(DECORATOR(decorators.back())->function->node)->rettp->end->col;
                    int startln=FUNCT(DECORATOR(decorators.back())->function->node)->rettp->start->line;

                    int cmpexpr=compile_expr_keep(compiler, ANONIDENT(n->node)->tp); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    uint32_t idx;
                    if (!_list_contains(compiler->consts, ANONIDENT(n->node)->name )){
                        //Create object
                        compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*ANONIDENT(n->node)->name));
                        idx = NAMEIDX(compiler->consts);
                    }
                    else{
                        idx=object_find(compiler->consts, str_new_fromstr(*ANONIDENT(n->node)->name));
                    }
                    add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, startcol, endcol, startln);
                }
            }
            
            for (Node* n: (*FUNCT(DECORATOR(decorators.back())->function->node)->kwargs)){
                if (n->type!=N_ASSIGN){
                    n_anno++; 

                    int startcol=FUNCT(DECORATOR(decorators.back())->function->node)->rettp->start->col;
                    int endcol=FUNCT(DECORATOR(decorators.back())->function->node)->rettp->end->col;
                    int startln=FUNCT(DECORATOR(decorators.back())->function->node)->rettp->start->line;
                    
                    int cmpexpr=compile_expr_keep(compiler, ANONIDENT(ASSIGN(n->node)->name->node)->tp); //Push data
                    if (cmpexpr==0x100){
                        return cmpexpr;
                    }

                    uint32_t idx;
                    if (!_list_contains(compiler->consts, ANONIDENT(ASSIGN(n->node)->name->node)->name)){
                        //Create object
                        compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*ANONIDENT(ASSIGN(n->node)->name->node)->name));
                        idx = NAMEIDX(compiler->consts);
                    }
                    else{
                        idx=object_find(compiler->consts, str_new_fromstr(*ANONIDENT(ASSIGN(n->node)->name->node)->name));
                    }
                    add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, startcol, endcol, startln);
                }
            }
            add_instruction(compiler, compiler->instructions,BUILD_DICT, n_anno, GET_ANNO_N(DECORATOR(decorators.back())->function));

            //
            

            //

            //Code
            parse_ret c;
            c.nodes=(*FUNCT(DECORATOR(decorators.back())->function->node)->code);
            struct compiler* comp=new_compiler();
            comp->scope=SCOPE_LOCAL;
            struct compiler* compiler_=compiler;
            compiler=comp;
            object* code=compile(comp, c, DECORATOR(decorators.back())->function->start->line);
            if (code==NULL){
                return 0x100;
            }
            compiler=compiler_;
            compile_expr_cleanup(&compiler->stack_size, &comp->stack_size, &compiler->blockstack_size, &comp->blockstack_size);

            bool isgen=false;
            for (int i=0; i<CAST_LIST(CAST_CODE(code)->co_code)->size; i+=2){
                if (*CAST_INT(list_index_int(CAST_CODE(code)->co_code, i))->val==YIELD_VALUE){
                    isgen=true;
                    break;
                }
            }
            
            object* lines=new_list();
            
            object* tuple=new_tuple();
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(0));
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(0));
            tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(DECORATOR(decorators.back())->function->start->line));
            
            lines->type->slot_mappings->slot_append(lines, tuple);
            for (int i=0; i<CAST_LIST(CAST_CODE(code)->co_lines)->size; i++){
                lines->type->slot_mappings->slot_append(lines, list_index_int(CAST_CODE(code)->co_lines, i));
            }
            FPLDECREF(CAST_CODE(code)->co_lines);
            CAST_CODE(code)->co_lines=lines; //No FPLINCREF necessary

            FPLDECREF(CAST_CODE(code)->co_file);
            CAST_CODE(code)->co_file=object_repr(str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->name->node)->name));
            
            
            if (!object_find_bool(compiler->consts, code)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, code);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, code);
            }
            add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(DECORATOR(decorators.back())->function));

            //Star args/kwargs
            int star=FUNC_STRICTARGS;
            uint32_t star_kwargs;
            uint32_t star_args;
            if (FUNCT(DECORATOR(decorators.back())->function->node)->starargs && FUNCT(DECORATOR(decorators.back())->function->node)->starkwargs){
                star=FUNC_STAR;
                if (!_list_contains(compiler->consts, IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stkwargs->node)->name)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stkwargs->node)->name));
                    star_kwargs = NAMEIDX(compiler->consts);
                }
                else{
                    star_kwargs=object_find(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stkwargs->node)->name));
                }
                
                if (!_list_contains(compiler->consts, IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stargs->node)->name)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stargs->node)->name));
                    star_args = NAMEIDX(compiler->consts);
                }
                else{
                    star_args=object_find(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stargs->node)->name));
                }
                add_instruction(compiler, compiler->instructions,LOAD_CONST, star_args, GET_ANNO_N(FUNCT(DECORATOR(decorators.back())->function->node)->stargs));
                add_instruction(compiler, compiler->instructions,LOAD_CONST, star_kwargs, GET_ANNO_N(FUNCT(DECORATOR(decorators.back())->function->node)->stkwargs));
            }
            else if(FUNCT(DECORATOR(decorators.back())->function->node)->starargs){
                star=FUNC_STARARGS;
                if (!_list_contains(compiler->consts, IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stargs->node)->name)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stargs->node)->name));
                    star_args = NAMEIDX(compiler->consts);
                }
                else{
                    star_args=object_find(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stargs->node)->name));
                }
                add_instruction(compiler, compiler->instructions,LOAD_CONST, star_args, GET_ANNO_N(FUNCT(DECORATOR(decorators.back())->function->node)->stargs));
            }
            else if(FUNCT(DECORATOR(decorators.back())->function->node)->starkwargs){
                star=FUNC_STARKWARGS;
                if (!_list_contains(compiler->consts, IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stkwargs->node)->name)){
                    //Create object
                    compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stkwargs->node)->name));
                    star_kwargs = NAMEIDX(compiler->consts);
                }
                else{
                    star_kwargs=object_find(compiler->consts, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->stkwargs->node)->name));
                }
                add_instruction(compiler, compiler->instructions,LOAD_CONST, star_kwargs, GET_ANNO_N(FUNCT(DECORATOR(decorators.back())->function->node)->stkwargs));
            }

            object* star_int=new_int_fromint(star);
            if (!object_find_bool(compiler->consts, star_int)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, star_int);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, star_int);
            }
            add_instruction(compiler, compiler->instructions,LOAD_CONST, idx, GET_ANNO_N(DECORATOR(decorators.back())->function));

            //Create callable
            if (compiler->scope!=SCOPE_GLOBAL && !compiler->inclass){
                if (!isgen){
                    add_instruction(compiler, compiler->instructions,MAKE_CLOSURE, argc, GET_ANNO_N(DECORATOR(decorators.back())->function));
                }
                else{
                    add_instruction(compiler, compiler->instructions,MAKE_CLOSURE_GENERATOR, argc, GET_ANNO_N(DECORATOR(decorators.back())->function));                    
                }
            }
            else{
                if (!isgen){
                    add_instruction(compiler, compiler->instructions,MAKE_FUNCTION, argc, GET_ANNO_N(DECORATOR(decorators.back())->function));
                }
                else{
                    add_instruction(compiler, compiler->instructions,MAKE_GENERATOR, argc, GET_ANNO_N(DECORATOR(decorators.back())->function));
                }
            }

            
            //TOS is now the function!
            
            add_instruction(compiler, compiler->instructions,CALL_FUNCTION_BOTTOM, 1, GET_ANNO_N(expr));
            

            for (size_t i_=decorators.size()-1; i_>0; i_--){
                size_t i=i_-1;
                
                add_instruction(compiler, compiler->instructions,CALL_FUNCTION_BOTTOM, 1, GET_ANNO_N(decorators.at(i)->name));
            }
            
            //Store function
            uint32_t nameidxstore;
            if (!_list_contains(compiler->names, IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->name->node)->name )){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->name->node)->name));
                nameidxstore = NAMEIDX(compiler->names);
            }
            else{
                nameidxstore=object_find(compiler->names, str_new_fromstr(*IDENTI(FUNCT(DECORATOR(decorators.back())->function->node)->name->node)->name));
            }
            switch (compiler->scope){
                case SCOPE_GLOBAL:
                    add_instruction(compiler, compiler->instructions,STORE_GLOBAL, nameidxstore, GET_ANNO_N(expr));
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler, compiler->instructions,STORE_NAME, nameidxstore, GET_ANNO_N(expr));
                    break;
            }

            break;
        }

        case N_ANONASSIGN: {
            int cmpexpr=compile_expr_keep(compiler, ASSIGN(expr->node)->right); //Push data
            if (cmpexpr==0x100){
                return cmpexpr;
            }

            uint32_t idx;
            if (ASSIGN(expr->node)->name->type==N_ANONIDENT){
                if (!_list_contains(compiler->names, ANONIDENT(ASSIGN(expr->node)->name->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*ANONIDENT(ASSIGN(expr->node)->name->node)->name));
                    idx=NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*ANONIDENT(ASSIGN(expr->node)->name->node)->name));
                }
                

                switch (compiler->scope){
                    case SCOPE_GLOBAL: {
                        add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
                        
                        cmpexpr=compile_expr_keep(compiler, ANONIDENT(ASSIGN(expr->node)->name->node)->tp);
                        if (cmpexpr==0x100){
                            return cmpexpr;
                        }
                        
                        
                        add_instruction(compiler, compiler->instructions,ANNOTATE_GLOBAL, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
                        break;
                    }

                    case SCOPE_LOCAL: {
                        add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
                        
                        cmpexpr=compile_expr_keep(compiler, ANONIDENT(ASSIGN(expr->node)->name->node)->tp);
                        if (cmpexpr==0x100){
                            return cmpexpr;
                        }
                        
                        add_instruction(compiler, compiler->instructions,ANNOTATE_NAME, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
                        break;
                    }
                }

                if (!compiler->keep_return){
                    add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
                }
            }
            else if (ASSIGN(expr->node)->name->type==N_ANONNONLOCAL){
                if (!_list_contains(compiler->names, ANONIDENT(ASSIGN(expr->node)->name->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*ANONIDENT(ASSIGN(expr->node)->name->node)->name));
                    idx=NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*ANONIDENT(ASSIGN(expr->node)->name->node)->name));
                }
                add_instruction(compiler, compiler->instructions,STORE_NONLOCAL, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
                if (!compiler->keep_return){
                    add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(ASSIGN(expr->node)->name));
                }
                
                int cmpexpr=compile_expr_keep(compiler, ANONIDENT(ASSIGN(expr->node)->name->node)->tp);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                        
                add_instruction(compiler, compiler->instructions,ANNOTATE_NONLOCAL, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
            }
            else if (ASSIGN(expr->node)->name->type==N_ANONGLBL_IDENT){
                if (!_list_contains(compiler->names, ANONIDENT(ASSIGN(expr->node)->name->node)->name)){
                    //Create object
                    compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*ANONIDENT(ASSIGN(expr->node)->name->node)->name));
                    idx=NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(*ANONIDENT(ASSIGN(expr->node)->name->node)->name));
                }
                
                add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
                if (!compiler->keep_return){
                    add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(ASSIGN(expr->node)->name));
                }
                
                int cmpexpr=compile_expr_keep(compiler, ANONIDENT(ASSIGN(expr->node)->name->node)->tp);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                
                        
                add_instruction(compiler, compiler->instructions,ANNOTATE_GLOBAL, idx, GET_ANNO_N(ASSIGN(expr->node)->name));
            }
            else if (ASSIGN(expr->node)->name->type==N_ANONDOT){
                vector<Node*>* names=ANONDOT(ASSIGN(expr->node)->name->node)->names;
                
                int cmpexpr=compile_expr_keep(compiler, names->at(0));
                if (cmpexpr==0x100){
                    return cmpexpr;
                }

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
                        int cmpexpr=compile_expr_keep(compiler, ANONDOT(ASSIGN(expr->node)->name->node)->tp);
                        if (cmpexpr==0x100){
                            return cmpexpr;
                        }

                        add_instruction(compiler, compiler->instructions,STORE_ATTR_ANNOTATE, idx, GET_ANNO_N(names->at(i)));
                        continue;
                    }     
                    add_instruction(compiler, compiler->instructions,LOAD_ATTR, idx, GET_ANNO_N(names->at(i)));
                }
            }
            break;
        }

        case N_YIELD: {
            int cmpexpr=compile_expr_keep(compiler, YIELD(expr->node)->expr); //Push data
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            
            add_instruction(compiler, compiler->instructions,YIELD_VALUE, 0, GET_ANNO_N(expr));
            break;
        }

        case N_SET: {
            for (size_t i=LIST(expr->node)->list->size(); i>0; i--){
                int cmpexpr=compile_expr_keep(compiler, LIST(expr->node)->list->at(i-1));
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
            }
            add_instruction(compiler, compiler->instructions, BUILD_SET, LIST(expr->node)->list->size(), GET_ANNO_N(expr));
            break;
        }

        case N_WITH: {
            int cmpexpr=compile_expr_keep(compiler, WITH(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }

            add_instruction(compiler, compiler->instructions,ENTER_WITH, 0, GET_ANNO_N(expr));

            uint32_t idx;
            if (!_list_contains(compiler->names, STRLIT(WITH(expr->node)->name->node)->literal)){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*STRLIT(WITH(expr->node)->name->node)->literal));
                idx = NAMEIDX(compiler->names);
            }
            else{
                idx=object_find(compiler->names, str_new_fromstr(*STRLIT(WITH(expr->node)->name->node)->literal));
            }

            uint32_t start=compiler->instructions->count*2;
            
            switch (compiler->scope){
                case SCOPE_GLOBAL:
                    add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(expr));
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(expr));
                    break;
            }

            uint32_t end=compiler->instructions->count*2;
            if (compiler->lines!=NULL){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(EXCEPT(expr->node)->name->start->line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            }

            for (Node* n: (*WITH(expr->node)->code)){
                uint32_t start=compiler->instructions->count*2;
                long line=n->start->line;        
                int _stack_size=compiler->stack_size;
                compiler->stack_size=0;
                int _blockstack_size=compiler->blockstack_size;
                compiler->blockstack_size=0;
                int i=compile_expr(compiler, n);
                compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
                if (i==0x100){
                    return 0x100;
                }
                uint32_t end=compiler->instructions->count*2;
                if (compiler->lines!=NULL && i!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(end));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                }
            }
            
            add_instruction(compiler, compiler->instructions,EXIT_WITH, 0, GET_ANNO_N(expr));
            add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));

            

            return 0x200;
        }

        case N_LISTCOMP: {
            add_instruction(compiler, compiler->instructions,BUILD_LIST, 0, GET_ANNO_N(expr));

            uint32_t start_=compiler->instructions->count*2;
            uint32_t target=start_;
            target+=num_instructions_keep(LISTCOMP(expr->node)->expr, compiler->scope)*2;
            target+=num_instructions_keep(LISTCOMP(expr->node)->left, compiler->scope)*2;
            target+=12; //All the other stuff
            if (LISTCOMP(expr->node)->condition!=NULL){
                target+=num_instructions_keep(LISTCOMP(expr->node)->condition, compiler->scope)*2;
                target+=2;
            }
            
            bool ret=compiler->keep_return;
            long line=LISTCOMP(expr->node)->expr->start->line;

            int startcol=LISTCOMP(expr->node)->expr->start->col;
            int endcol=LISTCOMP(expr->node)->expr->end->col;
            int startln=LISTCOMP(expr->node)->expr->start->line;

            int cmpexpr=compile_expr_keep(compiler, LISTCOMP(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            
            if (compiler->lines!=NULL && cmpexpr!=0x200){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            } 
            
            add_instruction(compiler, compiler->instructions,EXTRACT_ITER,0, startcol, endcol, startln);

            uint32_t start=compiler->instructions->count*2;
            add_instruction(compiler, compiler->instructions,FOR_TOS_ITER, target, startcol, endcol, startln);
            
            uint32_t idx;
            if (!_list_contains(compiler->names, IDENTI(LISTCOMP(expr->node)->ident->node)->name)){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(LISTCOMP(expr->node)->ident->node)->name));
                idx=NAMEIDX(compiler->names);
            }
            else{
                idx=object_find(compiler->names, str_new_fromstr(*IDENTI(LISTCOMP(expr->node)->ident->node)->name));
            }

            switch (compiler->scope){
                case SCOPE_GLOBAL:
                    add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(LISTCOMP(expr->node)->ident));
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(LISTCOMP(expr->node)->ident));
                    break;
            }
            add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(LISTCOMP(expr->node)->ident));
            
            if (LISTCOMP(expr->node)->condition!=NULL){
                uint32_t start_=compiler->instructions->count*2;
                int startcol=LISTCOMP(expr->node)->expr->start->col;
                int endcol=LISTCOMP(expr->node)->expr->end->col;
                int startln=LISTCOMP(expr->node)->expr->start->line;
                int cmpexpr=compile_expr_keep(compiler, LISTCOMP(expr->node)->condition);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                
                if (compiler->lines!=NULL && cmpexpr!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                } 
                
                
                add_instruction(compiler, compiler->instructions,POP_JMP_TOS_FALSE, num_instructions_keep(LISTCOMP(expr->node)->ident, compiler->scope)*2+2, startcol, endcol, startln);
            }


            start_=compiler->instructions->count*2;
            cmpexpr=compile_expr_keep(compiler, LISTCOMP(expr->node)->left);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (compiler->lines!=NULL && cmpexpr!=0x200){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            } 

            add_instruction(compiler, compiler->instructions,SEQ_APPEND, 2, GET_ANNO_N(expr));
            add_instruction(compiler, compiler->instructions,JUMP_TO, start, GET_ANNO_N(expr));
            break;
        }

        case N_TUPLECOMP: {
            add_instruction(compiler, compiler->instructions,BUILD_TUPLE, 0, GET_ANNO_N(expr));

            uint32_t start_=compiler->instructions->count*2;
            uint32_t target=start_;
            target+=num_instructions_keep(LISTCOMP(expr->node)->expr, compiler->scope)*2;
            target+=num_instructions_keep(LISTCOMP(expr->node)->left, compiler->scope)*2;
            target+=12; //All the other stuff
            if (LISTCOMP(expr->node)->condition!=NULL){
                target+=num_instructions_keep(LISTCOMP(expr->node)->condition, compiler->scope)*2;
                target+=2;
            }
            
            bool ret=compiler->keep_return;
            long line=LISTCOMP(expr->node)->expr->start->line;

            int startcol=LISTCOMP(expr->node)->expr->start->col;
            int endcol=LISTCOMP(expr->node)->expr->end->col;
            int startln=LISTCOMP(expr->node)->expr->start->line;

            int cmpexpr=compile_expr_keep(compiler, LISTCOMP(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            
            if (compiler->lines!=NULL && cmpexpr!=0x200){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            } 
            
            add_instruction(compiler, compiler->instructions,EXTRACT_ITER,0, startcol, endcol, startln);

            uint32_t start=compiler->instructions->count*2;
            add_instruction(compiler, compiler->instructions,FOR_TOS_ITER, target,GET_ANNO_N(expr));
            
            uint32_t idx;
            if (!_list_contains(compiler->names, IDENTI(LISTCOMP(expr->node)->ident->node)->name)){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(LISTCOMP(expr->node)->ident->node)->name));
                idx=NAMEIDX(compiler->names);
            }
            else{
                idx=object_find(compiler->names, str_new_fromstr(*IDENTI(LISTCOMP(expr->node)->ident->node)->name));
            }

            switch (compiler->scope){
                case SCOPE_GLOBAL:
                    add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(LISTCOMP(expr->node)->ident));
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(LISTCOMP(expr->node)->ident));
                    break;
            }
            add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(LISTCOMP(expr->node)->ident));
            
            if (LISTCOMP(expr->node)->condition!=NULL){
                uint32_t start_=compiler->instructions->count*2;

                int startcol=LISTCOMP(expr->node)->expr->start->col;
                int endcol=LISTCOMP(expr->node)->expr->end->col;
                int startln=LISTCOMP(expr->node)->expr->start->line;

                int cmpexpr=compile_expr_keep(compiler, LISTCOMP(expr->node)->condition);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                
                if (compiler->lines!=NULL && cmpexpr!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                } 
                
                
                add_instruction(compiler, compiler->instructions,POP_JMP_TOS_FALSE, num_instructions_keep(LISTCOMP(expr->node)->ident, compiler->scope)*2+2, startcol, endcol, startln);
            }


            start_=compiler->instructions->count*2;
            cmpexpr=compile_expr_keep(compiler, LISTCOMP(expr->node)->left);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (compiler->lines!=NULL && cmpexpr!=0x200){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            } 

            add_instruction(compiler, compiler->instructions,SEQ_APPEND, 2, GET_ANNO_N(expr));
            add_instruction(compiler, compiler->instructions,JUMP_TO, start, GET_ANNO_N(expr));
            break;
        }

        case N_SETCOMP: {
            add_instruction(compiler, compiler->instructions,BUILD_SET, 0, GET_ANNO_N(expr));

            uint32_t start_=compiler->instructions->count*2;
            uint32_t target=start_;
            target+=num_instructions_keep(LISTCOMP(expr->node)->expr, compiler->scope)*2;
            target+=num_instructions_keep(LISTCOMP(expr->node)->left, compiler->scope)*2;
            target+=12; //All the other stuff
            if (LISTCOMP(expr->node)->condition!=NULL){
                target+=num_instructions_keep(LISTCOMP(expr->node)->condition, compiler->scope)*2;
                target+=2;
            }
            
            bool ret=compiler->keep_return;
            long line=LISTCOMP(expr->node)->expr->start->line;

            int startcol=LISTCOMP(expr->node)->expr->start->col;
            int endcol=LISTCOMP(expr->node)->expr->end->col;
            int startln=LISTCOMP(expr->node)->expr->start->line;

            int cmpexpr=compile_expr_keep(compiler, LISTCOMP(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            
            if (compiler->lines!=NULL && cmpexpr!=0x200){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            } 
            
            add_instruction(compiler, compiler->instructions,EXTRACT_ITER,0, startcol, endcol, startln);

            uint32_t start=compiler->instructions->count*2;
            add_instruction(compiler, compiler->instructions,FOR_TOS_ITER, target, GET_ANNO_N(expr));
            
            uint32_t idx;
            if (!_list_contains(compiler->names, IDENTI(LISTCOMP(expr->node)->ident->node)->name)){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(LISTCOMP(expr->node)->ident->node)->name));
                idx=NAMEIDX(compiler->names);
            }
            else{
                idx=object_find(compiler->names, str_new_fromstr(*IDENTI(LISTCOMP(expr->node)->ident->node)->name));
            }

            switch (compiler->scope){
                case SCOPE_GLOBAL:
                    add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(LISTCOMP(expr->node)->ident));
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(LISTCOMP(expr->node)->ident));
                    break;
            }
            add_instruction(compiler, compiler->instructions,POP_TOS, 0,GET_ANNO_N(LISTCOMP(expr->node)->ident));
            
            if (LISTCOMP(expr->node)->condition!=NULL){
                uint32_t start_=compiler->instructions->count*2;

                int startcol=LISTCOMP(expr->node)->condition->start->col;
                int endcol=LISTCOMP(expr->node)->condition->end->col;
                int startln=LISTCOMP(expr->node)->condition->start->line;

                int cmpexpr=compile_expr_keep(compiler, LISTCOMP(expr->node)->condition);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                
                if (compiler->lines!=NULL && cmpexpr!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                } 
                
                
                add_instruction(compiler, compiler->instructions,POP_JMP_TOS_FALSE, num_instructions_keep(LISTCOMP(expr->node)->ident, compiler->scope)*2+2, startcol, endcol, startln);
            }


            start_=compiler->instructions->count*2;
            cmpexpr=compile_expr_keep(compiler, LISTCOMP(expr->node)->left);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (compiler->lines!=NULL && cmpexpr!=0x200){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            } 

            add_instruction(compiler, compiler->instructions,SEQ_APPEND, 2, GET_ANNO_N(expr));
            add_instruction(compiler, compiler->instructions,JUMP_TO, start, GET_ANNO_N(expr));
            break;
        }

        case N_DICTCOMP: {
            add_instruction(compiler, compiler->instructions,BUILD_DICT, 0, GET_ANNO_N(expr));

            uint32_t start_=compiler->instructions->count*2;
            uint32_t target=start_;
            target+=num_instructions_keep(DICTCOMP(expr->node)->expr, compiler->scope)*2;
            target+=num_instructions_keep(DICTCOMP(expr->node)->left, compiler->scope)*2;
            target+=num_instructions_keep(DICTCOMP(expr->node)->right, compiler->scope)*2;
            target+=12; //All the other stuff
            if (DICTCOMP(expr->node)->condition!=NULL){
                target+=num_instructions_keep(DICTCOMP(expr->node)->condition, compiler->scope)*2;
                target+=2;
            }
            
            bool ret=compiler->keep_return;
            long line=DICTCOMP(expr->node)->expr->start->line;

            int startcol=DICTCOMP(expr->node)->expr->start->col;
            int endcol=DICTCOMP(expr->node)->expr->end->col;
            int startln=DICTCOMP(expr->node)->expr->start->line;

            int cmpexpr=compile_expr_keep(compiler, DICTCOMP(expr->node)->expr);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            
            if (compiler->lines!=NULL && cmpexpr!=0x200){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            } 
            
            add_instruction(compiler, compiler->instructions,EXTRACT_ITER,0, startcol, endcol, startln);

            uint32_t start=compiler->instructions->count*2;
            add_instruction(compiler, compiler->instructions,FOR_TOS_ITER, target, GET_ANNO_N(expr));
            
            uint32_t idx;
            if (!_list_contains(compiler->names, IDENTI(DICTCOMP(expr->node)->ident->node)->name)){
                //Create object
                compiler->names->type->slot_mappings->slot_append(compiler->names, str_new_fromstr(*IDENTI(DICTCOMP(expr->node)->ident->node)->name));
                idx=NAMEIDX(compiler->names);
            }
            else{
                idx=object_find(compiler->names, str_new_fromstr(*IDENTI(DICTCOMP(expr->node)->ident->node)->name));
            }

            switch (compiler->scope){
                case SCOPE_GLOBAL:
                    add_instruction(compiler, compiler->instructions,STORE_GLOBAL, idx, GET_ANNO_N(DICTCOMP(expr->node)->ident));
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler, compiler->instructions,STORE_NAME, idx, GET_ANNO_N(DICTCOMP(expr->node)->ident));
                    break;
            }
            add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(DICTCOMP(expr->node)->ident));
            
            if (DICTCOMP(expr->node)->condition!=NULL){
                uint32_t start_=compiler->instructions->count*2;

                int startcol=DICTCOMP(expr->node)->condition->start->col;
                int endcol=DICTCOMP(expr->node)->condition->end->col;
                int startln=DICTCOMP(expr->node)->condition->start->line;
            
                int cmpexpr=compile_expr_keep(compiler, DICTCOMP(expr->node)->condition);
                if (cmpexpr==0x100){
                    return cmpexpr;
                }
                
                if (compiler->lines!=NULL && cmpexpr!=0x200){
                    object* tuple=new_tuple();
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                    tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                    compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
                } 
                
                
                add_instruction(compiler, compiler->instructions,POP_JMP_TOS_FALSE, num_instructions_keep(DICTCOMP(expr->node)->ident, compiler->scope)*2+2, startcol, endcol, startln);
            }


            start_=compiler->instructions->count*2;
            cmpexpr=compile_expr_keep(compiler, DICTCOMP(expr->node)->left);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (compiler->lines!=NULL && cmpexpr!=0x200){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            }

            start_=compiler->instructions->count*2;
            cmpexpr=compile_expr_keep(compiler, DICTCOMP(expr->node)->right);
            if (cmpexpr==0x100){
                return cmpexpr;
            }
            if (compiler->lines!=NULL && cmpexpr!=0x200){
                object* tuple=new_tuple();
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(start_));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(compiler->instructions->count*2));
                tuple->type->slot_mappings->slot_append(tuple, new_int_fromint(line));
                compiler->lines->type->slot_mappings->slot_append(compiler->lines, tuple);
            }  

            add_instruction(compiler, compiler->instructions,DICT_SET, 3, GET_ANNO_N(expr));
            add_instruction(compiler, compiler->instructions,JUMP_TO, start, GET_ANNO_N(expr));
            break;
        }

        case N_BSTRING: {
            uint32_t idx;
            if (!_list_contains(compiler->consts, STRLIT(expr->node)->literal)){
                //Create object
                compiler->consts->type->slot_mappings->slot_append(compiler->consts, str_new_fromstr(*STRLIT(expr->node)->literal));
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, str_new_fromstr(*STRLIT(expr->node)->literal));
            }
            add_instruction(compiler, compiler->instructions,BYTES_STRING,idx, GET_ANNO_N(expr));
            if (!compiler->keep_return){
                add_instruction(compiler, compiler->instructions,POP_TOS, 0, GET_ANNO_N(expr));
            }
            break;     
        }
    }
    
    if (!compiler->nofree){
        fpl_free(expr);
    }

    return 0;
}

uint32_t num_instructions(vector<Node*>* nodes, scope s){
    struct compiler* comp=new_compiler();
    comp->lines=NULL;
    comp->scope=s;
    comp->nofree=true;
    for (Node* n: (*nodes)){
        int cmpexpr=compile_expr(comp, n);
        if (cmpexpr==0x100){
            return -1;
        }
    }
    return comp->instructions->count;
}

uint32_t num_instructions(Node* node, scope s){
    struct compiler* comp=new_compiler(); 
    comp->lines=NULL;   
    comp->scope=s;  
    comp->nofree=true;
    int cmpexpr=compile_expr(comp, node);
    if (cmpexpr==0x100){
        return -1;
    }
    
    return comp->instructions->count;
}

uint32_t num_instructions_keep(Node* node, scope s){
    struct compiler* comp=new_compiler(); 
    comp->lines=NULL;   
    comp->scope=s;  
    comp->nofree=true;
    int cmpexpr=compile_expr_keep(comp, node);
    if (cmpexpr==0x100){
        return -1;
    }
    
    return comp->instructions->count;
}

struct object* compile(struct compiler* compiler, parse_ret ast, int fallback_line){
    compiler->lines=new_list();
    object* lines=compiler->lines;
    
    int i=0;
    for (Node* n: ast.nodes){
        uint32_t start=compiler->instructions->count*2;
        
        int line=n->start->line;

        int _stack_size=compiler->stack_size;
        compiler->stack_size=0;
        int _blockstack_size=compiler->blockstack_size;
        compiler->blockstack_size=0;
        int i=compile_expr(compiler, n);
        compile_expr_cleanup(&compiler->stack_size, &_stack_size, &compiler->blockstack_size, &_blockstack_size);
        if (i==0x100){
            return NULL;
        }
        uint32_t end=compiler->instructions->count*2;
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
    
    int siz=compiler->stack_size;
    add_instruction(compiler, compiler->instructions, LOAD_CONST, idx, 0, 0, 0);
    add_instruction(compiler, compiler->instructions, RETURN_VAL, 0,  0, 0, 0);
    compiler->stack_size=siz;

    if (ast.nodes.size()>0 && CAST_LIST(lines)->size>0){
        object* tuple=new_tuple();
        object* lineno=new_int_fromint(CAST_LIST(lines)->size-1);
        object* line=list_index_int(lines, CAST_LIST(lines)->size-1);
        tuple->type->slot_mappings->slot_append(tuple, tuple_index_int(line, 0));
        tuple->type->slot_mappings->slot_append(tuple, object_add(tuple_index_int(line, 1), new_int_fromint(2)));
        tuple->type->slot_mappings->slot_append(tuple, tuple_index_int(line, 2));
        
        object* res=lines->type->slot_mappings->slot_set(lines, lineno, tuple);
        if (res!=NULL && res!=CALL_ERR && res!=SUCCESS && res!=TERM_PROGRAM){
            FPLDECREF(res);
        }
        FPLDECREF(lineno);
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
    CAST_LIST(list)->type->slot_mappings->slot_append(list, compiler->lines_detailed);
    CAST_LIST(list)->type->slot_mappings->slot_append(list, new_int_fromint(compiler->stack_size));
    CAST_LIST(list)->type->slot_mappings->slot_append(list, new_int_fromint(compiler->blockstack_size));
    
    object* code=code_new_fromargs(list);
    CAST_CODE(code)->co_instructions=CAST_LIST(instructions)->size;
    CAST_CODE(code)->filedata=glblfildata;
    return code;
}