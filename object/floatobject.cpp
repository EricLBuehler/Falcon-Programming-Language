object* new_float_fromdouble(double v){
    object* obj=new_object(&FloatType);
    CAST_FLOAT(obj)->val=v;
    object* o = in_immutables(obj);
    if (o==NULL){
        return obj;
    }
    DECREF(obj);
    return o;
}

object* new_float_fromstr(string* v){
    object* obj=new_object(&FloatType);
    CAST_FLOAT(obj)->val=stod(v->c_str());
    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* new_float_fromstr(string v){
    object* obj=new_object(&FloatType);
    CAST_FLOAT(obj)->val=stod(v.c_str());
    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* float_float(object* self){
    return INCREF(self);
}


object* float_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (len==0){
        object* obj=new_object(CAST_TYPE(type));
        CAST_FLOAT(obj)->val=0;

        object* o = in_immutables((struct object*)obj);
        if (o==NULL){
            return (object*)obj;
        }
        DECREF((struct object*)obj);
        return o;
    }
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }
    
    object* val=list_index_int(args, 0);
    
    if (object_istype(val->type, CAST_TYPE(type))){
        return INCREF(val);
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

    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* float_pow(object* self, object* other){
    object* otherfloat=object_float(other);
    if (otherfloat==NULL || !object_istype(otherfloat->type, &FloatType)){
        return NULL;
    }
    double res=pow(CAST_FLOAT(self)->val,CAST_FLOAT(otherfloat)->val);
    DECREF(otherfloat);
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
    double res=fmod(CAST_FLOAT(self)->val,CAST_FLOAT(otherfloat)->val);
    DECREF(otherfloat);
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
    DECREF(otherfloat);
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
    DECREF(otherfloat);
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
    DECREF(otherfloat);
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
        DECREF(otherfloat);
        vm_add_err(&ZeroDivisionError, vm, "Divison by zero");
        return NULL;
    }
    double res=CAST_FLOAT(self)->val/CAST_FLOAT(otherfloat)->val;
    DECREF(otherfloat);
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
        DECREF(otherfloat);
        vm_add_err(&ZeroDivisionError, vm, "Divison by zero");
        return NULL;
    }
    double res=floor(CAST_FLOAT(self)->val/CAST_FLOAT(otherfloat)->val);
    DECREF(otherfloat);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* float_neg(object* self){
    return new_float_fromdouble(CAST_FLOAT(self)->val*-1);
}

object* float_repr(object* self){
    size_t size=to_string(CAST_FLOAT(self)->val).size();
    char buf[size];
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
            DECREF(otherfloat);
            return new_bool_true();
        }
        DECREF(otherfloat);
        return new_bool_false();
    }
    else if (type==CMP_GT){
        if (CAST_FLOAT(self)->val>CAST_FLOAT(otherfloat)->val){
            DECREF(otherfloat);
            return new_bool_true();
        }
        DECREF(otherfloat);
        return new_bool_false();
    }
    else if (type==CMP_GTE){
        if (CAST_FLOAT(self)->val>=CAST_FLOAT(otherfloat)->val){
            DECREF(otherfloat);
            return new_bool_true();
        }
        DECREF(otherfloat);
        return new_bool_false();
    }
    else if (type==CMP_LT){
        if (CAST_FLOAT(self)->val<CAST_FLOAT(otherfloat)->val){
            DECREF(otherfloat);
            return new_bool_true();
        }
        DECREF(otherfloat);
        return new_bool_false();
    }
    else if (type==CMP_LTE){
        if (CAST_FLOAT(self)->val<=CAST_FLOAT(otherfloat)->val){
            DECREF(otherfloat);
            return new_bool_true();
        }
        DECREF(otherfloat);
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
}

double round_double(double value, size_t prec){
  double pow_10 = pow(10.0d, (double)prec);
  return round(value * pow_10) / pow_10;
}