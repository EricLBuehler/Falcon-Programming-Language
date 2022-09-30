object* classmethod_new_impl(object* func, object* instance){
    object* method=new_object(&ClassMethodType);
    CAST_METHOD(method)->function=INCREF(func);
    CAST_METHOD(method)->instance=INCREF(instance);
    return method;
}

object* classmethod_repr(object* self){
    string s="<classmethod ";
    s+=object_cstr(CAST_METHOD(self)->function);
    s+=">";
    return str_new_fromstr(s);
}

object* classmethod_call(object* self, object* args, object* kwargs){
    object* args_=new_tuple();
    args_->type->slot_mappings->slot_append(args_, (object*)CAST_METHOD(self)->instance->type);
    for (int i=0; i<CAST_LIST(args)->size; i++){
        args_->type->slot_mappings->slot_append(args_, list_index_int(args, i));
    }
    object* val=object_call(CAST_METHOD(self)->function, args_, kwargs);
    return val;
}