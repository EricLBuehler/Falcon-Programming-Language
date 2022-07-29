
object* str_new_fromstr(string* val){
    object_var* obj=new_object_var(&StrType, val->size());

    ((StrObject*)obj)->val=new string((*val));
    ((StrObject*)obj)->var_size=val->size()+sizeof(StrObject);
    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    
    DECREF((object*)obj);
    return o;
}

object* str_new(object* type, object* args, object* kwargs){
    if (CAST_INT(args->type->slot_len(args))->val->to_int()==0){
        object_var* obj=new_object_var(&StrType, 0);
        ((StrObject*)obj)->val=new string("");

        object* o = in_immutables((struct object*)obj);
        if (o==NULL){
            return (object*)obj;
        }
        DECREF((struct object*)obj);
        return o;
    }
    object* val=INCREF(args->type->slot_get(args, new_int_fromint(0)));
    DECREF(args);
    DECREF(kwargs);
    if (!object_istype(val->type, &IntType) && !object_istype(val->type, &StrType)){
        DECREF(val);
        vm_add_err(ValueError, vm, "Expected argument to be int or str, got type '%s'",args->type->slot_get(args, new_int_fromint(0))->type->name->c_str());
        return NULL;
    }

    string s=object_cstr(val);

    object_var* obj=new_object_var(&StrType, s.size());
    ((StrObject*)obj)->val=new string(s);
    ((StrObject*)obj)->var_size=s.size()+sizeof(StrObject);

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
    return str_new_fromstr(new string("'"+replace_newlines((*CAST_STRING(self)->val))+"'"));
}

object* str_str(object* self){
    return str_new_fromstr(CAST_STRING(self)->val);
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