void module_del(object* self){
    FPLDECREF(CAST_MODULE(self)->dict);
    FPLDECREF(CAST_MODULE(self)->name);
}

object* module_new_fromdict(object* dict, object* name){
    object* m=new_object(&ModuleType);
    CAST_MODULE(m)->dict=FPLINCREF(dict);
    CAST_MODULE(m)->name=FPLINCREF(name);
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

object* module_self_getattr(object* obj, object* attr){
    //Check dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        cout<<dict<<obj;
        if (object_find_bool_dict_keys(dict, attr)){
            return dict_get(dict, attr);
        }
    }
    //Check type dict
    if (obj->type->dict!=0){
        object* dict = obj->type->dict;
        if (object_find_bool_dict_keys(dict, attr)){
            return dict_get(dict, attr);
        }
    }

    return NULL;
}

object* module_getattr(object* obj, object* attr){
    //Check dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        if (object_find_bool_dict_keys(dict, attr)){
            return dict_get(dict, attr);
        }
    }

    return object_genericgetattr(obj, attr);
}

object* module_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_MODULE(self)->dict, CAST_MODULE(other)->dict, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_MODULE(self)->dict, CAST_MODULE(other)->dict, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}