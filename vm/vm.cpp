struct datastack* new_datastack(){
    struct datastack* data=(struct datastack*)fpl_malloc(sizeof(struct datastack));
    data->head=NULL;
    data->size=0;
    return data;
}

struct callstack* new_callstack(){
    struct callstack* call=(struct callstack*)fpl_malloc(sizeof(struct callstack));
    call->head=NULL;
    call->size=0;
    return call;
}

struct blockstack* new_blockstack(){
    struct blockstack* block=(struct blockstack*)fpl_malloc(sizeof(struct blockstack));
    block->head=NULL;
    block->size=0;
    return block;
}

void add_blockframe(uint32_t* ip, struct vm* vm, struct blockstack* stack, uint32_t arg, enum blocktype tp){
    struct blockframe* frame=(struct blockframe*)fpl_malloc(sizeof(struct blockframe));
    frame->next=stack->head;
    frame->type=tp;
    frame->arg=arg;
    frame->obj=NULL;
    frame->callstack_size=vm->callstack->size;
    frame->start_ip=(*ip);

    stack->size++;
    stack->head=frame;
}

inline void add_dataframe(struct vm* vm, struct datastack* stack, struct object* obj){
    struct dataframe* frame=(struct dataframe*)fpl_malloc(sizeof(struct dataframe));
    frame->next=stack->head;
    frame->obj=obj;

    stack->size++;
    stack->head=frame;
}

inline struct object* pop_dataframe(struct datastack* stack){
    struct dataframe* frame=stack->head;

    stack->head=frame->next;
    stack->size--;
    
    object* o=frame->obj;
    fpl_free(frame);
    return o;
}

void pop_blockframe(struct blockstack* stack){
    struct blockframe* frame=stack->head;

    stack->head=frame->next;
    stack->size--;

    fpl_free(frame);
}

struct blockframe* in_blockstack(struct blockstack* stack, enum blocktype type){
    struct blockframe* frame=stack->head;

    while (frame){
        if (frame->type==type){
            return frame;
        }
        frame=frame->next;
    }
    return NULL;
}

struct object* peek_dataframe(struct datastack* stack){    
    return stack->head->obj;
}

inline void add_callframe(struct callstack* stack, object* line, string* name, object* code, object* callable, uint32_t* ip){
    struct callframe* frame=(struct callframe*)fpl_malloc(sizeof(struct callframe));
    frame->name=name;
    frame->next=stack->head;
    frame->line=line;
    frame->code=code;
    frame->locals=NULL;
    frame->filedata=vm->filedata;
    frame->callable=callable;
    frame->annotations=new_dict();
    frame->ip=ip;

    stack->size++;
    stack->head=frame;
    
    if (stack->size-2==MAX_RECURSION){
        vm_add_err(&RecursionError, vm, "Maximum stack depth exceeded.");
    }
}

inline void pop_callframe(struct callstack* stack){
    struct callframe* frame=stack->head;

    stack->head=frame->next;
    stack->size--;

    FPLDECREF(frame->annotations);
    
    fpl_free(frame);
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
    vm->objstack=new_datastack();
    vm->callstack=new_callstack();
    vm->blockstack=new_blockstack();
    
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
    vm->callstack->head->locals=FPLINCREF(vm->globals);
    vm->global_annotations=vm->callstack->head->annotations;
    
    return vm;
}

void vm_del(struct vm* vm){
    struct callframe* i=vm->callstack->head;
    while (i){
        struct callframe* i_=i->next;;
        FPLDECREF(i->locals);
        fpl_free(i);
        i=i_;
    }
    struct dataframe* j=vm->objstack->head;
    while (j){
        struct dataframe* j_=j->next;;
        fpl_free(j);
        j=j_;
    }

    FPLDECREF(vm->globals);
    delete vm->filedata;
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
    }
}

void vm_add_var_locals(struct vm* vm, object* name, object* value){
    for (auto k: (*CAST_DICT(vm->callstack->head->locals)->val)){
        if (istrue(object_cmp(name, k.first, CMP_EQ))){
            if (CAST_DICT(vm->callstack->head->locals)->val->at(k.first)->type->size==0){
                ((object_var*)CAST_DICT(vm->callstack->head->locals)->val->at(k.first))->gc_ref--;
            }
            break;
        }
    }

    if (value->type->size==0){
        ((object_var*)value)->gc_ref++;
    }
    

    dict_set(vm->callstack->head->locals, name, value); //If globals is same obj as locals then this will still update both
}

struct object* vm_get_var_locals(struct vm* vm, object* name){
    if (vm->callstack->head->callable!=NULL && object_istype(vm->callstack->head->callable->type, &FuncType)\
    && CAST_FUNC(vm->callstack->head->callable)->closure!=NULL){
        object* closure=CAST_FUNC(vm->callstack->head->callable)->closure;
        //Check if name in closure
        if (find(CAST_DICT(closure)->keys->begin(), CAST_DICT(closure)->keys->end(), name) != CAST_DICT(closure)->keys->end()){
            return CAST_DICT(closure)->val->at(name);
        }
    }
    
