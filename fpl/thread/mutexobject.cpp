object* mutex_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=0){
        vm_add_err(&ValueError, vm, "Expected 0 arguments, got %d",len);
        return NULL;
    }
    
    object* obj=new_object(CAST_TYPE(type));
    
    pthread_mutex_t* lock=(pthread_mutex_t*)fpl_malloc(sizeof(pthread_mutex_t));
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
        fpl_free(CAST_MUTEX(self)->lock);
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

object* mutex_acquire_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args,0);
    pthread_mutex_lock(CAST_MUTEX(self)->lock);
    return new_none();
}

object* mutex_release_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    
    pthread_mutex_unlock(CAST_MUTEX(tuple_index_int(args,0))->lock);
    CAST_MUTEX(tuple_index_int(args,0))->locked=false;
    return new_none();
}