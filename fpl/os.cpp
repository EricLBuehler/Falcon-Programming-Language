object* os_chdir(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* val=list_index_int(args, 0);
    if (!object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str object, got '%s' object", val->type->name->c_str());
        return NULL; 
    }

    string dir=*CAST_STRING(val)->val;

    int res=chdir(dir.c_str());
    if (res==0){
        return new_none();
    }

    vm_add_err(&FileNotFoundError, vm, "[ERRNO: %d]: Cannot find directory '%s'", errno, dir.c_str());
    errno=0;
    return NULL;
}

object* os_mkdir(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* val=list_index_int(args, 0);
    if (!object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str object, got '%s' object", val->type->name->c_str());
        return NULL; 
    }

    string dir=*CAST_STRING(val)->val;

    int res=mkdir(dir.c_str());
    if (res==0){
        return new_none();
    }

    vm_add_err(&FileNotFoundError, vm, "[ERRNO: %d]: Cannot create directory '%s'", errno, dir.c_str());
    errno=0;
    return NULL;
}


object* os_getcwd(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=0 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 0 arguments, got %d", len);
        return NULL; 
    }

    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        return str_new_fromstr(string(buffer));
    }
    else {
        vm_add_err(&FileNotFoundError, vm, "[ERRNO: %d]: Cannot get current working directory", errno);
        errno=0;
        return NULL;
    }    
}


object* os_rmdir(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* val=list_index_int(args, 0);
    if (!object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str object, got '%s' object", val->type->name->c_str());
        return NULL; 
    }

    string dir=*CAST_STRING(val)->val;

    int res=rmdir(dir.c_str());
    if (res==0){
        return new_none();
    }

    vm_add_err(&FileNotFoundError, vm, "[ERRNO: %d]: Cannot remove directory '%s'", errno, dir.c_str());
    errno=0;
    return NULL;
}

object* os_listdir(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=0 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 0 arguments, got %d", len);
        return NULL; 
    }

    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        vm_add_err(&FileNotFoundError, vm, "[ERRNO: %d]: Cannot get current working directory", errno);
        errno=0;
        return NULL;
    }  

    DIR *dr;
    struct dirent *en;
    dr = opendir(buffer);
    if (dr) {
        object* list=new_list();
        while ((en = readdir(dr)) != NULL) {
            if (string(en->d_name)==string(".") || string(en->d_name)==string("..")){
                continue;
            }
            list_append(list, str_new_fromstr(string(en->d_name)));
        }
        closedir(dr);
        return list;
    }
    vm_add_err(&FileNotFoundError, vm, "[ERRNO: %d]: Cannot open directory '%s'", errno, buffer);
    errno=0;
    return NULL;
}


object* os_system(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* val=list_index_int(args, 0);
    if (!object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str object, got '%s' object", val->type->name->c_str());
        return NULL; 
    }

    string cmd=*CAST_STRING(val)->val;
    
    return new_int_fromint(system(cmd.c_str()));
}

object* new_os_module(){
    object* dict=new_dict();
    
    dict_set(dict, str_new_fromstr("chdir"), cwrapper_new_fromfunc_null((cwrapperfunc)os_chdir, "chdir"));
    dict_set(dict, str_new_fromstr("mkdir"), cwrapper_new_fromfunc_null((cwrapperfunc)os_mkdir, "mkdir"));
    dict_set(dict, str_new_fromstr("getcwd"), cwrapper_new_fromfunc_null((cwrapperfunc)os_getcwd, "getcwd"));
    dict_set(dict, str_new_fromstr("rmdir"), cwrapper_new_fromfunc_null((cwrapperfunc)os_rmdir, "rmdir"));
    dict_set(dict, str_new_fromstr("listdir"), cwrapper_new_fromfunc_null((cwrapperfunc)os_listdir, "listdir"));
    dict_set(dict, str_new_fromstr("system"), cwrapper_new_fromfunc_null((cwrapperfunc)os_system, "system"));

    return module_new_fromdict(dict, str_new_fromstr("os"));
}