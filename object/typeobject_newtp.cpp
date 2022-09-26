object* newtp_init(object* self, object* args, object* kwargs){
    //Try to call __init__
    object* n=object_getattr(self, str_new_fromstr("__init__"));

    object* args_=new_tuple();
    args_->type->slot_mappings->slot_append(args_, self);
    for (int i=0; i<CAST_LIST(args)->size; i++){
        args_->type->slot_mappings->slot_append(args_, list_index_int(args, i));
    }
    
    object* val=object_call(n, args_, kwargs);
    return val;
}
object* newtp_new(object* self, object* args, object* kwargs){
    //Try to call __new__
    object* n=object_getattr(self, str_new_fromstr("__new__"));
    
    object* args_=new_tuple();
    args_->type->slot_mappings->slot_append(args_, self);
    for (int i=0; i<CAST_LIST(args)->size; i++){
        args_->type->slot_mappings->slot_append(args_, list_index_int(args, i));
    }
    object* val=object_call(n, args_, kwargs);

    if (val!=NULL && object_istype(val->type, &TypeType)){
        return object_new(val, args_, kwargs);
    }
    return val;
}

void newtp_del(object* self){
    object* n=object_getattr(self, str_new_fromstr("__del__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object_call_nokwargs(n, args);
}
object* newtp_next(object* self){
    object* n=object_getattr(self, str_new_fromstr("__next__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_get(object* self, object* idx){
    object* n=object_getattr(self, str_new_fromstr("__getitem__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, idx);
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_len(object* self){
    object* n=object_getattr(self, str_new_fromstr("__len__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_set(object* self, object* idx, object* val){
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, idx);
    
    object* n=object_getattr(self, str_new_fromstr("__setitem__"));
    if (n==NULL){
        DECREF(vm->exception);
        vm->exception=NULL;
        n=object_getattr(self, str_new_fromstr("__delitem__"));
    }
    else{
        args->type->slot_mappings->slot_append(args, val);
    }
    
    return object_call_nokwargs(n, args);
}

object* newtp_repr(object* self){
    object* n=object_getattr(self, str_new_fromstr("__repr__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_str(object* self){
    object* n=object_getattr(self, str_new_fromstr("__str__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_call(object* self, object* args, object* kwargs){
    //Try to call __call__
    object* function=object_getattr(self, str_new_fromstr("__call__"));
    uint32_t argc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->operator+((*CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val)).to_int()+1;
    uint32_t posargc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+1;
    uint32_t kwargc=argc-posargc;     

    if (function->type->slot_call==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
        return NULL;
    }
    
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(function, args, kwargs);
    return val;
}

object* newtp_cmp(object* self, object* other, uint8_t type){
    if (type==CMP_EQ){
        object* n=object_getattr(self, str_new_fromstr("__eq__"));
        if (n!=NULL){
            object* args=new_tuple();
            args->type->slot_mappings->slot_append(args, self);
            object* val=object_call_nokwargs(n, args);
            return val;
        }
    }

    if (type==CMP_NE){
        object* n=object_getattr(self, str_new_fromstr("__ne__"));
        if (n!=NULL){
            object* args=new_tuple();
            args->type->slot_mappings->slot_append(args, self);
            object* val=object_call_nokwargs(n, args);
            return val;
        }
    }

    if (type==CMP_LT){
        object* n=object_getattr(self, str_new_fromstr("__lt__"));
        if (n!=NULL){
            object* args=new_tuple();
            args->type->slot_mappings->slot_append(args, self);
            object* val=object_call_nokwargs(n, args);
            return val;
        }
        return NULL;
    }

    if (type==CMP_GT){
        object* n=object_getattr(self, str_new_fromstr("__gt__"));
        if (n!=NULL){
            object* args=new_tuple();
            args->type->slot_mappings->slot_append(args, self);
            object* val=object_call_nokwargs(n, args);
            return val;
        }
    }

    if (type==CMP_LTE){
        object* n=object_getattr(self, str_new_fromstr("__lte__"));
        if (n!=NULL){
            object* args=new_tuple();
            args->type->slot_mappings->slot_append(args, self);
            object* val=object_call_nokwargs(n, args);
            return val;
        }
    }

    if (type==CMP_GTE){
        object* n=object_getattr(self, str_new_fromstr("__gte__"));
        if (n!=NULL){
            object* args=new_tuple();
            args->type->slot_mappings->slot_append(args, self);
            object* val=object_call_nokwargs(n, args);
            return val;
        }
    }
    return NULL;
}

object* newtp_pow(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__pow__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_mod(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__mod__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_add(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__add__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_sub(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__sub__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_mul(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__mul__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    return val;
}
object* newtp_div(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__div__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_and(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__and__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_or(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__or__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_lshift(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__lshift__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_rshift(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__rshift__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, other);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_neg(object* self){
    object* n=object_getattr(self, str_new_fromstr("__neg__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_not(object* self){
    object* n=object_getattr(self, str_new_fromstr("__not__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_bool(object* self){
    object* n=object_getattr(self, str_new_fromstr("__bool__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_int(object* self){
    object* n=object_getattr(self, str_new_fromstr("__int__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_float(object* self){
    object* n=object_getattr(self, str_new_fromstr("__float__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_iter(object* self){
    object* n=object_getattr(self, str_new_fromstr("__iter__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call_nokwargs(n, args);
    return val;
}

object* newtp_getattr(object* self, object* attr){
    object* n=object_genericgetattr_notype(self, str_new_fromstr("__getattr__"));
    if (n!=NULL){
        object* args=new_tuple();
        args->type->slot_mappings->slot_append(args, self);
        args->type->slot_mappings->slot_append(args, attr);
        object* val=object_call_nokwargs(n, args);
        return val;
    }
    else{
        DECREF(vm->exception);
        vm->exception=NULL;
        object* n=object_genericgetattr(self, attr);
        if (n!=NULL && object_istype(n->type, &FuncType)){
            //Create bound
            return method_new_impl(n, self);
        }
        return n;
    }
}

void newtp_setattr(object* self, object* attr, object* val){
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, attr);
    
    object* n;
    n=object_genericgetattr_notype(self, str_new_fromstr("__setattr__"));
    if (n==NULL){
        DECREF(vm->exception);
        vm->exception=NULL;
        if (val!=NULL){
            object_genericsetattr(self, attr, val);
            return;
        }
        n=object_genericgetattr_notype(self, str_new_fromstr("__delattr__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
            object_genericsetattr(self, attr, val);
            return;
        }
    }
    else{
        args->type->slot_mappings->slot_append(args, val);
    }

    object* res=object_call_nokwargs(n, args);
    DECREF(res);
    
    return;
}

void newtp_post_tpcall(object* ob){
    (*(object**)((char*)ob + ob->type->dict_offset))=new_dict();
}