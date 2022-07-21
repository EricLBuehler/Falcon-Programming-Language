object* new_dict(){
    object_var* obj=new_object_var(&DictType, 0);
    CAST_DICT(obj)->val=new unordered_map<object*, object*>;
    CAST_DICT(obj)->val->clear();
    obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);
    
    return (object*)obj;
}

object* dict_new(object* type, object* args, object* kwargs){
    object_var* obj=new_object_var(&DictType, 0);
    CAST_DICT(obj)->val=new unordered_map<object*, object*>;
    CAST_DICT(obj)->val->clear();
    obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);
    
    return (object*)obj;
}

object* dict_len(object* self){
    return new_int_fromint(((DictObject*)self)->val->size());
}

object* dict_get(object* self, object* key){
    if (CAST_DICT(self)->val->find(key)==CAST_DICT(self)->val->end()){
        //Error!
        return NULL;
    }
    return CAST_DICT(self)->val->at(key);
}

void dict_set(object* self, object* key, object* val){
    (*CAST_DICT(self)->val)[INCREF(key)]=INCREF(val);
    CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
}

object* dict_repr(object* self){
    string s="";
    s+="{";
    int i=0;
    for (auto k: (*CAST_DICT(self)->val)){
        s+=(*CAST_STRING(object_repr(k.first))->val);
        s+=": ";
        s+=(*CAST_STRING(object_repr(k.second))->val);
        if (i!=CAST_DICT(self)->val->size()-1){
            s+=", ";
        }
        i++;
    }
    s+="}";
    return str_new_fromstr(new string(s));
}

object* dict_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
    }
    if (type==CMP_EQ){
        if ((*CAST_DICT(self)->val) == (*CAST_DICT(other)->val)){
            return new_bool_true();
        }
        if (CAST_DICT(self)->val->size() != CAST_DICT(other)->val->size()){
            return new_bool_false();
        }
        
        for(auto it_m1 = (*CAST_DICT(self)->val).cbegin(), end_m1 = (*CAST_DICT(self)->val).cend(), it_m2 = (*CAST_DICT(other)->val).cbegin(), end_m2 = (*CAST_DICT(other)->val).cend(); it_m1 != end_m1 || it_m2 != end_m2;){
            if (!istrue(object_cmp(it_m1->first, it_m2->first, type))){
                return new_bool_false(); 
            }
            if (!istrue(object_cmp(it_m1->second, it_m2->second, type))){
                return new_bool_false(); 
            }
            it_m1++;
            it_m2++;
        }
        return new_bool_true();
    }
    return new_bool_false();
}

void dict_del(object* obj){
    delete CAST_DICT(obj)->val;
}