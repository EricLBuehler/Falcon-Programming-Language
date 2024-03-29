#ifndef _WIN32
#include <sys/types.h>
#endif

object* os_chdir(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("dir"));
    if (!object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str object, got '%s' object", val->type->name->c_str());
        return NULL; 
    }

    string dir=*CAST_STRING(val)->val;
    FPLDECREF(val);

    int res=chdir(dir.c_str());
    if (res==0){
        return new_none();
    }

    vm_add_err(&FileNotFoundError, vm, "[ERRNO: %d]: Cannot find directory '%s'", errno, dir.c_str());
    errno=0;
    return NULL;
}

object* os_mkdir(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("dir"));
    if (!object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str object, got '%s' object", val->type->name->c_str());
        return NULL; 
    }

    string dir=*CAST_STRING(val)->val;
    FPLDECREF(val);

    #ifdef _WIN32
    int res=mkdir(dir.c_str());
    #else
    int res=mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    #endif
    if (res==0){
        return new_none();
    }

    vm_add_err(&FileNotFoundError, vm, "[ERRNO: %d]: Cannot create directory '%s'", errno, dir.c_str());
    errno=0;
    return NULL;
}


object* os_getcwd(object* self, object* args){
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


object* os_rmdir(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("dir"));
    if (!object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str object, got '%s' object", val->type->name->c_str());
        return NULL; 
    }

    string dir=*CAST_STRING(val)->val;
    FPLDECREF(val);

    int res=rmdir(dir.c_str());
    if (res==0){
        return new_none();
    }

    vm_add_err(&FileNotFoundError, vm, "[ERRNO: %d]: Cannot remove directory '%s'", errno, dir.c_str());
    errno=0;
    return NULL;
}

object* os_listdir(object* self, object* args){
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
            tuple_append_noinc(list, str_new_fromstr(string(en->d_name)));
        }
        closedir(dr);
        return list;
    }
    vm_add_err(&FileNotFoundError, vm, "[ERRNO: %d]: Cannot open directory '%s'", errno, buffer);
    errno=0;
    return NULL;
}


object* os_system(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("cmd"));
    if (!object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str object, got '%s' object", val->type->name->c_str());
        return NULL; 
    }

    string cmd=*CAST_STRING(val)->val;
    FPLDECREF(val);
    
    return new_int_fromint(system(cmd.c_str()));
}

object* new_os_module(){
    object* dict=new_dict();

    object* dirargs=new_tuple();
    tuple_append_noinc(dirargs, str_new_fromstr("dir"));
    object* emptykw_args=new_tuple();
    
    object* ob=new_builtin(os_chdir, str_new_fromstr("chdir"), dirargs, emptykw_args, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("chdir"), ob);

    ob=new_builtin(os_mkdir, str_new_fromstr("mkdir"), dirargs, emptykw_args, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("mkdir"), ob);    

    ob=new_builtin(os_getcwd, str_new_fromstr("getcwd"), emptykw_args, emptykw_args, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("getcwd"), ob);    

    ob=new_builtin(os_rmdir, str_new_fromstr("rmdir"), dirargs, emptykw_args, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("rmdir"), ob);

    ob=new_builtin(os_listdir, str_new_fromstr("listdir"), emptykw_args, emptykw_args, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("listdir"), ob);

    object* systemargs=new_tuple();
    tuple_append_noinc(systemargs, str_new_fromstr("cmd"));
    ob=new_builtin(os_system, str_new_fromstr("system"), systemargs, emptykw_args, 1, false);
    dict_set_noinc_noret(dict, str_new_fromstr("system"), ob);
    

    return module_new_fromdict(dict, str_new_fromstr("os"));
}