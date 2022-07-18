object* class_new(object* args, object* kwargs){
    object* obj=new_object(&ClassType);
    
    CAST_CLASS(obj)->dict=dict_new(NULL, NULL);
    
    if (args!=NULL){
        DECREF(args);
    }
    if (kwargs!=NULL){
        DECREF(kwargs);
    }
    return obj;
}

object* class_repr(object* self){
    string s="";
    s+="<class ";
    s+=object_cstr(CAST_CLASS(self)->name);
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