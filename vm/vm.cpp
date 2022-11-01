struct datastack* new_datastack(int size){
    struct datastack* data=(struct datastack*)fpl_malloc(sizeof(struct datastack));
    data->data=(object**)fpl_malloc(sizeof(object*)*size);
    data->size=0;
    data->capacity=size;
    return data;
}

struct callstack* new_callstack(){
    struct callstack* call=(struct callstack*)fpl_malloc(sizeof(struct callstack));
    call->data=(callframe*)fpl_malloc(sizeof(object*)*MAX_RECURSION);
    call->size=0;
    return call;
}

struct blockstack* new_blockstack(int size){
    struct blockstack* block=(struct blockstack*)fpl_malloc(sizeof(struct blockstack));
    block->data=(blockframe*)fpl_malloc(sizeof(blockframe)*size);
    block->capacity=size;
    block->size=0;
    return block;
}

struct blockframe* in_blockstack(struct blockstack* stack, enum blocktype type){
    for (int i=0; i<stack->size; i++){
        struct blockframe frame = stack->data[i];
        if (frame.type==type){
            return (struct blockframe*)(stack->data+(sizeof(struct blockframe)*i));
        }
    }
    return NULL;
}

void vm_add_err(TypeObject* exception, struct vm* vm, const char *_format, ...) {
    if (vm==NULL || vm->exception!=NULL){
        return;
    }
    
    va_list args;
    const int length=256;
    char format[length];
    sprintf(format, "%s", _format);
    
    object* eargs=new_tuple();
    object* kwargs=new_dict();
    vm->exception=exception->type->slot_call((object*)exception, eargs, kwargs); //Create new exception object
    FPLDECREF(eargs);
    FPLDECREF(kwargs);
    
    char* msg = (char*)fpl_malloc(sizeof(char)*length);

    va_start(args, _format);
    vsnprintf(msg, length, format, args);
    va_end(args);
    
    if (vm->exception!=NULL && CAST_EXCEPTION(vm->exception)->err!=NULL){
        FPLDECREF(CAST_EXCEPTION(vm->exception)->err);
    }
    CAST_EXCEPTION(vm->exception)->err=str_new_fromstr(msg);
    fpl_free(msg);
}

object* vm_setup_err(TypeObject* exception, struct vm* vm, const char *_format, ...) {  
    if (vm==NULL || vm->exception!=NULL){
        return NULL;
    }

    va_list args;
    const int length=256;
    char format[length];
    sprintf(format, "%s", _format);
    
    object* eargs=new_tuple();
    object* kwargs=new_dict();
    object* exc=exception->type->slot_call((object*)exception, eargs, kwargs); //Create new exception object
    FPLDECREF(eargs);
    FPLDECREF(kwargs);

    char *msg = (char*)fpl_malloc(sizeof(char)*length);

    va_start(args, _format);
    vsnprintf(msg, length, format, args);
    va_end(args);
    
    if (vm->exception!=NULL && CAST_EXCEPTION(vm->exception)->err!=NULL){
        FPLDECREF(CAST_EXCEPTION(vm->exception)->err);
    }
    CAST_EXCEPTION(exc)->err=str_new_fromstr(msg);
    return exc;
}

struct vm* new_vm(uint32_t id, object* code, struct instructions* instructions, string* filedata){
    struct vm* vm=(struct vm*)fpl_malloc(sizeof(struct vm));
    vm->id=id;
    vm->ret_val=0;
    vm->ip=0;
    vm->objstack=new_datastack(CAST_INT(CAST_CODE(code)->co_stack_size)->val->to_int());
    vm->callstack=new_callstack();
    vm->blockstack=new_blockstack(CAST_INT(CAST_CODE(code)->co_blockstack_size)->val->to_int());
    
    vm->exception=NULL;

    vm->filedata=filedata;

    vm->path=new_list();
    
    fstream newfile;
    newfile.open("fplpath.path",ios::in);
    if (newfile.is_open()){
        string tp;
        while(getline(newfile, tp)){
            vm->path->type->slot_mappings->slot_append(vm->path, str_new_fromstr(tp));
        }
        newfile.close();
    }
    else{
        vm->path->type->slot_mappings->slot_append(vm->path, str_new_fromstr("./"));
    }
    
    if (::vm==NULL){
        ::vm=vm;
    }

    add_callframe(vm->callstack, new_int_fromint(0), new string("<module>"), code, NULL, &vm->ip);
    vm->globals=new_dict();
    callstack_head(vm->callstack).locals=FPLINCREF(vm->globals);
    vm->global_annotations=callstack_head(vm->callstack).annotations;
    
    return vm;
}

void vm_del(struct vm* vm){
    free(vm->objstack->data);

    FPLDECREF(vm->globals);
    delete vm->filedata;
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
    }
}

inline int calculate_line_fromip(uint32_t ip, callframe* callframe){
    for (int i=0; i<CAST_LIST(CAST_CODE(callframe->code)->co_lines)->size; i++){
        object* tup=list_index_int(CAST_LIST(CAST_CODE(callframe->code)->co_lines), i);
        if (ip<=(*CAST_INT(list_index_int(tup, 1))->val)){
            return CAST_INT(list_index_int(tup, 2))->val->to_int();
        }
    }
    return -1;
}

void print_traceback(){
    for (int i=0; i<vm->callstack->size; i++){    
        struct callframe callframe=vm->callstack->data[i];
        object* tup=dict_get(CAST_CODE(callframe.code)->co_detailed_lines, new_int_fromint((*callframe.ip)));
        int line_=CAST_INT(tuple_index_int(tup, 2))->val->to_int();
        int startcol=CAST_INT(tuple_index_int(tup, 0))->val->to_int();
        int endcol=CAST_INT(tuple_index_int(tup, 1))->val->to_int();

        cout<<"In file '"+program+"', line "+to_string(line_+1)+", in "+(*callframe.name)<<endl;
        
        int line=0;
        int target=line_;
        int startidx=0;
        int endidx=0;
        int idx=0;
        bool entered=false;
        while (true){
            if (line==target && !entered){
                startidx=idx;
                entered=true;
            }
            if (entered && ((*CAST_CODE(callframe.code)->filedata)[idx]=='\n' || (*CAST_CODE(callframe.code)->filedata)[idx]=='\0')){
                endidx=idx;
                break;
            }
            else if ((*CAST_CODE(callframe.code)->filedata)[idx]=='\n'){
                line++;
            }
            idx++;
        }

        string snippet="";
        for (int i=startidx; i<endidx; i++){
            snippet+=(*CAST_CODE(callframe.code)->filedata)[i];
        }
        

        cout<<"    ";
        int diff=snippet.size();
        snippet=remove_spaces(snippet);
        diff-=snippet.size();
        for (int i=0; i<snippet.size(); i++){
            if (i>=startcol-diff && i<=endcol-diff){
                red_color();
            }
            cout<<snippet.at(i);
            reset_color();
        }
        cout<<endl;
    }
}

object* import_name(string data, object* name){

    program=object_cstr(name);

    Lexer lexer(data,kwds);
    lexer.pos=Position(program);

    Position end=lexer.tokenize();

    Parser p=parser;
    parser=Parser(lexer.tokens, data);
    parse_ret ast=parser.parse();
    parser=p;

    if (ast.errornum>0){
        cout<<ast.header<<endl;
        cout<<ast.snippet<<endl;
        cout<<ast.arrows<<endl;
        printf("%s\n",ast.error);
        return TERM_PROGRAM;
    }

    struct compiler* compiler = new_compiler();

    string* g=glblfildata;
    glblfildata=new string(data);
    compiler->scope=SCOPE_LOCAL;
    object* code=compile(compiler, ast, 0);
    glblfildata=g;
    
    if (code==NULL){
        cout<<parseretglbl.header<<endl;
        cout<<parseretglbl.snippet<<endl;
        printf("%s\n",parseretglbl.error);
        return TERM_PROGRAM;
    }

    CAST_CODE(code)->co_file=object_repr(name);
    struct vm* vm_=::vm;
    ::vm=new_vm(0, code, compiler->instructions, &data); //data is still in scope...
    
    ::vm->globals=new_dict();
    ::callstack_head(vm->callstack).locals=FPLINCREF(::vm->globals);
    dict_set(::vm->globals, str_new_fromstr("__annotations__"), ::callstack_head(vm->callstack).annotations);
    dict_set(::vm->globals, str_new_fromstr("__name__"), name);
    ::vm->global_annotations=::callstack_head(vm->callstack).annotations;

