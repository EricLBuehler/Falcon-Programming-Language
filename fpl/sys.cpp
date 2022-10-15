object* sys_getsizeof(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* val=list_index_int(args, 1);

    if (val->type->size==0){
        return new_int_frombigint(new BigInt(val->type->var_base_size));        
    }
    return new_int_frombigint(new BigInt(val->type->size));
}

object* sys_getrefcnt(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* val=list_index_int(args, 1);

    return new_int_fromint(val->refcnt);
}

object* sys_getpath(object* sys){
    return FPLINCREF(vm->path);
}

object* new_sys_module(){
    object* dict=new_dict();

    dict_set(dict, str_new_fromstr("getsizeof"), cwrapper_new_fromfunc_null((cwrapperfunc)sys_getsizeof, "getsizeof"));
    dict_set(dict, str_new_fromstr("getrefcnt"), cwrapper_new_fromfunc_null((cwrapperfunc)sys_getrefcnt, "getrefcnt"));
    object* getset=slotwrapper_new_fromfunc(sys_getpath, NULL, "path");
    dict_set(dict, str_new_fromstr("path"), getset);

    return module_new_fromdict(dict, str_new_fromstr("sys"));
}