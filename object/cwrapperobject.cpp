object* cwrapper_call(object* self, object* args, object* kwargs){
    return CAST_CWRAPPER(self)->function(args, kwargs);
}

object* cwrapper_new_fromfunc(cwrapperfunc func){
    object* o=new_object(&CWrapperType);
    CAST_CWRAPPER(o)->function=func;
    return o;
}