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
    object* res=module_self_getattr(obj, attr);

    if (res==NULL){
        //Check bases
        uint32_t total_bases = CAST_INT(list_len(obj->type->bases))->val->to_long_long();
        for (uint32_t i=0; i<total_bases; i++){
            TypeObject* base_tp=CAST_TYPE(list_index_int(obj->type->bases, i));
            //Check type dict
            if (base_tp->dict!=0){
                object* dict = base_tp->dict;
                if (object_find_bool_dict_keys(dict, attr)){
                    res=dict_get(dict, attr);
                    break;
                }
            }
        }
    }
    
    if (res==NULL){
        vm_add_err(&AttributeError, vm, "%s has no attribute '%s'",obj->type->name->c_str(), object_cstr(attr).c_str());
    }
    return res;
}


object* module_setattr(object* obj, object* attr, object* val){
    object* d=NULL;
    object* dict_=NULL;
    
    //Try instance dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        dict_=dict;
        if (object_find_bool_dict_keys(dict, attr)){
            d=dict;
        }
    }
    
    //Check type dict 
    else if (d==NULL && obj->type->dict!=0){
        object* dict = obj->type->dict;
        dict_=dict;
        if (object_find_bool_dict_keys(dict, attr)){
            d=dict;
        }
    }

    else if (d==NULL){
        //Check bases
        uint32_t total_bases = CAST_INT(list_len(obj->type->bases))->val->to_long_long();
        for (uint32_t i=0; i<total_bases; i++){
            TypeObject* base_tp=CAST_TYPE(list_index_int(obj->type->bases, i));

            //Check type dict
            if (base_tp->dict!=0){
                object* dict = base_tp->dict;
                dict_=dict;
                if (object_find_bool_dict_keys(dict, attr)){
                    d=dict;
                    break;
                }
            }
        }
    }

    if (d==NULL && dict_==NULL){
        vm_add_err(&AttributeError, vm, "%s object is read only",obj->type->name->c_str());
        return NULL;
    }
    
    dict_set(dict_, attr, val);
    return SUCCESS;
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