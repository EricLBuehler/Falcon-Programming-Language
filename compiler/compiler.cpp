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
    struct instructions* instr=(struct instructions*)malloc(sizeof(struct instructions));

    instr->count=0;
    instr->first=NULL;

    return instr;
}

void add_instruction(struct instructions* instructions, enum opcode opcode, uint32_t arg, Position* start, Position* end){
    struct instruction* instr=(struct instruction*)malloc(sizeof(struct instruction));
    instr->arg=arg;
    instr->opcode=opcode;
    instr->next=instructions->first;
    instr->start=start;
    instr->end=end;

    instructions->first=instr;
    instructions->count++;
}

struct compiler* new_compiler(){
    struct compiler* compiler=(struct compiler*)malloc(sizeof(struct compiler));
    compiler->consts=new_list();
    compiler->instructions=new_instructions();
    compiler->names=new_list();
    compiler->scope=SCOPE_GLOBAL;
    
    return compiler;
}

void compiler_del(struct compiler* compiler){
    DECREF(compiler->consts);
    DECREF(compiler->names);
    struct instruction* i=compiler->instructions->first;
    while (i){
        struct instruction* i_=i=i->next;;
        free(i);
        i=i_;
    }
}

int compile_expr(struct compiler* compiler, Node* expr){
    switch (expr->type){
        case N_ASSIGN: {
            uint32_t idx;
            if (!_list_contains(compiler->names, IDENTI(ASSIGN(expr->node)->name->node)->name)){
                //Create object
                compiler->names->type->slot_append(compiler->names, str_new_fromstr(IDENTI(ASSIGN(expr->node)->name->node)->name));
                idx=NAMEIDX(compiler->names);
            }
            else{
                idx=object_find(compiler->names, str_new_fromstr(IDENTI(ASSIGN(expr->node)->name->node)->name));
            }
            
            compile_expr(compiler, ASSIGN(expr->node)->right); //Push data
            switch (compiler->scope){
                case SCOPE_GLOBAL:
                    add_instruction(compiler->instructions,STORE_GLOBAL, idx, expr->start, expr->end);
                    break;

                case SCOPE_LOCAL:
                    add_instruction(compiler->instructions,STORE_NAME, idx, expr->start, expr->end);
                    break;
            }
            break;
        }

        case N_BINOP: {
            compile_expr(compiler, BINOP(expr->node)->left); //Push data
            compile_expr(compiler, BINOP(expr->node)->right); //Push data
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
            }
            break;
        }

        case N_UNARY: {
            compile_expr(compiler, UNARYOP(expr->node)->right); //Push data
            switch (UNARYOP(expr->node)->opr){
                case T_MINUS:
                    add_instruction(compiler->instructions,UNARY_NEG,0, expr->start, expr->end);
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
                compiler->consts->type->slot_append(compiler->consts, new_int_fromstr(INTLIT(expr->node)->literal));
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, new_int_fromstr(INTLIT(expr->node)->literal));
            }
            
            add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
            break;
        }

        case N_STR: {
            uint32_t idx;
            if (!_list_contains(compiler->consts, STRLIT(expr->node)->literal)){
                //Create object
                compiler->consts->type->slot_append(compiler->consts, str_new_fromstr(STRLIT(expr->node)->literal));
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, str_new_fromstr(STRLIT(expr->node)->literal));
            }
            add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);
            break;     
        }
        
        case N_IDENT: {
            uint32_t idx;
            if (!_list_contains(compiler->names, IDENTI(expr->node)->name)){
                //Create object
                compiler->names->type->slot_append(compiler->names, str_new_fromstr(IDENTI(expr->node)->name));
                idx = NAMEIDX(compiler->names);
            }
            else{
                idx=object_find(compiler->names, str_new_fromstr(IDENTI(expr->node)->name));
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
                compiler->consts->type->slot_append(compiler->consts, str_new_fromstr(IDENTI(FUNCT(expr->node)->name->node)->name));
                nameidx = NAMEIDX(compiler->consts);
            }
            else{
                nameidx=object_find(compiler->consts, str_new_fromstr(IDENTI(FUNCT(expr->node)->name->node)->name));
            }
            
            
            add_instruction(compiler->instructions,LOAD_CONST, nameidx, expr->start, expr->end);

            //Arguments
            size_t argc=0;

            object* args=new_tuple();
            object* kwargs=new_tuple();

            //Setup args
            for (Node* n: (*FUNCT(expr->node)->args)){
                argc++;
                args->type->slot_append(args, str_new_fromstr(IDENTI(n->node)->name));
            }
            
            //
            
            //Setup kwargs (code object)
            for (Node* n: (*FUNCT(expr->node)->kwargs)){
                argc++;
                args->type->slot_append(args, str_new_fromstr(IDENTI(ASSIGN(n->node)->name->node)->name));
                compile_expr(compiler, ASSIGN(n->node)->right);
            }
            //

            add_instruction(compiler->instructions,BUILD_TUPLE, FUNCT(expr->node)->kwargs->size(), expr->start, expr->end);
            
            uint32_t idx;
            if (!object_find_bool(compiler->consts, args)){
                //Create object
                compiler->consts->type->slot_append(compiler->consts, args);
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
            object* code=compile(comp, c);
            compiler=compiler_;

            DECREF(CAST_CODE(code)->co_file);
            CAST_CODE(code)->co_file=object_repr(str_new_fromstr(IDENTI(FUNCT(expr->node)->name->node)->name));
            
            
            if (!object_find_bool(compiler->consts, code)){
                //Create object
                compiler->consts->type->slot_append(compiler->consts, code);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, code);
            }
            add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);

            //Create callable
            add_instruction(compiler->instructions,MAKE_FUNCTION, argc, expr->start, expr->end);

            //Store function
            uint32_t nameidxstore;
            if (!_list_contains(compiler->names, IDENTI(FUNCT(expr->node)->name->node)->name )){
                //Create object
                compiler->names->type->slot_append(compiler->names, str_new_fromstr(IDENTI(FUNCT(expr->node)->name->node)->name));
                nameidxstore = NAMEIDX(compiler->names);
            }
            else{
                nameidxstore=object_find(compiler->names, str_new_fromstr(IDENTI(FUNCT(expr->node)->name->node)->name));
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

        case N_CALL: {
            //Args (iterate backwards)
            for (auto it =  (*CALL(expr->node)->args).rbegin(); it != (*CALL(expr->node)->args).rend(); ++it){
                compile_expr(compiler, *it);
            }
            //Kwargs (iterate backwards)
            for (auto it =  (*CALL(expr->node)->kwargs).rbegin(); it != (*CALL(expr->node)->kwargs).rend(); ++it){
                uint32_t idx;
                if (!_list_contains(compiler->consts, IDENTI(ASSIGN((*it)->node)->name->node)->name)){
                    //Create object
                    compiler->consts->type->slot_append(compiler->consts, str_new_fromstr(IDENTI(ASSIGN((*it)->node)->name->node)->name));
                    idx = NAMEIDX(compiler->consts);
                }
                else{
                    idx=object_find(compiler->consts, str_new_fromstr(IDENTI(ASSIGN((*it)->node)->name->node)->name));
                }
                add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);
                compile_expr(compiler, ASSIGN((*it)->node)->right);
            }
            
            //Num of pos args
            uint32_t idx;
            object* size=new_int_fromint(CALL(expr->node)->args->size());
            uint32_t argc=CALL(expr->node)->args->size()+CALL(expr->node)->kwargs->size();
            
            if (!object_find_bool(compiler->consts,size)){
                //Create object
                compiler->consts->type->slot_append(compiler->consts, size);
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, size);
            }
            
            add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
            //Object
            compile_expr(compiler, CALL(expr->node)->object);

            add_instruction(compiler->instructions,CALL_FUNCTION, argc, expr->start, expr->end);
            break;
        }

        case N_TRUE: {
            uint32_t idx;
            if (!object_find_bool(compiler->consts,trueobj)){
                //Create object
                compiler->consts->type->slot_append(compiler->consts, trueobj);
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
                compiler->consts->type->slot_append(compiler->consts, falseobj);
                idx=NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, falseobj);
            }
            
            add_instruction(compiler->instructions,LOAD_CONST,idx, expr->start, expr->end);
            break;
        }

        case N_NONE: {
            uint32_t idx;
            if (!object_find_bool(compiler->consts,noneobj)){
                //Create object
                compiler->consts->type->slot_append(compiler->consts, noneobj);
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
                compiler->consts->type->slot_append(compiler->consts, str_new_fromstr(IDENTI(CLASS(expr->node)->name->node)->name));
                nameidx = NAMEIDX(compiler->consts);
            }
            else{
                nameidx=object_find(compiler->consts, str_new_fromstr(IDENTI(CLASS(expr->node)->name->node)->name));
            }
            add_instruction(compiler->instructions,LOAD_CONST, nameidx, expr->start, expr->end);

            object* args=new_tuple();
            
            uint32_t idx;
            if (!object_find_bool(compiler->consts, args)){
                //Create object
                compiler->consts->type->slot_append(compiler->consts, args);
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
            object* code=compile(comp, c);
            compiler=compiler_;

            DECREF(CAST_CODE(code)->co_file);
            CAST_CODE(code)->co_file=object_repr(str_new_fromstr(IDENTI(FUNCT(expr->node)->name->node)->name));
            
            if (!object_find_bool(compiler->consts, code)){
                //Create object
                compiler->consts->type->slot_append(compiler->consts, code);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, code);
            }
            add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);

            add_instruction(compiler->instructions,MAKE_FUNCTION, 0, expr->start, expr->end);            
        
            add_instruction(compiler->instructions,LOAD_CONST, nameidx, expr->start, expr->end);

            object* bases=new_tuple();

            if (!object_find_bool(compiler->consts, bases)){
                //Create object
                compiler->consts->type->slot_append(compiler->consts, bases);
                idx = NAMEIDX(compiler->consts);
            }
            else{
                idx=object_find(compiler->consts, bases);
            }
            add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);


            object* i=new_int_fromint(3);
            if (!object_find_bool(compiler->consts, i)){
                //Create object
                compiler->consts->type->slot_append(compiler->consts, i);
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
                compiler->names->type->slot_append(compiler->names, str_new_fromstr(IDENTI(FUNCT(expr->node)->name->node)->name));
                nameidxstore = NAMEIDX(compiler->names);
            }
            else{
                nameidxstore=object_find(compiler->names, str_new_fromstr(IDENTI(FUNCT(expr->node)->name->node)->name));
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
            for (size_t i=0; i<names->size(); i++){
                uint32_t idx;
                if (!_list_contains(compiler->names, IDENTI(names->at(i)->node)->name)){
                    //Create object
                    compiler->names->type->slot_append(compiler->names, str_new_fromstr(IDENTI(names->at(i)->node)->name));
                    idx = NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(IDENTI(names->at(i)->node)->name));
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
                add_instruction(compiler->instructions,LOAD_ATTR, idx, expr->start, expr->end);
            }
            break;
        }

        case N_DOTASSIGN: {
            compile_expr(compiler, DOTASSIGN(expr->node)->right);
            vector<Node*>* names=DOT(DOTASSIGN(expr->node)->dot->node)->names;
            
            for (size_t i=0; i<names->size(); i++){
                uint32_t idx;
                if (!_list_contains(compiler->names, IDENTI(names->at(i)->node)->name)){
                    //Create object
                    compiler->names->type->slot_append(compiler->names, str_new_fromstr(IDENTI(names->at(i)->node)->name));
                    idx = NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(IDENTI(names->at(i)->node)->name));
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
                    add_instruction(compiler->instructions,STORE_ATTR, idx, expr->start, expr->end);
                    continue;
                }     
                add_instruction(compiler->instructions,LOAD_ATTR, idx, expr->start, expr->end);
            }
            break;
        }        

        case N_DOTCALL: {
            vector<Node*>* names=DOT(DOTCALL(expr->node)->dot->node)->names;
            
            for (size_t i=0; i<names->size(); i++){
                uint32_t idx;
                if (!_list_contains(compiler->names, IDENTI(names->at(i)->node)->name)){
                    //Create object
                    compiler->names->type->slot_append(compiler->names, str_new_fromstr(IDENTI(names->at(i)->node)->name));
                    idx = NAMEIDX(compiler->names);
                }
                else{
                    idx=object_find(compiler->names, str_new_fromstr(IDENTI(names->at(i)->node)->name));
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

                    //Args (iterate backwards)
                    for (auto it =  (*DOTCALL(expr->node)->args).rbegin(); it != (*DOTCALL(expr->node)->args).rend(); ++it){
                        compile_expr(compiler, *it);
                    }
                    //Kwargs (iterate backwards)
                    for (auto it =  (*DOTCALL(expr->node)->kwargs).rbegin(); it != (*DOTCALL(expr->node)->kwargs).rend(); ++it){
                        uint32_t idx;
                        if (!_list_contains(compiler->consts, IDENTI(ASSIGN((*it)->node)->name->node)->name)){
                            //Create object
                            compiler->consts->type->slot_append(compiler->consts, str_new_fromstr(IDENTI(ASSIGN((*it)->node)->name->node)->name));
                            idx = NAMEIDX(compiler->consts);
                        }
                        else{
                            idx=object_find(compiler->consts, str_new_fromstr(IDENTI(ASSIGN((*it)->node)->name->node)->name));
                        }
                        add_instruction(compiler->instructions,LOAD_CONST, idx, expr->start, expr->end);
                        compile_expr(compiler, ASSIGN((*it)->node)->right);
                    }
                    
                    //Num of pos args
                    uint32_t idx;
                    object* size=new_int_fromint(DOTCALL(expr->node)->args->size());
                    uint32_t argc=DOTCALL(expr->node)->args->size()+DOTCALL(expr->node)->kwargs->size();

                    if (!object_find_bool(compiler->consts,size)){
                        //Create object
                        compiler->consts->type->slot_append(compiler->consts, size);
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
                        compiler->names->type->slot_append(compiler->names, str_new_fromstr(IDENTI(names->at(i)->node)->name));
                        idx = NAMEIDX(compiler->names);
                    }
                    else{
                        idx=object_find(compiler->names, str_new_fromstr(IDENTI(names->at(i)->node)->name));
                    }


                    add_instruction(compiler->instructions,LOAD_ATTR, idx, expr->start, expr->end);

                    add_instruction(compiler->instructions,CALL_METHOD, argc, expr->start, expr->end);
                    continue;
                }     
                add_instruction(compiler->instructions,LOAD_ATTR, idx, expr->start, expr->end);
            }
            break;
        }        

    }

    return 0;
}

