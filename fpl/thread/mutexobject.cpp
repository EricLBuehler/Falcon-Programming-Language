object* mutex_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (len!=0 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 0 arguments, got %d",len);
        return NULL;
    }
    
    object* obj=new_object(CAST_TYPE(type));
    
    pthread_mutex_t* lock=(pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(lock, NULL);
    pthread_mutex_unlock(lock);
    
    CAST_MUTEX(obj)->lock=lock;
    CAST_MUTEX(obj)->locked=false;

    return obj;
}

void mutex_del(object* self){
    pthread_mutex_unlock(CAST_MUTEX(self)->lock);
    pthread_mutex_destroy(CAST_MUTEX(self)->lock);
    if (CAST_MUTEX(self)->lock!=NULL){
        free(CAST_MUTEX(self)->lock);
    }
}

object* mutex_repr(object* self){
    string s="";
    s+="<Mutex ";
    if (CAST_MUTEX(self)->locked){
        s+="Locked";
    }
    else{
        s+="Unlocked";
    }
    s+=">";
    return str_new_fromstr(s);
}

object* mutex_acquire_meth(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    if (CAST_MUTEX(tuple_index_int(args,0))->locked){
        return new_bool_false();
    }

    pthread_mutex_lock(CAST_MUTEX(tuple_index_int(args,0))->lock);
    
    CAST_MUTEX(tuple_index_int(args,0))->locked=true;
    return new_bool_true();
}

object* mutex_release_meth(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    
    pthread_mutex_unlock(CAST_MUTEX(tuple_index_int(args,0))->lock);
    CAST_MUTEX(tuple_index_int(args,0))->locked=false;
    return new_none();
}