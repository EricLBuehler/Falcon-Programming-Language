int iRand(int lo, int hi){
    int n = hi - lo + 1;
    int i = rand() % n;
    if (i < 0) i = -i;
    return lo + i;
}

double fRand(double lo, double hi)
{
    double f = (double)rand() / RAND_MAX;
    return lo + f * (hi - lo);
}

object* random_randint(object* self, object* args){
    object* low=dict_get_opti_deref(args, str_new_fromstr("lo"));
    object* high=dict_get_opti_deref(args, str_new_fromstr("hi"));

    if (low==NULL || !object_istype(low->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", low->type->name->c_str());
        return NULL; 
    }
    if (high==NULL || !object_istype(high->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", high->type->name->c_str());
        return NULL; 
    }
    
    
    object* res=new_int_fromint(iRand(CAST_INT(low)->val->to_int(), CAST_INT(high)->val->to_int()));
    
    FPLDECREF(low);
    FPLDECREF(high);

    return res;
}

object* random_random(object* self, object* args){
    double lo;
    double hi;     

    object* low=dict_get_opti_deref(args, str_new_fromstr("lo"));
    object* high=dict_get_opti_deref(args, str_new_fromstr("hi"));

    if (low==NULL || !object_istype(low->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", low->type->name->c_str());
        FPLDECREF(low);
        FPLDECREF(high);
        return NULL; 
    }
    if (high==NULL || !object_istype(high->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", high->type->name->c_str());
        FPLDECREF(low);
        FPLDECREF(high);
        return NULL; 
    }
    
    lo=CAST_INT(low)->val->to_int();
    hi=CAST_INT(high)->val->to_int();

    FPLDECREF(low);
    FPLDECREF(high);
    

    return new_float_fromdouble(fRand(lo, hi));
}

object* random_choice(object* self, object* args){
    object* ob=dict_get_opti_deref(args, str_new_fromstr("iter"));
    if (ob->type->slot_mappings==NULL || ob->type->slot_mappings->slot_get==NULL\
     || ob->type->slot_mappings->slot_len==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", ob->type->name->c_str());
        FPLDECREF(ob);
        return NULL; 
    }
    object* iterlen=ob->type->slot_mappings->slot_len(ob);
    
    int len_sub1=CAST_INT(iterlen)->val->to_int()-1;
    FPLDECREF(iterlen);

    object* o=dict_get_opti_deref(ob, new_int_fromint(iRand(0,len_sub1)));
    FPLDECREF(ob);
    return o;
}

object* new_random_module(){
    srand(time(NULL));

    object* dict=new_dict();

    object* emptykw_args=new_tuple();

    object* randintargs=new_tuple();
    tuple_append_noinc(randintargs, str_new_fromstr("lo"));
    tuple_append_noinc(randintargs, str_new_fromstr("hi"));
    object* ob=new_builtin(random_randint, str_new_fromstr("randint"), randintargs, emptykw_args, 2, false);
    dict_set_noinc_noret(dict, str_new_fromstr("randint"), ob);

    object* randkwargs=new_tuple();
    tuple_append_noinc(randkwargs, new_int_fromint(0));
    tuple_append_noinc(randkwargs, new_int_fromint(1));
    ob=new_builtin(random_random, str_new_fromstr("random"), randintargs, randkwargs, 2, false);
    dict_set_noinc_noret(dict, str_new_fromstr("random"), ob);
    
    object* randchoiceargs=new_tuple();
    tuple_append_noinc(randchoiceargs, str_new_fromstr("iter"));
    ob=new_builtin(random_choice, str_new_fromstr("choice"), randchoiceargs, emptykw_args, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("choice"), ob);
    
    dict_set_noinc_noret(dict, str_new_fromstr("RAND_MAX"), new_int_fromint(RAND_MAX));

    return module_new_fromdict(dict, str_new_fromstr("random"));
}