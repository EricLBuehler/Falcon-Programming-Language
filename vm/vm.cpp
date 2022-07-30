struct datastack* new_datastack(){
    struct datastack* data=(struct datastack*)malloc(sizeof(struct datastack));
    data->head=NULL;
    data->size=0;
    return data;
}

struct callstack* new_callstack(){
    struct callstack* call=(struct callstack*)malloc(sizeof(struct callstack));
    call->head=NULL;
    call->size=0;
    return call;
}

void add_dataframe(struct vm* vm, struct datastack* stack, struct object* obj){
    struct dataframe* frame=(struct dataframe*)malloc(sizeof(struct dataframe));
    frame->next=stack->head;
    frame->obj=obj;

    stack->size++;
    stack->head=frame;
}

struct object* pop_dataframe(struct datastack* stack){
    struct dataframe* frame=stack->head;

    stack->head=frame->next;
    stack->size--;
    
    return frame->obj;
}

struct object* peek_dataframe(struct datastack* stack){
    struct dataframe* frame=stack->head;
    
    return frame->obj;
}

void add_callframe(struct callstack* stack, object* line, string* name, object* code){
    struct callframe* frame=(struct callframe*)malloc(sizeof(struct callframe));
    frame->name=name;
    frame->next=stack->head;
    frame->line=INCREF(line);
    frame->code=code;
    if (code!=NULL){
        INCREF(frame->code);
    }

    stack->size++;
    stack->head=frame;
}

struct callframe* pop_callframe(struct callstack* stack){
    struct callframe* frame=stack->head;

    
    DECREF(frame->line);
    if (frame->code!=NULL){
        DECREF(frame->code);
    }
    DECREF(frame->locals);

    stack->head=frame->next;
    stack->size--;
    return frame;
}

void vm_add_err(TypeObject* exception, struct vm* vm, const char *_format, ...) {
    if (vm->exception!=NULL){
        return;
    }
    
    va_list args;
    const int length=256;
    char format[length];
    sprintf(format, "%s", _format);
    
    vm->exception=exception->type->slot_call((object*)exception, new_tuple(), new_dict()); //Create new exception object

    char *msg = (char*)malloc(sizeof(char)*length);

    va_start(args, _format);
    vsnprintf(msg, length, format, args);
    va_end(args);
    
    DECREF(CAST_EXCEPTION(vm->exception)->err);
    CAST_EXCEPTION(vm->exception)->err=str_new_fromstr(new string(msg));
}

object* vm_setup_err(TypeObject* exception, struct vm* vm, const char *_format, ...) {    
    va_list args;
    const int length=256;
    char format[length];
    sprintf(format, "%s", _format);
    
    object* exc=exception->type->slot_call((object*)exception, new_tuple(), new_dict()); //Create new exception object  

    char *msg = (char*)malloc(sizeof(char)*length);

    va_start(args, _format);
    vsnprintf(msg, length, format, args);
    va_end(args);
    
    DECREF(CAST_EXCEPTION(exc)->err);
    CAST_EXCEPTION(exc)->err=str_new_fromstr(new string(msg));

    return exc;
}

struct vm* new_vm(uint32_t id, object* code, struct instructions* instructions, string* filedata){
    struct vm* vm=(struct vm*)malloc(sizeof(struct vm));
    vm->id=id;
    vm->ret_val=0;
    vm->ip=0;
    vm->objstack=new_datastack();
    vm->callstack=new_callstack();
    
    vm->exception=NULL;

    vm->filedata=filedata;

    add_callframe(vm->callstack, new_int_fromint(0), new string("<module>"), code);
    
    return vm;
}

void vm_del(struct vm* vm){
    struct callframe* i=vm->callstack->head;
    while (i){
        struct callframe* i_=i=i->next;;
        free(i);
        i=i_;
    }
    struct dataframe* j=vm->objstack->head;
    while (j){
        struct dataframe* j_=j=j->next;;
        free(i);
        j=j_;
    }

    DECREF(vm->globals);
    delete vm->filedata;
    DECREF(vm->exception);
}

