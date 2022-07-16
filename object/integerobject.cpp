object* new_int_fromint(int v){
    object* obj=new_object(&IntType);
    ((IntObject*)obj)->val=new BigInt(v);
    object* o = in_immutables(obj);
    if (o==NULL){
        return obj;
    }
    DECREF(obj);
    return o;
}

object* new_int_fromstr(string* v){
    object* obj=new_object(&IntType);
    CAST_INT(obj)->val=new BigInt((*v));
    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* new_int_frombigint(BigInt* v){
    object* obj=new_object(&IntType);
    ((IntObject*)obj)->val=v;
    
    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* int_new(object* args, object* kwargs){
    object* obj=new_object(&IntType);
    object* val=args->type->slot_get(args, new_int_fromint(0));
    DECREF(args);
    DECREF(kwargs);
    if (!object_istype(val->type, &IntType) || !object_istype(val->type, &StrType)){
        DECREF(obj);
        return NULL;
    }
    if (object_istype(val->type, &IntType)){
        ((IntObject*)obj)->val=((IntObject*)val)->val;
    }
    if (object_istype(val->type, &StrType)){
        ((IntObject*)obj)->val=new BigInt((*((StrObject*)val)->val));
    }

    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* int_add(object* self, object* other){
    if (other->type==&IntType){
        return new_int_frombigint(new BigInt((*CAST_INT(self)->val)+(*CAST_INT(other)->val)));
    }
    return NULL;
}

object* int_sub(object* self, object* other){
    if (other->type==&IntType){
        return new_int_frombigint(new BigInt((*CAST_INT(self)->val)-(*CAST_INT(other)->val)));
    }
    return NULL;
}

object* int_mul(object* self, object* other){
    if (other->type==&IntType){
        return new_int_frombigint(new BigInt((*CAST_INT(self)->val)*(*CAST_INT(other)->val)));
    }
    return NULL;
}

object* int_div(object* self, object* other){
    if (other->type==&IntType){
        return new_int_frombigint(new BigInt((*CAST_INT(self)->val)/(*CAST_INT(other)->val)));
    }
    return NULL;
}

object* int_neg(object* self){
    return new_int_frombigint(new BigInt((*CAST_INT(self)->val)*-1));
}

object* int_repr(object* self){
    return str_new_fromstr(new string(CAST_INT(self)->val->to_string()));
}

object* int_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
    }
    if (type==CMP_EQ){
        if ((*CAST_INT(self)->val)==(*CAST_INT(other)->val)){
            return new_bool_true();
        }
    }
    return new_bool_false();
}

void int_del(object* obj){
    delete ((IntObject*)obj)->val;
}