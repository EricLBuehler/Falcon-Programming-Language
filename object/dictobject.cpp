object* new_dict(){
    object_var* obj=new_object_var(&DictType, 0);
    CAST_DICT(obj)->val=new unordered_map<object*, object*>;
    CAST_DICT(obj)->keys=new vector<object*>;
    CAST_DICT(obj)->val->clear();
    CAST_DICT(obj)->keys->clear();
    obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);
    
    return (object*)obj;
}

object* dict_new(object* type, object* args, object* kwargs){
    if (CAST_DICT(kwargs)->val->size()==0 && CAST_LIST(args)->size==0){
        object_var* obj=new_object_var(CAST_TYPE(type), 0);
        CAST_DICT(obj)->val=new unordered_map<object*, object*>;
        CAST_DICT(obj)->keys=new vector<object*>;
        CAST_DICT(obj)->val->clear();
        CAST_DICT(obj)->keys->clear();
        obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);
        return (object*)obj;    
    }
    if (CAST_LIST(args)->size!=0 && object_istype(list_index_int(args, 0)->type, &DictType)){
        object* o=list_index_int(args, 0);
        FPLINCREF(o);
        return o;
    }
    if (CAST_LIST(args)->size!=0){
        object* o=list_index_int(args, 0);
        
        if (o->type->slot_next==NULL){
            vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", o->type->name->c_str());
            return NULL;
        }

        object* iter=o->type->slot_iter(o);
        
        if (iter->type->slot_next==NULL){
            vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
            return NULL;
        }

        object_var* obj=new_object_var(CAST_TYPE(type), 0);
        CAST_DICT(obj)->val=new unordered_map<object*, object*>;
        CAST_DICT(obj)->keys=new vector<object*>;
        CAST_DICT(obj)->val->clear();
        CAST_DICT(obj)->keys->clear();
        obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);


        o=iter->type->slot_next(iter);
        while (vm->exception==NULL){
            if (o->type->slot_mappings->slot_len==NULL || *CAST_INT(o->type->slot_mappings->slot_len(o))->val!=2){
                object* len=o->type->slot_mappings->slot_len(o);
                if (*CAST_INT(len)->val!=2){
                    FPLDECREF(len);
                    FPLDECREF((object*)obj);
                    vm_add_err(&ValueError, vm, "Expected 2 values (key/value) for dictionary update, got '%d'",CAST_INT(len)->val->to_int());
                    return NULL;
                }
                FPLDECREF(len);
            }
            dict_set_noret((object*)obj, list_index_int(o, 0), list_index_int(o, 1));
            
            o=iter->type->slot_next(iter);
        }
        if (vm->exception!=NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        FPLDECREF(iter);
        return (object*)obj;
    }

    object_var* obj=new_object_var(CAST_TYPE(type), 0);
    CAST_DICT(obj)->val=new unordered_map<object*, object*>;
    CAST_DICT(obj)->keys=new vector<object*>;
    CAST_DICT(obj)->val->clear();
    CAST_DICT(obj)->keys->clear();
    obj->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(obj)->val->size())+sizeof(CAST_DICT(obj)->val);
    
    for (auto k: (*CAST_DICT(kwargs)->val)){
        dict_set_noret((object*)obj, k.first, k.second);
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
        FPLINCREF(CAST_DICT(self)->val->at(key));
        return CAST_DICT(self)->val->at(key);
    }
    
    for (auto k: (*CAST_DICT(self)->val)){
        if (istrue(object_cmp(key, k.first, CMP_EQ))){
            FPLINCREF(CAST_DICT(self)->val->at(k.first));
            return CAST_DICT(self)->val->at(k.first);
        }
    }
    vm_add_err(&KeyError, vm, "%s is not a key", object_crepr(key).c_str());
    return NULL;
}

