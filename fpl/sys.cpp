object* sys_getsizeof(object* self, object* args){
    object* val=dict_get(args, str_new_fromstr("obj"));
    object* size;
    if (val->type->size==0){
        size=new_int_frombigint(new BigInt(val->type->var_base_size));        
    }
    else{
        size=new_int_frombigint(new BigInt(val->type->size));
    }
    FPLDECREF(val);
    return size;
}

object* sys_getrefcnt(object* self, object* args){
    object* val=dict_get(args, str_new_fromstr("obj"));
    object* o=new_int_fromint(val->refcnt);
    FPLDECREF(val);
    return o;
}

object* sys_getpath(object* sys){
    FPLINCREF(vm->path);
    return vm->path;
}

object* new_sys_module(){
    object* dict=new_dict();

    object* emptykw_args=new_tuple();

    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, str_new_fromstr("obj"));
    object* ob=new_builtin(sys_getsizeof, str_new_fromstr("getsizeof"), args, emptykw_args, 1, false);
    dict_set_noret(dict, str_new_fromstr("getsizeof"), ob);
    FPLDECREF(ob);    
    
    
    ob=new_builtin(sys_getrefcnt, str_new_fromstr("getrefcnt"), args, emptykw_args, 1, false);
    dict_set_noret(dict, str_new_fromstr("getrefcnt"), ob);
    FPLDECREF(ob);    
    

    object* getset=slotwrapper_new_fromfunc(sys_getpath, NULL, "path");
    dict_set_noret(dict, str_new_fromstr("path"), getset);

    //Argc and argv
    dict_set_noret(dict, str_new_fromstr("argc"), new_int_fromint(glblargc_raw));
    object* argv=new_tuple();
    for (int i=0; i<glblargc_raw; i++){
        tuple_append(argv, str_new_fromstr(string(glblargv_raw[i])));
    }
    dict_set_noret(dict, str_new_fromstr("argv"), argv);
    FPLDECREF(argv);
    
    dict_set_noret(dict, str_new_fromstr("version"), str_new_fromstr(FPL_VERSION));

    return module_new_fromdict(dict, str_new_fromstr("sys"));
}