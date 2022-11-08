object* map_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (!(len>=2) || CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()==0){
        vm_add_err(&ValueError, vm, "Expected at least 2 arguments, got %d", len);
        return NULL;
    }

    object* map=new_object(CAST_TYPE(type));
    uint32_t niterators=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long();
    CAST_MAP(map)->idx=0;
    CAST_MAP(map)->n_iterators=niterators-1;
    CAST_MAP(map)->iterators=(object**)fpl_malloc(sizeof(object*)*(niterators-1));
    object* o=list_index_int(args,0);
    FPLINCREF(o);
    CAST_MAP(map)->func=o;
    
    for (uint32_t i=1; i<niterators; i++){
        object* o=list_index_int(args, i);
        FPLINCREF(o);
        CAST_MAP(map)->iterators[i-1]=o;
        if (CAST_MAP(map)->iterators[i-1]->type->slot_iter!=NULL){
            FPLDECREF(CAST_MAP(map)->iterators[i-1]);
            CAST_MAP(map)->iterators[i-1]=CAST_MAP(map)->iterators[i-1]->type->slot_iter(CAST_MAP(map)->iterators[i-1]);
        }
        else{
            FPLDECREF(map);
            vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", CAST_MAP(map)->iterators[i-1]->type->name->c_str());
            FPLDECREF(CAST_MAP(map)->iterators[i-1]);
            return NULL;
        }
    }
    return map;
}

void map_del(object* self){
    for (uint32_t i=0; i<CAST_MAP(self)->n_iterators; i++){
        FPLDECREF(CAST_MAP(self)->iterators[i]);
    }
}

object* map_next(object* self){
    object* tup=new_tuple();
    object* one=new_int_fromint(0);
    object* v;
    for (uint32_t i=0; i<CAST_MAP(self)->n_iterators; i++){
        object* o=CAST_MAP(self)->iterators[i]->type->slot_next(CAST_MAP(self)->iterators[i]);
        ERROR_RET(o);
        
        if (o->type->slot_mappings->slot_len==NULL){
            v=o;
            goto cont;
        }
        if (o->type->slot_mappings==NULL || o->type->slot_mappings->slot_get==NULL){
            FPLDECREF(one);
            vm_add_err(&TypeError, vm, "'%s' object is not subscriptable", o->type->name->c_str());
            return NULL;
        }
        v=o->type->slot_mappings->slot_get(o, one);
        ERROR_RET(v);
        
        cont:
        object* args=new_tuple();
        tuple_append(args, v);
        object* ret=object_call_nokwargs(CAST_MAP(self)->func, args);
        FPLDECREF(args);
        if (CAST_MAP(self)->n_iterators==1){
            FPLDECREF(one);
            FPLDECREF(tup);
            return ret;
        }
        tup->type->slot_mappings->slot_append(tup, ret);
    }
    FPLDECREF(one);
    return tup;
}

object* map_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_MAP(self)->n_iterators != CAST_MAP(self)->n_iterators){
            return new_bool_false();
        }

        for (uint32_t i=0; i<CAST_MAP(self)->n_iterators; i++){
            if (CAST_MAP(self)->iterators[i]==CAST_MAP(other)->iterators[i]){
                return new_bool_false();
            }
        }
        return new_bool_true();
    }
    if (type==CMP_NE){
        if (CAST_MAP(self)->n_iterators != CAST_MAP(self)->n_iterators){
            return new_bool_true();
        }

        bool t=false;
        for (uint32_t i=0; i<CAST_MAP(self)->n_iterators; i++){
            if (!istrue(object_cmp(CAST_MAP(self)->iterators[i],CAST_MAP(other)->iterators[i], CMP_EQ))){
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