    object* ret=run_vm(code, &::vm->ip);

    ERROR_RET(ret);

    object* dict=FPLINCREF(::callstack_head(vm->callstack).locals);

    object* o=module_new_fromdict(dict, name);
    
    vm_del(::vm);
    ::vm=vm_;

    return o;
}

void annotate_var(object* tp, object* name){
    dict_set(callstack_head(vm->callstack).annotations, name, tp);
}

void annotate_global(object* tp, object* name){
    dict_set(vm->global_annotations, name, tp);
}

void annotate_nonlocal(object* tp, object* name){
    for (int i=vm->callstack->size-1; i>=0; i--){
        struct callframe frame=vm->callstack->data[i];
        if (i==0){
            if (find(CAST_DICT(frame.locals)->keys->begin(), CAST_DICT(frame.locals)->keys->end(), name) != CAST_DICT(frame.locals)->keys->end()){
                if (CAST_DICT(frame.locals)->val->at(name)->type->size==0){
                    ((object_var*)CAST_DICT(frame.locals)->val->at(name))->gc_ref--;
                }
                FPLDECREF(CAST_DICT(frame.locals)->val->at(name));
                
                if (tp->type->size==0){
                    ((object_var*)tp)->gc_ref++;
                }
                dict_set(frame.locals, name, tp);

                return;
            }
        }
        if (frame.callable!=NULL && object_istype(frame.callable->type, &FuncType)\
        && CAST_FUNC(frame.callable)->closure!=NULL){
            object* closure=CAST_FUNC(frame.callable)->closure;
            //Check if name in closure
            if (find(CAST_DICT(closure)->keys->begin(), CAST_DICT(closure)->keys->end(), name) != CAST_DICT(closure)->keys->end()){
                if (CAST_DICT(closure)->val->at(name)->type->size==0){
                    ((object_var*)CAST_DICT(closure)->val->at(name))->gc_ref--;
                }
                FPLDECREF(CAST_DICT(closure)->val->at(name));
                
                if (tp->type->size==0){
                    ((object_var*)tp)->gc_ref++;
                }
                dict_set(closure, name, tp);

                return;
            }
        }
    }
}

object* run_vm(object* codeobj, uint32_t* ip){
    object* code=CAST_CODE(codeobj)->co_code;
    object* lines=CAST_CODE(codeobj)->co_lines;

    uint32_t arg;
    
    static void* dispatch_table[]={
        &&LOAD_CONST,
        &&STORE_NAME,
        &&LOAD_NAME,
        &&STORE_GLOBAL,
        &&LOAD_GLOBAL,
        &&BINOP_ADD,
        &&BINOP_SUB,
        &&UNARY_NEG,
        &&BINOP_MUL,
        &&BINOP_DIV,
        &&MAKE_FUNCTION,
        &&RETURN_VAL,
        &&CALL_FUNCTION,
        &&BUILD_TUPLE,
        &&BUILD_DICT,
        &&LOAD_BUILD_CLASS,
        &&LOAD_ATTR,
        &&STORE_ATTR,
        &&CALL_METHOD,
        &&BUILD_LIST,
        &&BINOP_IS,
        &&BINOP_EE,
        &&POP_JMP_TOS_FALSE,
        &&JUMP_DELTA,
        &&BINOP_SUBSCR,
        &&RAISE_EXC,
        &&STORE_SUBSCR,
        &&DUP_TOS,
        &&POP_TOS,
        &&SETUP_TRY,
        &&FINISH_TRY,
        &&BINOP_EXC_CMP,
        &&BINOP_GT,
        &&BINOP_GTE,
        &&BINOP_LT,
        &&BINOP_LTE,
        &&FOR_TOS_ITER,
        &&JUMP_TO,
        &&EXTRACT_ITER,
        &&BREAK_LOOP,
        &&CONTINUE_LOOP,
        &&UNPACK_SEQ,
        &&BINOP_IADD,
        &&BINOP_ISUB,
        &&BINOP_IMUL,
        &&BINOP_IDIV,
        &&IMPORT_NAME,
        &&IMPORT_FROM_MOD,
        &&MAKE_SLICE,
        &&BINOP_NE,
        &&DEL_SUBSCR,
        &&DEL_NAME,
        &&BINOP_MOD,
        &&BINOP_POW,
        &&BINOP_IPOW,
        &&BINOP_IMOD,
        &&BINOP_AND,
        &&BINOP_OR,
        &&UNARY_NOT,
        &&BUILD_STRING,
        &&POP_JMP_TOS_TRUE,
        &&RAISE_ASSERTIONERR,
        &&DEL_GLBL,
        &&DEL_ATTR,
        &&MAKE_CLOSURE,
        &&LOAD_NONLOCAL,
        &&STORE_NONLOCAL,
        &&DEL_NONLOCAL,
        &&BITWISE_NOT,
        &&BITWISE_AND,
        &&BITWISE_OR,
        &&BITWISE_LSHIFT,
        &&BITWISE_RSHIFT,
        &&BINOP_IAND,
        &&BINOP_IOR,
        &&BINOP_ILSH,
        &&BINOP_IRSH,
        &&BINOP_NOTIN,
        &&BINOP_IN,
        &&BINOP_ISNOT,
        &&BINOP_FLDIV,
        &&BINOP_IFLDIV,
        &&LOAD_METHOD,
        &&TERNARY_TEST,
        &&CALL_FUNCTION_BOTTOM,
        &&ANNOTATE_GLOBAL,
        &&ANNOTATE_NAME,
        &&ANNOTATE_NONLOCAL,
        &&STORE_ATTR_ANNOTATE,
        &&YIELD_VALUE,
        &&MAKE_GENERATOR,
        &&MAKE_CLOSURE_GENERATOR,
        &&BUILD_SET,
        &&CLEAR_EXC,
        &&ENTER_WHILE,
        &&EXIT_WHILE,
        &&ENTER_WITH,
        &&EXIT_WITH,
        &&SEQ_APPEND,
        &&DICT_SET,
        &&BITWISE_XOR,
    };
    
    uint32_t* code_array=CAST_CODE(codeobj)->code;
    
    DISPATCH();
    while(1){
        LOAD_CONST:{
            add_dataframe(vm, vm->objstack, list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_consts, arg));
            DISPATCH();
        }

