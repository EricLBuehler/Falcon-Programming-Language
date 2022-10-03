object* bool_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",len);
        return NULL;
    }
    object* boolv=list_index_int(args,0)->type->slot_number->slot_bool(list_index_int(args,0));
    if (boolv==NULL){
        return NULL;
    }
    object* o=new_object(CAST_TYPE(type));
    CAST_BOOL(o)->val=CAST_BOOL(boolv)->val;
    DECREF(boolv);
    return o;
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

object* bool_pow(object* self, object* other){
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    double selfv=(double)CAST_BOOL(self)->val;
    double otherval=CAST_FLOAT(otherv)->val;
    double res=pow(selfv, otherval);
    DECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* bool_mod(object* self, object* other){
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    double selfv=(double)CAST_BOOL(self)->val;
    double otherval=CAST_FLOAT(otherv)->val;
    double res=fmod(selfv, otherval);
    DECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* bool_add(object* self, object* other){
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    double selfv=(double)CAST_BOOL(self)->val;
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv+otherval;
    DECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* bool_sub(object* self, object* other){
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    double selfv=(double)CAST_BOOL(self)->val;
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv-otherval;
    DECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);   
}

object* bool_mul(object* self, object* other){
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    double selfv=(double)CAST_BOOL(self)->val;
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv*otherval;
    DECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);      
}

object* bool_div(object* self, object* other){
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    double selfv=(double)CAST_BOOL(self)->val;
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv/otherval;
    DECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);   
}

object* bool_and(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* res=new_int_fromint(CAST_BOOL(self)->val & CAST_INT(otherv)->val->to_int());
    DECREF(otherv);
    return res;
}

object* bool_or(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* res=new_int_fromint(CAST_BOOL(self)->val | CAST_INT(otherv)->val->to_int());
    DECREF(otherv);
    return res;
}

object* bool_lshift(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    
    int val=CAST_INT(otherv)->val->to_int();
    DECREF(otherv);
    if (val<0){
        vm_add_err(&ValueError, vm, "Cannot left shift by negative number of bits");
        return NULL;
    }
    
    return new_int_fromint(CAST_BOOL(self)->val << val);
}

object* bool_rshift(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    int val=CAST_INT(otherv)->val->to_int();
    DECREF(otherv);
    if (val<0){
        vm_add_err(&ValueError, vm, "Cannot left shift by negative number of bits");
        return NULL;
    }
    
    return new_int_fromint(CAST_BOOL(self)->val >> val);
}

object* bool_neg(object* self){
    return new_int_frombigint(new BigInt(CAST_BOOL(self)->val*-1));
}

object* bool_not(object* self){
    return new_float_fromdouble(~CAST_BOOL(self)->val);
}

object* bool_repr(object* self){
    if (CAST_BOOL(self)->val==1){
        return str_new_fromstr("True");
    }
    return str_new_fromstr("False");
}

object* bool_int(object* self){
    if (CAST_BOOL(self)->val==1){
        return new_int_fromint(1);
    }
    return new_int_fromint(0);
}

object* bool_float(object* self){
    if (CAST_BOOL(self)->val==1){
        return new_float_fromdouble(1);
    }
    return new_float_fromdouble(0);    
}

object* bool_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_BOOL(self)->val==CAST_BOOL(other)->val){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (CAST_BOOL(self)->val!=CAST_BOOL(other)->val){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

object* bool_bool(object* self){
    if (CAST_BOOL(self)->val==1){
        return new_bool_true();
    }
    return new_bool_false();
}

void bool_del(object* obj){}