object* slotwrapper_new_fromfunc(getter get, setter set, string name, TypeObject* basetype){
    object* o=new_object(&SlotWrapperType);
    CAST_SLOTWRAPPER(o)->get=get;
    CAST_SLOTWRAPPER(o)->set=set;
    CAST_SLOTWRAPPER(o)->name=new string(name);
    CAST_SLOTWRAPPER(o)->basetype=basetype;
    return o;
}

object* slotwrapper_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);
    string s="<";
    s+=self->type->name->c_str();
    s+=" '";
    s+=(*CAST_SLOTWRAPPER(self)->name);
    s+="' @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(s);
}

object* slotwrapper_str(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);
    if (CAST_SLOTWRAPPER(self)->get==NULL){
        string s="<";
        s+=self->type->name->c_str();
        s+=" '";
        s+=(*CAST_SLOTWRAPPER(self)->name);
        s+="' @ ";
        s+=buf;
        s+=">";
        return str_new_fromstr(s);
    }
    string s="<";
    s+=self->type->name->c_str();
    s+=" '";
    s+=(*CAST_SLOTWRAPPER(self)->name);
    s+="' @ ";
    s+=buf;
    s+=":\n";
    s+=object_crepr(CAST_SLOTWRAPPER(self)->get(self));
    s+=">";
    return str_new_fromstr(s);
}

object* slotwrapper_descrget(object* obj, object* self){
    return CAST_SLOTWRAPPER(self)->get(self);
}

object* slotwrapper_descrset(object* obj, object* self, object* val){
    return CAST_SLOTWRAPPER(self)->set(self, val);
}