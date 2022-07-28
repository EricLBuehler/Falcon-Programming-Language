void binop_is(struct vm* vm, object* left, object* right){
    object* ret=left==right ? new_bool_true() : new_bool_false();
    if (ret!=NULL){
        add_dataframe(vm, vm->objstack, ret);
    }
    else{
        vm_add_err(vm, "TypeError: Invalid operand type for +: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
    }
}