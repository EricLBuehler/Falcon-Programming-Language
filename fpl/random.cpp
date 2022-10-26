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

object* random_randint(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }

    object* low=object_int(list_index_int(args, 0));
    object* high=object_int(list_index_int(args, 1));

    if (low==NULL || !object_istype(low->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 0)->type->name->c_str());
        return NULL; 
    }
    if (high==NULL || !object_istype(high->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 1)->type->name->c_str());
        return NULL; 
    }
    
    
    object* res=new_int_fromint(iRand(CAST_INT(low)->val->to_int(), CAST_INT(high)->val->to_int()));
    
    FPLDECREF(low);
    FPLDECREF(high);

    return res;
}

object* random_random(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if ((len!=2 && len!=0) || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 0 or 2 arguments, got %d", len);
        return NULL; 
    }

    double lo=0;
    double hi=1;
    if (len==2){
        object* low=object_int(list_index_int(args, 0));
        object* high=object_int(list_index_int(args, 1));

        if (low==NULL || !object_istype(low->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 0)->type->name->c_str());
            return NULL; 
        }
        if (high==NULL || !object_istype(high->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 1)->type->name->c_str());
            return NULL; 
        }
        
        lo=CAST_INT(low)->val->to_int();
        hi=CAST_INT(high)->val->to_int();

        FPLDECREF(low);
        FPLDECREF(high);
    }

    return new_float_fromdouble(fRand(lo, hi));
}

object* random_choice(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* ob=list_index_int(args, 0);
    if (ob->type->slot_mappings==NULL || ob->type->slot_mappings->slot_get==NULL\
     || ob->type->slot_mappings->slot_len==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", ob->type->name->c_str());
        return NULL; 
    }
    object* iterlen=ob->type->slot_mappings->slot_len(ob);
    
    int len_sub1=CAST_INT(iterlen)->val->to_int()-1;

    return FPLINCREF(ob->type->slot_mappings->slot_get(ob, new_int_fromint(iRand(0,len_sub1))));
}

object* new_random_module(){
    srand(time(NULL));

    object* dict=new_dict();

    object* randint=cwrapper_new_fromfunc_null((cwrapperfunc)random_randint, "randint");
    dict_set(dict, str_new_fromstr("randint"), randint);

    object* random=cwrapper_new_fromfunc_null((cwrapperfunc)random_random, "random");
    dict_set(dict, str_new_fromstr("random"), random);

    object* choice=cwrapper_new_fromfunc_null((cwrapperfunc)random_choice, "choice");
    dict_set(dict, str_new_fromstr("choice"), choice);
    
    dict_set(dict, str_new_fromstr("RAND_MAX"), new_int_fromint(RAND_MAX));

    return module_new_fromdict(dict, str_new_fromstr("random"));
}