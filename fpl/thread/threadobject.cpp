object* thread_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    if (len!=1 || CAST_DICT(kwargs)->val->size()!=0){
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
    if (args->args==NULL && args->kwargs==NULL){
        GIL.lock();
        object* v=object_call_nokwargs(args->callable, new_tuple());
        if (v!=NULL){
            FPLDECREF(v);
        }
    }
    else if (args->args!=NULL && args->kwargs==NULL){
        GIL.lock();
        object* v=object_call_nokwargs(args->callable, args->args);
        if (v!=NULL){
            FPLDECREF(v);
        }
    }
    else{
        GIL.lock();
        object* v=object_call(args->callable, args->args, args->kwargs);
        if (v!=NULL){
            FPLDECREF(v);
        }
    }
    
    pthread_exit(NULL);
    return NULL;
}

object* thread_start_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    if ((len!=3 && CAST_DICT(kwargs)->val->size() != 2) &&\
    (len!=2 && CAST_DICT(kwargs)->val->size() != 1) &&\
    (len!=1 && CAST_DICT(kwargs)->val->size() != 0)){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    
    pthread_t* t=(pthread_t*)fpl_malloc(sizeof(pthread_t));
    ThreadArgs* args_=(ThreadArgs*)fpl_malloc(sizeof(ThreadArgs));
    memset(args_, 0, sizeof(ThreadArgs));

    object* kw=str_new_fromstr("kwargs");
    if (object_find_bool_dict_keys(kwargs, kw)){
        args_->kwargs=kwargs->type->slot_mappings->slot_get(kwargs, kw);
    }
    
    object* ar=str_new_fromstr("args");
    if (object_find_bool_dict_keys(kwargs, ar)){
        args_->args=kwargs->type->slot_mappings->slot_get(kwargs, ar);
    }
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
    long len= CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    if (len!=1 || CAST_DICT(kwargs)->val->size() != 0){
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