    struct callframe* frame=vm->callstack->head;
    while(frame){
        for (auto k: (*CAST_DICT(frame->locals)->val)){
            if (istrue(object_cmp(name, k.first, CMP_EQ))){
                return  CAST_DICT(frame->locals)->val->at(k.first);
            }
        }

        frame=frame->next;
    }
    
    for (size_t i=0; i<nbuiltins; i++){
        if (object_istype(builtins[i]->type, &BuiltinType)){
            if (istrue(object_cmp(CAST_BUILTIN(builtins[i])->name, name, CMP_EQ))){
                return builtins[i];
            }
        }
        if (object_istype(builtins[i]->type, &TypeType)){
            if (CAST_TYPE(builtins[i])->name->compare((*CAST_STRING(name)->val))==0){
                return builtins[i];
            } 
        }
    }

    vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
    return NULL;
}

void vm_add_var_globals(struct vm* vm, object* name, object* value){
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
    dict_set(vm->globals, name, value); //If globals is same obj as locals then this will still update both
}

object* vm_get_var_nonlocal(struct vm* vm, object* name){
    int i=0;
    struct callframe* frame=vm->callstack->head->next;
    while (frame){
        if (frame->next==NULL){
            break;
        }
        if (i==0){
            if (find(CAST_DICT(frame->locals)->keys->begin(), CAST_DICT(frame->locals)->keys->end(), name) != CAST_DICT(frame->locals)->keys->end()){
                return CAST_DICT(frame->locals)->val->at(name);
            }
        }
        i+=1;
        if (frame->callable!=NULL && object_istype(frame->callable->type, &FuncType)\
        && CAST_FUNC(frame->callable)->closure!=NULL){
            object* closure=CAST_FUNC(frame->callable)->closure;
            //Check if name in closure
            if (find(CAST_DICT(closure)->keys->begin(), CAST_DICT(closure)->keys->end(), name) != CAST_DICT(closure)->keys->end()){
                return CAST_DICT(closure)->val->at(name);
            }
        }
        frame=frame->next;
    }

    vm_add_err(&NameError, vm, "Nonlocal %s referenced before assignment", object_crepr(name).c_str());
    return NULL;
}

void vm_add_var_nonlocal(struct vm* vm, object* name, object* val){
    int i=0;
    struct callframe* frame=vm->callstack->head->next;
    while (frame){
        if (frame->next==NULL){
            break;
        }
        if (i==0){
            if (find(CAST_DICT(frame->locals)->keys->begin(), CAST_DICT(frame->locals)->keys->end(), name) != CAST_DICT(frame->locals)->keys->end()){
                if (CAST_DICT(frame->locals)->val->at(name)->type->size==0){
                    ((object_var*)CAST_DICT(frame->locals)->val->at(name))->gc_ref--;
                }
                
                if (val->type->size==0){
                    ((object_var*)val)->gc_ref++;
                }
                dict_set(frame->locals, name, val);

                return;
            }
        }
        i+=1;
        if (frame->callable!=NULL && object_istype(frame->callable->type, &FuncType)\
        && CAST_FUNC(frame->callable)->closure!=NULL){
            object* closure=CAST_FUNC(frame->callable)->closure;
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

                return;
            }
        }
        frame=frame->next;
    }

    vm_add_err(&NameError, vm, "Nonlocal %s referenced before assignment", object_crepr(name).c_str());
}

void vm_del_var_nonlocal(struct vm* vm, object* name){
    int i=0;
    struct callframe* frame=vm->callstack->head;
    while (frame){
        if (frame->next==NULL){
            break;
        }
        if (i==0){
            if (find(CAST_DICT(frame->locals)->keys->begin(), CAST_DICT(frame->locals)->keys->end(), name) != CAST_DICT(frame->locals)->keys->end()){
                if (CAST_DICT(frame->locals)->val->at(name)->type->size==0){
                    ((object_var*)CAST_DICT(frame->locals)->val->at(name))->gc_ref--;
                }
                if (object_find_bool_dict_keys(frame->annotations, name)){
                    if (CAST_DICT(frame->annotations)->val->at(name)->type->size==0){
                        ((object_var*)CAST_DICT(frame->annotations)->val->at(name))->gc_ref--;
                    }
                    dict_set(frame->annotations, name, NULL);
                }
                dict_set(frame->locals, name, NULL);
                return;
            }
        }
        i+=1;
        if (frame->callable!=NULL && object_istype(frame->callable->type, &FuncType)\
        && CAST_FUNC(frame->callable)->closure!=NULL){
            object* closure=CAST_FUNC(frame->callable)->closure;
            //Check if name in closure
            if (find(CAST_DICT(closure)->keys->begin(), CAST_DICT(closure)->keys->end(), name) != CAST_DICT(closure)->keys->end()){
                if (CAST_DICT(closure)->val->at(name)->type->size==0){
                    ((object_var*)CAST_DICT(closure)->val->at(name))->gc_ref--;
                }
                if (object_find_bool_dict_keys(CAST_FUNC(frame->callable)->closure_annotations, name)){
                    if (CAST_DICT(CAST_FUNC(frame->callable)->closure_annotations)->val->at(name)->type->size==0){
                        ((object_var*)CAST_DICT(CAST_FUNC(frame->callable)->closure_annotations)->val->at(name))->gc_ref--;
                    }
                    dict_set(CAST_FUNC(frame->callable)->closure_annotations, name, NULL);
                }
                dict_set(closure, name, NULL);

                return;
            }
        }
        frame=frame->next;
    }

    vm_add_err(&NameError, vm, "Nonlocal %s referenced before assignment", object_crepr(name).c_str());
}

