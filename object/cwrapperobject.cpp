object* cwrapper_call(object* selftp, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len<1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected at least 1 argument, got %d", len);
        return NULL;    
    }
    object* self=list_index_int(args,0);
    if (!object_issubclass(self, CAST_TYPE(CAST_CWRAPPER(selftp)->tp))){
        vm_add_err(&ValueError, vm, "Descriptor '%s' expected '%s' object, got '%s' object.", CAST_CWRAPPER(selftp)->name->c_str(), CAST_TYPE(CAST_CWRAPPER(selftp)->tp)->name->c_str(), self->type->name->c_str());
        return NULL;
    }
    return CAST_CWRAPPER(selftp)->function(CAST_CWRAPPER(selftp)->tp, args, kwargs);
}

object* cwrapper_new_fromfunc(cwrapperfunc func, string name, object* tp){
    object* o=new_object(&CWrapperType);
    CAST_CWRAPPER(o)->function=func;
    CAST_CWRAPPER(o)->name=new string(name);
    FPLINCREF(tp);
    CAST_CWRAPPER(o)->tp=tp;
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
    sprintf(buf, "0x%p", self);

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
        FPLDECREF(CAST_CWRAPPER(self)->tp);
    }
}

object* cwrapper_descrget(object* obj, object* self, object* owner){
    if (owner==NULL || object_istype(owner->type, &NoneType)){
        FPLINCREF(self);
        return self;
    }
    return wrappermethod_new_impl(self, obj);
}