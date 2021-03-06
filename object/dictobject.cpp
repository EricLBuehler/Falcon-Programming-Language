object* new_dict(){
    object_var* obj=new_object_var(&DictType, 0);
    CAST_DICT(obj)->val=new map<object*, object*>;
    CAST_DICT(obj)->val->clear();
    obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);
    
    return (object*)obj;
}

object* dict_new(object* type, object* args, object* kwargs){
    if (CAST_INT(kwargs->type->slot_len(kwargs))->val->to_int()==0){
        object_var* obj=new_object_var(&DictType, 0);
        CAST_DICT(obj)->val=new map<object*, object*>;
        CAST_DICT(obj)->val->clear();
        obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);
        return (object*)obj;    
    }
    if (object_istype(args->type->slot_get(args, new_int_fromint(0))->type, &DictType)){
        return INCREF(args->type->slot_get(args, new_int_fromint(0)));
    }

    object_var* obj=new_object_var(&DictType, 0);
    CAST_DICT(obj)->val=new map<object*, object*>;
    CAST_DICT(obj)->val->clear();
    obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);
    
    for (auto k: (*CAST_DICT(kwargs)->val)){
        dict_set((object*)obj, k.first, k.second);
    }
    
    return (object*)obj;    
}

object* dict_bool(object* self){
    return new_bool_true();
}


object* dict_len(object* self){
    return new_int_fromint(((DictObject*)self)->val->size());
}

object* dict_get(object* self, object* key){
    for (auto k: (*CAST_DICT(self)->val)){
        if (istrue(object_cmp(key, k.first, CMP_EQ))){
            return  (*CAST_DICT(self)->val)[k.first];
        }
    }
    
    vm_add_err(&KeyError, vm, "%s is not a key", object_crepr(key).c_str());
    return NULL;
}

void dict_set(object* self, object* key, object* val){
    //Fast path for immutables
    if (CAST_DICT(self)->val->find(key)!=CAST_DICT(self)->val->end()){
        //Do not incref key!
        if ((*CAST_DICT(self)->val)[key]==val){ //Same val
            //Do not incref val!
            return;
        }
        (*CAST_DICT(self)->val)[key]=INCREF(val);
        CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
        return;
    }

    for (auto k: (*CAST_DICT(self)->val)){
        if (istrue(object_cmp(key, k.first, CMP_EQ))){
            if (istrue(object_cmp(val, k.second, CMP_EQ))){ //Same val
                //Do not incref val!
                return;
            }
            if (key->type->size==0){
                ((object_var*)key)->gc_ref++;
            }
            DECREF(key);
            (*CAST_DICT(self)->val)[key]=INCREF(val);
            CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
            return;
        }
    }

    (*CAST_DICT(self)->val)[INCREF(key)]=INCREF(val);
    CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
}

object* dict_repr(object* self){
    string s="";
    s+="{";
    int i=0;
    for (auto k: (*CAST_DICT(self)->val)){
        s+=object_crepr(k.first);
        s+=": ";
        s+=object_crepr(k.second);
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