struct object* vm_get_var_globals(struct vm* vm, object* name){
    for (auto k: (*CAST_DICT(vm->globals)->val)){
        if (istrue(object_cmp(name, k.first, CMP_EQ))){
            return  CAST_DICT(vm->globals)->val->at(k.first);
        }
    }
    for (size_t i=0; i<nbuiltins; i++){
        if (object_istype(builtins[i]->type, &BuiltinType)){
            if (istrue(object_cmp(CAST_BUILTIN(builtins[i])->name, name, CMP_EQ))){
                return builtins[i];
            }
        }
        if (object_istype(builtins[i]->type, &TypeType)){
            if (CAST_TYPE(builtins[i])->name->compare((*CAST_STRING(name)->val))==0){
                return builtins[i];
            }
        }
    }
    vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
    return NULL;
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
    struct callframe* callframe=vm->callstack->head;
    while (callframe){    
        if (callframe->name==NULL){
            callframe=callframe->next;
        }

        int line_=calculate_line_fromip(*callframe->ip, callframe);

        cout<<"In file '"+program+"', line "+to_string(line_+1)+", in "+(*callframe->name)<<endl;
        
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
            if (entered && ((*CAST_CODE(callframe->code)->filedata)[idx]=='\n' || (*CAST_CODE(callframe->code)->filedata)[idx]=='\0')){
                endidx=idx;
                break;
            }
            else if ((*CAST_CODE(callframe->code)->filedata)[idx]=='\n'){
                line++;
            }
            idx++;
        }

        string snippet="";
        for (int i=startidx; i<endidx; i++){
            snippet+=(*CAST_CODE(callframe->code)->filedata)[i];
        }

        cout<<"    "<<remove_spaces(snippet)<<endl;
        
        callframe=callframe->next;
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
    ::vm->callstack->head->locals=FPLINCREF(::vm->globals);
    dict_set(::vm->globals, str_new_fromstr("__annotations__"), ::vm->callstack->head->annotations);
    dict_set(::vm->globals, str_new_fromstr("__name__"), name);
    ::vm->global_annotations=::vm->callstack->head->annotations;

    object* ret=run_vm(code, &::vm->ip);
    object* dict=::vm->callstack->head->locals;

    object* o=module_new_fromdict(dict, name);
    
    vm_del(::vm);
    ::vm=vm_;

    return o;
}

void vm_del_var_locals(struct vm* vm, object* name){
    for (auto k: (*CAST_DICT(vm->callstack->head->locals)->val)){
        if (istrue(object_cmp(name, k.first, CMP_EQ))){
            if (CAST_DICT(vm->callstack->head->locals)->val->at(name)->type->size==0){
                ((object_var*)CAST_DICT(vm->callstack->head->locals)->val->at(k.first))->gc_ref--;
            }

            if (object_find_bool_dict_keys(vm->callstack->head->annotations, name)){
                if (CAST_DICT(vm->callstack->head->annotations)->val->at(name)->type->size==0){
                    ((object_var*)CAST_DICT(vm->callstack->head->annotations)->val->at(name))->gc_ref--;
                }
                dict_set(vm->callstack->head->annotations, name, NULL);
            }
            dict_set(vm->callstack->head->locals, name, NULL);
            return;
        }
    }
    vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
    return;
}

void vm_del_var_globals(struct vm* vm, object* name){
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
            return;
        }
    }
    vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
    return;
}

dataframe* reverse(dataframe* head, int k){
    // base case
    if (!head)
        return NULL;
    dataframe* current = head;
    dataframe* next = NULL;
    dataframe* prev = NULL;
    int count = 0;
  
    /*reverse first k nodes of the linked list */
    while (current != NULL && count < k) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
        count++;
    }
  
    /* next is now a pointer to (k+1)th node
    Recursively call for the list starting from current.
    And make rest of the list as next of first node */
    if (next != NULL)
        head->next = reverse(next, k);
  
    /* prev is new head of the input list */
    return prev;
}

void annotate_var(object* tp, object* name){
    dict_set(vm->callstack->head->annotations, name, tp);
}

void annotate_global(object* tp, object* name){
    dict_set(vm->global_annotations, name, tp);
}

