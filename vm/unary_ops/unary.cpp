void unary_neg(struct vm* vm, object* right){
    if (right->type->slot_number==NULL || right->type->slot_number->slot_sub==NULL){
        vm_add_err(vm, "TypeError: Invalid unary operand -: '%s'.", right->type->name->c_str());
        return;
    }
    object* ret=right->type->slot_number->slot_neg(right);
    if (ret!=NULL){
        add_dataframe(vm, vm->objstack, ret);
    }
    else{
        vm_add_err(vm, "TypeError: Invalid unary operand -: '%s'.", right->type->name->c_str());
    }
}