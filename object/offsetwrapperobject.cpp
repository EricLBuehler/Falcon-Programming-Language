object* offsetwrapper_new_fromoffset(string name, size_t offset, bool readonly){
    object* o;
    if (readonly){
        o=new_object(&OffsetWrapperReadonlyType);
    }
    else{
        o=new_object(&OffsetWrapperType);
    }
    CAST_OFFSETWRAPPER(o)->offset=offset;
    CAST_OFFSETWRAPPER(o)->name=new string(name);
    return o;
}

object* offsetwrapper_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%p", self);
    string s="<";
    s+=self->type->name->c_str();
    s+=" '";
    s+=(*CAST_OFFSETWRAPPER(self)->name);
    s+="' @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(s);
}

object* offsetwrapper_descrget(object* obj, object* self, object* owner){
    if (owner==NULL || object_istype(owner->type, &NoneType)){
        FPLINCREF(self);
        return self;
    }
    object* ob= (*(object**)((char*)obj + CAST_OFFSETWRAPPER(self)->offset));
    return ob;
}

object* offsetwrapper_descrset(object* obj, object* self, object* val){
    FPLDECREF((*(object**)((char*)obj + CAST_OFFSETWRAPPER(self)->offset)));
    (*(object**)((char*)obj + CAST_OFFSETWRAPPER(self)->offset))=val;
    return new_none();
}