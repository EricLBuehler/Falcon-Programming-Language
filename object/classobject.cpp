object* class_new(object* type, object* args, object* kwargs){
    object* obj=new_object(&ClassType);
    
    CAST_CLASS(obj)->dict=CAST_TYPE_(type)->otype.dict;
    CAST_CLASS(obj)->name=str_new_fromstr(CAST_TYPE_(type)->otype.name);
    cout<<object_cstr(CAST_CLASS(obj)->dict);
    
    return obj;
}

object* class_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);
    string s="";
    s+="<";
    s+=object_cstr(CAST_CLASS(self)->name);
    s+=" @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(new string(s));
}

object* class_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_CLASS(self)->name, CAST_CLASS(other)->name, type)) && \
        istrue(object_cmp(CAST_CLASS(self)->dict, CAST_CLASS(other)->dict, type))){
            return new_bool_true();
        }
    }
    return new_bool_false();
}

void class_del(object* obj){
    DECREF(CAST_CLASS(obj)->name);
    DECREF(CAST_CLASS(obj)->dict);
}