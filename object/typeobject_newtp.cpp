object* newtp_init(object* self, object* args, object* kwargs){
    //Try to call __new__
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
    object* o=new_object((TypeObject*)self);
    (*(object**)((char*)o + o->type->dict_offset))=new_dict();

    //Setup dunder attributes
    object_setattr(o, str_new_fromstr("__class__"), self);

    //Try to call __new__
    object* n=object_getattr(o, str_new_fromstr("__new__"));
    
    object* args_=new_tuple();
    args_->type->slot_mappings->slot_append(args_, o);
    for (int i=0; i<CAST_LIST(args)->size; i++){
        args_->type->slot_mappings->slot_append(args_, list_index_int(args, i));
    }

    object* val=object_call(n, args_, kwargs);
    return val;
}
void newtp_del(object* self){
    object* n=object_getattr(self, str_new_fromstr("__del__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object_call(n, args, new_dict());
}
object* newtp_next(object* self){
    object* n=object_getattr(self, str_new_fromstr("__next__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}
object* newtp_get(object* self, object* idx){
    object* n=object_getattr(self, str_new_fromstr("__get__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, idx);
    object* val=object_call(n, args, new_dict());
    return val;
}
object* newtp_len(object* self){
    object* n=object_getattr(self, str_new_fromstr("__len__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}

object* newtp_set(object* self, object* idx, object* val){
    object* n=object_getattr(self, str_new_fromstr("__set__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    args->type->slot_mappings->slot_append(args, idx);
    args->type->slot_mappings->slot_append(args, val);
    object* ret=object_call(n, args, new_dict());
    return ret;
}
object* newtp_repr(object* self){
    object* n=object_getattr(self, str_new_fromstr("__repr__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}
object* newtp_str(object* self){
    object* n=object_getattr(self, str_new_fromstr("__str__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
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
            object* val=object_call(n, args, new_dict());
            return val;
        }
    }

    if (type==CMP_NE){
        object* n=object_getattr(self, str_new_fromstr("__ne__"));
        if (n!=NULL){
            object* args=new_tuple();
            args->type->slot_mappings->slot_append(args, self);
            object* val=object_call(n, args, new_dict());
            return val;
        }
    }

    if (type==CMP_LT){
        object* n=object_getattr(self, str_new_fromstr("__lt__"));
        if (n!=NULL){
            object* args=new_tuple();
            args->type->slot_mappings->slot_append(args, self);
            object* val=object_call(n, args, new_dict());
            return val;
        }
        return NULL;
    }

    if (type==CMP_GT){
        object* n=object_getattr(self, str_new_fromstr("__gt__"));
        if (n!=NULL){
            object* args=new_tuple();
            args->type->slot_mappings->slot_append(args, self);
            object* val=object_call(n, args, new_dict());
            return val;
        }
    }

    if (type==CMP_LTE){
        object* n=object_getattr(self, str_new_fromstr("__lte__"));
        if (n!=NULL){
            object* args=new_tuple();
            args->type->slot_mappings->slot_append(args, self);
            object* val=object_call(n, args, new_dict());
            return val;
        }
    }

    if (type==CMP_GTE){
        object* n=object_getattr(self, str_new_fromstr("__gte__"));
        if (n!=NULL){
            object* args=new_tuple();
            args->type->slot_mappings->slot_append(args, self);
            object* val=object_call(n, args, new_dict());
            return val;
        }
    }
    return NULL;
}

object* newtp_pow(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__pow__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}
object* newtp_mod(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__mod__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}
object* newtp_add(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__add__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}
object* newtp_sub(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__sub__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}
object* newtp_mul(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__mul__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}
object* newtp_div(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr("__div__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}

object* newtp_neg(object* self){
    object* n=object_getattr(self, str_new_fromstr("__neg__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}

object* newtp_bool(object* self){
    object* n=object_getattr(self, str_new_fromstr("__bool__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}

object* newtp_int(object* self){
    object* n=object_getattr(self, str_new_fromstr("__int__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}

object* newtp_float(object* self){
    object* n=object_getattr(self, str_new_fromstr("__float__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}

object* newtp_iter(object* self){
    object* n=object_getattr(self, str_new_fromstr("__iter__"));
    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, self);
    object* val=object_call(n, args, new_dict());
    return val;
}

void newtp_post_tpcall(object* ob){
    (*(object**)((char*)ob + ob->type->dict_offset))=new_dict();
}