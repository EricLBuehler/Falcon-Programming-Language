object* time_sleep(object* args, object* kwargs){
    cout<<args;
    return new_none();
}

object* new_time_module(){
    object* dict=new_dict();
    object* sleep=cwrapper_new_fromfunc((cwrapperfunc)time_sleep, "sleep");
    dict_set(dict, str_new_fromstr("sleep"), sleep);

    return module_new_fromdict(dict, str_new_fromstr("time"));
}