void module_del(object* self){
    DECREF(CAST_MODULE(self)->dict);
    DECREF(CAST_MODULE(self)->name);
}

object* module_new_fromdict(object* dict, object* name){
    object* m=new_object(&ModuleType);
    CAST_MODULE(m)->dict=dict;
    CAST_MODULE(m)->name=name;
    return m;
}

object* module_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="<";
    s+=self->type->name->c_str();
    s+=" ";
    s+=object_cstr(CAST_MODULE(self)->name);
    s+=" @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(s);
}

object* module_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_MODULE(self)->dict, CAST_MODULE(other)->dict, CMP_EQ))){
            return new_bool_true();
        }
    }
    return new_bool_true();
}