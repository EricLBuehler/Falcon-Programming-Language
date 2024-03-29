object* new_builtins_module(){
    object* dict=new_dict();

    for (size_t i=0; i<nbuiltins; i++){
        string name="";
        if (object_istype(builtins[i]->type, &BuiltinType)){
            name=*(CAST_STRING(CAST_BUILTIN(builtins[i])->name)->val);
        }
        else if (object_istype(builtins[i]->type, &TypeType)){
            name=*(CAST_TYPE(builtins[i])->name);
        }
        object* o=str_new_fromstr(name);
        dict_set_noret(dict, o, builtins[i]);
        FPLDECREF(o);
    }
    
    object* obj=module_new_fromdict(dict, str_new_fromstr("builtins"));

    object* d= (*(object**)((char*)obj + obj->type->dict_offset));
    return obj;
}