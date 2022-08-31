object* zip_new(object* type, object* args, object* kwargs){
    object* zip=new_object(CAST_TYPE(type));
    uint32_t niterators=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long();
    CAST_ZIP(zip)->idx=0;
    CAST_ZIP(zip)->n_iterators=niterators;
    CAST_ZIP(zip)->iterators=(object**)malloc(sizeof(object*)*niterators);
    for (uint32_t i=0; i<niterators; i++){
        CAST_ZIP(zip)->iterators[i]=INCREF(args->type->slot_mappings->slot_get(args, new_int_fromint(i)));
        if (CAST_ZIP(zip)->iterators[i]->type->slot_iter!=NULL){
            DECREF(CAST_ZIP(zip)->iterators[i]);
            CAST_ZIP(zip)->iterators[i]=INCREF(CAST_ZIP(zip)->iterators[i]->type->slot_iter(CAST_ZIP(zip)->iterators[i]));
        }
        else{
            DECREF(zip);
            vm_add_err(&TypeError, vm, "Expected iterator, got '%s'", CAST_ZIP(zip)->iterators[i]->type->name->c_str());
            DECREF(CAST_ZIP(zip)->iterators[i]);
            return NULL;
        }
    }
    return zip;
}

void zip_del(object* self){
    for (uint32_t i=0; i<CAST_ZIP(self)->n_iterators; i++){
        DECREF(CAST_ZIP(self)->iterators[i]);
    }
}

object* zip_next(object* self){
    object* tup=new_tuple();
    for (uint32_t i=0; i<CAST_ZIP(self)->n_iterators; i++){
        object* o=CAST_ZIP(self)->iterators[i]->type->slot_next(CAST_ZIP(self)->iterators[i]);
        if (o==NULL){
            return NULL;
        }
        tup->type->slot_mappings->slot_append(tup, o);
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