object* slotwrapper_call(object* self, object* args, object* kwargs){
    return CAST_SLOTWRAPPER(self)->function(self);
}

object* slotwrapper_get(object* self, object* key){
    return CAST_SLOTWRAPPER(self)->get(self, key);
}

void slotwrapper_set(object* self, object* key, object* val){
    CAST_SLOTWRAPPER(self)->set(self, key, val);
}

object* slotwrapper_len(object* self){
    return CAST_SLOTWRAPPER(self)->len(self);
}

object* slotwrapper_new_fromfunc(getsetfunc func, getfunc get, setfunc set, lenfunc len,string name, TypeObject* type){
    object* o=new_object(&SlotWrapperType);
    CAST_SLOTWRAPPER(o)->function=func;
    CAST_SLOTWRAPPER(o)->get=get;
    CAST_SLOTWRAPPER(o)->set=set;
    CAST_SLOTWRAPPER(o)->len=len;
    CAST_SLOTWRAPPER(o)->name=new string(name);
    CAST_SLOTWRAPPER(o)->basetype=type;
    return o;
}

object* slotwrapper_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="<";
    s+=self->type->name->c_str();
    s+=" '";
    s+=CAST_SLOTWRAPPER(self)->name->c_str();
    s+="' @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(new string(s));
}

object* slotwrapper_str(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="<";
    s+=self->type->name->c_str();
    s+=" '";
    s+=CAST_SLOTWRAPPER(self)->name->c_str();
    s+="' @ ";
    s+=buf;
    s+=":\n";
    s+=object_crepr(CAST_SLOTWRAPPER(self)->function(self));
    s+=">";
    return str_new_fromstr(new string(s));
}