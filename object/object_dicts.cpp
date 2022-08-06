void setup_int_dir(){
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_new, "__new__");

    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_add, "__add__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_sub, "__sub__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_mul, "__mul__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_div, "__div__");
    
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_neg, "__neg__");
    
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_repr, "__repr__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_repr, "__str__");
    object* dict=new_dict();
    
    for (auto k: (*CAST_DICT(IntType.dict)->val)){
        dict->type->slot_set(dict, k.first, k.second);
    }



    IntType.dict->type->slot_set(IntType.dict, str_new_fromstr(new string("__dict__")), dict);
}