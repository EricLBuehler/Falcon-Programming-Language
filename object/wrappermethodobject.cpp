object* wrappermethod_new_impl(object* func, object* instance){
    object* method=new_object(&WrapperMethodType);
    FPLINCREF(func);
    CAST_METHOD(method)->function=func;
    FPLINCREF(instance);
    CAST_METHOD(method)->instance=instance;
    return method;
}

object* wrappermethod_call(object* self, object* args, object* kwargs){
    object* args_=new_tuple();
    args_->type->slot_mappings->slot_append(args_, CAST_METHOD(self)->instance);
    for (int i=0; i<CAST_LIST(args)->size; i++){
        args_->type->slot_mappings->slot_append(args_, list_index_int(args, i));
    }
    object* cwrapper=CAST_METHOD(self)->function;
    object* val=CAST_CWRAPPER(cwrapper)->function(CAST_CWRAPPER(cwrapper)->tp, args_, kwargs);
    FPLDECREF(args_);
    return val;
}

object* wrappermethod_repr(object* self){
    string s="<wrapper_method ";
    s+=object_cstr(CAST_METHOD(self)->function);
    s+=">";
    return str_new_fromstr(s);
}