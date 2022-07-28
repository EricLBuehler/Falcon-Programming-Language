void binop_is(struct vm* vm, object* left, object* right){
    object* ret=left==right ? new_bool_true() : new_bool_false();
    add_dataframe(vm, vm->objstack, ret);
}

void binop_ee(struct vm* vm, object* left, object* right){
    object* ret=object_cmp(left, right, CMP_EQ);
    add_dataframe(vm, vm->objstack, ret);
}