struct object* compile(struct compiler* compiler, parse_ret ast){
    object* lines=new_list();
    for (Node* n: ast.nodes){
        uint32_t start=compiler->instructions->count;
        compile_expr(compiler, n);
        uint32_t end=compiler->instructions->count;
        
        object* tuple=new_tuple();
        tuple->type->slot_append(tuple, new_int_fromint(start));
        tuple->type->slot_append(tuple, new_int_fromint(end));
        tuple->type->slot_append(tuple, new_int_fromint(n->start->line));
        lines->type->slot_append(lines, tuple);
    }

    uint32_t idx;
    if (!object_find_bool(compiler->consts, noneobj)){
        //Create object
        compiler->consts->type->slot_append(compiler->consts, new_none());
        idx=NAMEIDX(compiler->consts);
    }
    else{
        idx=object_find(compiler->consts, new_none());
    }
    
    add_instruction(compiler->instructions, LOAD_CONST, idx, new Position, new Position);
    add_instruction(compiler->instructions, RETURN_VAL, 0, new Position, new Position);

    if (ast.nodes.size()>0){
        object* tuple=new_tuple();
        object* lineno=object_sub(lines->type->slot_len(lines), new_int_fromint(1));
        object* line=lines->type->slot_get(lines, lineno);
        tuple->type->slot_append(tuple, line->type->slot_get(line, new_int_fromint(0)));
        tuple->type->slot_append(tuple, object_add(line->type->slot_get(line, new_int_fromint(1)), new_int_fromint(2)));
        tuple->type->slot_append(tuple, line->type->slot_get(line, new_int_fromint(2)));


        DECREF(line);
        lines->type->slot_set(lines, lineno, tuple);
    }
    else{
        object* tuple=new_tuple();
        tuple->type->slot_append(tuple, new_int_fromint(0));
        tuple->type->slot_append(tuple, new_int_fromint(2));
        tuple->type->slot_append(tuple, new_int_fromint(0));
        lines->type->slot_append(lines, tuple);
    }

    reverse_instructions(compiler->instructions);
    object* instructions=new_list();
    struct instruction* instruction=compiler->instructions->first;
    while (instruction){
        CAST_LIST(instructions)->type->slot_append(instructions, new_int_fromint(instruction->opcode));
        CAST_LIST(instructions)->type->slot_append(instructions, new_int_fromint(instruction->arg));
        instruction=instruction->next;
    }

    object* list=new_list();
    CAST_LIST(list)->type->slot_append(list, compiler->names);
    CAST_LIST(list)->type->slot_append(list, compiler->consts);
    CAST_LIST(list)->type->slot_append(list, instructions);
    CAST_LIST(list)->type->slot_append(list, object_repr(str_new_fromstr(new string(program))));
    CAST_LIST(list)->type->slot_append(list, lines);

    object* code=code_new_fromargs(list);
    return code;
}
