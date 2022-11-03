object* newtp_init(object* self, object* args, object* kwargs){
    //Try to call __init__
    object* n=object_getattr(self, str_new_fromstr("__init__"));
    ERROR_RET(n);
    object* val=object_call(n, args, kwargs);
    return val;
}
object* newtp_new(object* self, object* args, object* kwargs){
    //Try to call __new__
    object* n=object_genericgetattr(self, str_new_fromstr("__new__"));
    ERROR_RET(n);
    
    object* val=object_call(n, args, kwargs);
    if (val!=NULL && object_istype(val->type, &TypeType)){
        return object_new(val, args, kwargs);
    }
    return val;
}

void _newtp_del(object* self){
    object* dict= (*(object**)((char*)self + self->type->dict_offset));
    FPLDECREF(dict);
}

void newtp_del(object* self){
    object* n=object_getattr(self, str_new_fromstr("__del__"));
    if (n==NULL || n==TERM_PROGRAM || n==CALL_ERR){
        return;
    }
    object* args=new_tuple();
    
    object_call_nokwargs(n, args);

    _newtp_del(self);
}

object* newtp_next(object* self){
    object* n=object_getattr(self, str_new_fromstr("__next__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_get(object* self, object* idx){
    object* n=object_getattr(self, str_new_fromstr("__getitem__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, idx);
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_len(object* self){
    object* n=object_getattr(self, str_new_fromstr("__len__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_set(object* self, object* idx, object* val){
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, idx);
    
    object* n=object_getattr(self, str_new_fromstr("__setitem__"));
    if (n==NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
        n=object_getattr(self, str_new_fromstr("__delitem__"));    
    }
    else{
        args->type->slot_mappings->slot_append(args, val);
    }
    ERROR_RET(n);
    
    val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_repr(object* self){
    object* n=object_getattr(self, str_new_fromstr("__repr__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_str(object* self){
    object* n=object_getattr(self, str_new_fromstr("__str__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    object* val=object_call_nokwargs(n, args);
    
    return val;
}
object* newtp_call(object* self, object* args, object* kwargs){
    //Try to call __call__
    object* function=object_getattr(self, str_new_fromstr("__call__"));
    ERROR_RET(function);
    
    uint32_t argc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->operator+((*CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val)).to_int()+1;
    uint32_t posargc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+1;
    uint32_t kwargc=argc-posargc;     

    if (function->type->slot_call==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
        return NULL;
    }
    
    
    object* val=object_call(function, args, kwargs);
    
    return val;
}

object* newtp_cmp(object* self, object* other, uint8_t type){
    if (type==CMP_EQ){
        object* n=object_getattr(self, str_new_fromstr("__eq__"));
        ERROR_RET(n);
    
        if (n!=NULL){
            object* args=new_tuple();
            
            object* val=object_call_nokwargs(n, args);
            
            return val;
        }
    }

    if (type==CMP_NE){
        object* n=object_getattr(self, str_new_fromstr("__ne__"));
        ERROR_RET(n);
    
        if (n!=NULL){
            object* args=new_tuple();
            
            object* val=object_call_nokwargs(n, args);
            
            return val;
        }
    }

    if (type==CMP_LT){
        object* n=object_getattr(self, str_new_fromstr("__lt__"));
        ERROR_RET(n);
    
        if (n!=NULL){
            object* args=new_tuple();
            
            object* val=object_call_nokwargs(n, args);
            
            return val;
        }
        return NULL;
    }

    if (type==CMP_GT){
        object* n=object_getattr(self, str_new_fromstr("__gt__"));
        ERROR_RET(n);
    
        if (n!=NULL){
            object* args=new_tuple();
            
            object* val=object_call_nokwargs(n, args);
            
            return val;
        }
    }

    if (type==CMP_LTE){
        object* n=object_getattr(self, str_new_fromstr("__lte__"));
        ERROR_RET(n);
    
        if (n!=NULL){
            object* args=new_tuple();
            
            object* val=object_call_nokwargs(n, args);
            
            return val;
        }
    }

    if (type==CMP_GTE){
        object* n=object_getattr(self, str_new_fromstr("__gte__"));
        ERROR_RET(n);
    
        if (n!=NULL){
            object* args=new_tuple();
            
            object* val=object_call_nokwargs(n, args);
            
            return val;
        }
    }

    if (type==CMP_IN){
        object* n=object_getattr(self, str_new_fromstr("__in__"));
        ERROR_RET(n);
    
        if (n!=NULL){
            object* args=new_tuple();
            tuple_append(args, other);
            
            object* val=object_call_nokwargs(n, args);
            
            return val;
        }
    }
    return NULL;
}

object* newtp_pow(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__pow__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}
object* newtp_mod(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__mod__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}
object* newtp_add(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__add__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}
object* newtp_sub(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__sub__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}
object* newtp_mul(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__mul__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}
object* newtp_div(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__div__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}
object* newtp_fldiv(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__floordiv__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_and(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__and__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_or(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__or__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_xor(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__xor__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_lshift(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__lshift__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_rshift(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__rshift__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_neg(object* self){
    object* n=object_getattr(self, str_new_fromstr("__neg__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_not(object* self){
    object* n=object_getattr(self, str_new_fromstr("__not__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_abs(object* self){
    object* n=object_getattr(self, str_new_fromstr("__abs__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_bool(object* self){
    object* n=object_getattr(self, str_new_fromstr("__bool__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_int(object* self){
    object* n=object_getattr(self, str_new_fromstr("__int__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_float(object* self){
    object* n=object_getattr(self, str_new_fromstr("__float__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_iter(object* self){
    object* n=object_getattr(self, str_new_fromstr("__iter__"));
    ERROR_RET(n);
    
    object* args=new_tuple();
    
    object* val=object_call_nokwargs(n, args);
    
    return val;
}

object* newtp_getattr(object* self, object* attr){
    object* n=object_genericgetattr_notype(self, str_new_fromstr("__getattr__"));
    
    if (n!=NULL){
        object* args=new_tuple();
        
        args->type->slot_mappings->slot_append(args, attr);
        object* val=object_call_nokwargs(n, args);
        
        return val;
    }
    else{
        FPLDECREF(vm->exception);
        vm->exception=NULL;
        
        object* n=object_genericgetattr(self, attr);
        return n;
    }
}

object* newtp_setattr(object* self, object* attr, object* val){
    object* args=new_tuple();
    
    args->type->slot_mappings->slot_append(args, attr);
    
    object* n;
    n=object_genericgetattr_notype(self, str_new_fromstr("__setattr__"));
    
    if (n==NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
        if (val!=NULL){
            return object_genericsetattr(self, attr, val);;
        }
        n=object_genericgetattr_notype(self, str_new_fromstr("__delattr__"));
    
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
            return object_genericsetattr(self, attr, val);;
        }
    }
    else{
        args->type->slot_mappings->slot_append(args, val);
    }

    object* res=object_call_nokwargs(n, args);    
    return res;
}

object* newtp_descrget(object* obj, object* self, object* owner){    
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, obj);
    
    object* n;
    n=object_getattr(self, str_new_fromstr("__get__"));
    ERROR_RET(n);
    
    if (n==NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
        return NULL;
    }
    object* kwargs=new_dict();
    if (owner==NULL){
        dict_set(kwargs, str_new_fromstr("owner"), new_none());
    }
    else{
        dict_set(kwargs, str_new_fromstr("owner"), owner);
    }
    object* res=object_call(n, args, kwargs);
    return res;
}

object* newtp_descrset(object* obj, object* self, object* val){    
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, obj);
    
    object* n;
    n=object_getattr(self, str_new_fromstr("__set__"));
    ERROR_RET(n);
    
    if (n==NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
        
        n=object_getattr(self, str_new_fromstr("__delete__"));
        ERROR_RET(n);
    
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
            return NULL;
        }
    }
    else{
        args->type->slot_mappings->slot_append(args, val);
    }

    object* res=object_call_nokwargs(n, args);
    return res;
}

object* newtp_enter(object* self){    
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    
    object* n;
    n=object_getattr(self, str_new_fromstr("__enter__"));
    ERROR_RET(n);
    
    if (n==NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
        return NULL;
    }

    object* res=object_call_nokwargs(n, args);
    return res;
}

object* newtp_exit(object* self){    
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    
    object* n;
    n=object_getattr(self, str_new_fromstr("__exit__"));
    ERROR_RET(n);
    
    if (n==NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
        return NULL;
    }

    object* res=object_call_nokwargs(n, args);
    return res;
}


void newtp_post_tpcall(object* ob){
    (*(object**)((char*)ob + ob->type->dict_offset))=new_dict();
}