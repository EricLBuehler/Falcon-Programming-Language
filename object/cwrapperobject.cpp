object* cwrapper_call(object* self, object* args, object* kwargs){
    return CAST_CWRAPPER(self)->function(args, kwargs);
}

object* cwrapper_new_fromfunc(cwrapperfunc func, string name){
    object* o=new_object(&CWrapperType);
    CAST_CWRAPPER(o)->function=func;
    CAST_CWRAPPER(o)->name=new string(name);
    return o;
}

object* cwrapper_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="<";
    s+=self->type->name->c_str();
    s+=" '";
    s+=(*CAST_CWRAPPER(self)->name);
    s+="' @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(s);
}