void annotate_nonlocal(object* tp, object* name){
    int i=0;
    struct callframe* frame=vm->callstack->head;
    while (frame){
        if (frame->next==NULL){
            break;
        }
        if (i==0){
            if (find(CAST_DICT(frame->locals)->keys->begin(), CAST_DICT(frame->locals)->keys->end(), name) != CAST_DICT(frame->locals)->keys->end()){
                if (CAST_DICT(frame->locals)->val->at(name)->type->size==0){
                    ((object_var*)CAST_DICT(frame->locals)->val->at(name))->gc_ref--;
                }
                FPLDECREF(CAST_DICT(frame->locals)->val->at(name));
                
                if (tp->type->size==0){
                    ((object_var*)tp)->gc_ref++;
                }
                dict_set(frame->locals, name, tp);

                return;
            }
        }
        i+=1;
        if (frame->callable!=NULL && object_istype(frame->callable->type, &FuncType)\
        && CAST_FUNC(frame->callable)->closure!=NULL){
            object* closure=CAST_FUNC(frame->callable)->closure;
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
        frame=frame->next;
    }
}

object* run_vm(object* codeobj, uint32_t* ip){
    object* code=CAST_CODE(codeobj)->co_code;
    object* lines=CAST_CODE(codeobj)->co_lines;

    object* arg;
    
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
        &&LIST_APPEND,
    };
    
    object** code_array=CAST_LIST(code)->array;
    
    DISPATCH();
    while(1){
        LOAD_CONST:{
            add_dataframe(vm, vm->objstack, list_get(CAST_CODE(vm->callstack->head->code)->co_consts, arg));
            DISPATCH();
        }

        STORE_GLOBAL:{
            vm_add_var_globals(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), peek_dataframe(vm->objstack));
            DISPATCH();
        }

        LOAD_GLOBAL:{
            add_dataframe(vm, vm->objstack, vm_get_var_globals(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg) ));
            DISPATCH();
        }

        STORE_NAME:{
            vm_add_var_locals(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), peek_dataframe(vm->objstack));
            DISPATCH();
        }

        LOAD_NAME:{
            add_dataframe(vm, vm->objstack, vm_get_var_locals(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg) ));
            DISPATCH();
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
            
            object* func=func_new_code(code, args, kwargs, CAST_INT(arg)->val->to_int(), name, NULL, FUNCTION_NORMAL, flags, stargs, stkwargs, annotations, false, NULL);

            add_dataframe(vm, vm->objstack, func);
            DISPATCH();
        }

        RETURN_VAL: {
            GIL.unlock();
            return FPLINCREF(pop_dataframe(vm->objstack));
        }

        CALL_METHOD: {
            uint32_t argc=CAST_INT(arg)->val->to_int();
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
                    
                    if (ob->type->slot_iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", ob->type->name->c_str());
                        DISPATCH();
                    }
                    
                    object* iter=ob->type->slot_iter(ob);
                    
                    if (iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
                        DISPATCH();
                    }

                    object* one=new_int_fromint(0);
                    object* two=new_int_fromint(1);
                    object* res=NULL;
                    
                    object* o=iter->type->slot_next(iter);
                    object* v;
                    while (vm->exception==NULL){
                        if (o->type->slot_mappings->slot_len==NULL){
                            vm_add_err(&TypeError, vm, "'%s' object has no __len__", o->type->name->c_str());
                            FPLDECREF(iter);
                            FPLDECREF(one);
                            DISPATCH();
                        }
                        int len=CAST_INT(o->type->slot_mappings->slot_len(o))->val->to_int();
                        if (len!=2){
                            vm_add_err(&TypeError, vm, "Expected 2 elements, got %d", len);
                            FPLDECREF(iter);
                            FPLDECREF(one);
                            DISPATCH();
                        }
                        if (o->type->slot_mappings==NULL || o->type->slot_mappings->slot_get==NULL){
                            FPLDECREF(iter);
                            FPLDECREF(one);
                            vm_add_err(&TypeError, vm, "'%s' object is not subscriptable", o->type->name->c_str());
                            DISPATCH();
                        }
                        object* a=o->type->slot_mappings->slot_get(o, one);
                        ERROR_RET(a);
                        object* b=o->type->slot_mappings->slot_get(o, two);
                        ERROR_RET(b);

                        dict_set(kwargs, a, b);
                        
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
                    
                    if (ob->type->slot_iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", ob->type->name->c_str());
                        DISPATCH();
                    }
                    
                    object* iter=ob->type->slot_iter(ob);
                    
                    if (iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
                        DISPATCH();
                    }

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
                DISPATCH();
            }

            //Call
            object* ret=function->type->slot_call(function, args, kwargs);
            if (ret==NULL || ret==CALL_ERR){
                DISPATCH();
            }
            if (ret==TERM_PROGRAM){
                GIL.unlock();
                return TERM_PROGRAM;
            }
            add_dataframe(vm, vm->objstack, ret);

            DISPATCH();
        }

        CALL_FUNCTION: {
            object* function=pop_dataframe(vm->objstack);
            
            uint32_t argc=CAST_INT(arg)->val->to_int();
            uint32_t posargc=CAST_INT(pop_dataframe(vm->objstack))->val->to_int();
            uint32_t kwargc=argc-posargc;

            object* stkwargs=pop_dataframe(vm->objstack);
            object* stargs=pop_dataframe(vm->objstack);

            if (function->type->slot_call==NULL){
                vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
                DISPATCH();
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
                    
                    if (ob->type->slot_iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", ob->type->name->c_str());
                        DISPATCH();
                    }
                    
                    object* iter=ob->type->slot_iter(ob);
                    
                    if (iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
                        DISPATCH();
                    }

                    object* one=new_int_fromint(0);
                    object* two=new_int_fromint(1);
                    object* res=NULL;
                    
                    object* o=iter->type->slot_next(iter);
                    object* v;
                    while (vm->exception==NULL){
                        if (o->type->slot_mappings->slot_len==NULL){
                            vm_add_err(&TypeError, vm, "'%s' object has no __len__", o->type->name->c_str());
                            FPLDECREF(iter);
                            FPLDECREF(one);
                            DISPATCH();
                        }
                        int len=CAST_INT(o->type->slot_mappings->slot_len(o))->val->to_int();
                        if (len!=2){
                            vm_add_err(&TypeError, vm, "Expected 2 elements, got %d", len);
                            FPLDECREF(iter);
                            FPLDECREF(one);
                            DISPATCH();
                        }
                        if (o->type->slot_mappings==NULL || o->type->slot_mappings->slot_get==NULL){
                            FPLDECREF(iter);
                            FPLDECREF(one);
                            vm_add_err(&TypeError, vm, "'%s' object is not subscriptable", o->type->name->c_str());
                            DISPATCH();
                        }
                        object* a=o->type->slot_mappings->slot_get(o, one);
                        ERROR_RET(a);
                        object* b=o->type->slot_mappings->slot_get(o, two);
                        ERROR_RET(b);

                        dict_set(kwargs, a, b);
                        
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
                    
                    if (ob->type->slot_iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", ob->type->name->c_str());
                        DISPATCH();
                    }
                    
                    object* iter=ob->type->slot_iter(ob);
                    
                    if (iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
                        DISPATCH();
                    }

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
            if (ret==NULL || ret==CALL_ERR){
                DISPATCH();
            }
            if (ret==TERM_PROGRAM){
                GIL.unlock();
                return TERM_PROGRAM;
            }
            
            add_dataframe(vm, vm->objstack, ret);
            
            DISPATCH();            
        }

        CALL_FUNCTION_BOTTOM: {
            uint32_t argc=CAST_INT(arg)->val->to_int();
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
                    
                    if (ob->type->slot_iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", ob->type->name->c_str());
                        DISPATCH();
                    }
                    
                    object* iter=ob->type->slot_iter(ob);
                    
                    if (iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
                        DISPATCH();
                    }

                    object* one=new_int_fromint(0);
                    object* two=new_int_fromint(1);
                    object* res=NULL;
                    
                    object* o=iter->type->slot_next(iter);
                    object* v;
                    while (vm->exception==NULL){
                        if (o->type->slot_mappings->slot_len==NULL){
                            vm_add_err(&TypeError, vm, "'%s' object has no __len__", o->type->name->c_str());
                            FPLDECREF(iter);
                            FPLDECREF(one);
                            DISPATCH();
                        }
                        int len=CAST_INT(o->type->slot_mappings->slot_len(o))->val->to_int();
                        if (len!=2){
                            vm_add_err(&TypeError, vm, "Expected 2 elements, got %d", len);
                            FPLDECREF(iter);
                            FPLDECREF(one);
                            DISPATCH();
                        }
                        if (o->type->slot_mappings==NULL || o->type->slot_mappings->slot_get==NULL){
                            FPLDECREF(iter);
                            FPLDECREF(one);
                            vm_add_err(&TypeError, vm, "'%s' object is not subscriptable", o->type->name->c_str());
                            DISPATCH();
                        }
                        object* a=o->type->slot_mappings->slot_get(o, one);
                        ERROR_RET(a);
                        object* b=o->type->slot_mappings->slot_get(o, two);
                        ERROR_RET(b);

                        dict_set(kwargs, a, b);
                        
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
                    
                    if (ob->type->slot_iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", ob->type->name->c_str());
                        DISPATCH();
                    }
                    
                    object* iter=ob->type->slot_iter(ob);
                    
                    if (iter==NULL){
                        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
                        DISPATCH();
                    }

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
                DISPATCH();
            }
            
            //Call
            object* ret=function->type->slot_call(function, args, kwargs);
            if (ret==CALL_ERR){ 
                DISPATCH();
            }
            if (ret==NULL || ret==TERM_PROGRAM){
                GIL.unlock();
                return TERM_PROGRAM;
            }
            
            add_dataframe(vm, vm->objstack, ret);
            
            DISPATCH();            
        }

        BUILD_LIST: {
            object* list=new_list();
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                list_append(list, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, list);
            DISPATCH();
        }

        BUILD_TUPLE: {
            object* tuple=new_tuple();
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                tuple_append(tuple, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, tuple);
            DISPATCH();
        }

        BUILD_DICT: {
            object* dict=new_dict();
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
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
            object* attr=list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg);
            object* ret=object_getattr(obj, attr);            
            
            if (ret==NULL || ret==CALL_ERR){ 
                DISPATCH();
            }
            if (ret==TERM_PROGRAM){
                return TERM_PROGRAM;
            }
            
            add_dataframe(vm, vm->objstack, ret);
            DISPATCH();
        }    

        LOAD_METHOD: {
            object* obj=peek_dataframe(vm->objstack);
            object* attr=list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg);
            object* ret=object_getattr(obj, attr);            
            
            if (ret==NULL || ret==CALL_ERR){ 
                DISPATCH();
            }
            if (ret==TERM_PROGRAM){
                return TERM_PROGRAM;
            }
            add_dataframe(vm, vm->objstack, ret);
            DISPATCH();
        }  

        STORE_ATTR: {
            object* obj=pop_dataframe(vm->objstack);
            object* val=peek_dataframe(vm->objstack); //For multiple assignment
            object* attr=list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg);
            object* ret=object_setattr(obj, attr, val);
            
            if (ret==NULL || ret==CALL_ERR){ 
                DISPATCH();
            }
            if (ret==TERM_PROGRAM){
                return TERM_PROGRAM;
            }
            DISPATCH();
        }

        POP_JMP_TOS_FALSE: {
            object* o=pop_dataframe(vm->objstack);
            object* val=object_istruthy(o);
            if (!istrue(val)){
                (*ip)=(*ip)+CAST_INT(arg)->val->to_long();
                DISPATCH();
            }
            DISPATCH();
        }

        JUMP_DELTA: {
            (*ip)=(*ip)+CAST_INT(arg)->val->to_long();
            DISPATCH();
        }

        BINOP_SUBSCR: {
            object* idx=pop_dataframe(vm->objstack);
            object* base=pop_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, object_get(base, idx));
            DISPATCH();
        }

        RAISE_EXC: {
            if (!object_issubclass(peek_dataframe(vm->objstack), &ExceptionType)){
                vm_add_err(&TypeError, vm, "Exceptions must be subclass of Exception");
                DISPATCH();
            }
            if (vm->exception!=NULL){
                FPLDECREF(vm->exception);
            }
            vm->exception=pop_dataframe(vm->objstack);
            DISPATCH();
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
            add_blockframe(ip, vm, vm->blockstack, CAST_INT(arg)->val->to_int(), TRY_BLOCK);
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
            }
            DISPATCH();
        }

        EXTRACT_ITER: {
            object* it=pop_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, it->type->slot_iter(it));
            DISPATCH();
        }

        FOR_TOS_ITER: {
            if (vm->blockstack->size==0 || vm->blockstack->head->type!=FOR_BLOCK || (vm->blockstack->head->type==FOR_BLOCK && vm->blockstack->head->arg!=CAST_INT(arg)->val->to_int()) ){
                add_blockframe(ip, vm, vm->blockstack, CAST_INT(arg)->val->to_int(), FOR_BLOCK);
            }
            
            object* it=peek_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, it->type->slot_next(it));
            if (vm->exception!=NULL && object_istype(vm->exception->type, &StopIteration)){
                FPLDECREF(vm->exception);
                vm->exception=NULL;
                (*ip)=vm->blockstack->head->arg;
                pop_blockframe(vm->blockstack);
                pop_dataframe(vm->objstack);
                pop_dataframe(vm->objstack);
            }
            DISPATCH();
        }

        JUMP_TO: {
            (*ip)=CAST_INT(arg)->val->to_long();
            DISPATCH();
        }

        BREAK_LOOP: {
            if (vm->blockstack->head==NULL){    
                DISPATCH();
            }
            if (vm->blockstack->head!=NULL && vm->blockstack->head->type!=FOR_BLOCK && vm->blockstack->head->type!=WHILE_BLOCK){
                DISPATCH();
            }
            
            (*ip)=vm->blockstack->head->arg;
            pop_blockframe(vm->blockstack);
            DISPATCH();
        }

        CONTINUE_LOOP: {
            if (vm->blockstack->head==NULL){
                DISPATCH();
            }
            if (vm->blockstack->head!=NULL && vm->blockstack->head->type!=FOR_BLOCK && vm->blockstack->head->type!=WHILE_BLOCK){
                DISPATCH();
            }
            (*ip)=vm->blockstack->head->start_ip;
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
            if (len>CAST_INT(arg)->val->to_int()){
                vm_add_err(&ValueError, vm, "Too many values to unpack, expected %d", CAST_INT(arg)->val->to_int());
                DISPATCH();
            }
            if (len<CAST_INT(arg)->val->to_int()){
                vm_add_err(&ValueError, vm, "Not enough values to unpack, expected %d", CAST_INT(arg)->val->to_int());
                DISPATCH();
            }
            
            DISPATCH();
        }

        BINOP_IADD:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_add(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for +: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                DISPATCH();
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            DISPATCH();
        }

        BINOP_ISUB:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_sub(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for -: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                DISPATCH();
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            DISPATCH();
        }

        BINOP_IMUL:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_mul(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for *: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                DISPATCH();
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            DISPATCH();  
        }

        BINOP_IDIV:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_div(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for /: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                DISPATCH();
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            DISPATCH();
        }

        IMPORT_NAME: {
            object* name=CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg);
            string nm_plain=*CAST_STRING(name)->val;

            string data="";
                    
            string name_plain=nm_plain+".fpl";

            for (size_t i=0; i<CAST_LIST(vm->path)->size; i++){
                string name_=object_cstr(list_index_int(vm->path, i))+name_plain;
                string nm=object_cstr(list_index_int(vm->path, i))+nm_plain;
                struct stat st;
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

                                //try en->d_name
                                //Later try nm as folder
                                
                                FILE* f=fopen((nm+"/"+string(en->d_name)).c_str(), "rb");
                                if (f==NULL){
                                    vm_add_err(&ImportError, vm, "'%s' not found", (nm+"/"+string(en->d_name)).c_str());
                                    DISPATCH();
                                }


                                fseek(f, 0, SEEK_END);
                                long fsize = ftell(f);
                                fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

                                char *s = (char*)fpl_malloc(fsize + 1);
                                int i=fread(s, fsize, 1, f);
                                if (i==0 && fsize>0){
                                    vm_add_err(&InvalidOperationError, vm, "Unable to read from file");
                                    DISPATCH();
                                }
                                s[fsize] = 0;
                                string str(s);
                                
                                object* o=import_name(str, str_new_fromstr(filename));
                                if (o==TERM_PROGRAM){
                                    return TERM_PROGRAM;
                                }
                                dict_set(dict, str_new_fromstr(filename), o);
                            }
                            closedir(dr);
                        }
                        
                        object* o=module_new_fromdict(dict, str_new_fromstr(string(nm_plain)));
                        add_dataframe(vm, vm->objstack, o);

                        DISPATCH();
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
                            DISPATCH();
                        }
                        s[fsize] = 0;
                        string str(s);
                        data=str;
                    }
                }
                else{
                    bool done=false;
                    for (int i=0; i<nmodules; i++){
                        if (istrue(object_cmp(name, CAST_MODULE(modules[i])->name, CMP_EQ))){
                            add_dataframe(vm, vm->objstack, modules[i]);
                            done=true;
                            DISPATCH();
                        }
                    }

                    continue;
                }
                
                object* o=import_name(data, name);
                if (o==TERM_PROGRAM){
                    return TERM_PROGRAM;
                }
                add_dataframe(vm, vm->objstack, o);
                DISPATCH();
            }

            vm_add_err(&ImportError, vm, "'%s' not found", nm_plain.c_str());
            DISPATCH();
        }

        IMPORT_FROM_MOD: {
            object* names=pop_dataframe(vm->objstack);
            object* lib=pop_dataframe(vm->objstack);
            uint32_t len=CAST_INT(names->type->slot_mappings->slot_len(names))->val->to_int();
            if (len==0){
                for (auto k: *CAST_DICT(CAST_MODULE(lib)->dict)->val){
                    vm_add_var_locals(vm, k.first, k.second);
                }
            }
            for (uint32_t i=0; i<len; i++){
                object* o=object_getattr(lib, list_index_int(names, i));
            
                if (o==CALL_ERR){ 
                    DISPATCH();
                }
                if (o==TERM_PROGRAM){
                    return TERM_PROGRAM;
                }
                if (o==NULL){
                    FPLDECREF(vm->exception);
                    vm->exception=NULL;
                    vm_add_err(&ImportError,vm, "Cannot import name '%s' from '%s'",CAST_STRING(list_index_int(names, i))->val->c_str(), CAST_STRING(CAST_MODULE(lib)->name)->val->c_str());
                    DISPATCH();
                }
                vm_add_var_locals(vm, list_index_int(names, i), o);
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
            vm_del_var_locals(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg));
            DISPATCH();
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
            }
            DISPATCH();
        }

        BINOP_IPOW:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_pow(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for **: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                DISPATCH();
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            DISPATCH();
        }

        BINOP_IMOD:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_mod(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for **: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                DISPATCH();
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
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
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                object* flag = pop_dataframe(vm->objstack);
                object* o=pop_dataframe(vm->objstack);
                if (istrue(flag)){
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
                (*ip)=(*ip)+CAST_INT(arg)->val->to_long();
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
            vm_del_var_globals(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg));
            DISPATCH();
        }

        DEL_ATTR: {
            object* obj=pop_dataframe(vm->objstack);
            object* attr=list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg);
            object* ret=object_setattr(obj, attr, NULL);
            
            if (ret==NULL || ret==CALL_ERR){ 
                DISPATCH()
            }
            if (ret==TERM_PROGRAM){
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
            
            object* func=func_new_code(code, args, kwargs, CAST_INT(arg)->val->to_int(), name, FPLINCREF(vm->callstack->head->locals), FUNCTION_NORMAL, flags, stargs, stkwargs, annotations, false, FPLINCREF(vm->callstack->head->annotations));
            add_dataframe(vm, vm->objstack, func);
            DISPATCH();
        }

        LOAD_NONLOCAL:{
            add_dataframe(vm, vm->objstack, vm_get_var_nonlocal(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg) ));
            DISPATCH();
        }

        STORE_NONLOCAL:{
            vm_add_var_nonlocal(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), peek_dataframe(vm->objstack));
            DISPATCH();
        }

        DEL_NONLOCAL:{
            vm_del_var_nonlocal(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg));
            DISPATCH();
        }
        
        BITWISE_NOT:{
            struct object* right=pop_dataframe(vm->objstack);
            
            object* ret=object_not(right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid bitwise operand ~: '%s'.", right->type->name->c_str());
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
            }
            DISPATCH();
        }

        BINOP_IAND:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_and(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for &: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            DISPATCH();
        }

        BINOP_IOR:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_or(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for |: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            
            DISPATCH();
        }

        BINOP_ILSH:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_lshift(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for <<: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            
            DISPATCH();
        }

        BINOP_IRSH:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_rshift(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid bitwise operand types for >>: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            
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
            }
            DISPATCH();
        }

        BINOP_IFLDIV:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_fldiv(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand types for //: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                DISPATCH();
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
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
            annotate_global(tp, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg));
            DISPATCH();
        }

        ANNOTATE_NAME:{
            object* tp=pop_dataframe(vm->objstack);
            annotate_var(tp, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg));
            DISPATCH();
        }

        ANNOTATE_NONLOCAL:{
            object* tp=pop_dataframe(vm->objstack);
            annotate_nonlocal(tp, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg));
            DISPATCH();
        } 

        STORE_ATTR_ANNOTATE: {
            object* tp=pop_dataframe(vm->objstack); //Type to annotate

            object* obj=pop_dataframe(vm->objstack);
            object* val=peek_dataframe(vm->objstack); //For multiple assignment
            object* attr=list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg);
            object* ret=object_setattr(obj, attr, val);
            
            object* annon=object_getattr(obj, str_new_fromstr("__annotations__"));
            if (annon==NULL){
                FPLDECREF(vm->exception);
                vm->exception=NULL;
                annon=new_dict();
                object_setattr(obj, str_new_fromstr("__annotations__"), annon);
            }
            dict_set(annon, attr, tp);
            
            if (ret==NULL || ret==CALL_ERR){ 
                DISPATCH();
            }
            if (ret==TERM_PROGRAM){
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
            
            object* func=func_new_code(code, args, kwargs, CAST_INT(arg)->val->to_int(), name, NULL, FUNCTION_NORMAL, flags, stargs, stkwargs, annotations, true, NULL);
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
            
            object* func=func_new_code(code, args, kwargs, CAST_INT(arg)->val->to_int(), name, FPLINCREF(vm->callstack->head->locals), FUNCTION_NORMAL, flags, stargs, stkwargs, annotations, true, FPLINCREF(vm->callstack->head->annotations));
            add_dataframe(vm, vm->objstack, func);
            DISPATCH();
        }

        BUILD_SET: {
            object* set=new_set();
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                set_append(set, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, set);
            DISPATCH();
        }

        ENTER_WHILE: {
            add_blockframe(ip, vm, vm->blockstack, CAST_INT(arg)->val->to_int(), WHILE_BLOCK);
            DISPATCH();
        }

        EXIT_WHILE: {
            pop_blockframe(vm->blockstack);
            DISPATCH();
        }

        ENTER_WITH: {
            add_blockframe(ip, vm, vm->blockstack, 0, WITH_BLOCK);
            vm->blockstack->head->obj=peek_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, object_enter_with(pop_dataframe(vm->objstack)));
            DISPATCH();
        }

        EXIT_WITH: {
            pop_blockframe(vm->blockstack);
            add_dataframe(vm, vm->objstack, object_exit_with(pop_dataframe(vm->objstack)));
            DISPATCH();
        }

        LIST_APPEND: {
            dataframe* d=vm->objstack->head;
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                d=d->next;
            }
            list_append(d->obj, pop_dataframe(vm->objstack));
            DISPATCH();
        }
    }

    exc:
    if (vm->exception!=NULL){
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

        if (vm->exception!=NULL){
            FPLDECREF(vm->exception);
        }
        vm->exception=NULL;
        
        //Free GIL
        GIL.unlock();
        //

        return TERM_PROGRAM;
        if (vm->callstack->size>1){
            return CALL_ERR;
        }
        return NULL;
    }

    //Free GIL
    GIL.unlock();
    //

    return new_none();
}
