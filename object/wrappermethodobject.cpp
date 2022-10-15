object* wrappermethod_new_impl(object* func, object* instance){
    object* method=new_object(&WrapperMethodType);
    CAST_METHOD(method)->function=FPLINCREF(func);
    CAST_METHOD(method)->instance=FPLINCREF(instance);
    return method;
}

object* wrappermethod_call(object* self, object* args, object* kwargs){
    object* args_=new_tuple();
    args_->type->slot_mappings->slot_append(args_, CAST_METHOD(self)->instance);
    for (int i=0; i<CAST_LIST(args)->size; i++){
        args_->type->slot_mappings->slot_append(args_, list_index_int(args, i));
    }
    object* val=CAST_METHOD(self)->function->type->slot_call(CAST_METHOD(self)->function, args_, kwargs);
    return val;
}

object* wrappermethod_repr(object* self){
    string s="<wrapper_method ";
    s+=object_cstr(CAST_METHOD(self)->function);
    s+=">";
    return str_new_fromstr(s);
}