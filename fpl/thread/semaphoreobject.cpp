object* semaphore_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    if (len!=1 || CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",len);
        return NULL;
    }

    object* val=object_int(list_index_int(args, 0));
    if (val==NULL || !object_istype(val->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 0)->type->name->c_str());
        return NULL; 
    }
    int v=CAST_INT(val)->val->to_int();

    if (v<0){
        vm_add_err(&ValueError, vm, "Expected value greater than 0, got %d", v);
        return NULL;
    }

    object* obj=new_object(CAST_TYPE(type));
    
    sem_t* sem=(sem_t*)fpl_malloc(sizeof(sem_t));
    sem_init(sem, 0, v);
    
    CAST_SEMAPHORE(obj)->sem=sem;

    return obj;
}

void semaphore_del(object* self){
    sem_destroy(CAST_SEMAPHORE(self)->sem);
}

object* semaphore_repr(object* self){
    string s="";
    s+="<Semaphore ";
    int i;
    sem_getvalue(CAST_SEMAPHORE(self)->sem, &i);
    s+=to_string(i);
    s+=">";
    return str_new_fromstr(s);
}

object* semaphore_acquire_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    if ((len!=1 || CAST_DICT(kwargs)->val->size() != 0) && //No args
        (len!=2 || CAST_DICT(kwargs)->val->size() != 1)){ //KW arg
        vm_add_err(&ValueError, vm, "Expected 1 or 2 arguments, got %d", len);
        return NULL; 
    }

    object* flag=NULL;
    object* self=tuple_index_int(args,0);

    if (len==2){
        object* k=str_new_fromstr("block");
        object* flag=dict_get(kwargs, k);
        FPLDECREF(k);
    }

    if (flag==NULL){
        sem_wait(CAST_SEMAPHORE(self)->sem);
        return new_bool_true();
    }
    else{
        if (istrue(flag)){
            sem_trywait(CAST_SEMAPHORE(self)->sem);
            if (errno==EAGAIN){
                errno=0;
                return new_bool_false();
            }
            return new_bool_true();
        }
        else{
            sem_wait(CAST_SEMAPHORE(self)->sem);
            return new_bool_true();
        }
    }
}

object* semaphore_release_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    if ((len!=1 || CAST_DICT(kwargs)->val->size() != 0) && //No args
        (len!=2 || CAST_DICT(kwargs)->val->size() != 1)){ //KW arg
        vm_add_err(&ValueError, vm, "Expected 1 or 2 arguments, got %d", len);
        return NULL; 
    }

    object* v=NULL;
    object* self=tuple_index_int(args,0);

    if (len==2){
        object* k=str_new_fromstr("n");
        object* v=dict_get(kwargs, k);
        FPLDECREF(k);
    }

    int v_=1;

    if (v!=NULL){
        v_=CAST_INT(v)->val->to_int();
        FPLDECREF(v);
    }

    for (int i=0; i<v_; i++){
        sem_post(CAST_SEMAPHORE(self)->sem);
    }

    return new_none();
}