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

struct blockstack* new_blockstack(){
    struct blockstack* block=(struct blockstack*)malloc(sizeof(struct blockstack));
    block->head=NULL;
    block->size=0;
    return block;
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
    frame->code=INCREF(code);

    stack->size++;
    stack->head=frame;
}

struct callframe* pop_callframe(struct callstack* stack){
    struct callframe* frame=stack->head;

    
    DECREF(frame->line);
    DECREF(frame->code);
    DECREF(frame->locals);

    stack->head=frame->next;
    stack->size--;
    return frame;
}

void vm_add_err(struct vm* vm, const char *_format, ...) {
    if (vm->haserr){
        return;
    }
    va_list args;
    const int length=256;
    char format[length];
    sprintf(format, "%s", _format);

    vm->haserr=true;

    struct callframe* callframe=vm->callstack->head;
    while (callframe){    
        vm->headers->push_back(new string("In file "+object_cstr(CAST_CODE(vm->callstack->head->code)->co_file)+", line "+to_string(CAST_INT(callframe->line)->val->to_int()+1)+", in "+(*callframe->name)));
        
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

        vm->snippets->push_back(new string(snippet));
        
        callframe=callframe->next;
    }

    char *msg = (char*)malloc(sizeof(char)*length);

    va_start(args, _format);
    vsnprintf(msg, length, format, args);
    va_end(args);

    vm->err=(string*)msg;
}

struct vm* new_vm(uint32_t id, object* code, struct instructions* instructions, string* filedata){
    struct vm* vm=(struct vm*)malloc(sizeof(struct vm));
    vm->id=id;
    vm->ret_val=0;
    vm->ip=0;
    vm->objstack=new_datastack();
    vm->callstack=new_callstack();
    vm->blockstack=new_blockstack();
    
    vm->haserr=false;
    vm->headers=new vector<string*>;
    vm->headers->clear();

    vm->snippets=new vector<string*>;
    vm->snippets->clear();

    vm->err=NULL;

    vm->filedata=filedata;

    add_callframe(vm->callstack, new_int_fromint(0), new string("<module>"), code);
    
    return vm;
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
    vm_add_err(vm, "NameError: Cannot find name %s.", object_cstr(object_repr(name)).c_str());
    return NULL;
}

void vm_add_var_globals(struct vm* vm, object* name, object* value){
    if (value->type->size==0){
        ((object_var*)value)->gc_ref++;
    }
    vm->globals->type->slot_set(vm->callstack->head->locals, name, value); //If globals is same obj as locals then this will still update both
}

struct object* vm_get_var_globals(struct vm* vm, object* name){
    object* o=vm->globals->type->slot_get(vm->callstack->head->locals, name);
    if (o==NULL){
        vm_add_err(vm, "NameError: Cannot find name %s.", object_cstr(object_repr(name)).c_str());
        return NULL;
    }
    return o;
}



object* _vm_step(object* instruction, object* arg, struct vm* vm){
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

        case CALL_FUNCTION: {
            object* function=pop_dataframe(vm->objstack);
            
            uint32_t argc=CAST_INT(arg)->val->to_int();
            uint32_t posargc=CAST_INT(pop_dataframe(vm->objstack))->val->to_int();
            uint32_t kwargc=argc-posargc;                    

            if (CAST_FUNC(function)->argc-CAST_INT(CAST_FUNC(function)->args->type->slot_len(CAST_FUNC(function)->args))->val->to_int()>posargc \
            || CAST_INT(CAST_FUNC(function)->kwargs->type->slot_len(CAST_FUNC(function)->kwargs))->val->to_int()<kwargc \
            || CAST_FUNC(function)->argc<argc){
                if (CAST_INT(CAST_FUNC(function)->kwargs->type->slot_len(CAST_FUNC(function)->kwargs))->val->to_int()==0){
                    vm_add_err(vm, "ValueError: expected %d argument(s).",CAST_INT(CAST_FUNC(function)->args->type->slot_len(CAST_FUNC(function)->args))->val->to_int());
                    return NULL;
                }
                vm_add_err(vm, "ValueError: expected %d to %d arguments.",CAST_INT(CAST_FUNC(function)->args->type->slot_len(CAST_FUNC(function)->args))->val->to_int(), CAST_FUNC(function)->argc);
                return NULL;
            }

            //Setup kwargs
            object* kwargs=dict_new(NULL, NULL);
            object* val;
            for (uint32_t i=0; i<kwargc; i++){
                val=pop_dataframe(vm->objstack);
                kwargs->type->slot_set(kwargs, pop_dataframe(vm->objstack), val);
            }
            //

            //Setup args
            object* args=tuple_new(NULL, NULL);
            for (uint32_t i=0; i<posargc; i++){
                args->type->slot_append(args, pop_dataframe(vm->objstack));
            }
            
            //

            //Call

            add_callframe(vm->callstack, INCREF(vm->callstack->head->line), CAST_STRING(object_repr(function))->val, INCREF(CAST_FUNC(function)->code));
            vm->callstack->head->locals=dict_new(NULL, NULL);
            function->type->slot_call(function, args, kwargs);
            cout<<"--------\n";
            for (auto k: (*CAST_DICT(vm->callstack->head->locals)->val)){
                cout<<(*CAST_STRING(object_str(k.first))->val)<<" = "<<(*CAST_STRING(object_str(k.second))->val)<<endl;
            }
            cout<<"\n--------";
            
            pop_callframe(vm->callstack);
            
            break;            
        }

        case BUILD_TUPLE: {
            object* tuple=tuple_new(NULL, NULL);
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                tuple->type->slot_append(tuple, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, tuple);
            break;
        }

        case BUILD_DICT: {
            object* dict=dict_new(NULL, NULL);
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                object* name=pop_dataframe(vm->objstack);
                dict->type->slot_set(dict, name, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, dict);
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
            vm->callstack->head->line=linetup->type->slot_get(linetup, idx2);
        }
        
        object* obj=_vm_step(instruction, code->type->slot_next(code), vm);
        if (obj!=NULL){
            return obj;
        }
        if (vm->haserr){
            for (int i=0; i<vm->headers->size(); i++){
                cout<<(*(*vm->headers)[i])<<endl;
                cout<<"  "<<(*(*vm->snippets)[i])<<endl;
            }
            
            printf("%s\n",vm->err);
            break;
        }
        instruction=code->type->slot_next(code);
    }

    return new_none();
}
