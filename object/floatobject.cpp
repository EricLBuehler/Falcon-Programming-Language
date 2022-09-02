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


object* float_int(object* self){
    char buf[to_string(round(CAST_FLOAT(self)->val)).size()];
    sprintf(buf, "%g", round(CAST_FLOAT(self)->val));
    string s(buf);
    return new_int_fromint(round(CAST_FLOAT(self)->val));
}

object* float_float(object* self){
    return self;
}


object* float_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
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
    
    object* val=list_index_int(args, 0);
    
    if (object_istype(val->type, CAST_TYPE(type))){
        return INCREF(val);
    }
    
    object* obj=object_float(val);

    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* float_pow(object* self, object* other){
    object* otherfloat=object_float(other);
    if (otherfloat==NULL){
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
    if (otherfloat==NULL){
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
    if (otherfloat==NULL){
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
    if (otherfloat==NULL){
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
    if (otherfloat==NULL){
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
    if (otherfloat==NULL){
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

object* float_neg(object* self){
    return new_float_fromdouble(CAST_FLOAT(self)->val*-1);
}

object* float_repr(object* self){
    char buf[to_string(CAST_FLOAT(self)->val).size()];
    sprintf(buf, "%.17g", CAST_FLOAT(self)->val);
    return str_new_fromstr(string(buf));
}

object* float_cmp(object* self, object* other, uint8_t type){
    object* otherfloat=object_float(other);
    if (otherfloat==NULL){
        return NULL;
    }
    
    if (type==CMP_EQ){
        if (CAST_FLOAT(self)->val==CAST_FLOAT(otherfloat)->val){
            return new_bool_true();
        }
        return new_bool_false();
    }
    else if (type==CMP_GT){
        if (CAST_FLOAT(self)->val>CAST_FLOAT(otherfloat)->val){
            return new_bool_true();
        }
        return new_bool_false();
    }
    else if (type==CMP_GTE){
        if (CAST_FLOAT(self)->val>=CAST_FLOAT(otherfloat)->val){
            return new_bool_true();
        }
        return new_bool_false();
    }
    else if (type==CMP_LT){
        if (CAST_FLOAT(self)->val<CAST_FLOAT(otherfloat)->val){
            return new_bool_true();
        }
        return new_bool_false();
    }
    else if (type==CMP_LTE){
        if (CAST_FLOAT(self)->val<=CAST_FLOAT(otherfloat)->val){
            return new_bool_true();
        }
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




object* float_wrapper_add(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    return self->type->slot_number->slot_add(self, args->type->slot_mappings->slot_get(args, new_int_fromint(1)));
}

object* float_wrapper_sub(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    return self->type->slot_number->slot_sub(self, args->type->slot_mappings->slot_get(args, new_int_fromint(1)));
}

object* float_wrapper_mul(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    return self->type->slot_number->slot_mul(self, args->type->slot_mappings->slot_get(args, new_int_fromint(1)));
}

object* float_wrapper_div(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    return self->type->slot_number->slot_div(self, args->type->slot_mappings->slot_get(args, new_int_fromint(1)));
}

object* float_wrapper_neg(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    return self->type->slot_number->slot_neg(self);
}

object* float_wrapper_repr(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    return self->type->slot_repr(self);
}

object* float_wrapper_bool(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    return self->type->slot_number->slot_bool(self);
}

object* float_wrapper_new(object* args, object* kwargs){
    //!! Copied

    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    object* obj=new_object(&IntType);
    if (len==0){
        ((IntObject*)obj)->val=new BigInt(0);

        object* o = in_immutables((struct object*)obj);
        if (o==NULL){
            return (object*)obj;
        }
        DECREF((struct object*)obj);
        return o;
    }
    object* val=INCREF(args->type->slot_mappings->slot_get(args, new_int_fromint(1)));
    if (!object_istype(val->type, &IntType) && !object_istype(val->type, &StrType)){
        DECREF(obj);
        vm_add_err(&ValueError, vm, "Expected argument to be int or str, got type '%s'",args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type->name->c_str());
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


object* float_wrapper_ne(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    object* other=args->type->slot_mappings->slot_get(args, new_int_fromint(1));

    return self->type->slot_cmp(self, other, CMP_NE);
}

object* float_wrapper_eq(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    object* other=args->type->slot_mappings->slot_get(args, new_int_fromint(1));

    return self->type->slot_cmp(self, other, CMP_EQ);
}

object* float_wrapper_gt(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    object* other=args->type->slot_mappings->slot_get(args, new_int_fromint(1));
    return self->type->slot_cmp(self, other, CMP_GT);
}

object* float_wrapper_gte(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    object* other=args->type->slot_mappings->slot_get(args, new_int_fromint(1));
    return self->type->slot_cmp(self, other, CMP_GTE);
}

object* float_wrapper_lt(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    object* other=args->type->slot_mappings->slot_get(args, new_int_fromint(1));
    return self->type->slot_cmp(self, other, CMP_LT);
}

object* float_wrapper_lte(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type;
            if (!object_istype(tp, &FloatType)){
                vm_add_err(&TypeError, vm, "Expected 'float' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    object* other=args->type->slot_mappings->slot_get(args, new_int_fromint(1));
    return self->type->slot_cmp(self, other, CMP_LTE);
}

double round_double(double value, size_t prec){
  double pow_10 = pow(10.0d, (double)prec);
  return round(value * pow_10) / pow_10;
}