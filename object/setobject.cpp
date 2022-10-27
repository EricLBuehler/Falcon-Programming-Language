object* new_set(){
    object* obj=new_object(&SetType);
    CAST_SET(obj)->vec=new vector<object*>;
    CAST_SET(obj)->vec->clear();
    
    return obj;
}


object* set_new(object* type, object* args, object* kwargs){
    if (CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()==0){
        object* obj=new_object(&SetType);
        CAST_SET(obj)->vec=new vector<object*>;
        CAST_SET(obj)->vec->clear();
        
        return obj;
    }
    if (CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()>0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    if (object_istype(list_index_int(args, 0)->type, &SetType)){
        return FPLINCREF(list_index_int(args, 0));
    }
    if (list_index_int(args, 0)->type->slot_iter!=NULL){
        object* o=list_index_int(args, 0);
        object* iter=o->type->slot_iter(o);
        
        if (iter==NULL){
            vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
            return NULL;
        }

        object* obj=new_object(CAST_TYPE(type));
        CAST_SET(obj)->vec=new vector<object*>;
        CAST_SET(obj)->vec->clear();

        object* one=new_int_fromint(0);
        
        o=iter->type->slot_next(iter);
        while (vm->exception==NULL){
            set_append(obj, o);
            CAST_SET(obj)->vec->push_back(o);
            
            o=iter->type->slot_next(iter);
        }
        if (vm->exception!=NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        FPLDECREF(iter);
        FPLDECREF(one);
        return obj;
    }

    //Append
    object* obj=new_object(&SetType);
    CAST_SET(obj)->vec=new vector<object*>;
    CAST_SET(obj)->vec->clear();

    for (size_t i=0; i<CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int(); i++){
        set_append(obj, list_index_int(args, i));
    }
    
    return obj;
}

object* set_bool(object* self){
    return new_bool_true();
}

object* set_len(object* self){
    return new_int_fromint(CAST_SET(self)->vec->size());
}

void set_append(object* self, object* obj){
    if (find(CAST_SET(self)->vec->begin(), CAST_SET(self)->vec->end(), obj)!=CAST_SET(self)->vec->end()){
        return;
    }
    for (object* o: (*CAST_SET(self)->vec)){
        if (istrue(object_cmp(o, obj, CMP_EQ))){
            return;
        }
    }
    CAST_SET(self)->vec->push_back(FPLINCREF(obj));
}

void set_del(object* obj){
    for (object* o: (*CAST_SET(obj)->vec)){
        FPLDECREF(o);
    }
    delete CAST_SET(obj)->vec;
}

object* set_repr(object* self){
    string s="";
    s+="{";
    int i=0;
    for (object* obj: *CAST_SET(self)->vec) {
        s+=object_crepr(obj);
        if (i!=CAST_SET(self)->vec->size()-1){
            s+=", ";
        }
        i++;
    }
    s+="}";
    return str_new_fromstr(s);
}

object* set_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if ((*CAST_SET(self)->vec) == (*CAST_SET(other)->vec)){
            return new_bool_true();
        }
        if (CAST_SET(self)->vec->size() != CAST_SET(other)->vec->size()){
            return new_bool_false();
        }
        
        for (int i = 0; i<CAST_SETITER(self)->vec->size(); i++){
            if (!istrue(object_cmp(CAST_SETITER(self)->vec->at(i), CAST_SETITER(other)->vec->at(i), type))){
                return new_bool_false(); 
            }
        }
        return new_bool_true();
    }
    if (type==CMP_NE){
        if ((*CAST_SET(self)->vec) == (*CAST_SET(other)->vec)){
            return new_bool_false();
        }
        if (CAST_SET(self)->vec->size() != CAST_SET(other)->vec->size()){
            return new_bool_true();
        }
        
        bool t=false;
        for (int i = 0; i<CAST_SETITER(self)->vec->size(); i++){
            if (!istrue(object_cmp(CAST_SETITER(self)->vec->at(i), CAST_SETITER(other)->vec->at(i), type))){
                t=true;
            }
        }
        if (!t){
            return new_bool_false();
        }
        return new_bool_true();
    }
    return NULL;
}

