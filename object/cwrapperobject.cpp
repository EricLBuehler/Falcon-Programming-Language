object* cwrapper_call(object* self, object* args, object* kwargs){
    return CAST_CWRAPPER(self)->function(CAST_CWRAPPER(self)->tp, args, kwargs);
}

object* cwrapper_new_fromfunc(cwrapperfunc func, string name, object* tp){
    object* o=new_object(&CWrapperType);
    CAST_CWRAPPER(o)->function=func;
    CAST_CWRAPPER(o)->name=new string(name);
    CAST_CWRAPPER(o)->tp=INCREF(tp);
    return o;
}

object* cwrapper_new_fromfunc_null(cwrapperfunc func, string name){
    object* o=new_object(&CWrapperType);
    CAST_CWRAPPER(o)->function=func;
    CAST_CWRAPPER(o)->name=new string(name);
    CAST_CWRAPPER(o)->tp=NULL;
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

void cwrapper_del(object* self){
    if (CAST_CWRAPPER(self)->tp!=NULL){
        DECREF(CAST_CWRAPPER(self)->tp);
    }
}

object* cwrapper_descrget(object* obj, object* self){
    return wrappermethod_new_impl(self, obj);
}