void dict_del_item(object* self, object* key){
    for (auto k: (*CAST_DICT(self)->val)){
        if (istrue(object_cmp(key, k.first, CMP_EQ))){
            FPLDECREF(k.first);
            FPLDECREF(k.second);
            CAST_DICT(self)->val->erase(CAST_DICT(self)->val->find(k.first));
            CAST_DICT(self)->keys->erase(find(CAST_DICT(self)->keys->begin(), CAST_DICT(self)->keys->end(), k.first));
            return;
        }
        if (vm->exception!=NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
    }
    
    vm_add_err(&KeyError, vm, "%s is not a key", object_crepr(key).c_str());
}

void dict_set_noinc_noret(object* self, object* key, object* val){
    if (val==NULL){
        dict_del_item(self, key);
        return;
    }
    //Fast path for immutables
    if (CAST_DICT(self)->val->find(key)!=CAST_DICT(self)->val->end()){
        //Do not FPLINCREF key!
        if ((*CAST_DICT(self)->val)[key]==val){ //Same val
            //Do not FPLINCREF val!
            return;
        } 
        (*CAST_DICT(self)->val)[key]=val;
        CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
        return;
    }
    
    for (auto k: (*CAST_DICT(self)->val)){
        if (istrue(object_cmp(key, k.first, CMP_EQ))){
            if (istrue(object_cmp(val, k.second, CMP_EQ))){ //Same val
                //Do not FPLINCREF val!
                return;
            }
            (*CAST_DICT(self)->val)[k.first]=val;
            CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
            return;
        }
    }
    
    CAST_DICT(self)->keys->push_back(key);

    (*CAST_DICT(self)->val)[key]=val;
    CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
    return;
}

object* dict_set_noinc(object* self, object* key, object* val){
    if (val==NULL){
        dict_del_item(self, key);
        return new_none();
    }
    //Fast path for immutables
    if (CAST_DICT(self)->val->find(key)!=CAST_DICT(self)->val->end()){
        //Do not FPLINCREF key!
        if ((*CAST_DICT(self)->val)[key]==val){ //Same val
            //Do not FPLINCREF val!
            return new_none();
        } 
        (*CAST_DICT(self)->val)[key]=val;
        CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
        return new_none();
    }
    
    for (auto k: (*CAST_DICT(self)->val)){
        if (istrue(object_cmp(key, k.first, CMP_EQ))){
            if (istrue(object_cmp(val, k.second, CMP_EQ))){ //Same val
                //Do not FPLINCREF val!
                return new_none();
            }
            (*CAST_DICT(self)->val)[k.first]=val;
            CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
            return new_none();
        }
    }
    
    CAST_DICT(self)->keys->push_back(key);

    (*CAST_DICT(self)->val)[key]=val;
    CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
    return new_none();
}

void dict_set_noret_opti(object* self, object* key, object* val){
    //Fast path for immutables
    if (CAST_DICT(self)->val->find(key)!=CAST_DICT(self)->val->end()){
        //Do not FPLINCREF key!
        if ((*CAST_DICT(self)->val)[key]==val){ //Same val
            //Do not FPLINCREF val!
            return;
        }  
        object* o=(*CAST_DICT(self)->val)[key];
        FPLINCREF(val);
        (*CAST_DICT(self)->val)[key]=val;
        FPLDECREF(o);
        CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
        return;
    }
    
    CAST_DICT(self)->keys->push_back(key);

    FPLINCREF(key);
    FPLINCREF(val);
    (*CAST_DICT(self)->val)[key]=val;
    CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
}

void dict_set_noret(object* self, object* key, object* val){
    if (val==NULL){
        dict_del_item(self, key);
        return;
    }
    //Fast path for immutables
    if (CAST_DICT(self)->val->find(key)!=CAST_DICT(self)->val->end()){
        //Do not FPLINCREF key!
        if ((*CAST_DICT(self)->val)[key]==val){ //Same val
            //Do not FPLINCREF val!
            return;
        }  
        object* o=(*CAST_DICT(self)->val)[key];
        FPLINCREF(val);
        (*CAST_DICT(self)->val)[key]=val;
        FPLDECREF(o);
        CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
        return;
    }
    
    for (auto k: (*CAST_DICT(self)->val)){
        if (istrue(object_cmp(key, k.first, CMP_EQ))){
            if (istrue(object_cmp(val, k.second, CMP_EQ))){ //Same val
                //Do not FPLINCREF val!
                return;
            }
            object* o=(*CAST_DICT(self)->val)[k.first];
            FPLINCREF(val);
            (*CAST_DICT(self)->val)[k.first]=val;
            FPLDECREF(o);
            CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
            return;
        }
    }
    
    CAST_DICT(self)->keys->push_back(key);

    FPLINCREF(key);
    FPLINCREF(val);
    (*CAST_DICT(self)->val)[key]=val;
    CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
}



object* dict_set(object* self, object* key, object* val){
    if (val==NULL){
        dict_del_item(self, key);
        return new_none();
    }
    //Fast path for immutables
    if (CAST_DICT(self)->val->find(key)!=CAST_DICT(self)->val->end()){
        //Do not FPLINCREF key!
        if ((*CAST_DICT(self)->val)[key]==val){ //Same val
            //Do not FPLINCREF val!
            return new_none();
        } 
        object* o=(*CAST_DICT(self)->val)[key];
        FPLINCREF(val);
        (*CAST_DICT(self)->val)[key]=val;
        FPLDECREF(o);
        CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
        return new_none();
    }
    
    for (auto k: (*CAST_DICT(self)->val)){
        if (istrue(object_cmp(key, k.first, CMP_EQ))){
            if (istrue(object_cmp(val, k.second, CMP_EQ))){ //Same val
                //Do not FPLINCREF val!
                return new_none();
            }
            object* o=(*CAST_DICT(self)->val)[k.first];
            FPLINCREF(val);
            (*CAST_DICT(self)->val)[k.first]=val;
            FPLDECREF(o);
            CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
            return new_none();
        }
    }
    
    CAST_DICT(self)->keys->push_back(key);

    FPLINCREF(key);
    FPLINCREF(val);
    (*CAST_DICT(self)->val)[key]=val;
    CAST_VAR(self)->var_size=((sizeof(object*)+sizeof(object*))* CAST_DICT(self)->val->size())+sizeof((*CAST_DICT(self)->val));
    return new_none();
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
    for (object* o: *CAST_DICT(self)->keys ){
        s+=object_crepr(o);
        s+=": ";
        s+=object_crepr(CAST_DICT(self)->val->at(o));
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
        return NULL;
    }
    if (type==CMP_EQ){
        if ((*CAST_DICT(self)->val) == (*CAST_DICT(other)->val)){
            return new_bool_true();
        }
        if (CAST_DICT(self)->val->size() != CAST_DICT(other)->val->size()){
            return new_bool_false();
        }
        
        for (int i=0; i<CAST_DICT(self)->keys->size(); i++){
            object* key1=CAST_DICT(self)->keys->at(i);
            object* val1=(*CAST_DICT(self)->val)[key1];
            object* key2=CAST_DICT(other)->keys->at(i);
            object* val2=(*CAST_DICT(other)->val)[key2];

            if (!istrue(object_cmp(key1, key1, type))){
                return new_bool_false(); 
            }
            if (!istrue(object_cmp(key2, key2, type))){
                return new_bool_false(); 
            }
        }
        return new_bool_true();
    }
    if (type==CMP_NE){
        if ((*CAST_DICT(self)->val) == (*CAST_DICT(other)->val)){
            return new_bool_false();
        }
        if (CAST_DICT(self)->val->size() != CAST_DICT(other)->val->size()){
            return new_bool_true();
        }
        
        bool t=false;
        for (int i=0; i<CAST_DICT(self)->keys->size(); i++){
            object* key1=CAST_DICT(self)->keys->at(i);
            object* val1=(*CAST_DICT(self)->val)[key1];
            object* key2=CAST_DICT(other)->keys->at(i);
            object* val2=(*CAST_DICT(other)->val)[key2];
            
            if (!istrue(object_cmp(key1, key1, type)) && !istrue(object_cmp(key2, key2, type))){
                bool t=true;
            }
        }
        if (!t){
            return new_bool_false();
        }
        return new_bool_true();
    }
    return NULL;
}

void dict_del(object* obj){
    for (auto k: (*CAST_DICT(obj)->val)){
        FPLDECREF(k.first);
        FPLDECREF(k.second);
    }
    delete CAST_DICT(obj)->val;
    delete CAST_DICT(obj)->keys;
}

object* dict_iter(object* self){
    //Make an iterator
    object* iter=new_object(&DictIterType);
    CAST_DICTITER(iter)->val=new unordered_map<object*,object*>;
    CAST_DICTITER(iter)->keys=new vector<object*>;
    CAST_DICTITER(iter)->val->clear();
    CAST_DICTITER(iter)->keys->clear();
    CAST_DICTITER(iter)->idx=0;
    for (auto k: *CAST_DICT(self)->val){
        FPLINCREF(k.first);
        FPLINCREF(k.second);
        (*CAST_DICTITER(iter)->val)[k.first]=k.second;
        CAST_DICTITER(iter)->keys->push_back(k.first);
    }
    return iter;
}

void dict_iter_del(object* self){
    for (auto k: (*CAST_DICTITER(self)->val)){
        FPLDECREF(k.first);
        FPLDECREF(k.second);
    }
    delete CAST_DICTITER(self)->val;
    delete CAST_DICTITER(self)->keys;
}

object* dict_iter_next(object* self){
    if (CAST_DICTITER(self)->idx+1>CAST_DICTITER(self)->val->size()){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }
    return (*CAST_DICTITER(self)->keys)[CAST_DICTITER(self)->idx++];
}

object* dict_iter_cmp(object* self, object* other, uint8_t type){
    if (type==CMP_IN){
        return object_in_iter(other, self);
    }
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if ((*CAST_DICTITER(self)->val) == (*CAST_DICTITER(other)->val)){
            return new_bool_true();
        }
        if (CAST_DICTITER(self)->val->size() != CAST_DICTITER(other)->val->size()){
            return new_bool_false();
        }
        
        for (int i=0; i<CAST_DICTITER(self)->keys->size(); i++){
            object* key1=CAST_DICTITER(self)->keys->at(i);
            object* val1=(*CAST_DICTITER(self)->val)[key1];
            object* key2=CAST_DICTITER(other)->keys->at(i);
            object* val2=(*CAST_DICTITER(other)->val)[key2];

            if (!istrue(object_cmp(key1, key1, type))){
                return new_bool_false(); 
            }
            if (!istrue(object_cmp(key2, key2, type))){
                return new_bool_false(); 
            }
        }
        return new_bool_true();
    }
    if (type==CMP_NE){
        if ((*CAST_DICTITER(self)->val) == (*CAST_DICTITER(other)->val)){
            return new_bool_false();
        }
        if (CAST_DICTITER(self)->val->size() != CAST_DICTITER(other)->val->size()){
            return new_bool_true();
        }
        
        bool t=false;
        for (int i=0; i<CAST_DICTITER(self)->keys->size(); i++){
            object* key1=CAST_DICTITER(self)->keys->at(i);
            object* val1=(*CAST_DICTITER(self)->val)[key1];
            object* key2=CAST_DICTITER(other)->keys->at(i);
            object* val2=(*CAST_DICTITER(other)->val)[key2];
            
            if (!istrue(object_cmp(key1, key1, type)) && !istrue(object_cmp(key2, key2, type))){
                bool t=true;
            }
        }
        if (!t){
            return new_bool_false();
        }
        return new_bool_true();
    }
    return NULL;
}

object* dict_iter_bool(object* self){
    return new_bool_true();
}

object* dict_keys_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    if (len!=1 || CAST_DICT(kwargs)->val->size() != 0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);

    object* list=new_list();
    for (object* o: *CAST_DICT(self)->keys){
        list_append(list, o);
    }
    return list;
}

object* dict_values_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    if (len!=1 || CAST_DICT(kwargs)->val->size() != 0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);

    object* list=new_list();
    for (object* o: *CAST_DICT(self)->keys){
        list_append(list, CAST_DICT(self)->val->at(o));
    }
    return list;
}

object* dict_flip_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    if (len!=1 || CAST_DICT(kwargs)->val->size() != 0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);
    
    object* dict=new_dict();
    for (object* o: *CAST_DICT(self)->keys){
        dict_set_noret(dict, CAST_DICT(self)->val->at(o), o);
    }
    return dict;
}