void vm_add_var_locals(struct vm* vm, object* name, object* value){
    if (value->type->size==0){
        ((object_var*)value)->gc_ref++;
    }
    vm->callstack->head->locals->type->slot_set(vm->callstack->head->locals, name, value); //If globals is same obj as locals then this will still update both
}

struct object* vm_get_var_locals(struct vm* vm, object* name){
    struct callframe* frame=vm->callstack->head;
    while(frame){
        object* o=frame->locals->type->slot_get(frame->locals, name);
        if (o!=NULL){
            return o;
        }
        frame=frame->next;        
    }
    for (size_t i=0; i<nbuiltins; i++){
        if (object_istype(builtins[i]->type, &BuiltinType)){
            if (istrue(object_cmp(CAST_BUILTIN(builtins[i])->name, name, CMP_EQ))){
                return builtins[i];
            }
        }
        else{
            if (builtins[i]->type->name->compare((*CAST_STRING(name)->val))==0){
                return builtins[i];
            }
            if (object_istype(builtins[i]->type, &TypeType) && \
            CAST_TYPE(builtins[i])->name->compare((*CAST_STRING(name)->val))==0){
                return builtins[i];
            }
        }
    }

    vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
    return NULL;
}

void vm_add_var_globals(struct vm* vm, object* name, object* value){
    if (value->type->size==0){
        ((object_var*)value)->gc_ref++;
    }
    vm->globals->type->slot_set(vm->globals, name, value); //If globals is same obj as locals then this will still update both
}

struct object* vm_get_var_globals(struct vm* vm, object* name){
    object* o=vm->globals->type->slot_get(vm->globals, name);
    if (o!=NULL){
        return o;
    }
    for (size_t i=0; i<nbuiltins; i++){
        if (object_istype(builtins[i]->type, &BuiltinType)){
            if (istrue(object_cmp(CAST_BUILTIN(builtins[i])->name, name, CMP_EQ))){
                return builtins[i];
            }
        }
        else{
            if (builtins[i]->type->name->compare((*CAST_STRING(name)->val))==0){
                return builtins[i];
            }
            if (object_istype(builtins[i]->type, &TypeType) && \
            CAST_TYPE(builtins[i])->name->compare((*CAST_STRING(name)->val))==0){
                return builtins[i];
            }
        }
    }
    
    vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
    return NULL;
}



