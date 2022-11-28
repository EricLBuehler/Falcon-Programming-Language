object* thread_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",len);
        return NULL;
    }

    object* callable=list_index_int(args,0);
    
    object* obj=new_object(CAST_TYPE(type));

    if (!istrue(object_iscallable(callable))){
        vm_add_err(&TypeError, vm, "Expected callable, got %s object",callable->type->name->c_str());
        return NULL;
    }

    FPLINCREF(callable);
    CAST_THREAD(obj)->callable=callable;
    CAST_THREAD(obj)->thread=NULL;

    return obj;
}

void thread_del(object* self){
    FPLDECREF(CAST_THREAD(self)->callable);
    if (CAST_THREAD(self)->thread!=NULL){
        fpl_free(CAST_THREAD(self)->thread);
    }
}

object* thread_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_THREAD(self)->callable, CAST_THREAD(other)->callable, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_THREAD(self)->callable, CAST_THREAD(other)->callable, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

object* thread_repr(object* self){
    string s="";
    s+="<Thread ";
    s+=object_cstr(CAST_THREAD(self)->callable);
    s+=">";
    return str_new_fromstr(s);
}

void* _thread_start_wrap(void* args_){
    ThreadArgs* args=(ThreadArgs*)args_;
    //new vm
    struct vm* vm=new_vm_raw(interpreter.vm_map->size());
    //Copy callstack
    for (int i=0; i<::vm->callstack->size-1; i++){
        add_callframe(vm->callstack, ::vm->callstack->data[i].line, ::vm->callstack->data[i].name, ::vm->callstack->data[i].code, ::vm->callstack->data[i].callable, ::vm->callstack->data[i].ip);
        FPLINCREF(::vm->callstack->data[i].line);
        FPLINCREF(::vm->callstack->data[i].locals);
        FPLINCREF(::vm->callstack->data[i].annotations);
        FPLINCREF(::vm->callstack->data[i].code);
        if (::vm->callstack->data[i].callable!=NULL){
            FPLINCREF(::vm->callstack->data[i].callable);
        }

        vm->callstack->data[i].line=::vm->callstack->data[i].line;
        vm->callstack->data[i].locals=::vm->callstack->data[i].locals;
        vm->callstack->data[i].annotations=::vm->callstack->data[i].annotations;
        vm->callstack->data[i].callable=::vm->callstack->data[i].callable;
        vm->callstack->data[i].code=::vm->callstack->data[i].code;
    }
    vm->globals=::vm->globals;
    FPLINCREF(::vm->globals);
    vm->global_annotations=::vm->global_annotations;
    FPLINCREF(::vm->global_annotations);
    interpreter_add_vm(interpreter.vm_map->size(), vm);
    gil_lock(vm->id);
    if (args->args==NULL && args->kwargs==NULL){  
        object* v=object_call_nokwargs(args->callable, new_tuple());
        if (v!=NULL && v!=TERM_PROGRAM){
            FPLDECREF(v);
        }
    }
    else if (args->args!=NULL && args->kwargs==NULL){
        object* v=object_call_nokwargs(args->callable, args->args);
        if (v!=NULL && v!=TERM_PROGRAM){
            FPLDECREF(v);
        }
    }
    else{
        object* v=object_call(args->callable, args->args, args->kwargs);
        if (v!=NULL && v!=TERM_PROGRAM){
            FPLDECREF(v);
        }
    }

    vm_del(vm);
    
    pthread_exit(NULL);
    return NULL;
}

object* thread_start_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if ((len!=3 && CAST_DICT(kwargs)->val->size() != 2) &&\
    (len!=2 && CAST_DICT(kwargs)->val->size() != 1) &&\
    (len!=1 && CAST_DICT(kwargs)->val->size() != 0)){
        vm_add_err(&ValueError, vm, "Expected 1, 2, or 3 arguments, got %d", len);
        return NULL; 
    }
    
    pthread_t* t=(pthread_t*)fpl_malloc(sizeof(pthread_t));
    ThreadArgs* args_=(ThreadArgs*)fpl_malloc(sizeof(ThreadArgs));
    memset(args_, 0, sizeof(ThreadArgs));

    object* kw=str_new_fromstr("kwargs");
    if (object_find_bool_dict_keys(kwargs, kw)){
        args_->kwargs=dict_get_opti_deref(kwargs, kw);
    }
    
    object* ar=str_new_fromstr("args");
    if (object_find_bool_dict_keys(kwargs, ar)){
        args_->args=dict_get_opti_deref(kwargs, ar);
    }
    FPLDECREF(ar);
    FPLDECREF(kw);
    
    object* self=tuple_index_int(args, 0);
    args_->callable=CAST_THREAD(self)->callable;

    pthread_create(t, NULL, &_thread_start_wrap, (void*)args_);
    fpl_threads.push_back(t);
    
    CAST_THREAD(self)->thread=t;
    if (vm->exception!=NULL){
        return NULL;
    }
    return new_none();
}

object* thread_join_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    if (CAST_THREAD(tuple_index_int(args,0))->thread==NULL){
        vm_add_err(&InvalidOperationError, vm, "Cannot join non-started thread");
        return NULL; 
    }

    pthread_join(*CAST_THREAD(tuple_index_int(args,0))->thread, NULL);
    return new_none();
}

object* thread_getid_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    if (CAST_THREAD(tuple_index_int(args,0))->thread==NULL){
        vm_add_err(&InvalidOperationError, vm, "Cannot get id of non-started thread");
        return NULL; 
    }
    
    return new_int_fromint(pthread_self());
}
