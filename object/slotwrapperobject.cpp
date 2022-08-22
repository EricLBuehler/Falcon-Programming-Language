object* slotwrapper_call(object* self, object* args, object* kwargs){
    return CAST_SLOTWRAPPER(self)->function(self);
}

object* slotwrapper_new_fromfunc(getsetfunc func, string name, TypeObject* basetype){
    object* o=new_object(&SlotWrapperType);
    CAST_SLOTWRAPPER(o)->function=func;
    CAST_SLOTWRAPPER(o)->name=new string(name);
    CAST_SLOTWRAPPER(o)->basetype=basetype;
    return o;
}

object* slotwrapper_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len>0){
        vm_add_err(&ValueError, vm, "Expected 0 arguments, got %d", len);
        return NULL;
    }
    object* o=new_object(&SlotWrapperType);
    CAST_SLOTWRAPPER(o)->function=NULL;
    CAST_SLOTWRAPPER(o)->name=NULL;
    CAST_SLOTWRAPPER(o)->basetype=NULL;
    return o;
}

object* slotwrapper_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);
    if (CAST_SLOTWRAPPER(self)->function==NULL){
        string s="<";
        s+=self->type->name->c_str();
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
    s+=">";
    return str_new_fromstr(s);
}

object* slotwrapper_str(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);
    if (CAST_SLOTWRAPPER(self)->function==NULL){
        string s="<";
        s+=self->type->name->c_str();
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
    s+=object_crepr(CAST_SLOTWRAPPER(self)->function(self));
    s+=">";
    return str_new_fromstr(s);
}

object* slotwrapper_iter(object* self){
    object* o=CAST_SLOTWRAPPER(self)->function(self);
    return o->type->slot_iter(o);
}