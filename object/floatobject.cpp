object* new_float_fromdouble(double v){
    if(float_map.find(v)!=float_map.end()){
        FPLINCREF(float_map[v]);
        return float_map[v];
    }
    
    object* obj=new_object(&FloatType);
    CAST_FLOAT(obj)->val=v;

    float_map[v]=obj;
    
    return obj;
}

object* new_float_fromstr(string* v){
    object* obj=new_object(&FloatType);
    CAST_FLOAT(obj)->val=stod(v->c_str());

    if(float_map.find(CAST_FLOAT(obj)->val)!=float_map.end()){
        FPLINCREF(float_map[CAST_FLOAT(obj)->val]);
        return float_map[CAST_FLOAT(obj)->val];
    }

    float_map[CAST_FLOAT(obj)->val]=obj;
    return obj;
}

object* new_float_fromstr(string v){
    object* obj=new_object(&FloatType);
    CAST_FLOAT(obj)->val=stod(v.c_str());
    if(float_map.find(CAST_FLOAT(obj)->val)!=float_map.end()){
        FPLINCREF(float_map[CAST_FLOAT(obj)->val]);
        return float_map[CAST_FLOAT(obj)->val];
    }

    float_map[CAST_FLOAT(obj)->val]=obj;
    return obj;
}

object* float_float(object* self){
    FPLINCREF(self);
    return self;
}

object* float_int(object* self){
    return new_int_fromint((int)(CAST_FLOAT(self)->val));
}

object* float_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    if (len==0){
        if(float_map.find(0)!=float_map.end()){
            FPLINCREF(float_map[0]);
            return float_map[0];
        }

        object* obj=new_object(CAST_TYPE(type));
        CAST_FLOAT(obj)->val=0;        

        float_map[0]=obj;
        return obj;
    }
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }
    
    object* val=list_index_int(args, 0);
    
    if (object_istype(val->type, CAST_TYPE(type))){
        FPLINCREF(val);
        return val;
    }
    
    object* obj=object_float(val);
    if (obj==NULL && !object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Float argument must be str or a number, not '%s'",val->type->name->c_str());
        return NULL;
    }
    else if(obj==NULL || !object_istype(obj->type, &FloatType)){
        vm_add_err(&ValueError, vm, "Could not convert string '%s' to float",object_cstr(val).c_str());
        return NULL;
    }

    if(float_map.find(CAST_FLOAT(obj)->val)!=float_map.end()){
        FPLINCREF(float_map[CAST_FLOAT(obj)->val]);
        return float_map[CAST_FLOAT(obj)->val];
    }

    float_map[CAST_FLOAT(obj)->val]=obj;
    return obj;
}

