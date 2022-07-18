
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

object* str_new(object* args, object* kwargs){
    object* val=args->type->slot_get(args, new_int_fromint(0));
    DECREF(args);
    DECREF(kwargs);
    if (!object_istype(val->type, &IntType)){
        return NULL;
    }

    string* str=new string(((IntObject*)val)->val->to_string());

    object_var* obj=new_object_var(&StrType, str->size());
    ((StrObject*)obj)->val=str;
    ((StrObject*)obj)->var_size=str->size()+sizeof(StrObject);

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