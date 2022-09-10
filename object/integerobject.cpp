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

object* new_int_fromstr(string v){
    object* obj=new_object(&IntType);
    try{    
        CAST_INT(obj)->val=new BigInt(v);
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
    DECREF((struct object*)obj);
    return o;
}


object* new_int_fromstr(string* v){
    object* obj=new_object(&IntType);
    try{    
        CAST_INT(obj)->val=new BigInt(*v);
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

object* int_int(object* self){
    return self;
}

object* int_float(object* self){
    return new_float_fromdouble((double)CAST_INT(self)->val->to_int());
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
        DECREF((struct object*)obj);
        return o;
    }
    if (len!=1 || CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()==0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }

    object* val=list_index_int(args, 0);
    
    if (object_istype(val->type, CAST_TYPE(type))){
        return INCREF(val);
    }
    
    object* obj=object_int(val);
    if (obj==NULL){
        vm_add_err(&ValueError, vm, "Invalid literal '%s'", object_cstr(val));
        return NULL;
    }

    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* int_pow(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        double selfv =(double)CAST_INT(self)->val->to_int();
        double otherv=(double)CAST_INT(other)->val->to_int();
        double res=pow(selfv,otherv);
        int ires=(int)res;
        if (res-ires==0){
            return new_int_fromint(ires);
        }
        return new_float_fromdouble(res);
    }
    object* otherv=object_float(other);
    if (otherv==NULL){
        return NULL;
    }
    double selfv =(double)CAST_INT(self)->val->to_int();
    double otherval=CAST_FLOAT(otherv)->val;
    double res=pow(selfv,otherval);
    DECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_mod(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        double selfv =(double)CAST_INT(self)->val->to_int();
        double otherv=(double)CAST_INT(other)->val->to_int();
        double res=fmod(selfv,otherv);
        int ires=(int)res;
        if (res-ires==0){
            return new_int_fromint(ires);
        }
        return new_float_fromdouble(res);
    }
    object* otherv=object_float(other);
    if (otherv==NULL){
        return NULL;
    }
    double selfv =(double)CAST_INT(self)->val->to_int();
    double otherval=CAST_FLOAT(otherv)->val;
    double res=fmod(selfv,otherval);
    DECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_add(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        double selfv =(double)CAST_INT(self)->val->to_int();
        double otherv=(double)CAST_INT(other)->val->to_int();
        double res=selfv+otherv;
        int ires=(int)res;
        if (res-ires==0){
            return new_int_fromint(ires);
        }
        return new_float_fromdouble(res);
    }
    
    object* otherv=object_float(other);
    if (otherv==NULL){
        return NULL;
    }
    double selfv =(double)CAST_INT(self)->val->to_int();
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv+otherval;
    DECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_sub(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        double selfv =(double)CAST_INT(self)->val->to_int();
        double otherv=(double)CAST_INT(other)->val->to_int();
        double res=selfv-otherv;
        int ires=(int)res;
        if (res-ires==0){
            return new_int_fromint(ires);
        }
        return new_float_fromdouble(res);
    }
    object* otherv=object_float(other);
    if (otherv==NULL){
        return NULL;
    }
    double selfv =(double)CAST_INT(self)->val->to_int();
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv-otherval;
    DECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_mul(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        double selfv =(double)CAST_INT(self)->val->to_int();
        double otherv=(double)CAST_INT(other)->val->to_int();
        double res=selfv*otherv;
        int ires=(int)res;
        if (res-ires==0){
            return new_int_fromint(ires);
        }
        return new_float_fromdouble(res);
    }
    object* otherv=object_float(other);
    if (otherv==NULL){
        return NULL;
    }
    double selfv =(double)CAST_INT(self)->val->to_int();
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv*otherval;
    DECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_div(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        double selfv =(double)CAST_INT(self)->val->to_int();
        double otherv=(double)CAST_INT(other)->val->to_int();
        double res=selfv/otherv;
        int ires=(int)res;
        if (res-ires==0){
            return new_int_fromint(ires);
        }
        return new_float_fromdouble(res);
    }
    object* otherv=object_float(other);
    if (otherv==NULL){
        return NULL;
    }
    double selfv =(double)CAST_INT(self)->val->to_int();
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv/otherval;
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_neg(object* self){
    return new_int_frombigint(new BigInt((*CAST_INT(self)->val)*-1));
}

object* int_repr(object* self){
    return str_new_fromstr(CAST_INT(self)->val->to_string());
}

object* int_cmp(object* self, object* other, uint8_t type){
    object* otherint=object_int(other);
    if (otherint==NULL){
        return NULL;
    }
    //Other type is int
    if (type==CMP_EQ){
        if (*CAST_INT(self)->val==*CAST_INT(otherint)->val){
            return new_bool_true();
        }
        return new_bool_false();
    }
    else if (type==CMP_GT){
        if (*CAST_INT(self)->val>*CAST_INT(otherint)->val){
            return new_bool_true();
        }
        return new_bool_false();
    }
    else if (type==CMP_GTE){
        if (*CAST_INT(self)->val>=*CAST_INT(otherint)->val){
            return new_bool_true();
        }
        return new_bool_false();
    }
    else if (type==CMP_LT){
        if (*CAST_INT(self)->val<*CAST_INT(otherint)->val){
            return new_bool_true();
        }
        return new_bool_false();
    }
    else if (type==CMP_LTE){
        if (*CAST_INT(self)->val<=*CAST_INT(otherint)->val){
            return new_bool_true();
        }
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
