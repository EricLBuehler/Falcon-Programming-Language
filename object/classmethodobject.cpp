object* classmethod_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }

    object* method=new_object(CAST_TYPE(type));
    object* func=list_index_int(args, 0);
    FPLINCREF(list_index_int(args, 0));
    CAST_CLASSMETHOD(method)->function=func;
    return method;
}

void classmethod_del(object* self){
    FPLDECREF(CAST_CLASSMETHOD(self)->function);
}

object* classmethod_repr(object* self){
    string s="<classmethod ";
    s+=object_cstr(CAST_CLASSMETHOD(self)->function);
    s+=">";
    return str_new_fromstr(s);
}

object* classmethod_descrget(object* instance, object* self, object* owner){
    if (owner==NULL || object_istype(owner->type, &NoneType)){
        FPLINCREF(self);
        return self;
    }
    return method_new_impl(CAST_STATICMETHOD(self)->function, (object*)instance->type);
}

object* classmethod_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_CLASSMETHOD(self)->function,CAST_CLASSMETHOD(other)->function, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_CLASSMETHOD(self)->function,CAST_CLASSMETHOD(other)->function, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}