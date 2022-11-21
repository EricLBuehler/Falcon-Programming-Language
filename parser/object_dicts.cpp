void setup_int_dir(){
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_new, "__new__");

    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_add, "__add__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_sub, "__sub__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_mul, "__mul__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_div, "__div__");
    
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_neg, "__neg__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_bool, "__bool__");
    
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_repr, "__repr__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_repr, "__str__");

    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_ne, "__ne__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_eq, "__eq__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_gt, "__gt__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_gte, "__gte__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_lt, "__lt__");
    type_set_cwrapper(&IntType, (cwrapperfunc)int_wrapper_lte, "__lte__");
}

void setup_str_dir(){
    type_set_cwrapper(&StrType, (cwrapperfunc)str_wrapper_new, "__new__");
    
    type_set_cwrapper(&StrType, (cwrapperfunc)str_wrapper_len, "__len__");
    type_set_cwrapper(&StrType, (cwrapperfunc)str_wrapper_bool, "__bool__");
    
    type_set_cwrapper(&StrType, (cwrapperfunc)str_wrapper_repr, "__repr__");
    type_set_cwrapper(&StrType, (cwrapperfunc)str_wrapper_str, "__str__");

    type_set_cwrapper(&StrType, (cwrapperfunc)str_wrapper_ne, "__ne__");
    type_set_cwrapper(&StrType, (cwrapperfunc)str_wrapper_eq, "__eq__");
}