object* _vm_step(object* instruction, object* arg, struct vm* vm, uint32_t* ip){
    //Run one instruction
    switch (CAST_INT(instruction)->val->to_int()){
        case LOAD_CONST:{
            add_dataframe(vm, vm->objstack, CAST_CODE(vm->callstack->head->code)->co_consts->type->slot_get(CAST_CODE(vm->callstack->head->code)->co_consts, arg));
            break;
        }

        case STORE_GLOBAL:{
            vm_add_var_globals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), peek_dataframe(vm->objstack));
            break;
        }

        case LOAD_GLOBAL:{
            add_dataframe(vm, vm->objstack, vm_get_var_globals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg) ));
            break;
        }

        case STORE_NAME:{
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), peek_dataframe(vm->objstack));
            break;
        }

        case LOAD_NAME:{
            add_dataframe(vm, vm->objstack, vm_get_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg) ));
            break;
        }

        case BINOP_ADD:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            binop_add(vm, left, right);
            break;
        }

        case BINOP_SUB:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            binop_sub(vm, left, right);
            break;
        }

        case BINOP_MUL:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            binop_mul(vm, left, right);
            break;  
        }

        case BINOP_DIV:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            binop_div(vm, left, right);
            break;
        }

        case BINOP_IS:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            binop_is(vm, left, right);
            break;
        }

        case BINOP_EE:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            binop_ee(vm, left, right);
            break;
        }

        case UNARY_NEG:{
            struct object* right=pop_dataframe(vm->objstack);
            unary_neg(vm, right);
            break;
        }

        case MAKE_FUNCTION:{
            object* code=pop_dataframe(vm->objstack); //<- Code
            object* args=pop_dataframe(vm->objstack); //<- Args
            object* kwargs=pop_dataframe(vm->objstack); //<- Kwargs
            object* name=pop_dataframe(vm->objstack); //<- Name

            object* func=func_new_code(code, args, kwargs, CAST_INT(arg)->val->to_int(), name);
            add_dataframe(vm, vm->objstack, func);
            break;
        }

        case RETURN_VAL: {
            return pop_dataframe(vm->objstack);
        }

        case CALL_METHOD: {
            object* function=pop_dataframe(vm->objstack);
            object* head=pop_dataframe(vm->objstack);

            if (function->type->slot_call==NULL){
                vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
                return NULL;
            }

            uint32_t argc=CAST_INT(arg)->val->to_int()+1;
            uint32_t posargc=CAST_INT(pop_dataframe(vm->objstack))->val->to_int()+1;
            uint32_t kwargc=argc-posargc;     

            if (function->type->slot_call==NULL){
                vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
                return NULL;
            }
            
            if (object_istype(function->type, &FuncType)){
                if (CAST_FUNC(function)->argc-CAST_INT(CAST_FUNC(function)->kwargs->type->slot_len(CAST_FUNC(function)->kwargs))->val->to_int()>posargc \
                || CAST_INT(CAST_FUNC(function)->kwargs->type->slot_len(CAST_FUNC(function)->kwargs))->val->to_int()<kwargc \
                || CAST_FUNC(function)->argc<argc){
                    if (CAST_INT(CAST_FUNC(function)->kwargs->type->slot_len(CAST_FUNC(function)->kwargs))->val->to_int()==0){
                        vm_add_err(&ValueError, vm, "expected %d argument(s), got %d including self.",CAST_INT(CAST_FUNC(function)->args->type->slot_len(CAST_FUNC(function)->args))->val->to_int(), argc);
                        return NULL;
                    }
                    vm_add_err(&ValueError, vm, "expected %d to %d arguments, got %d including self.",CAST_INT(CAST_FUNC(function)->args->type->slot_len(CAST_FUNC(function)->args))->val->to_int(), CAST_FUNC(function)->argc, argc);
                    return NULL;
                }
            }
                

            //Setup kwargs
            object* kwargs=new_dict();
            object* val;
            for (uint32_t i=0; i<kwargc; i++){
                val=pop_dataframe(vm->objstack);
                kwargs->type->slot_set(kwargs, pop_dataframe(vm->objstack), val);
            }
            //

            //Setup args
            object* args=new_tuple();
            args->type->slot_append(args, head);
            for (uint32_t i=0; i<posargc-1; i++){
                args->type->slot_append(args, pop_dataframe(vm->objstack));
            }
            
            //

            //Call
            if (object_istype(function->type, &FuncType)){
                add_callframe(vm->callstack, INCREF(new_int_fromint(0)), CAST_STRING(CAST_FUNC(function)->name)->val, INCREF(CAST_FUNC(function)->code));
                vm->callstack->head->locals=new_dict();
            }
            object* ret=object_call(function, args, kwargs);
            pop_callframe(vm->callstack);
            if (ret==NULL){
                return CALL_ERR;
            }
            add_dataframe(vm, vm->objstack, ret);
            break;
        }

        case CALL_FUNCTION: {
            object* function=pop_dataframe(vm->objstack);

            uint32_t argc=CAST_INT(arg)->val->to_int();
            uint32_t posargc=CAST_INT(pop_dataframe(vm->objstack))->val->to_int();
            uint32_t kwargc=argc-posargc;     

            if (function->type->slot_call==NULL){
                vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
                return NULL;
            }
            
            if (object_istype(function->type, &FuncType)){
                if (CAST_FUNC(function)->argc-CAST_INT(CAST_FUNC(function)->kwargs->type->slot_len(CAST_FUNC(function)->kwargs))->val->to_int()>posargc \
                || CAST_INT(CAST_FUNC(function)->kwargs->type->slot_len(CAST_FUNC(function)->kwargs))->val->to_int()<kwargc \
                || CAST_FUNC(function)->argc<argc){
                    if (CAST_INT(CAST_FUNC(function)->kwargs->type->slot_len(CAST_FUNC(function)->kwargs))->val->to_int()==0){
                        vm_add_err(&ValueError, vm, "expected %d argument(s), got %d.",CAST_INT(CAST_FUNC(function)->args->type->slot_len(CAST_FUNC(function)->args))->val->to_int(), argc);
                        return NULL;
                    }
                    vm_add_err(&ValueError, vm, "expected %d to %d arguments, got %d.",CAST_INT(CAST_FUNC(function)->args->type->slot_len(CAST_FUNC(function)->args))->val->to_int(), CAST_FUNC(function)->argc, argc);
                    return NULL;
                }
            }

            if (object_istype(function->type, &BuiltinType)){
                if (CAST_BUILTIN(function)->argc-CAST_INT(CAST_BUILTIN(function)->kwargs->type->slot_len(CAST_BUILTIN(function)->kwargs))->val->to_int()>posargc \
                || CAST_INT(CAST_BUILTIN(function)->kwargs->type->slot_len(CAST_BUILTIN(function)->kwargs))->val->to_int()<kwargc \
                || CAST_BUILTIN(function)->argc<argc){
                    if (CAST_INT(CAST_BUILTIN(function)->kwargs->type->slot_len(CAST_BUILTIN(function)->kwargs))->val->to_int()==0 || \
                    CAST_INT(CAST_BUILTIN(function)->args->type->slot_len(CAST_BUILTIN(function)->args))->val->to_int()==CAST_BUILTIN(function)->argc){
                        vm_add_err(&ValueError, vm, "expected %d argument(s).",CAST_INT(CAST_BUILTIN(function)->args->type->slot_len(CAST_BUILTIN(function)->args))->val->to_int());
                        return NULL;
                    }
                    vm_add_err(&ValueError, vm, "expected %d to %d arguments.",CAST_INT(CAST_BUILTIN(function)->args->type->slot_len(CAST_BUILTIN(function)->args))->val->to_int(), CAST_FUNC(function)->argc);
                    return NULL;
                }
            }
                

            //Setup kwargs
            object* kwargs=new_dict();
            object* val;
            for (uint32_t i=0; i<kwargc; i++){
                val=pop_dataframe(vm->objstack);
                kwargs->type->slot_set(kwargs, pop_dataframe(vm->objstack), val);
            }
            //

            //Setup args
            object* args=new_tuple();
            for (uint32_t i=0; i<posargc; i++){
                args->type->slot_append(args, pop_dataframe(vm->objstack));
            }
            
            
            //

            //Call
            if (object_istype(function->type, &FuncType)){
                add_callframe(vm->callstack, INCREF(new_int_fromint(0)),  CAST_STRING(CAST_FUNC(function)->name)->val, INCREF(CAST_FUNC(function)->code));
                vm->callstack->head->locals=new_dict();
            }
            object* ret=object_call(function, args, kwargs);
            if (object_istype(function->type, &FuncType)){
                pop_callframe(vm->callstack);
            }
            if (ret==NULL){
                return CALL_ERR;
            }
            add_dataframe(vm, vm->objstack, ret);
            
            break;            
        }

        case BUILD_LIST: {
            object* list=new_list();
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                list->type->slot_append(list, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, list);
            break;
        }

        case BUILD_TUPLE: {
            object* tuple=new_tuple();
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                tuple->type->slot_append(tuple, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, tuple);
            break;
        }

        case BUILD_DICT: {
            object* dict=new_dict();
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                object* name=pop_dataframe(vm->objstack);
                dict->type->slot_set(dict, name, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, dict);
            break;
        }

        case LOAD_BUILD_CLASS: {
            add_dataframe(vm, vm->objstack, BUILTIN_BUILD_CLASS);
            break;
        }

        case LOAD_REGISTER_POP: {
            vm->accumulator=pop_dataframe(vm->objstack);
            break;
        }

        case READ_REGISTER_PUSH: {
            add_dataframe(vm, vm->objstack, vm->accumulator);
            break;
        }      

        case LOAD_ATTR: {
            object* obj=pop_dataframe(vm->objstack);
            object* attr=CAST_CODE(vm->callstack->head->code)->co_names->type->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg);
            add_dataframe(vm, vm->objstack, object_getattr(obj, attr));
            break;
        }  

        case STORE_ATTR: {
            object* obj=pop_dataframe(vm->objstack);
            object* val=peek_dataframe(vm->objstack); //For multiple assignment
            object* attr=CAST_CODE(vm->callstack->head->code)->co_names->type->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg);
            object_setattr(obj, attr, val);
            break;
        }

        case POP_JMP_TOS_FALSE: {
            if (!istrue(object_istruthy(peek_dataframe(vm->objstack)))){
                pop_dataframe(vm->objstack);
                (*ip)=(*ip)+CAST_INT(arg)->val->to_long();
            }
            break;
        }

        case JUMP_DELTA: {
            (*ip)=(*ip)+CAST_INT(arg)->val->to_long();
            break;
        }

        case BINOP_SUBSCR: {
            object* idx=pop_dataframe(vm->objstack);
            object* base=pop_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, object_subscript(base, idx));
            break;
        }

        case RAISE_EXC: {
            if (!object_issubclass(peek_dataframe(vm->objstack), &ExceptionType)){
                vm_add_err(&TypeError, vm, "Exceptions must be subclass of Exception");
                break;
            }
            vm->exception=pop_dataframe(vm->objstack);
            break;
        }

        default:
            return NULL;
            
    };
    return NULL;
}