        STORE_GLOBAL:{
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=peek_dataframe(vm->objstack);
            for (auto k: (*CAST_DICT(vm->globals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(vm->globals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(vm->globals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }
            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            dict_set(vm->globals, name, value);
            DISPATCH();
        }

        LOAD_GLOBAL:{
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);

            for (auto k: (*CAST_DICT(vm->globals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    add_dataframe(vm, vm->objstack, CAST_DICT(vm->globals)->val->at(name));
                    DISPATCH();
                }
            }
            for (size_t i=0; i<nbuiltins; i++){
                if (object_istype(builtins[i]->type, &BuiltinType)){
                    if (istrue(object_cmp(CAST_BUILTIN(builtins[i])->name, name, CMP_EQ))){
                        add_dataframe(vm, vm->objstack, builtins[i]);
                        DISPATCH();
                    }
                }
                if (object_istype(builtins[i]->type, &TypeType)){
                    if (CAST_TYPE(builtins[i])->name->compare((*CAST_STRING(name)->val))==0){
                        add_dataframe(vm, vm->objstack, builtins[i]);
                        DISPATCH();
                    }
                }
            }
            
            vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
            goto exc;
        }

        STORE_NAME:{
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=peek_dataframe(vm->objstack);
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
        }

        LOAD_NAME:{
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);

            for (int i=vm->callstack->size-1; i>=0; i--){
                struct callframe frame=vm->callstack->data[i];
                for (auto k: (*CAST_DICT(frame.locals)->val)){
                    if (istrue(object_cmp(name, k.first, CMP_EQ))){
                        add_dataframe(vm, vm->objstack, CAST_DICT(frame.locals)->val->at(name));
                        DISPATCH();
                    }
                }

                if (frame.callable!=NULL && object_istype(frame.callable->type, &FuncType)\
                && CAST_FUNC(frame.callable)->closure!=NULL){
                    object* closure=CAST_FUNC(frame.callable)->closure;
                    //Check if name in closure
                    if (find(CAST_DICT(closure)->keys->begin(), CAST_DICT(closure)->keys->end(), name) != CAST_DICT(closure)->keys->end()){
                        add_dataframe(vm, vm->objstack, CAST_DICT(closure)->val->at(name));
                        DISPATCH();
                    }
                }
            }

            for (size_t i=0; i<nbuiltins; i++){
                if (object_istype(builtins[i]->type, &BuiltinType)){
                    if (istrue(object_cmp(CAST_BUILTIN(builtins[i])->name, name, CMP_EQ))){
                        add_dataframe(vm, vm->objstack, builtins[i]);
                        DISPATCH();
                    }
                }
                if (object_istype(builtins[i]->type, &TypeType)){
                    if (CAST_TYPE(builtins[i])->name->compare((*CAST_STRING(name)->val))==0){
                        add_dataframe(vm, vm->objstack, builtins[i]);
                        DISPATCH();
                    } 
                }
            }

            
            for (auto k: (*CAST_DICT(vm->globals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    add_dataframe(vm, vm->objstack, CAST_DICT(vm->globals)->val->at(k.first));
                    DISPATCH();
                }
            }

            vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
            goto exc;
        }

        BINOP_ADD:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_add(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand types for +: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BINOP_SUB:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_sub(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand types for -: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BINOP_MUL:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_mul(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand types for *: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();  
        }

        BINOP_DIV:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_div(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand types for /: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BINOP_IS:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=left==right ? new_bool_true() : new_bool_false();
            add_dataframe(vm, vm->objstack, ret);
            DISPATCH();
        }

        BINOP_EE:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_cmp(left, right, CMP_EQ);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                FPLDECREF(vm->exception);
                vm->exception=NULL;
                add_dataframe(vm, vm->objstack, new_bool_false());
            }
            DISPATCH();
        }

        BINOP_NE:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_cmp(left, right, CMP_NE);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                FPLDECREF(vm->exception);
                vm->exception=NULL;
                add_dataframe(vm, vm->objstack, new_bool_false());
            }
            DISPATCH();
        }

        UNARY_NEG:{
            struct object* right=pop_dataframe(vm->objstack);
            
            object* ret=object_neg(right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid unary operand -: '%s'.", right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        MAKE_FUNCTION:{
            int flags=CAST_INT(pop_dataframe(vm->objstack))->val->to_int(); //<- Flags
            object* stargs=NULL;
            object* stkwargs=NULL;
            
            if (flags==FUNC_STARARGS){
                stargs=pop_dataframe(vm->objstack);
            }
            else if (flags==FUNC_STARKWARGS){
                stkwargs=pop_dataframe(vm->objstack); 
            }
            else if (flags==FUNC_STAR){
                stkwargs=pop_dataframe(vm->objstack); 
                stargs=pop_dataframe(vm->objstack); 
            }
            object* code=pop_dataframe(vm->objstack); //<- Code
            object* annotations=pop_dataframe(vm->objstack); //<- Annotations
            object* args=pop_dataframe(vm->objstack); //<- Args
            object* kwargs=pop_dataframe(vm->objstack); //<- Kwargs
            object* name=pop_dataframe(vm->objstack); //<- Name
            object* func=func_new_code(code, args, kwargs, arg, name, NULL, FUNCTION_NORMAL, flags, stargs, stkwargs, annotations, false, NULL, vm->globals, vm->global_annotations);

            add_dataframe(vm, vm->objstack, func);
            DISPATCH();
        }

        RETURN_VAL: {
            GIL.unlock();
            return FPLINCREF(pop_dataframe(vm->objstack));
        }

        CALL_METHOD: {
            uint32_t argc=arg;
            uint32_t posargc=CAST_INT(pop_dataframe(vm->objstack))->val->to_int();
            uint32_t kwargc=argc-posargc;

            object* stkwargs=pop_dataframe(vm->objstack);
            object* stargs=pop_dataframe(vm->objstack);

            int stkwargsidx=0;
            int stargsidx=0;
            
            //Setup kwargs
            object* kwargs=new_dict();
            object* val;
            for (uint32_t i=0; i<kwargc; i++){
                val=pop_dataframe(vm->objstack);
                dict_set(kwargs, pop_dataframe(vm->objstack), val);
            }
            //

            //Setup args
            object* args=new_tuple();
            for (uint32_t i=0; i<posargc; i++){
                if (stkwargsidx<CAST_LIST(stkwargs)->size && CAST_INT(list_index_int(stkwargs, stkwargsidx))->val->to_int()==i){
                    //pop dict, insert values
                    object* ob=pop_dataframe(vm->objstack);
                    
                    if (!object_istype(ob->type, &DictType)){
                        vm_add_err(&TypeError, vm, "Expected dict object, got '%s' object", ob->type->name->c_str());
                        goto exc;
                    }
                    
                    object* iter=ob->type->slot_iter(ob);

                    object* res=NULL;
                    
                    object* o=iter->type->slot_next(iter);
                    object* v;
                    while (vm->exception==NULL){
                        v=ob->type->slot_mappings->slot_get(ob, o);
                        ERROR_RET(v);
                        dict_set(kwargs, o, v);
                        
                        o=iter->type->slot_next(iter);
                    }
                    if (vm->exception!=NULL){
                        FPLDECREF(vm->exception);
                        vm->exception=NULL;
                    }


                    stkwargsidx++;
                    continue;
                }
                if (stargsidx<CAST_LIST(stargs)->size && CAST_INT(list_index_int(stargs, stargsidx))->val->to_int()==i){
                    //pop dict, insert values
                    object* ob=pop_dataframe(vm->objstack);
                    
                    if (!object_istype(ob->type, &ListType) && !object_istype(ob->type, &TupleType)){
                        vm_add_err(&TypeError, vm, "Expected list or tuple object, got '%s' object", ob->type->name->c_str());
                        DISPATCH();
                    }
                    
                    object* iter=ob->type->slot_iter(ob);

                    object* one=new_int_fromint(0);
                    object* two=new_int_fromint(1);
                    object* res=NULL;
                    
                    object* o=iter->type->slot_next(iter);
                    object* v;
                    while (vm->exception==NULL){
                        tuple_append(args, o);
                        
                        o=iter->type->slot_next(iter);
                    }
                    if (vm->exception!=NULL){
                        FPLDECREF(vm->exception);
                        vm->exception=NULL;
                    }


                    stargsidx++;
                    continue;
                }
                tuple_append(args, pop_dataframe(vm->objstack));
            }
            //

            object* function=pop_dataframe(vm->objstack);
            object* head=pop_dataframe(vm->objstack);
            
            if (function->type->slot_call==NULL){
                vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
                goto exc;
            }

            //Call
            object* ret=function->type->slot_call(function, args, kwargs);
            FPLDECREF(args);
            FPLDECREF(kwargs);
            if (ret==NULL){
                goto exc;
            }
            if (ret==TERM_PROGRAM|| ret==CALL_ERR){
                GIL.unlock();
                return TERM_PROGRAM;
            }
            add_dataframe(vm, vm->objstack, ret);

            DISPATCH();
        }

        CALL_FUNCTION: {
            object* function=pop_dataframe(vm->objstack);
            
            uint32_t argc=arg;
            uint32_t posargc=CAST_INT(pop_dataframe(vm->objstack))->val->to_int();
            uint32_t kwargc=argc-posargc;

            object* stkwargs=pop_dataframe(vm->objstack);
            object* stargs=pop_dataframe(vm->objstack);

            if (function->type->slot_call==NULL){
                vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
                goto exc;
            }

            int stkwargsidx=0;
            int stargsidx=0;

            //Setup kwargs
            object* kwargs=new_dict();
            object* val;
            for (uint32_t i=0; i<kwargc; i++){
                val=pop_dataframe(vm->objstack);
                dict_set(kwargs, pop_dataframe(vm->objstack), val);
            }
            //

            //Setup args
            object* args=new_tuple();
            for (uint32_t i=0; i<posargc; i++){
                if (stkwargsidx<CAST_LIST(stkwargs)->size && CAST_INT(list_index_int(stkwargs, stkwargsidx))->val->to_int()==i){
                    //pop dict, insert values
                    object* ob=pop_dataframe(vm->objstack);
                    
                    if (!object_istype(ob->type, &DictType)){
                        vm_add_err(&TypeError, vm, "Expected dict object, got '%s' object", ob->type->name->c_str());
                        DISPATCH();
                    }
                    
                    object* iter=ob->type->slot_iter(ob);

                    object* res=NULL;
                    
                    object* o=iter->type->slot_next(iter);
                    object* v;
                    while (vm->exception==NULL){
                        v=ob->type->slot_mappings->slot_get(ob, o);
                        ERROR_RET(v);
                        dict_set(kwargs, o, v);
                        
                        o=iter->type->slot_next(iter);
                    }
                    if (vm->exception!=NULL){
                        FPLDECREF(vm->exception);
                        vm->exception=NULL;
                    }


                    stkwargsidx++;
                    continue;
                }
                if (stargsidx<CAST_LIST(stargs)->size && CAST_INT(list_index_int(stargs, stargsidx))->val->to_int()==i){
                    //pop dict, insert values
                    object* ob=pop_dataframe(vm->objstack);
                    
                    if (!object_istype(ob->type, &ListType) && !object_istype(ob->type, &TupleType)){
                        vm_add_err(&TypeError, vm, "Expected list or tuple object, got '%s' object", ob->type->name->c_str());
                        goto exc;
                    }
                    
                    object* iter=ob->type->slot_iter(ob);

                    object* one=new_int_fromint(0);
                    object* two=new_int_fromint(1);
                    object* res=NULL;
                    
                    object* o=iter->type->slot_next(iter);
                    object* v;
                    while (vm->exception==NULL){
                        tuple_append(args, o);
                        
                        o=iter->type->slot_next(iter);
                    }
                    if (vm->exception!=NULL){
                        FPLDECREF(vm->exception);
                        vm->exception=NULL;
                    }


                    stargsidx++;
                    continue;
                }
                tuple_append(args, pop_dataframe(vm->objstack));
            }
            //
            
            //Call
            object* ret=function->type->slot_call(function, args, kwargs);
            FPLDECREF(args);
            FPLDECREF(kwargs);
            if (ret==NULL){
                goto exc;
            }
            if (ret==TERM_PROGRAM || ret==CALL_ERR){
                GIL.unlock();
                return TERM_PROGRAM;
            }
            
            add_dataframe(vm, vm->objstack, ret);
            
            DISPATCH();            
        }

        CALL_FUNCTION_BOTTOM: {
            uint32_t argc=arg;
            uint32_t posargc=CAST_INT(pop_dataframe(vm->objstack))->val->to_int();
            uint32_t kwargc=argc-posargc;

            object* stkwargs=pop_dataframe(vm->objstack);
            object* stargs=pop_dataframe(vm->objstack);

            int stkwargsidx=0;
            int stargsidx=0;

            //Setup kwargs
            object* kwargs=new_dict();
            object* val;
            for (uint32_t i=0; i<kwargc; i++){
                val=pop_dataframe(vm->objstack);
                dict_set(kwargs, pop_dataframe(vm->objstack), val);
            }
            //

            //Setup args
            object* args=new_tuple();
            for (uint32_t i=0; i<posargc; i++){
                if (stkwargsidx<CAST_LIST(stkwargs)->size && CAST_INT(list_index_int(stkwargs, stkwargsidx))->val->to_int()==i){
                    //pop dict, insert values
                    object* ob=pop_dataframe(vm->objstack);
                    
                    if (!object_istype(ob->type, &DictType)){
                        vm_add_err(&TypeError, vm, "Expected dict or tuple object, got '%s' object", ob->type->name->c_str());
                        goto exc;
                    }
                    
                    object* iter=ob->type->slot_iter(ob);

                    object* res=NULL;
                    
                    object* o=iter->type->slot_next(iter);
                    object* v;
                    while (vm->exception==NULL){
                        v=ob->type->slot_mappings->slot_get(ob, o);
                        ERROR_RET(v);
                        dict_set(kwargs, o, v);
                        
                        o=iter->type->slot_next(iter);
                    }
                    if (vm->exception!=NULL){
                        FPLDECREF(vm->exception);
                        vm->exception=NULL;
                    }


                    stkwargsidx++;
                    continue;
                }
                if (stargsidx<CAST_LIST(stargs)->size && CAST_INT(list_index_int(stargs, stargsidx))->val->to_int()==i){
                    //pop dict, insert values
                    object* ob=pop_dataframe(vm->objstack);
                    
                    if (!object_istype(ob->type, &ListType) && !object_istype(ob->type, &TupleType)){
                        vm_add_err(&TypeError, vm, "Expected list or tuple object, got '%s' object", ob->type->name->c_str());
                        DISPATCH();
                    }
                    
                    object* iter=ob->type->slot_iter(ob);

                    object* one=new_int_fromint(0);
                    object* two=new_int_fromint(1);
                    object* res=NULL;
                    
                    object* o=iter->type->slot_next(iter);
                    object* v;
                    while (vm->exception==NULL){
                        tuple_append(args, o);
                        
                        o=iter->type->slot_next(iter);
                    }
                    if (vm->exception!=NULL){
                        FPLDECREF(vm->exception);
                        vm->exception=NULL;
                    }


                    stargsidx++;
                    continue;
                }
                tuple_append(args, pop_dataframe(vm->objstack));
            }
            //

            
            object* function=pop_dataframe(vm->objstack);
            if (function->type->slot_call==NULL){
                vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
                goto exc;
            }
            
            //Call
            object* ret=function->type->slot_call(function, args, kwargs);
            FPLDECREF(args);
            FPLDECREF(kwargs);
            if (ret==NULL){ 
                goto exc;
            }
            if (ret==TERM_PROGRAM || ret==CALL_ERR){
                GIL.unlock();
                return TERM_PROGRAM;
            }
            
            add_dataframe(vm, vm->objstack, ret);
            
            DISPATCH();            
        }

        BUILD_LIST: {
            object* list=new_list();
            for (int i=0; i<arg; i++){
                list_append(list, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, list);
            DISPATCH();
        }

        BUILD_TUPLE: {
            object* tuple=new_tuple();
            for (int i=0; i<arg; i++){
                tuple_append(tuple, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, tuple);
            DISPATCH();
        }

        BUILD_DICT: {
            object* dict=new_dict();
            for (int i=0; i<arg; i++){
                object* name=pop_dataframe(vm->objstack);
                
                dict_set(dict, name, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, dict);
            DISPATCH();
        }

        LOAD_BUILD_CLASS: {
            add_dataframe(vm, vm->objstack, BUILTIN_BUILD_CLASS);
            DISPATCH();
        }

        LOAD_ATTR: {
            object* obj=pop_dataframe(vm->objstack);
            object* attr=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* ret=object_getattr(obj, attr);            
            
            if (ret==NULL){ 
                goto exc;
            }
            if (ret==TERM_PROGRAM || ret==CALL_ERR){
                GIL.unlock();
                return TERM_PROGRAM;
            }
            
            add_dataframe(vm, vm->objstack, ret);
            DISPATCH();
        }    

        LOAD_METHOD: {
            object* obj=peek_dataframe(vm->objstack);
            object* attr=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* ret=object_getattr(obj, attr);            
            
            
            if (ret==NULL){
                goto exc;
            }
            if (ret==TERM_PROGRAM|| ret==CALL_ERR){
                GIL.unlock();
                return TERM_PROGRAM;
            }

            add_dataframe(vm, vm->objstack, ret);
            DISPATCH();
        }  

        STORE_ATTR: {
            object* obj=pop_dataframe(vm->objstack);
            object* val=peek_dataframe(vm->objstack); //For multiple assignment
            object* attr=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* ret=object_setattr(obj, attr, val);
            
            
            if (ret==NULL){
                goto exc;
            }
            if (ret==TERM_PROGRAM|| ret==CALL_ERR){
                GIL.unlock();
                return TERM_PROGRAM;
            }

            DISPATCH();
        }

        POP_JMP_TOS_FALSE: {
            object* o=pop_dataframe(vm->objstack);
            object* val=object_istruthy(o);
            if (!istrue(val)){
                (*ip)=(*ip)+arg;
                DISPATCH();
            }
            DISPATCH();
        }

        JUMP_DELTA: {
            (*ip)=(*ip)+arg;
            DISPATCH();
        }

        BINOP_SUBSCR: {
            object* idx=pop_dataframe(vm->objstack);
            object* base=pop_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, object_get(base, idx));
            if (peek_dataframe(vm->objstack)==NULL){
                goto exc;
            }
            DISPATCH();
        }

        RAISE_EXC: {
            if (!object_issubclass(peek_dataframe(vm->objstack), &ExceptionType)){
                vm_add_err(&TypeError, vm, "Exceptions must be subclass of Exception");
                goto exc;
            }
            if (vm->exception!=NULL){
                FPLDECREF(vm->exception);
            }
            vm->exception=pop_dataframe(vm->objstack);
            goto exc;
        }

        CLEAR_EXC: {
            if (vm->exception!=NULL){
                FPLDECREF(vm->exception);
            }
            vm->exception=NULL;
            DISPATCH();
        }

        STORE_SUBSCR: {
            object* val=pop_dataframe(vm->objstack);
            object* idx=pop_dataframe(vm->objstack);
            object* base=pop_dataframe(vm->objstack);
            object_set(base, idx, val);
            DISPATCH();
        }

        DUP_TOS: {
            add_dataframe(vm, vm->objstack, peek_dataframe(vm->objstack));
            DISPATCH();
        }

        POP_TOS: {
            FPLDECREF(pop_dataframe(vm->objstack));
            DISPATCH();
        }

        SETUP_TRY: {
            add_blockframe(ip, vm, vm->blockstack, arg, TRY_BLOCK);
            DISPATCH();
        }

        FINISH_TRY: {
            pop_blockframe(vm->blockstack);
            DISPATCH();
        }

        BINOP_EXC_CMP: {
            object* self=pop_dataframe(vm->objstack);
            object* other=pop_dataframe(vm->objstack);
            if (object_issubclass(other, CAST_TYPE(self))){
                add_dataframe(vm, vm->objstack, new_bool_true());
                DISPATCH();
            }
            add_dataframe(vm, vm->objstack, new_bool_false());
            DISPATCH();
        }

        BINOP_GT: {
            object* right=pop_dataframe(vm->objstack);
            object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_gt(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand types for >: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BINOP_GTE: {
            object* right=pop_dataframe(vm->objstack);
            object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_gte(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand types for >=: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BINOP_LT: {
            object* right=pop_dataframe(vm->objstack);
            object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_lt(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand types for <: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BINOP_LTE: {
            object* right=pop_dataframe(vm->objstack);
            object* left=pop_dataframe(vm->objstack);
            object* ret=object_lte(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand types for <=: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        EXTRACT_ITER: {
            object* it=pop_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, it->type->slot_iter(it));
            DISPATCH();
        }

        FOR_TOS_ITER: {
            if (vm->blockstack->size==0 || blockstack_head(vm->blockstack).type!=FOR_BLOCK || (blockstack_head(vm->blockstack).type==FOR_BLOCK && blockstack_head(vm->blockstack).arg!=arg) ){
                add_blockframe(ip, vm, vm->blockstack, arg, FOR_BLOCK);
            }
            
            object* it=peek_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, it->type->slot_next(it));
            if (vm->exception!=NULL && object_istype(vm->exception->type, &StopIteration)){
                FPLDECREF(vm->exception);
                vm->exception=NULL;
                (*ip)=blockstack_head(vm->blockstack).arg;
                pop_blockframe(vm->blockstack);
                pop_dataframe(vm->objstack);
                pop_dataframe(vm->objstack);
            }
            DISPATCH();
        }

        JUMP_TO: {
            (*ip)=arg;
            DISPATCH();
        }

        BREAK_LOOP: {
            (*ip)=blockstack_head(vm->blockstack).arg;
            pop_blockframe(vm->blockstack);
            DISPATCH();
        }

        CONTINUE_LOOP: {
            (*ip)=blockstack_head(vm->blockstack).start_ip;
            pop_blockframe(vm->blockstack);
            DISPATCH();
        }

        UNPACK_SEQ: {
            object* o=peek_dataframe(vm->objstack);

            object* one;
            object* res;
            object* ob;
            uint32_t len=0;
            object* iter;
            
            if (o->type->slot_iter==NULL){
                goto fail;
            }
            
            iter=o->type->slot_iter(o);
            
            if (iter==NULL){
                goto fail;
            }

            one=new_int_fromint(0);
            res=NULL;
            
            ob=iter->type->slot_next(iter);

            while (vm->exception==NULL){
                add_dataframe(vm, vm->objstack, ob);
                
                cont:
                ob=iter->type->slot_next(iter);
                len++;
            }
            if (vm->exception!=NULL){
                FPLDECREF(vm->exception);
                vm->exception=NULL;
            }
            
            fail:
            if (len>arg){
                vm_add_err(&ValueError, vm, "Too many values to unpack, expected %d", arg);
                goto exc;
            }
            if (len<arg){
                vm_add_err(&ValueError, vm, "Not enough values to unpack, expected %d", arg);
                goto exc;
            }
            
            DISPATCH();
        }

        BINOP_IADD:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_add(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for +: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=ret;
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
            DISPATCH();
        }

        BINOP_ISUB:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_sub(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for -: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=ret;
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
            DISPATCH();
        }

        BINOP_IMUL:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_mul(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for *: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=ret;
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
            DISPATCH();  
        }

        BINOP_IDIV:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_div(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for /: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=ret;
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
            DISPATCH();
        }

        IMPORT_NAME: {
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            string nm_plain=*CAST_STRING(name)->val;

            string data="";
                    
            string name_plain=nm_plain+".fpl";

            for (size_t i=0; i<CAST_LIST(vm->path)->size; i++){
                string name_=object_cstr(list_index_int(vm->path, i))+name_plain;
                string nm=object_cstr(list_index_int(vm->path, i))+nm_plain;
                struct stat st;
                bool foundfpl=false;
                if( stat(nm.c_str(),&st) == 0 || stat(name_.c_str(),&st) == 0 ){
                    if( st.st_mode & S_IFDIR ){//Directory
                        object* dict=new_dict();
                        
                        DIR *dr;
                        struct dirent *en;
                        dr = opendir(nm.c_str());
                        if (dr) {
                            while ((en = readdir(dr)) != NULL) {
                                if (string(en->d_name)==string(".") || string(en->d_name)==string("..")){
                                    continue;
                                }

                                
                                string extension="";
                                string filename="";
                                bool extwr=true;
                                for (int i=string(en->d_name).size(); i>0; i--){
                                    if (string(en->d_name).at(i-1)=='.' && extwr){
                                        extwr=false;
                                        continue;
                                    }
                                    if (!extwr){
                                        filename+=string(en->d_name).at(i-1);
                                    }
                                    if (extwr){
                                        extension+=string(en->d_name).at(i-1);
                                    }
                                }
                                reverse(extension.begin(), extension.end());
                                reverse(filename.begin(), filename.end());
                                if (extension!="fpl"){
                                    continue;
                                }
                                foundfpl=true;

                                //try en->d_name
                                //Later try nm as folder
                                
                                FILE* f=fopen((nm+"/"+string(en->d_name)).c_str(), "rb");
                                if (f==NULL){
                                    vm_add_err(&ImportError, vm, "'%s' not found", (nm+"/"+string(en->d_name)).c_str());
                                    goto exc;
                                }


                                fseek(f, 0, SEEK_END);
                                long fsize = ftell(f);
                                fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

                                char *s = (char*)fpl_malloc(fsize + 1);
                                int i=fread(s, fsize, 1, f);
                                if (i==0 && fsize>0){
                                    vm_add_err(&InvalidOperationError, vm, "Unable to read from file");
                                    goto exc;
                                }
                                s[fsize] = 0;
                                string str(s);
                                
                                object* o=import_name(str, str_new_fromstr(filename));
                                ERROR_RET(o);
                                dict_set(dict, str_new_fromstr(filename), o);
                            }
                            closedir(dr);
                        }
                        if (foundfpl){
                            object* o=module_new_fromdict(dict, str_new_fromstr(string(nm_plain)));
                            add_dataframe(vm, vm->objstack, o);

                            DISPATCH();
                        }
                        goto builtin;
                    }
                    else{ //File
                        //try nm.fpl
                        //Later try nm as folder
                        FILE* f=fopen(name_.c_str(), "rb");

                        fseek(f, 0, SEEK_END);
                        long fsize = ftell(f);
                        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

                        char *s = (char*)fpl_malloc(fsize + 1);
                        int i=fread(s, fsize, 1, f);
                        if (i==0 && fsize>0){
                            vm_add_err(&InvalidOperationError, vm, "Unable to read from file");
                            goto exc;
                        }
                        s[fsize] = 0;
                        string str(s);
                        data=str;
                    }
                }
                else{
                    builtin:
                    bool done=false;
                    for (int i=0; i<nmodules; i++){
                        if (istrue(object_cmp(name, CAST_MODULE(modules[i])->name, CMP_EQ))){
                            add_dataframe(vm, vm->objstack, FPLINCREF(modules[i]));
                            done=true;
                            DISPATCH();
                        }
                    }

                    continue;
                }
                
                object* o=import_name(data, name);
                ERROR_RET(o);
                add_dataframe(vm, vm->objstack, o);
                DISPATCH();
            }

            vm_add_err(&ImportError, vm, "'%s' not found", nm_plain.c_str());
            goto exc;
        }

        IMPORT_FROM_MOD: {
            object* names=pop_dataframe(vm->objstack);
            object* lib=pop_dataframe(vm->objstack);
            uint32_t len=CAST_INT(names->type->slot_mappings->slot_len(names))->val->to_int();
            if (len==0){
                for (auto k: *CAST_DICT(CAST_MODULE(lib)->dict)->val){
                    object* name=k.first;
                    object* value=k.second;
                    
                    for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                        if (istrue(object_cmp(name, k.first, CMP_EQ))){
                            if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                                ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                            }
                            break;
                        }
                    }

                    if (value->type->size==0){
                        ((object_var*)value)->gc_ref++;
                    }

                    dict_set(callstack_head(vm->callstack).locals, name, value);
                    DISPATCH();
                }
            }
            for (uint32_t i=0; i<len; i++){
                object* o=object_getattr(lib, list_index_int(names, i));
            
                if (o==TERM_PROGRAM || o==CALL_ERR){
                    GIL.unlock();
                    return TERM_PROGRAM;
                }
                if (o==NULL){
                    FPLDECREF(vm->exception);
                    vm->exception=NULL;
                    vm_add_err(&ImportError,vm, "Cannot import name '%s' from '%s'",CAST_STRING(list_index_int(names, i))->val->c_str(), CAST_STRING(CAST_MODULE(lib)->name)->val->c_str());
                    goto exc;
                }

                object* name=list_index_int(names, i);
                object* value=o;
                
                for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                    if (istrue(object_cmp(name, k.first, CMP_EQ))){
                        if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                            ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                        }
                        break;
                    }
                }

                if (value->type->size==0){
                    ((object_var*)value)->gc_ref++;
                }

                dict_set(callstack_head(vm->callstack).locals, name, value);
                DISPATCH();
                
            }
            DISPATCH();
        }

        MAKE_SLICE: {
            object* end=pop_dataframe(vm->objstack);
            object* start=pop_dataframe(vm->objstack);
            object* base=peek_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, slice_new_fromnums(start, end));
            DISPATCH();
        }

        DEL_SUBSCR: {
            object* idx=pop_dataframe(vm->objstack);
            object* base=pop_dataframe(vm->objstack);
            object_del_item(base, idx);
            DISPATCH();
        }

        DEL_NAME: {
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);

            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(name)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }

                    if (object_find_bool_dict_keys(callstack_head(vm->callstack).annotations, name)){
                        if (CAST_DICT(callstack_head(vm->callstack).annotations)->val->at(name)->type->size==0){
                            ((object_var*)CAST_DICT(callstack_head(vm->callstack).annotations)->val->at(name))->gc_ref--;
                        }
                        dict_set(callstack_head(vm->callstack).annotations, name, NULL);
                    }
                    dict_set(callstack_head(vm->callstack).locals, name, NULL);
                    DISPATCH();
                }
            }
            vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
            goto exc;
        }

        BINOP_MOD:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_mod(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand types for %: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BINOP_POW:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            object* ret=object_pow(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand types for **: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BINOP_IPOW:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_pow(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for **: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=ret;
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
            DISPATCH();
        }

        BINOP_IMOD:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_mod(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for **: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=ret;
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
            DISPATCH();
        }

        BINOP_AND:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            right=right->type->slot_number->slot_bool(right);
            left=right->type->slot_number->slot_bool(left);
            bool r=CAST_BOOL(right)->val;
            bool l=CAST_BOOL(left)->val;
            FPLDECREF(right);
            FPLDECREF(left);
            if (r && l){
                add_dataframe(vm, vm->objstack, new_bool_true());
                DISPATCH();
            }
            add_dataframe(vm, vm->objstack, new_bool_false());
            DISPATCH();
        }

        BINOP_OR:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            right=right->type->slot_number->slot_bool(right);
            left=right->type->slot_number->slot_bool(left);
            bool r=CAST_BOOL(right)->val;
            bool l=CAST_BOOL(left)->val;
            FPLDECREF(right);
            FPLDECREF(left);
            if (r || l){
                add_dataframe(vm, vm->objstack, new_bool_true());
                DISPATCH();
            }
            add_dataframe(vm, vm->objstack, new_bool_false());
            DISPATCH();
        }

        UNARY_NOT:{
            struct object* right=pop_dataframe(vm->objstack);
            
            right=right->type->slot_number->slot_bool(right);
            bool v=CAST_BOOL(right)->val;
            FPLDECREF(right);
            if (!v){
                add_dataframe(vm, vm->objstack, new_bool_true());
                DISPATCH();
            }
            add_dataframe(vm, vm->objstack, new_bool_false());
            DISPATCH();
        }

        BUILD_STRING: {
            string s="";
            vector<string> strs;
            strs.clear();
            for (int i=0; i<arg; i++){
                object* flag = pop_dataframe(vm->objstack);
                object* o=pop_dataframe(vm->objstack);
                if (istrue(FPLINCREF(flag))){
                    strs.push_back(object_crepr(o));
                    continue;
                }
                strs.push_back(object_cstr(o));
            }
            
            for (int i=strs.size(); i>0; i--){
                s+=strs.at(i-1);
            }
            add_dataframe(vm, vm->objstack, str_new_fromstr(s));
            DISPATCH();
        }

        POP_JMP_TOS_TRUE: {
            object* o=pop_dataframe(vm->objstack);
            object* val=object_istruthy(o);
            if (istrue(val)){
                (*ip)=arg;
                DISPATCH();
            }
            DISPATCH();
        }

        RAISE_ASSERTIONERR: {
            object* exc=vm_setup_err(&AssertionError, vm, "");
            FPLDECREF(CAST_EXCEPTION(exc)->err);
            CAST_EXCEPTION(exc)->err=NULL;
            vm->exception=exc;
            DISPATCH();
        }        

        DEL_GLBL: {
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            
            for (auto k: (*CAST_DICT(vm->globals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(vm->globals)->val->at(name)->type->size==0){
                        ((object_var*)CAST_DICT(vm->globals)->val->at(k.first))->gc_ref--;
                    }

                    if (object_find_bool_dict_keys(vm->global_annotations, name)){
                        if (CAST_DICT(vm->global_annotations)->val->at(name)->type->size==0){
                            ((object_var*)CAST_DICT(vm->global_annotations)->val->at(k.first))->gc_ref--;
                        }
                        dict_set(vm->global_annotations, name, NULL);
                    }
                    dict_set(vm->globals, name, NULL);
                    DISPATCH();
                }
            }
            vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
            goto exc;
        }

        DEL_ATTR: {
            object* obj=pop_dataframe(vm->objstack);
            object* attr=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* ret=object_setattr(obj, attr, NULL);
            
            if (ret==NULL){
                goto exc;
            }
            if (ret==TERM_PROGRAM|| ret==CALL_ERR){
                GIL.unlock();
                return TERM_PROGRAM;
            }
            DISPATCH();
        }

        MAKE_CLOSURE:{
            int flags=CAST_INT(pop_dataframe(vm->objstack))->val->to_int(); //<- Flags
            object* stargs=NULL;
            object* stkwargs=NULL;
            if (flags==FUNC_STARARGS){
                stargs=pop_dataframe(vm->objstack); 
            }
            else if (flags==FUNC_STARKWARGS){
                stkwargs=pop_dataframe(vm->objstack); 
            }
            else if (flags==FUNC_STAR){
                stkwargs=pop_dataframe(vm->objstack); 
                stargs=pop_dataframe(vm->objstack); 
            }
            object* code=pop_dataframe(vm->objstack); //<- Code
            object* annotations=pop_dataframe(vm->objstack); //<- Annotations
            object* args=pop_dataframe(vm->objstack); //<- Args
            object* kwargs=pop_dataframe(vm->objstack); //<- Kwargs
            object* name=pop_dataframe(vm->objstack); //<- Name
            
            object* func=func_new_code(code, args, kwargs, arg, name, FPLINCREF(callstack_head(vm->callstack).locals), FUNCTION_NORMAL, flags, stargs, stkwargs, annotations, false, FPLINCREF(callstack_head(vm->callstack).annotations), vm->globals, vm->global_annotations);
            add_dataframe(vm, vm->objstack, func);
            DISPATCH();
        }

        LOAD_NONLOCAL:{
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);

            for (int i=vm->callstack->size-1; i>=0; i--){
                struct callframe frame=vm->callstack->data[i];
                if (i==vm->callstack->size-1){
                    for (auto k: (*CAST_DICT(frame.locals)->val)){
                        if (istrue(object_cmp(name, k.first, CMP_EQ))){
                            add_dataframe(vm, vm->objstack, CAST_DICT(frame.locals)->val->at(name));
                            DISPATCH();
                        }
                    }
                }

                if (frame.callable!=NULL && object_istype(frame.callable->type, &FuncType)\
                && CAST_FUNC(frame.callable)->closure!=NULL){
                    object* closure=CAST_FUNC(frame.callable)->closure;
                    //Check if name in closure
                    if (find(CAST_DICT(closure)->keys->begin(), CAST_DICT(closure)->keys->end(), name) != CAST_DICT(closure)->keys->end()){
                        add_dataframe(vm, vm->objstack, CAST_DICT(closure)->val->at(name));
                        DISPATCH();
                    }
                }
            }

            for (size_t i=0; i<nbuiltins; i++){
                if (object_istype(builtins[i]->type, &BuiltinType)){
                    if (istrue(object_cmp(CAST_BUILTIN(builtins[i])->name, name, CMP_EQ))){
                        add_dataframe(vm, vm->objstack, builtins[i]);
                        DISPATCH();
                    }
                }
                if (object_istype(builtins[i]->type, &TypeType)){
                    if (CAST_TYPE(builtins[i])->name->compare((*CAST_STRING(name)->val))==0){
                        add_dataframe(vm, vm->objstack, builtins[i]);
                        DISPATCH();
                    } 
                }
            }

            
            for (auto k: (*CAST_DICT(vm->globals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    add_dataframe(vm, vm->objstack, CAST_DICT(vm->globals)->val->at(k.first));
                    DISPATCH();
                }
            }

            vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
            goto exc;
        }

        STORE_NONLOCAL:{
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* val=peek_dataframe(vm->objstack);

            for (int i=vm->callstack->size-1; i>=0; i--){
                struct callframe frame=vm->callstack->data[i];
                if (i==vm->callstack->size-1){
                    if (find(CAST_DICT(frame.locals)->keys->begin(), CAST_DICT(frame.locals)->keys->end(), name) != CAST_DICT(frame.locals)->keys->end()){
                        if (CAST_DICT(frame.locals)->val->at(name)->type->size==0){
                            ((object_var*)CAST_DICT(frame.locals)->val->at(name))->gc_ref--;
                        }
                        
                        if (val->type->size==0){
                            ((object_var*)val)->gc_ref++;
                        }
                        dict_set(frame.locals, name, val);
                        DISPATCH();
                    }
                }
                if (frame.callable!=NULL && object_istype(frame.callable->type, &FuncType)\
                && CAST_FUNC(frame.callable)->closure!=NULL){
                    object* closure=CAST_FUNC(frame.callable)->closure;
                    //Check if name in closure
                    if (find(CAST_DICT(closure)->keys->begin(), CAST_DICT(closure)->keys->end(), name) != CAST_DICT(closure)->keys->end()){
                        if (CAST_DICT(closure)->val->at(name)->type->size==0){
                            ((object_var*)CAST_DICT(closure)->val->at(name))->gc_ref--;
                        }
                        FPLDECREF(CAST_DICT(closure)->val->at(name));
                        
                        if (val->type->size==0){
                            ((object_var*)val)->gc_ref++;
                        }
                        dict_set(closure, name, val);

                        DISPATCH();
                    }
                }
            }

            vm_add_err(&NameError, vm, "Nonlocal %s referenced before assignment", object_crepr(name).c_str());
            goto exc;
        }

        DEL_NONLOCAL:{
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);

            for (int i=vm->callstack->size-1; i>=0; i--){
                struct callframe frame=vm->callstack->data[i];
                if (i==vm->callstack->size-1){
                    if (find(CAST_DICT(frame.locals)->keys->begin(), CAST_DICT(frame.locals)->keys->end(), name) != CAST_DICT(frame.locals)->keys->end()){
                        if (CAST_DICT(frame.locals)->val->at(name)->type->size==0){
                            ((object_var*)CAST_DICT(frame.locals)->val->at(name))->gc_ref--;
                        }
                        if (object_find_bool_dict_keys(frame.annotations, name)){
                            if (CAST_DICT(frame.annotations)->val->at(name)->type->size==0){
                                ((object_var*)CAST_DICT(frame.annotations)->val->at(name))->gc_ref--;
                            }
                            dict_set(frame.annotations, name, NULL);
                        }
                        dict_set(frame.locals, name, NULL);
                        DISPATCH();
                    }
                }
                if (frame.callable!=NULL && object_istype(frame.callable->type, &FuncType)\
                && CAST_FUNC(frame.callable)->closure!=NULL){
                    object* closure=CAST_FUNC(frame.callable)->closure;
                    //Check if name in closure
                    if (find(CAST_DICT(closure)->keys->begin(), CAST_DICT(closure)->keys->end(), name) != CAST_DICT(closure)->keys->end()){
                        if (CAST_DICT(closure)->val->at(name)->type->size==0){
                            ((object_var*)CAST_DICT(closure)->val->at(name))->gc_ref--;
                        }
                        if (object_find_bool_dict_keys(CAST_FUNC(frame.callable)->closure_annotations, name)){
                            if (CAST_DICT(CAST_FUNC(frame.callable)->closure_annotations)->val->at(name)->type->size==0){
                                ((object_var*)CAST_DICT(CAST_FUNC(frame.callable)->closure_annotations)->val->at(name))->gc_ref--;
                            }
                            dict_set(CAST_FUNC(frame.callable)->closure_annotations, name, NULL);
                        }
                        dict_set(closure, name, NULL);

                        DISPATCH();
                    }
                }
            }

            vm_add_err(&NameError, vm, "Nonlocal %s referenced before assignment", object_crepr(name).c_str());
            goto exc;
        }
        
        BITWISE_NOT:{
            struct object* right=pop_dataframe(vm->objstack);
            
            object* ret=object_not(right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid bitwise operand ~: '%s'.", right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BITWISE_AND:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_and(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for &: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BITWISE_OR:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_or(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for |: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BITWISE_LSHIFT:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_lshift(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for <<: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BITWISE_RSHIFT:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_rshift(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for >>: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BINOP_IAND:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_and(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for &: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=ret;
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
            DISPATCH();
        }

        BINOP_IOR:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_or(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for |: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=ret;
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
            
            DISPATCH();
        }

        BINOP_ILSH:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_lshift(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for <<: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=ret;
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
            
            DISPATCH();
        }

        BINOP_IRSH:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_rshift(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for >>: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=ret;
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
            
            DISPATCH();
        }
        
        BINOP_NOTIN:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            object* res=object_in_iter(left, right);

            if (res!=NULL){
                FPLDECREF(res);
                add_dataframe(vm, vm->objstack, new_bool_false());
            }
            add_dataframe(vm, vm->objstack, new_bool_true());
            DISPATCH();
        }
        
        BINOP_IN:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            object* res=object_in_iter(left, right);

            if (res!=NULL){
                add_dataframe(vm, vm->objstack, res);
            }
            else{
                add_dataframe(vm, vm->objstack, new_bool_false());
            }
            DISPATCH();
        }