object* set_iter(object* self){
    object* obj=new_object(&SetIterType);
    CAST_SETITER(obj)->vec=new vector<object*>;
    CAST_SETITER(obj)->vec->clear();
    CAST_SETITER(obj)->idx=0;

    for (object* o: (*CAST_SET(self)->vec)){
        CAST_SETITER(obj)->vec->push_back(FPLINCREF(o));
    }
    
    return obj;
}

void set_iter_del(object* self){
    for (object* o: (*CAST_SETITER(self)->vec)){
        FPLDECREF(o);
    }
    delete CAST_SETITER(self)->vec;
}

object* set_iter_next(object* self){
    if (CAST_SETITER(self)->idx+1>CAST_SETITER(self)->vec->size()){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }
    return CAST_SETITER(self)->vec->at(CAST_SETITER(self)->idx++);
}

object* set_iter_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if ((*CAST_SETITER(self)->vec) == (*CAST_SETITER(other)->vec)){
            return new_bool_true();
        }
        if (CAST_SETITER(self)->vec->size() != CAST_SETITER(other)->vec->size()){
            return new_bool_false();
        }

        for (int i = 0; i<CAST_SETITER(self)->vec->size(); i++){
            if (!istrue(object_cmp(CAST_SETITER(self)->vec->at(i), CAST_SETITER(other)->vec->at(i), type))){
                return new_bool_false(); 
            }
        }
        return new_bool_true();
    }
    if (type==CMP_NE){
        if ((*CAST_SETITER(self)->vec) == (*CAST_SETITER(other)->vec)){
            return new_bool_false();
        }
        if (CAST_SETITER(self)->vec->size() != CAST_SETITER(other)->vec->size()){
            return new_bool_true();
        }
        
        bool t=false;
        for (int i = 0; i<CAST_SETITER(self)->vec->size(); i++){
            if (!istrue(object_cmp(CAST_SETITER(self)->vec->at(i), CAST_SETITER(other)->vec->at(i), type))){
                t=true;
            }
        }
        if (!t){
            return new_bool_false();
        }
        return new_bool_true();
    }
    return NULL;
}

object* set_iter_bool(object* self){
    if (CAST_SETITER(self)->idx+1>CAST_SETITER(self)->vec->size()){
        return new_bool_false();
    }
    return new_bool_true();
}

object* set_find_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);  
    object* val=tuple_index_int(args, 1);  

    int i=0;
    for (object* o: (*CAST_SET(self)->vec)){
        if (istrue(object_cmp(o, val, CMP_EQ))){
            return new_int_fromint(i);
        }
        i++;
    }

    return new_int_fromint(-1);
}

object* set_add_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);  
    object* val=tuple_index_int(args, 1);  
    set_append(self, val);
    return new_none();
}

object* set_remove_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);  
    object* val=tuple_index_int(args, 1);  

    for (object* o: ((*CAST_SET(self)->vec))){
        if (istrue(object_cmp(o, val, CMP_EQ))){
            CAST_SET(self)->vec->erase(find(CAST_SET(self)->vec->begin(), CAST_SET(self)->vec->end(), o));
            FPLDECREF(o);
            return new_none();
        }
    }
    
    vm_add_err(&KeyError, vm, "%s", object_crepr(val).c_str());

    return new_none();
}

object* set_union_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);  
    object* other=tuple_index_int(args, 1);  
    if (!object_istype(other->type, &SetType)){
        vm_add_err(&TypeError, vm, "Expected set object, got '%s' object", other->type->name->c_str());
        return NULL; 
    }

    object* set=new_set();
    for (object* o: (*CAST_SET(self)->vec)){
        set_append(set, o);
    }
    for (object* o: (*CAST_SET(other)->vec)){
        set_append(set, o);
    }
    return set;
}