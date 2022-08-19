object* str_new_fromstr(string val){
    object* obj=new_object(&StrType);
    
    ((StrObject*)obj)->val=new string(val);
    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    
    DECREF((object*)obj);
    return o;
}


object* str_int(object* self){
    BigInt* b;
    try{
        b=new BigInt(CAST_STRING(self)->val->c_str());
    }
    catch (std::invalid_argument){
        vm_add_err(&ValueError, vm, "Invalid literal %s", CAST_STRING(self)->val->c_str());
        return NULL;
    }
    return new_int_frombigint(b);
}

object* str_float(object* self){
    double d;
    try{
        d=stod(CAST_STRING(self)->val->c_str());
    }
    catch (std::invalid_argument){
        vm_add_err(&ValueError, vm, "Invalid literal %s", CAST_STRING(self)->val->c_str());
        return NULL;
    }
    return new_float_fromdouble(d);
}


object* str_new(object* type, object* args, object* kwargs){
    if (CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()==0){
        object* obj=new_object(&StrType);
        ((StrObject*)obj)->val=new string("");

        object* o = in_immutables((struct object*)obj);
        if (o==NULL){
            return (object*)obj;
        }
        DECREF((struct object*)obj);
        return o;
    }
    object* val=INCREF(args->type->slot_mappings->slot_get(args, new_int_fromint(0)));
    string s=object_cstr(val);

    object* obj=new_object(&StrType);
    ((StrObject*)obj)->val=new string(s);

    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* str_len(object* self){
    return new_int_fromint(CAST_STRING(self)->val->size());
}

object* str_repr(object* self){
    return str_new_fromstr("'"+replace_newlines((*CAST_STRING(self)->val))+"'");
}

object* str_str(object* self){
    return self; //str_new_fromstr(new string((*CAST_STRING(self)->val)));
}

object* str_bool(object* self){
    return new_bool_true();
}

object* str_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
    }
    if (type==CMP_EQ){
        if (CAST_STRING(self)->val->size()!=CAST_STRING(other)->val->size()){
            return new_bool_false();
        }
        if ((*CAST_STRING(self)->val)==(*CAST_STRING(other)->val)){
            return new_bool_true();
        }
    }
    return new_bool_false();
}

void str_del(object* obj){
    delete ((StrObject*)obj)->val;
}

object* str_wrapper_new(object* args, object* kwargs){
    if (CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()==0){
        object* obj=new_object(&StrType);
        ((StrObject*)obj)->val=new string("");

        object* o = in_immutables((struct object*)obj);
        if (o==NULL){
            return (object*)obj;
        }
        DECREF((struct object*)obj);
        return o;
    }
    object* val=INCREF(args->type->slot_mappings->slot_get(args, new_int_fromint(1)));
    string s=object_cstr(val);

    object* obj=new_object(&StrType);
    ((StrObject*)obj)->val=new string(s);

    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* str_wrapper_len(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    return self->type->slot_mappings->slot_len(self);
}

object* str_wrapper_repr(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    return self->type->slot_repr(self);
}

object* str_wrapper_str(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    return self->type->slot_str(self);
}

object* str_wrapper_bool(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    return self->type->slot_number->slot_bool(self);
}

object* str_wrapper_ne(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    object* other=args->type->slot_mappings->slot_get(args, new_int_fromint(1));

    if (self->type!=other->type){
        return new_bool_true();
    }
    if (CAST_STRING(self)->val->size()!=CAST_STRING(other)->val->size()){
        return new_bool_true();
    }
    if ((*CAST_STRING(self)->val)==(*CAST_STRING(other)->val)){
        return new_bool_false();
    }
    return new_bool_true();
}

object* str_wrapper_eq(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    object* other=args->type->slot_mappings->slot_get(args, new_int_fromint(1));

    if (self->type!=other->type){
        return new_bool_false();
    }
    if (CAST_STRING(self)->val->size()!=CAST_STRING(other)->val->size()){
        return new_bool_false();
    }
    if ((*CAST_STRING(self)->val)==(*CAST_STRING(other)->val)){
        return new_bool_true();
    }
    return new_bool_false();
}