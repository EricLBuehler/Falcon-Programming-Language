#include <cmath>

object* math_sin(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to floa", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=sin(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_cos(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=cos(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_tan(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=tan(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_acos(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 1);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=acos(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_acosh(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=acosh(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_asin(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument1, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=asin(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_asinh(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=asinh(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_atan(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=atan(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_atanh(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=atanh(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_atan2(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    object* val2=list_index_int(args, 1);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    object* flval2=object_float(val2);
    if (flval2==NULL || !object_istype(flval->type, &IntType)){
        return NULL;
    }
    
    double otherval=CAST_FLOAT(flval)->val;
    double otherval2=CAST_FLOAT(flval2)->val;
    double res=atan2(otherval,otherval2);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_cbrt(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=cbrt(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_ceil(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=ceil(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_cosh(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=cosh(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_exp(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=exp(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_abs(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=fabs(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_floor(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=floor(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_hypot(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    object* val2=list_index_int(args, 1);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    object* flval2=object_float(val2);
    if (flval2==NULL || !object_istype(flval2->type, &IntType)){
        return NULL;
    }
    
    double otherval=CAST_FLOAT(flval)->val;
    double otherval2=CAST_FLOAT(flval2)->val;
    double res=hypot(otherval,otherval2);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_log(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=log(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_log10(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=log10(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_pow(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    object* val2=list_index_int(args, 1);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    object* flval2=object_float(val2);
    if (flval2==NULL || !object_istype(flval2->type, &IntType)){
        return NULL;
    }
    
    double otherval=CAST_FLOAT(flval)->val;
    double otherval2=CAST_FLOAT(flval2)->val;
    double res=pow(otherval,otherval2);
    FPLDECREF(flval);
    FPLDECREF(flval2);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_sinh(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=sinh(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_sqrt(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=sqrt(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_tanh(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* val=list_index_int(args, 0);
    
    object* flval=object_float(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }        
    
    double otherval=CAST_FLOAT(flval)->val;
    double res=tanh(otherval);
    FPLDECREF(flval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_factorial(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* val=list_index_int(args, 0);
    if (!object_istype(val->type, &IntType)){
        vm_add_err(&ValueError, vm, "Expected int, got '%s'", val->type->name->c_str());
        return NULL; 
    }
    BigInt n=*CAST_INT(val)->val;
    if (n<0){
        vm_add_err(&ValueError, vm, "Expected argument to be greater than 0");
    }

    BigInt fact=1;
    for (BigInt i=1; i<n+1; i++){
        fact=fact*i;
    }
    return new_int_frombigint(new BigInt(fact));
}


BigInt fib(BigInt n){
    if (n <= 1)
        return n;
    return fib(n - 1) + fib(n - 2);
}
 

object* math_fib(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* val=list_index_int(args, 0);
    if (!object_istype(val->type, &IntType)){
        vm_add_err(&ValueError, vm, "Expected int, got '%s'", val->type->name->c_str());
        return NULL; 
    }
    BigInt n=*CAST_INT(val)->val;
    if (n<0){
        vm_add_err(&ValueError, vm, "Expected argument to be greater than 0");
    }
    
    return new_int_frombigint(new BigInt(fib(n)));
}


object* math_todeg(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* val=object_float(list_index_int(args, 1));
    
    if (val==NULL || !object_istype(val->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }

    double otherval=CAST_FLOAT(val)->val;
    double res=otherval*(180/M_PI);
    FPLDECREF(val);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* new_math_module(){
    object* dict=new_dict();

    object* sin=cwrapper_new_fromfunc_null((cwrapperfunc)math_sin, "sin");
    dict_set(dict, str_new_fromstr("sin"), sin);

    object* tan=cwrapper_new_fromfunc_null((cwrapperfunc)math_tan, "tan");
    dict_set(dict, str_new_fromstr("tan"), tan);

    object* cos=cwrapper_new_fromfunc_null((cwrapperfunc)math_cos, "cos");
    dict_set(dict, str_new_fromstr("cos"), cos);

    object* acos=cwrapper_new_fromfunc_null((cwrapperfunc)math_acos, "acos");
    dict_set(dict, str_new_fromstr("acos"), acos);

    object* acosh=cwrapper_new_fromfunc_null((cwrapperfunc)math_acosh, "acosh");
    dict_set(dict, str_new_fromstr("acosh"), acosh);

    object* asin=cwrapper_new_fromfunc_null((cwrapperfunc)math_asin, "asin");
    dict_set(dict, str_new_fromstr("asin"), asin);

    object* asinh=cwrapper_new_fromfunc_null((cwrapperfunc)math_asinh, "asinh");
    dict_set(dict, str_new_fromstr("asinh"), asinh);

    object* atan=cwrapper_new_fromfunc_null((cwrapperfunc)math_atan, "atan");
    dict_set(dict, str_new_fromstr("atan"), atan);

    object* atanh=cwrapper_new_fromfunc_null((cwrapperfunc)math_atanh, "atanh");
    dict_set(dict, str_new_fromstr("atanh"), atanh);

    object* atan2=cwrapper_new_fromfunc_null((cwrapperfunc)math_atan2, "atan2");
    dict_set(dict, str_new_fromstr("atan2"), atan2);

    object* cbrt=cwrapper_new_fromfunc_null((cwrapperfunc)math_cbrt, "cbrt");
    dict_set(dict, str_new_fromstr("cbrt"), cbrt);

    object* ceil=cwrapper_new_fromfunc_null((cwrapperfunc)math_ceil, "ceil");
    dict_set(dict, str_new_fromstr("ceil"), ceil);

    object* cosh=cwrapper_new_fromfunc_null((cwrapperfunc)math_cosh, "cosh");
    dict_set(dict, str_new_fromstr("cosh"), cosh);

    object* exp=cwrapper_new_fromfunc_null((cwrapperfunc)math_exp, "exp");
    dict_set(dict, str_new_fromstr("exp"), exp);

    object* abs=cwrapper_new_fromfunc_null((cwrapperfunc)math_abs, "abs");
    dict_set(dict, str_new_fromstr("abs"), abs);

    object* floor=cwrapper_new_fromfunc_null((cwrapperfunc)math_floor, "floor");
    dict_set(dict, str_new_fromstr("floor"), floor);

    object* hypot=cwrapper_new_fromfunc_null((cwrapperfunc)math_hypot, "hypot");
    dict_set(dict, str_new_fromstr("hypot"), hypot);

    object* log=cwrapper_new_fromfunc_null((cwrapperfunc)math_log, "log");
    dict_set(dict, str_new_fromstr("log"), log);

    object* log10=cwrapper_new_fromfunc_null((cwrapperfunc)math_log10, "log10");
    dict_set(dict, str_new_fromstr("log10"), log10);

    object* pow=cwrapper_new_fromfunc_null((cwrapperfunc)math_pow, "pow");
    dict_set(dict, str_new_fromstr("pow"), pow);

    object* sinh=cwrapper_new_fromfunc_null((cwrapperfunc)math_sinh, "sinh");
    dict_set(dict, str_new_fromstr("sinh"), sinh);

    object* sqrt=cwrapper_new_fromfunc_null((cwrapperfunc)math_sqrt, "sqrt");
    dict_set(dict, str_new_fromstr("sqrt"), sqrt);

    object* tanh=cwrapper_new_fromfunc_null((cwrapperfunc)math_tanh, "tanh");
    dict_set(dict, str_new_fromstr("tanh"), tanh);

    object* factorial=cwrapper_new_fromfunc_null((cwrapperfunc)math_factorial, "factorial");
    dict_set(dict, str_new_fromstr("factorial"), factorial);

    object* fib=cwrapper_new_fromfunc_null((cwrapperfunc)math_fib, "fib");
    dict_set(dict, str_new_fromstr("fib"), fib);

    object* deg=cwrapper_new_fromfunc_null((cwrapperfunc)math_todeg, "todeg");
    dict_set(dict, str_new_fromstr("todeg"), deg);


    dict_set(dict, str_new_fromstr("pi"), new_float_fromdouble(M_PI));
    dict_set(dict, str_new_fromstr("tau"), new_float_fromdouble(M_PI*2));
    dict_set(dict, str_new_fromstr("e"), new_float_fromdouble(M_E));

    return module_new_fromdict(dict, str_new_fromstr("math"));
}