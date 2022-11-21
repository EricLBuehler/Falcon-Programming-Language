#include <cmath>

object* math_sin(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_cos(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_tan(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_acos(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_acosh(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_asin(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_asinh(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_atan(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_atanh(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_atan2(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    object* val2=dict_get_opti_deref(args, str_new_fromstr("y"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
        return NULL; 
    }        
    object* flval2=object_float(val2);
    FPLDECREF(val2);
    if (flval2==NULL || !object_istype(flval->type, &IntType)){
        return NULL;
    }
    
    double otherval=CAST_FLOAT(flval)->val;
    double otherval2=CAST_FLOAT(flval2)->val;
    double res=atan2(otherval,otherval2);
    FPLDECREF(flval);
    FPLDECREF(flval2);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_cbrt(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_ceil(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_cosh(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_exp(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_abs(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_floor(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_hypot(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("a"));
    object* val2=dict_get_opti_deref(args, str_new_fromstr("b"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
        return NULL; 
    }        
    object* flval2=object_float(val2);
    FPLDECREF(val2);
    if (flval2==NULL || !object_istype(flval2->type, &IntType)){
        return NULL;
    }
    
    double otherval=CAST_FLOAT(flval)->val;
    double otherval2=CAST_FLOAT(flval2)->val;
    double res=hypot(otherval,otherval2);
    FPLDECREF(flval);
    FPLDECREF(flval2);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* math_log(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_log10(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_pow(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    object* val2=dict_get_opti_deref(args, str_new_fromstr("y"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
        return NULL; 
    }        
    object* flval2=object_float(val2);
    FPLDECREF(val2);
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

object* math_sinh(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_sqrt(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_tanh(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    
    object* flval=object_float(val);
    FPLDECREF(val);
    if (flval==NULL || !object_istype(flval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
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

object* math_factorial(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    if (!object_istype(val->type, &IntType)){
        vm_add_err(&TypeError, vm, "Expected int, got '%s'", val->type->name->c_str());
        return NULL; 
    }
    BigInt n=*CAST_INT(val)->val;
    FPLDECREF(val);
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
 

object* math_fib(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("x"));
    if (!object_istype(val->type, &IntType)){
        vm_add_err(&TypeError, vm, "Expected int, got '%s'", val->type->name->c_str());
        return NULL; 
    }
    BigInt n=*CAST_INT(val)->val;
    FPLDECREF(val);
    if (n<0){
        vm_add_err(&ValueError, vm, "Expected argument to be greater than 0");
    }
    
    return new_int_frombigint(new BigInt(fib(n)));
}


object* math_todeg(object* self, object* args){
    object* val_=dict_get_opti_deref(args, str_new_fromstr("x"));
    object* val=object_float(val_);
    
    FPLDECREF(val_);
    
    if (val==NULL || !object_istype(val->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", val->type->name->c_str());
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

    object* sinargs=new_tuple();
    tuple_append_noinc(sinargs, str_new_fromstr("x"));
    object* sinkwargs=new_tuple();
    object* sin=new_builtin(math_sin, str_new_fromstr("sin"), sinargs, sinkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("sin"), sin);
    FPLDECREF(sin);

    object* tanargs=new_tuple();
    tuple_append_noinc(tanargs, str_new_fromstr("x"));
    object* tankwargs=new_tuple();
    object* tan=new_builtin(math_tan, str_new_fromstr("tan"), tanargs, tankwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("tan"), tan);
    FPLDECREF(tan);

    object* cosargs=new_tuple();
    tuple_append_noinc(cosargs, str_new_fromstr("x"));
    object* coskwargs=new_tuple();
    object* cos=new_builtin(math_cos, str_new_fromstr("cos"), cosargs, coskwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("cos"), cos);
    FPLDECREF(cos);

    object* acosargs=new_tuple();
    tuple_append_noinc(acosargs, str_new_fromstr("x"));
    object* acoskwargs=new_tuple();
    object* acos=new_builtin(math_acos, str_new_fromstr("acos"), acosargs, acoskwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("acos"), acos);
    FPLDECREF(acos);

    object* acoshargs=new_tuple();
    tuple_append_noinc(acoshargs, str_new_fromstr("x"));
    object* acoshkwargs=new_tuple();
    object* acosh=new_builtin(math_acosh, str_new_fromstr("acosh"), acoshargs, acoshkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("acosh"), acosh);
    FPLDECREF(acosh);

    object* asinargs=new_tuple();
    tuple_append_noinc(asinargs, str_new_fromstr("x"));
    object* asinkwargs=new_tuple();
    object* asin=new_builtin(math_asin, str_new_fromstr("asin"), asinargs, asinkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("asin"), asin);
    FPLDECREF(asin);

    object* asinhargs=new_tuple();
    tuple_append_noinc(asinhargs, str_new_fromstr("x"));
    object* asinhkwargs=new_tuple();
    object* asinh=new_builtin(math_asinh, str_new_fromstr("asinh"), asinhargs, asinhkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("asinh"), asinh);

    object* atanargs=new_tuple();
    tuple_append_noinc(atanargs, str_new_fromstr("x"));
    object* atankwargs=new_tuple();
    object* atanh=new_builtin(math_atanh, str_new_fromstr("atanh"), atanargs, atankwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("atanh"), atanh);
    FPLDECREF(atanh);

    object* atanhargs=new_tuple();
    tuple_append_noinc(atanhargs, str_new_fromstr("x"));
    object* atanhkwargs=new_tuple();
    object* atan=new_builtin(math_atan, str_new_fromstr("atan"), atanhargs, atanhkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("atan"), atan);
    FPLDECREF(atan);
    
    object* atan2args=new_tuple();
    tuple_append_noinc(atan2args, str_new_fromstr("x"));
    tuple_append_noinc(atan2args, str_new_fromstr("y"));
    object* atan2kwargs=new_tuple();
    object* atan2=new_builtin(math_atan2, str_new_fromstr("atan2"), atan2args, atan2kwargs, 2, false);
    dict_set_noinc_noret(dict, str_new_fromstr("atan2"), atan2);  
    FPLDECREF(atan2);

    object* cbrtargs=new_tuple();
    tuple_append_noinc(cbrtargs, str_new_fromstr("x"));
    object* cbrtkwargs=new_tuple();
    object* cbrt=new_builtin(math_cbrt, str_new_fromstr("cbrt"), cbrtargs, cbrtkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("cbrt"), cbrt); 
    FPLDECREF(cbrt);

    object* ceilargs=new_tuple();
    tuple_append_noinc(ceilargs, str_new_fromstr("x"));
    object* ceilkwargs=new_tuple();
    object* ceil=new_builtin(math_ceil, str_new_fromstr("ceil"), ceilargs, ceilkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("ceil"), ceil); 
    FPLDECREF(ceil);

    object* coshargs=new_tuple();
    tuple_append_noinc(coshargs, str_new_fromstr("x"));
    object* coshkwargs=new_tuple();
    object* cosh=new_builtin(math_cosh, str_new_fromstr("cosh"), coshargs, coshkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("cosh"), cosh); 
    FPLDECREF(cosh);

    object* expargs=new_tuple();
    tuple_append_noinc(expargs, str_new_fromstr("x"));
    object* expkwargs=new_tuple();
    object* exp=new_builtin(math_exp, str_new_fromstr("exp"), expargs, expkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("exp"), exp);
    FPLDECREF(exp);

    object* absargs=new_tuple();
    tuple_append_noinc(absargs, str_new_fromstr("x"));
    object* abskwargs=new_tuple();
    object* abs=new_builtin(math_abs, str_new_fromstr("abs"), absargs, abskwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("abs"), abs); 
    FPLDECREF(abs);
    
    object* floorargs=new_tuple();
    tuple_append_noinc(floorargs, str_new_fromstr("x"));
    object* floorkwargs=new_tuple();
    object* floor=new_builtin(math_floor, str_new_fromstr("floor"), floorargs, floorkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("floor"), floor); 
    FPLDECREF(floor);
    
    object* hypotargs=new_tuple();
    tuple_append_noinc(hypotargs, str_new_fromstr("a"));
    tuple_append_noinc(hypotargs, str_new_fromstr("b"));
    object* hypotkwargs=new_tuple();
    object* hypot=new_builtin(math_hypot, str_new_fromstr("hypot"), hypotargs, hypotkwargs, 2, false);
    dict_set_noinc_noret(dict, str_new_fromstr("hypot"), hypot); 
    FPLDECREF(hypot);
    
    object* logargs=new_tuple();
    tuple_append_noinc(logargs, str_new_fromstr("x"));
    object* logkwargs=new_tuple();
    object* log=new_builtin(math_log, str_new_fromstr("log"), logargs, logkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("log"), log); 
    FPLDECREF(log);
    
    object* log10args=new_tuple();
    tuple_append_noinc(log10args, str_new_fromstr("x"));
    object* log10kwargs=new_tuple();
    object* log10=new_builtin(math_log10, str_new_fromstr("log"), log10args, log10kwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("log10"), log10); 
    FPLDECREF(log10);
    
    object* powargs=new_tuple();
    tuple_append_noinc(powargs, str_new_fromstr("x"));
    tuple_append_noinc(powargs, str_new_fromstr("y"));
    object* powkwargs=new_tuple();
    object* pow=new_builtin(math_pow, str_new_fromstr("log"), powargs, powkwargs, 2, false);
    dict_set_noinc_noret(dict, str_new_fromstr("pow"), pow); 
    FPLDECREF(pow);

    object* sinhargs=new_tuple();
    tuple_append_noinc(sinhargs, str_new_fromstr("x"));
    object* sinhkwargs=new_tuple();
    object* sinh=new_builtin(math_sinh, str_new_fromstr("sinh"), sinhargs, sinhkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("sinh"), sinh); 
    FPLDECREF(sinh);
    
    object* sqrtargs=new_tuple();
    tuple_append_noinc(sqrtargs, str_new_fromstr("x"));
    object* sqrtkwargs=new_tuple();
    object* sqrt=new_builtin(math_sqrt, str_new_fromstr("sqrt"), sqrtargs, sqrtkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("sqrt"), sqrt); 
    FPLDECREF(sqrt);

    object* tanhargs=new_tuple();
    tuple_append_noinc(tanhargs, str_new_fromstr("x"));
    object* tanhkwargs=new_tuple();
    object* tanh=new_builtin(math_tanh, str_new_fromstr("tanh"), tanhargs, tanhkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("tanh"), tanh); 
    FPLDECREF(tanh);

    object* factorialargs=new_tuple();
    tuple_append_noinc(factorialargs, str_new_fromstr("x"));
    object* factorialkwargs=new_tuple();
    object* factorial=new_builtin(math_factorial, str_new_fromstr("factorial"), factorialargs, factorialkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("factorial"), factorial); 
    FPLDECREF(factorial);

    object* fibargs=new_tuple();
    tuple_append_noinc(fibargs, str_new_fromstr("x"));
    object* fibkwargs=new_tuple();
    object* fib=new_builtin(math_fib, str_new_fromstr("fib"), fibargs, fibkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("fib"), fib); 
    FPLDECREF(fib);

    object* todegargs=new_tuple();
    tuple_append_noinc(todegargs, str_new_fromstr("x"));
    object* todegkwargs=new_tuple();
    object* todeg=new_builtin(math_todeg, str_new_fromstr("todeg"), todegargs, todegkwargs, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("todeg"), todeg); 
    FPLDECREF(todeg);


    dict_set_noinc_noret(dict, str_new_fromstr("pi"), new_float_fromdouble(M_PI));
    dict_set_noinc_noret(dict, str_new_fromstr("tau"), new_float_fromdouble(M_PI*2));
    dict_set_noinc_noret(dict, str_new_fromstr("e"), new_float_fromdouble(M_E));

    return module_new_fromdict(dict, str_new_fromstr("math"));
}