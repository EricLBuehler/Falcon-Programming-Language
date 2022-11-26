object* enum_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1 || CAST_LIST(args)->size==0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }

    object* enumer=new_object(CAST_TYPE(type));
    object* o=list_index_int(args, 0);
    FPLINCREF(o);
    CAST_ENUM(enumer)->iterator=o;
    if (CAST_ENUM(enumer)->iterator->type->slot_iter!=NULL){
        FPLDECREF(CAST_ENUM(enumer)->iterator);
        CAST_ENUM(enumer)->iterator=CAST_ENUM(enumer)->iterator->type->slot_iter(CAST_ENUM(enumer)->iterator);
 
        if (CAST_ENUM(enumer)->iterator==NULL){
            vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", CAST_ENUM(enumer)->iterator->type->name->c_str());
            return NULL;
        }
    }
    else{
        FPLDECREF(enumer);
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", CAST_ENUM(enumer)->iterator->type->name->c_str());
        FPLDECREF(CAST_ENUM(enumer)->iterator);
        return NULL;
    }
    CAST_ENUM(enumer)->idx=0;
    return enumer;
}

void enum_del(object* self){
    FPLDECREF(CAST_ENUM(self)->iterator);
}

object* enum_next(object* self){
    object* next=CAST_ENUM(self)->iterator->type->slot_next(CAST_ENUM(self)->iterator);
    if (next==NULL){
        return NULL;
    }
    object* idx=new_int_fromint(CAST_ENUM(self)->idx++);

    object* tup=new_tuple();
    tuple_append_noinc(tup, idx);
    tuple_append_noinc(tup, next);
    return tup;
}

object* enum_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_ENUM(self)->iterator, CAST_ENUM(other)->iterator, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_ENUM(self)->iterator, CAST_ENUM(other)->iterator, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}