object* zip_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (!(len>=1)){
        vm_add_err(&ValueError, vm, "Expected at least 1 argument, got %d", len);
        return NULL;
    }

    object* zip=new_object(CAST_TYPE(type));
    uint32_t niterators=CAST_LIST(args)->size;
    CAST_ZIP(zip)->idx=0;
    CAST_ZIP(zip)->n_iterators=niterators;
    CAST_ZIP(zip)->iterators=(object**)fpl_malloc(sizeof(object*)*niterators);
    for (uint32_t i=0; i<niterators; i++){
        object* o=list_index_int(args, i);
        FPLINCREF(o);
        CAST_ZIP(zip)->iterators[i]=o;
        if (CAST_ZIP(zip)->iterators[i]->type->slot_iter!=NULL){
            FPLDECREF(CAST_ZIP(zip)->iterators[i]);
            CAST_ZIP(zip)->iterators[i]=CAST_ZIP(zip)->iterators[i]->type->slot_iter(CAST_ZIP(zip)->iterators[i]);
        }
        else{
            FPLDECREF(zip);
            vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", CAST_ZIP(zip)->iterators[i]->type->name->c_str());
            FPLDECREF(CAST_ZIP(zip)->iterators[i]);
            return NULL;
        }
    }
    return zip;
}

void zip_del(object* self){
    for (uint32_t i=0; i<CAST_ZIP(self)->n_iterators; i++){
        FPLDECREF(CAST_ZIP(self)->iterators[i]);
    }
}

object* zip_next(object* self){
    object* tup=new_tuple();
    for (uint32_t i=0; i<CAST_ZIP(self)->n_iterators; i++){
        object* o=CAST_ZIP(self)->iterators[i]->type->slot_next(CAST_ZIP(self)->iterators[i]);
        if (o==NULL){
            return NULL;
        }
        if (CAST_ZIP(self)->n_iterators==1){
            FPLDECREF(tup);
            return o;
        }
        tuple_append_noinc(tup, o);
    }
    return tup;
}

object* zip_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_ZIP(self)->n_iterators != CAST_ZIP(self)->n_iterators){
            return new_bool_false();
        }

        for (uint32_t i=0; i<CAST_ZIP(self)->n_iterators; i++){
            if (CAST_ZIP(self)->iterators[i]==CAST_ZIP(other)->iterators[i]){
                return new_bool_false();
            }
        }
        return new_bool_true();
    }
    if (type==CMP_NE){
        if (CAST_ZIP(self)->n_iterators != CAST_ZIP(self)->n_iterators){
            return new_bool_true();
        }

        bool t=false;
        for (uint32_t i=0; i<CAST_ZIP(self)->n_iterators; i++){
            if (!istrue(object_cmp(CAST_ZIP(self)->iterators[i],CAST_ZIP(other)->iterators[i], CMP_EQ))){
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