object* float_pow(object* self, object* other){
    object* otherfloat=object_float(other);
    if (otherfloat==NULL || !object_istype(otherfloat->type, &FloatType)){
        return NULL;
    }
    double res=pow(CAST_FLOAT(self)->val,CAST_FLOAT(otherfloat)->val);
    FPLDECREF(otherfloat);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* float_mod(object* self, object* other){
    object* otherfloat=object_float(other);
    if (otherfloat==NULL || !object_istype(otherfloat->type, &FloatType)){
        return NULL;
    }
    if (CAST_FLOAT(otherfloat)->val==0){
        vm_add_err(&ZeroDivisionError, vm, "Modulus by zero");
        FPLDECREF(otherfloat);
        return NULL;
    }
    double res=fmod(CAST_FLOAT(self)->val,CAST_FLOAT(otherfloat)->val);
    FPLDECREF(otherfloat);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* float_add(object* self, object* other){
    object* otherfloat=object_float(other);
    if (otherfloat==NULL || !object_istype(otherfloat->type, &FloatType)){
        return NULL;
    }
    double res=CAST_FLOAT(self)->val+CAST_FLOAT(otherfloat)->val;
    FPLDECREF(otherfloat);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* float_sub(object* self, object* other){
    object* otherfloat=object_float(other);
    if (otherfloat==NULL || !object_istype(otherfloat->type, &FloatType)){
        return NULL;
    }
    double res=CAST_FLOAT(self)->val-CAST_FLOAT(otherfloat)->val;
    FPLDECREF(otherfloat);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* float_mul(object* self, object* other){
    object* otherfloat=object_float(other);
    if (otherfloat==NULL || !object_istype(otherfloat->type, &FloatType)){
        return NULL;
    }
    double res=CAST_FLOAT(self)->val*CAST_FLOAT(otherfloat)->val;
    FPLDECREF(otherfloat);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* float_div(object* self, object* other){
    object* otherfloat=object_float(other);
    if (otherfloat==NULL || !object_istype(otherfloat->type, &FloatType)){
        return NULL;
    }
    if (CAST_FLOAT(otherfloat)->val==0){
        FPLDECREF(otherfloat);
        vm_add_err(&ZeroDivisionError, vm, "Divison by zero");
        return NULL;
    }
    double res=CAST_FLOAT(self)->val/CAST_FLOAT(otherfloat)->val;
    FPLDECREF(otherfloat);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* float_fldiv(object* self, object* other){
    object* otherfloat=object_float(other);
    if (otherfloat==NULL || !object_istype(otherfloat->type, &FloatType)){
        return NULL;
    }
    if (CAST_FLOAT(otherfloat)->val==0){
        FPLDECREF(otherfloat);
        vm_add_err(&ZeroDivisionError, vm, "Divison by zero");
        return NULL;
    }
    double res=floor(CAST_FLOAT(self)->val/CAST_FLOAT(otherfloat)->val);
    FPLDECREF(otherfloat);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* float_abs(object* self){
    if (CAST_FLOAT(self)->val<0){
        return new_float_fromdouble(CAST_FLOAT(self)->val*-1);
    }
    FPLINCREF(self);
    return self;
}

object* float_neg(object* self){
    return new_float_fromdouble(CAST_FLOAT(self)->val*-1);
}

object* float_repr(object* self){
    char buf[128];
    
    sprintf(buf, "%.17lg", CAST_FLOAT(self)->val);
    string s(buf);
    int count = std::count(s.begin(), s.end(), '.');
    if (count==0){
        s+=".0";
    }
    return str_new_fromstr(s);
}

object* float_cmp(object* self, object* other, uint8_t type){
    object* otherfloat=object_float(other);
    if (otherfloat==NULL || !object_istype(otherfloat->type, &FloatType)){
        return NULL;
    }
    
    if (type==CMP_EQ){
        if (CAST_FLOAT(self)->val==CAST_FLOAT(otherfloat)->val){
            FPLDECREF(otherfloat);
            return new_bool_true();
        }
        FPLDECREF(otherfloat);
        return new_bool_false();
    }
    else if (type==CMP_NE){
        if (CAST_FLOAT(self)->val!=CAST_FLOAT(otherfloat)->val){
            FPLDECREF(otherfloat);
            return new_bool_true();
        }
        FPLDECREF(otherfloat);
        return new_bool_false();
    }
    else if (type==CMP_GT){
        if (CAST_FLOAT(self)->val>CAST_FLOAT(otherfloat)->val){
            FPLDECREF(otherfloat);
            return new_bool_true();
        }
        FPLDECREF(otherfloat);
        return new_bool_false();
    }
    else if (type==CMP_GTE){
        if (CAST_FLOAT(self)->val>=CAST_FLOAT(otherfloat)->val){
            FPLDECREF(otherfloat);
            return new_bool_true();
        }
        FPLDECREF(otherfloat);
        return new_bool_false();
    }
    else if (type==CMP_LT){
        if (CAST_FLOAT(self)->val<CAST_FLOAT(otherfloat)->val){
            FPLDECREF(otherfloat);
            return new_bool_true();
        }
        FPLDECREF(otherfloat);
        return new_bool_false();
    }
    else if (type==CMP_LTE){
        if (CAST_FLOAT(self)->val<=CAST_FLOAT(otherfloat)->val){
            FPLDECREF(otherfloat);
            return new_bool_true();
        }
        FPLDECREF(otherfloat);
        return new_bool_false();
    }

    return NULL;
}

object* float_bool(object* self){
    if (CAST_FLOAT(self)->val>0){
        return new_bool_true();
    }
    return new_bool_false();
}

void float_del(object* obj){
    float_map.erase(CAST_FLOAT(obj)->val);
}

double round_double(double value, size_t prec){
    double pow_10 = pow(10.0d, (double)prec);
    return round(value * pow_10) / pow_10;
}

object* float_round(object* self, object* prec){
    object* prec_=object_int(prec);
    if (prec_==NULL || !object_istype(prec_->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", prec_->type->name->c_str());
        return NULL; 
    }
    if (*CAST_INT(prec_)->val>LONG_MAX || *CAST_INT(prec_)->val<LONG_MIN || *CAST_INT(prec_)->val<0){
        vm_add_err(&OverflowError, vm, "Value out of range of C long");
        return NULL;
    }

    object* o=new_float_fromdouble(round_double(CAST_FLOAT(self)->val, CAST_INT(prec_)->val->to_long()));
    FPLDECREF(prec_);

    return o;
}