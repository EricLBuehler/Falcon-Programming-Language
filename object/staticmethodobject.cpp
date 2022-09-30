object* staticmethod_new_impl(object* func, object* instance){
    object* method=new_object(&StaticMethodType);
    CAST_METHOD(method)->function=INCREF(func);
    CAST_METHOD(method)->instance=INCREF(instance);
    return method;
}

object* staticmethod_repr(object* self){
    string s="<staticmethod ";
    s+=object_cstr(CAST_METHOD(self)->function);
    s+=">";
    return str_new_fromstr(s);
}

object* staticmethod_call(object* self, object* args, object* kwargs){
    object* val=object_call(CAST_METHOD(self)->function, args, kwargs);
    return val;
}