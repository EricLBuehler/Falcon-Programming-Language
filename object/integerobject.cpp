object* new_int_fromint(int v){
    object* obj=new_object(&IntType);
    ((IntObject*)obj)->val=new BigInt(v);
    object* o = in_immutables(obj);
    if (o==NULL){
        return obj;
    }
    FPLDECREF(obj);
    return o;
}

string trim(string s);

object* new_int_fromstr(string v){
    object* obj=new_object(&IntType);
    try{
        CAST_INT(obj)->val=new BigInt(trim(v));
    }
    catch (std::invalid_argument){
        if (::vm!=NULL){
            vm_add_err(&ValueError, vm, "Invalid literal '%s'", v.c_str());
            return NULL;
        }
    }
    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    FPLDECREF((struct object*)obj);
    return o;
}


object* new_int_fromstr(string* v){
    object* obj=new_object(&IntType);
    try{
        CAST_INT(obj)->val=new BigInt(trim(*v));
    }
    catch (std::invalid_argument){
        if (::vm!=NULL){
            vm_add_err(&ValueError, vm, "Invalid literal '%s'", v->c_str());
            return NULL;
        }
    }
    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    FPLDECREF((struct object*)obj);
    return o;
}

object* new_int_frombigint(BigInt* v){
    object* obj=new_object(&IntType);
    ((IntObject*)obj)->val=v;
    
    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    FPLDECREF((struct object*)obj);
    return o;
}

object* int_int(object* self){
    return FPLINCREF(self);
}

object* int_float(object* self){
    return new_float_fromdouble(stod(CAST_INT(self)->val->to_string()));
}

object* int_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (len==0){
        object* obj=new_object(CAST_TYPE(type));
        ((IntObject*)obj)->val=new BigInt(0);

        object* o = in_immutables((struct object*)obj);
        if (o==NULL){
            return (object*)obj;
        }
        FPLDECREF((struct object*)obj);
        return o;
    }
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }

    object* val=list_index_int(args, 0);
    
    if (object_istype(val->type, CAST_TYPE(type))){
        return FPLINCREF(val);
    }
    
    object* obj=object_int(val);
    if (obj==NULL && !object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Int argument must be str or a number, not '%s'",val->type->name->c_str());
        return NULL;
    }
    else if(obj==NULL || !object_istype(obj->type, &IntType)){
        vm_add_err(&ValueError, vm, "Could not convert string '%s' to int",object_cstr(val).c_str());
        return NULL;
    }

    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    FPLDECREF((struct object*)obj);
    return o;
}

object* int_pow(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        BigInt out=1;
        if (otherv>0){
            for (BigInt i=0; i<otherv; i++){
                out*=selfv;
            }
            return new_int_frombigint(new BigInt(out));
        }
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    double res=pow(selfv,otherval);
    FPLDECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_mod(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        return new_int_frombigint(new BigInt(selfv%otherv));
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    double res=fmod(selfv,otherval);
    FPLDECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_add(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        return new_int_frombigint(new BigInt(selfv+otherv));
    }
    
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv+otherval;
    FPLDECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_sub(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        return new_int_frombigint(new BigInt(selfv-otherv));
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv-otherval;
    FPLDECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_mul(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        return new_int_frombigint(new BigInt(selfv*otherv));
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv*otherval;
    FPLDECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_div(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        if (selfv%otherv==0){
            return new_int_frombigint(new BigInt(selfv/otherv));
        }
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    FPLDECREF(otherv);
    if (otherval==0){
        vm_add_err(&ZeroDivisionError, vm, "Divison by zero");
        return NULL;
    }
    double res=selfv/otherval;
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_fldiv(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        if (selfv%otherv==0){
            return new_int_frombigint(new BigInt(selfv/otherv));
        }
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    FPLDECREF(otherv);
    if (otherval==0){
        vm_add_err(&ZeroDivisionError, vm, "Divison by zero");
        return NULL;
    }
    double res=floor(selfv/otherval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_and(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &IntType)){
        return NULL;
    }
    object* res=new_int_fromint(CAST_INT(self)->val->to_int() & CAST_INT(otherv)->val->to_int());
    FPLDECREF(otherv);
    return res;
}

object* int_or(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &IntType)){
        return NULL;
    }
    object* res=new_int_fromint(CAST_INT(self)->val->to_int() | CAST_INT(otherv)->val->to_int());
    FPLDECREF(otherv);
    return res;
}

object* int_lshift(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &IntType)){
        return NULL;
    }
    
    int val=CAST_INT(otherv)->val->to_int();
    FPLDECREF(otherv);
    if (val<0){
        vm_add_err(&ValueError, vm, "Cannot left shift by negative number of bits");
        return NULL;
    }
    
    return new_int_fromint(CAST_INT(self)->val->to_int() << val);
}

object* int_rshift(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &IntType)){
        return NULL;
    }
    
    int val=CAST_INT(otherv)->val->to_int();
    FPLDECREF(otherv);
    if (val<0){
        vm_add_err(&ValueError, vm, "Cannot right shift by negative number of bits");
        return NULL;
    }
    
    return new_int_fromint(CAST_INT(self)->val->to_int() >> val);
}

object* int_neg(object* self){
    return new_int_frombigint(new BigInt((*CAST_INT(self)->val)*-1));
}

object* int_not(object* self){
    long val=CAST_INT(self)->val->to_long();
    return new_int_frombigint(new BigInt(~val));
}

object* int_repr(object* self){
    return str_new_fromstr(CAST_INT(self)->val->to_string());
}

object* int_cmp(object* self, object* other, uint8_t type){
    object* otherv=object_float(other);
    object* selfv=object_float(self);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    //Other type is int
    if (type==CMP_EQ){
        if (CAST_FLOAT(selfv)->val==CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }
    else if (type==CMP_NE){
        if (CAST_FLOAT(selfv)->val!=CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }
    else if (type==CMP_GT){
        if (CAST_FLOAT(selfv)->val>CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }
    else if (type==CMP_GTE){
        if (CAST_FLOAT(selfv)->val>=CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }
    else if (type==CMP_LT){
        if (CAST_FLOAT(selfv)->val<CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }
    else if (type==CMP_LTE){
        if (CAST_FLOAT(selfv)->val<=CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }

    return NULL;
}

object* int_bool(object* self){
    if ((*CAST_INT(self)->val)>0){
        return new_bool_true();
    }
    return new_bool_false();
}

void int_del(object* obj){
    delete ((IntObject*)obj)->val;
}