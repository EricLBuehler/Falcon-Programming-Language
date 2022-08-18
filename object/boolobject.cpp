object* bool_new(object* type, object* args, object* kwargs){
    object* obj=new_object(&BoolType);
    CAST_BOOL(obj)->val=true;
    return obj;
}

object* _new_bool_true(){
    object* obj=new_object(&BoolType);
    CAST_BOOL(obj)->val=1;
    return obj;
}

object* _new_bool_false(){
    object* obj=new_object(&BoolType);
    CAST_BOOL(obj)->val=0;
    return obj;
}

object* new_bool_true(){
    return INCREF(trueobj);
}

object* new_bool_false(){
    return INCREF(falseobj);
}

object* bool_add(object* self, object* other){
    if (other->type==&IntType){
        return new_int_frombigint(new BigInt(CAST_BOOL(self)->val+(*CAST_INT(other)->val)));
    }
    return NULL;
}

object* bool_sub(object* self, object* other){
    if (other->type==&IntType){
        return new_int_frombigint(new BigInt(CAST_BOOL(self)->val-(*CAST_INT(other)->val)));
    }
    return NULL;    
}

object* bool_mul(object* self, object* other){
    if (other->type==&IntType){
        return new_int_frombigint(new BigInt(CAST_BOOL(self)->val*(*CAST_INT(other)->val)));
    }
    return NULL;    
}

object* bool_div(object* self, object* other){
    if (other->type==&IntType){
        return new_int_frombigint(new BigInt(CAST_BOOL(self)->val/(*CAST_INT(other)->val)));
    }
    return NULL;    
}

object* bool_neg(object* self){
    return new_int_frombigint(new BigInt(CAST_BOOL(self)->val*-1));
}

object* bool_repr(object* self){
    if (CAST_BOOL(self)->val==1){
        return str_new_fromstr("True");
    }
    return str_new_fromstr("False");
}

object* bool_int(object* self){
    if (CAST_BOOL(self)->val==1){
        return new_float_fromdouble(1);
    }
    return new_float_fromdouble(0);
}

object* bool_float(object* self){
    if (CAST_BOOL(self)->val==1){
        return new_float_fromdouble(1);
    }
    return new_float_fromdouble(0);    
}

object* bool_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
    }
    if (type==CMP_EQ){
        if (CAST_BOOL(self)->val==CAST_BOOL(other)->val){
            return new_bool_true();
        }
    }
    return new_bool_false();
}

object* bool_bool(object* self){
    if (CAST_BOOL(self)->val==1){
        return new_bool_true();
    }
    return new_bool_false();
}

void bool_del(object* obj){}