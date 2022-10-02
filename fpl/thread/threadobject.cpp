object* thread_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",len);
        return NULL;
    }

    object* callable=list_index_int(args,0);
    
    object* obj=new_object(CAST_TYPE(type));

    if (!istrue(object_iscallable(callable))){
        vm_add_err(&TypeError, vm, "Expected callable, got %s object",callable->type->name->c_str());
        return NULL;
    }

    CAST_THREAD(obj)->callable=INCREF(callable);
    CAST_THREAD(obj)->thread=NULL;

    return obj;
}

void thread_del(object* self){
    DECREF(CAST_THREAD(self)->callable);
    if (CAST_THREAD(self)->thread!=NULL){
        free(CAST_THREAD(self)->thread);
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
        object* v=object_call_nokwargs(args->callable, new_tuple());
        if (v!=NULL){
            DECREF(v);
        }
    }
    else if (args->args!=NULL && args->kwargs==NULL){
        object* v=object_call_nokwargs(args->callable, args->args);
        if (v!=NULL){
            DECREF(v);
        }
    }
    else{
        object* v=object_call(args->callable, args->args, args->kwargs);
        if (v!=NULL){
            DECREF(v);
        }
    }
    
    pthread_exit(NULL);
    return NULL;
}

object* thread_start_meth(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if ((len!=3 && CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 2) &&\
    (len!=2 && CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 1) &&\
    (len!=1 && CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0)){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    
    pthread_t* t=(pthread_t*)malloc(sizeof(pthread_t));
    ThreadArgs* args_=(ThreadArgs*)malloc(sizeof(ThreadArgs));
    memset(args_, 0, sizeof(ThreadArgs));

    object* kw=str_new_fromstr("kwargs");
    if (object_find_bool_dict_keys(kwargs, kw)){
        args_->kwargs=kwargs->type->slot_mappings->slot_get(kwargs, kw);
    }
    
    object* ar=str_new_fromstr("args");
    if (object_find_bool_dict_keys(kwargs, ar)){
        args_->args=kwargs->type->slot_mappings->slot_get(kwargs, ar);
    }

    args_->callable=CAST_THREAD(tuple_index_int(args,0))->callable;

    pthread_create(t, NULL, &_thread_start_wrap, (void*)args_);
    fpl_threads.push_back(t);
    
    CAST_THREAD(self)->thread=t;
    if (vm->exception!=NULL){
        return NULL;
    }
    return new_none();
}

object* thread_join_meth(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
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