object* run_vm(object* codeobj, uint32_t* ip){
    object* code=CAST_CODE(codeobj)->co_code;
    object* lines=CAST_CODE(codeobj)->co_lines;
    
    uint32_t linetup_cntr=0;
    object* instruction=code->type->slot_next(code);
    object* linetup=lines->type->slot_get(lines, new_int_fromint(linetup_cntr));
    object* idx0=new_int_fromint(0);
    object* idx1=new_int_fromint(1);
    object* idx2=new_int_fromint(2);
    while (instruction){
        (*ip)++;
        if ((*ip)>(*CAST_INT(linetup->type->slot_get(linetup, idx1))->val).to_int()){
            linetup_cntr++;
            linetup=lines->type->slot_get(lines, new_int_fromint(linetup_cntr));
        }
        vm->callstack->head->line=linetup->type->slot_get(linetup, idx2);

        uint32_t ip_=(*ip);
        object* obj=_vm_step(instruction, code->type->slot_next(code), vm, ip);
        if ((*ip)!=ip_){
            for (uint32_t i=0; i<((*ip)-ip_)*2; i++){
                code->type->slot_next(code);
            }
        }
        if (obj==CALL_ERR){
            CAST_LIST(code)->idx=0;
            return CALL_ERR;
        }
        if (obj!=NULL){
            CAST_LIST(code)->idx=0;
            return obj;
        }
        if (vm->exception!=NULL){
            struct callframe* callframe=vm->callstack->head;
            while (callframe){    
                if (callframe->name==NULL){
                    callframe=callframe->next;
                }
                cout<<"In file "+object_cstr(CAST_CODE(vm->callstack->head->code)->co_file)+", line "+to_string(CAST_INT(callframe->line)->val->to_int()+1)+", in "+(*callframe->name)<<endl;
                
                int line=0;
                int target=CAST_INT(callframe->line)->val->to_int();
                int startidx=0;
                int endidx=0;
                int idx=0;
                bool entered=false;
                while (true){
                    if (line==target && !entered){
                        startidx=idx;
                        entered=true;
                    }
                    if (entered && ((*vm->filedata)[idx]=='\n' || (*vm->filedata)[idx]=='\0')){
                        endidx=idx;
                        break;
                    }
                    else if ((*vm->filedata)[idx]=='\n'){
                        line++;
                    }
                    idx++;
                }

                string snippet="";
                for (int i=startidx; i<endidx; i++){
                    snippet+=(*vm->filedata)[i];
                }

                cout<<snippet<<endl;
                
                callframe=callframe->next;
            }

            cout<<vm->exception->type->name->c_str()<<": "<<object_cstr(CAST_EXCEPTION(vm->exception)->err)<<endl;
            CAST_LIST(code)->idx=0;
            return NULL;
        }
        instruction=code->type->slot_next(code);
    }

    CAST_LIST(code)->idx=0;

    return new_none();
}
