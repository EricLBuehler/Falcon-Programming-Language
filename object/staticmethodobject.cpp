object* staticmethod_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }

    object* method=new_object(CAST_TYPE(type));
    CAST_STATICMETHOD(method)->function=FPLINCREF(list_index_int(args, 0));
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

object* staticmethod_descrget(object* instance, object* self){
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