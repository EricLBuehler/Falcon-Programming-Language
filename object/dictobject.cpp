object* new_dict(){
    object_var* obj=new_object_var(&DictType, 0);
    CAST_DICT(obj)->val=new map<object*, object*>;
    CAST_DICT(obj)->keys=new vector<object*>;
    CAST_DICT(obj)->val->clear();
    CAST_DICT(obj)->keys->clear();
    obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);
    
    return (object*)obj;
}

object* dict_new(object* type, object* args, object* kwargs){
    if (object_istype(args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type, &DictType)){
        return INCREF(args->type->slot_mappings->slot_get(args, new_int_fromint(0)));
    }
    if (args->type->slot_mappings->slot_get(args, new_int_fromint(0))->type->slot_iter!=NULL){
        object* o=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
        object* iter=o->type->slot_iter(o);

        object_var* obj=new_object_var(&DictType, 0);
        CAST_DICT(obj)->val=new map<object*, object*>;
        CAST_DICT(obj)->keys=new vector<object*>;
        CAST_DICT(obj)->val->clear();
        CAST_DICT(obj)->keys->clear();
        obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);


        o=iter->type->slot_next(iter);
        while (vm->exception==NULL){
            if (*CAST_INT(o->type->slot_mappings->slot_len(o))->val!=2){
                DECREF((object*)obj);
                vm_add_err(&ValueError, vm, "Expected 2 values (key/value) for dictionary update, got '%d'",CAST_INT(o->type->slot_mappings->slot_len(o))->val->to_int());
                return NULL;
            }
            dict_set((object*)obj, o->type->slot_mappings->slot_get(o, new_int_fromint(0)), o->type->slot_mappings->slot_get(o, new_int_fromint(1)));
            
            o=iter->type->slot_next(iter);
        }
        if (vm->exception!=NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        return (object*)obj;
    }
    
    if (CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()==0 && CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()==0){
        object_var* obj=new_object_var(&DictType, 0);
        CAST_DICT(obj)->val=new map<object*, object*>;
        CAST_DICT(obj)->keys=new vector<object*>;
        CAST_DICT(obj)->val->clear();
        CAST_DICT(obj)->keys->clear();
        obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);
        return (object*)obj;    
    }

    object_var* obj=new_object_var(&DictType, 0);
    CAST_DICT(obj)->val=new map<object*, object*>;
    CAST_DICT(obj)->keys=new vector<object*>;
    CAST_DICT(obj)->val->clear();
    CAST_DICT(obj)->keys->clear();
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
    //Fast path for immutables
    if (CAST_DICT(self)->val->find(key)!=CAST_DICT(self)->val->end()){
        return CAST_DICT(self)->val->at(key);
    }
    
    for (auto k: (*CAST_DICT(self)->val)){
        if (istrue(object_cmp(key, k.first, CMP_EQ))){
            return  CAST_DICT(self)->val->at(k.first);
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
            (*CAST_DICT(self)->val)[key]=INCREF(val);
            CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
            return;
        }
    }

    CAST_DICT(self)->keys->push_back(key);

    (*CAST_DICT(self)->val)[INCREF(key)]=INCREF(val);
    CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
}

object* dict_repr(object* self){
    string s="";
    s+="{";
    int i=0;
    for (object* o: *CAST_DICT(self)->keys ){
        s+=object_crepr(o);
        s+=": ";
        s+=object_crepr(CAST_DICT(self)->val->at(o));
        if (i!=CAST_DICT(self)->val->size()-1){
            s+=", ";
        }
        i++;
    }
    s+="}";
    return str_new_fromstr(s);
}

object* dict_str(object* self){
    string s="";
    s+="{";
    int i=0;
    for (auto k: (*CAST_DICT(self)->val)){
        s+=object_crepr(k.first);
        s+=": ";
        s+=object_crepr(k.second);
        if (i!=CAST_DICT(self)->val->size()-1){
            s+=",\n";
        }
        i++;
    }
    s+="}";
    return str_new_fromstr(s);
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
    for (auto k: (*CAST_DICT(obj)->val)){
        DECREF(k.first);
        DECREF(k.second);
    }
    delete CAST_DICT(obj)->val;
}

object* dict_iter(object* self){
    //Make an iterator
    object* iter=new_object(&DictIterType);
    CAST_DICTITER(iter)->val=new map<object*,object*>;
    CAST_DICTITER(iter)->keys=new vector<object*>;
    CAST_DICTITER(iter)->val->clear();
    CAST_DICTITER(iter)->keys->clear();
    CAST_DICTITER(iter)->idx=0;
    for (auto k: *CAST_DICT(self)->val){
        (*CAST_DICTITER(iter)->val)[INCREF(k.first)]=INCREF(k.second);
        CAST_DICTITER(iter)->keys->push_back(INCREF(k.first));
    }
    return iter;
}

void dict_iter_del(object* self){
    for (auto k: (*CAST_DICTITER(self)->val)){
        DECREF(k.first);
        DECREF(k.second);
    }
    delete CAST_DICTITER(self)->val;
}

object* dict_iter_next(object* self){
    if (CAST_DICTITER(self)->idx+1>CAST_DICTITER(self)->val->size()){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }
    object* l=new_list();
    list_append(l, (*CAST_DICTITER(self)->keys)[CAST_DICTITER(self)->idx++]);
    list_append(l, CAST_DICTITER(self)->val->at((*CAST_DICTITER(self)->keys)[CAST_DICTITER(self)->idx-1]));
    return l;
}

object* dict_iter_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
    }
    if (type==CMP_EQ){
        if ((*CAST_DICTITER(self)->val) == (*CAST_DICTITER(other)->val)){
            return new_bool_true();
        }
        if (CAST_DICTITER(self)->val->size() != CAST_DICTITER(other)->val->size()){
            return new_bool_false();
        }
        
        for(auto it_m1 = (*CAST_DICTITER(self)->val).cbegin(), end_m1 = (*CAST_DICTITER(self)->val).cend(), it_m2 = (*CAST_DICTITER(other)->val).cbegin(), end_m2 = (*CAST_DICTITER(other)->val).cend(); it_m1 != end_m1 || it_m2 != end_m2;){
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

object* dict_iter_bool(object* self){
    return new_bool_true();
}