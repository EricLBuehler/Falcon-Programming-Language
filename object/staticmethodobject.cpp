object* staticmethod_new(object* type, object* args, object* kwargs){
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
    object* o=list_index_int(args, 0);
    FPLINCREF(o);
    CAST_STATICMETHOD(method)->function=o;
    return method;
}

void staticmethod_del(object* self){
    FPLDECREF(CAST_STATICMETHOD(self)->function);
}

object* staticmethod_repr(object* self){
    string s="<staticmethod ";
    s+=object_cstr(CAST_STATICMETHOD(self)->function);
    s+=">";
    return str_new_fromstr(s);
}

object* staticmethod_descrget(object* instance, object* self, object* owner){
    if (owner==NULL || object_istype(owner->type, &NoneType)){
        FPLINCREF(self);
        return self;
    }
    return method_new_impl(CAST_STATICMETHOD(self)->function, NULL);
}

object* staticmethod_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_STATICMETHOD(self)->function,CAST_STATICMETHOD(other)->function, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_STATICMETHOD(self)->function,CAST_STATICMETHOD(other)->function, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}