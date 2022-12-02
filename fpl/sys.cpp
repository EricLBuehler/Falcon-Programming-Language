object* sys_getsizeof(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("obj"));
    object* size;
    if (val->type->size==0){
        size=new_int_frombigint(new BigInt(CAST_VAR(val)->var_size));        
    }
    else{
        size=new_int_frombigint(new BigInt(val->type->size));
    }
    FPLDECREF(val);
    return size;
}

object* sys_getrefcnt(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("obj"));
    object* o=new_int_fromint(val->refcnt);
    FPLDECREF(val);
    return o;
}

object* sys_getpath(object* sys){
    FPLINCREF(interpreter.path);
    return interpreter.path;
}

object* sys_getterm_color(object* sys){
    if (interpreter.has_color){
        return new_bool_true();
    }
    return new_bool_false();
}

object* sys_setterm_color(object* sys, object* val){
    FPLINCREF(val);
    interpreter.has_color=istrue(val);
    return new_none();
}

object* new_sys_module(){
    object* dict=new_dict();

    object* emptykw_args=new_tuple();

    object* args=new_tuple();
    tuple_append_noinc(args, str_new_fromstr("obj"));
    object* ob=new_builtin(sys_getsizeof, str_new_fromstr("getsizeof"), args, emptykw_args, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("getsizeof"), ob);    
    
    ob=new_builtin(sys_getrefcnt, str_new_fromstr("getrefcnt"), args, emptykw_args, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("getrefcnt"), ob);
    

    object* getset=slotwrapper_new_fromfunc(sys_getpath, NULL, "path");
    dict_set_noinc_noret(dict, str_new_fromstr("path"), getset);
    

    getset=slotwrapper_new_fromfunc(sys_getterm_color, sys_setterm_color, "has_color");
    dict_set_noinc_noret(dict, str_new_fromstr("has_color"), getset);

    //Argc and argv
    dict_set_noinc_noret(dict, str_new_fromstr("argc"), new_int_fromint(glblargc_raw));
    object* argv=new_tuple();
    for (int i=0; i<glblargc_raw; i++){
        tuple_append_noinc(argv, str_new_fromstr(string(glblargv_raw[i])));
    }
    dict_set_noinc_noret(dict, str_new_fromstr("argv"), argv);
    
    dict_set_noinc_noret(dict, str_new_fromstr("version"), str_new_fromstr(FPL_VERSION));
    
    FPLINCREF(Stdout);
    FPLINCREF(Stdin);
    dict_set_noinc_noret(dict, str_new_fromstr("stdout"), Stdout);
    dict_set_noinc_noret(dict, str_new_fromstr("stdin"), Stdin);

    return module_new_fromdict(dict, str_new_fromstr("sys"));
}