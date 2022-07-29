void binop_add(struct vm* vm, object* left, object* right){
    object* ret=object_add(left, right);
    if (ret!=NULL){
        add_dataframe(vm, vm->objstack, ret);
    }
    else{
        vm_add_err(TypeError, vm, "Invalid operand type for +: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
    }
}

void binop_sub(struct vm* vm, object* left, object* right){
    object* ret=object_sub(left, right);
    if (ret!=NULL){
        add_dataframe(vm, vm->objstack, ret);
    }
    else{
        vm_add_err(TypeError, vm, "Invalid operand type for -: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
    }
}

void binop_mul(struct vm* vm, object* left, object* right){
    object* ret=object_mul(left, right);
    if (ret!=NULL){
        add_dataframe(vm, vm->objstack, ret);
    }
    else{
        vm_add_err(TypeError, vm, "Invalid operand type for *: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
    }
}

void binop_div(struct vm* vm, object* left, object* right){
    object* ret=object_div(left, right);
    if (ret!=NULL){
        add_dataframe(vm, vm->objstack, ret);
    }
    else{
        vm_add_err(TypeError, vm, "Invalid operand type for /: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
    }
}