        BINOP_ISNOT:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=left==right ? new_bool_false() : new_bool_true();
            add_dataframe(vm, vm->objstack, ret);
            DISPATCH();
        }
        
        BINOP_FLDIV:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_fldiv(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand types for //: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }

        BINOP_IFLDIV:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_fldiv(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for //: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            
            object* name=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* value=ret;
            
            for (auto k: (*CAST_DICT(callstack_head(vm->callstack).locals)->val)){
                if (istrue(object_cmp(name, k.first, CMP_EQ))){
                    if (CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first)->type->size==0){
                        ((object_var*)CAST_DICT(callstack_head(vm->callstack).locals)->val->at(k.first))->gc_ref--;
                    }
                    break;
                }
            }

            if (value->type->size==0){
                ((object_var*)value)->gc_ref++;
            }
            

            dict_set(callstack_head(vm->callstack).locals, name, value);
            DISPATCH();
        }

        TERNARY_TEST:{
            object* expr1=pop_dataframe(vm->objstack);
            object* expr2=pop_dataframe(vm->objstack);
            object* left=pop_dataframe(vm->objstack);
            if (istrue(object_bool(left))){
                add_dataframe(vm, vm->objstack, expr2);
            }
            else{
                add_dataframe(vm, vm->objstack, expr1);
            }
            DISPATCH();
        }

        ANNOTATE_GLOBAL:{
            object* tp=pop_dataframe(vm->objstack);
            annotate_global(tp, list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg));
            DISPATCH();
        }

        ANNOTATE_NAME:{
            object* tp=pop_dataframe(vm->objstack);
            annotate_var(tp, list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg));
            DISPATCH();
        }

        ANNOTATE_NONLOCAL:{
            object* tp=pop_dataframe(vm->objstack);
            annotate_nonlocal(tp, list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg));
            DISPATCH();
        } 

        STORE_ATTR_ANNOTATE: {
            object* tp=pop_dataframe(vm->objstack); //Type to annotate

            object* obj=pop_dataframe(vm->objstack);
            object* val=peek_dataframe(vm->objstack); //For multiple assignment
            object* attr=list_index_int(CAST_CODE(callstack_head(vm->callstack).code)->co_names, arg);
            object* ret=object_setattr(obj, attr, val);
            
            object* annon=object_getattr(obj, str_new_fromstr("__annotations__"));
            if (annon==NULL){
                FPLDECREF(vm->exception);
                vm->exception=NULL;
                annon=new_dict();
                object_setattr(obj, str_new_fromstr("__annotations__"), annon);
            }
            dict_set(annon, attr, tp);
            
            if (ret==NULL){
                goto exc;
            }
            if (ret==TERM_PROGRAM|| ret==CALL_ERR){
                GIL.unlock();
                return TERM_PROGRAM;
            }
            DISPATCH();
        }

        YIELD_VALUE: {
            object* o = pop_dataframe(vm->objstack);
            GIL.unlock();
            return o;
        }

        MAKE_GENERATOR:{
            int flags=CAST_INT(pop_dataframe(vm->objstack))->val->to_int(); //<- Flags
            object* stargs=NULL;
            object* stkwargs=NULL;
            
            if (flags==FUNC_STARARGS){
                stargs=pop_dataframe(vm->objstack);
            }
            else if (flags==FUNC_STARKWARGS){
                stkwargs=pop_dataframe(vm->objstack); 
            }
            else if (flags==FUNC_STAR){
                stkwargs=pop_dataframe(vm->objstack); 
                stargs=pop_dataframe(vm->objstack); 
            }
            object* code=pop_dataframe(vm->objstack); //<- Code
            object* annotations=pop_dataframe(vm->objstack); //<- Annotations
            object* args=pop_dataframe(vm->objstack); //<- Args
            object* kwargs=pop_dataframe(vm->objstack); //<- Kwargs
            object* name=pop_dataframe(vm->objstack); //<- Name
            
            object* func=func_new_code(code, args, kwargs, arg, name, NULL, FUNCTION_NORMAL, flags, stargs, stkwargs, annotations, true, NULL, vm->globals, vm->global_annotations);
            add_dataframe(vm, vm->objstack, func);
            DISPATCH();
        }

        MAKE_CLOSURE_GENERATOR:{
            int flags=CAST_INT(pop_dataframe(vm->objstack))->val->to_int(); //<- Flags
            object* stargs=NULL;
            object* stkwargs=NULL;
            if (flags==FUNC_STARARGS){
                stargs=pop_dataframe(vm->objstack); 
            }
            else if (flags==FUNC_STARKWARGS){
                stkwargs=pop_dataframe(vm->objstack); 
            }
            else if (flags==FUNC_STAR){
                stkwargs=pop_dataframe(vm->objstack); 
                stargs=pop_dataframe(vm->objstack); 
            }
            object* code=pop_dataframe(vm->objstack); //<- Code
            object* annotations=pop_dataframe(vm->objstack); //<- Annotations
            object* args=pop_dataframe(vm->objstack); //<- Args
            object* kwargs=pop_dataframe(vm->objstack); //<- Kwargs
            object* name=pop_dataframe(vm->objstack); //<- Name
            
            object* func=func_new_code(code, args, kwargs, arg, name, FPLINCREF(callstack_head(vm->callstack).locals), FUNCTION_NORMAL, flags, stargs, stkwargs, annotations, true, FPLINCREF(callstack_head(vm->callstack).annotations), vm->globals, vm->global_annotations);
            add_dataframe(vm, vm->objstack, func);
            DISPATCH();
        }

        BUILD_SET: {
            object* set=new_set();
            for (int i=0; i<arg; i++){
                set_append(set, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, set);
            DISPATCH();
        }

        ENTER_WHILE: {
            add_blockframe(ip, vm, vm->blockstack, arg, WHILE_BLOCK);
            DISPATCH();
        }

        EXIT_WHILE: {
            pop_blockframe(vm->blockstack);
            DISPATCH();
        }

        ENTER_WITH: {
            add_blockframe(ip, vm, vm->blockstack, 0, WITH_BLOCK);
            blockstack_head(vm->blockstack).obj=peek_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, object_enter_with(pop_dataframe(vm->objstack)));
            DISPATCH();
        }

        EXIT_WITH: {
            pop_blockframe(vm->blockstack);
            add_dataframe(vm, vm->objstack, object_exit_with(pop_dataframe(vm->objstack)));
            DISPATCH();
        }

        SEQ_APPEND: {
            object* o=vm->objstack->data[vm->objstack->size-1-arg];
            o->type->slot_mappings->slot_append(o, pop_dataframe(vm->objstack));
            DISPATCH();
        }

        DICT_SET: {
            object* v=pop_dataframe(vm->objstack);
            dict_set(vm->objstack->data[vm->objstack->size-1-arg], pop_dataframe(vm->objstack), v);
            DISPATCH();
        }
        
        BITWISE_XOR:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_xor(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for ^: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                goto exc;
            }
            DISPATCH();
        }
    }

    exc:
    if (vm->exception!=NULL){
        (*ip)-=2;
        struct blockframe* frame=in_blockstack(vm->blockstack, TRY_BLOCK);
        if (frame!=NULL && (frame->arg==3 || frame->arg%2==0)){
            if (vm->callstack->size-frame->callstack_size!=0){
                //Free GIL
                GIL.unlock();
                //
                return NULL;
            }
            add_dataframe(vm, vm->objstack, vm->exception);
            frame->obj=FPLINCREF(vm->exception);
            if (vm->exception!=NULL){
                FPLDECREF(vm->exception);
                vm->exception=NULL;
            }
            
            frame->start_ip=(*ip);
            (*ip)=frame->arg+4; //skip jump
            frame->arg=1;
            DISPATCH();
        }
        else if (frame!=NULL && frame->obj!=NULL && frame->arg!=3){
            uint32_t ip_=(*ip);
            (*ip)=frame->start_ip;
            print_traceback();
            (*ip)=ip_;
            
            cout<<frame->obj->type->name->c_str();
            if (CAST_EXCEPTION(frame->obj)->err!=NULL){
                cout<<": "<<object_cstr(CAST_EXCEPTION(frame->obj)->err);
            }
            cout<<endl;
            if ((void*)frame->obj==(void*)vm->exception){ //Reraised
                //Free GIL
                GIL.unlock();
                //
                return NULL;
            }
            cout<<endl<<"While handling the above exception, another exception was raised."<<endl<<endl;
        }
        print_traceback();
        
        cout<<vm->exception->type->name->c_str();
        if (CAST_EXCEPTION(vm->exception)->err!=NULL){
            cout<<": "<<object_cstr(CAST_EXCEPTION(vm->exception)->err);
        }
        cout<<endl;
        /*
        if (vm->callstack->size>1){
            return CALL_ERR;
        */

        if (vm->exception!=NULL){
            FPLDECREF(vm->exception);
        }
        vm->exception=NULL;
        
        //Free GIL
        GIL.unlock();
        //
        return NULL;
    }

    //Free GIL
    GIL.unlock();
    //

    return new_none();
}
