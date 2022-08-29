object* range_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=2 && len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 or 2 arguments, got %d", len);
        return NULL;
    }
    object* range=new_object(CAST_TYPE(type));
    object* arg=object_int(args->type->slot_mappings->slot_get(args, new_int_fromint(0)));
    if (arg==NULL){
        DECREF(range);
        return arg;
    }
    CAST_RANGE(range)->start=CAST_INT(arg)->val->to_int();
    if (len!=1){
        arg=object_int(args->type->slot_mappings->slot_get(args, new_int_fromint(1)));
        if (arg==NULL){
            DECREF(range);
            return arg;
        }
        CAST_RANGE(range)->end=CAST_INT(arg)->val->to_int();
    }
    else{
        CAST_RANGE(range)->end=CAST_RANGE(range)->start;
        CAST_RANGE(range)->start=0;
    }

    CAST_RANGE(range)->idx=0;
    return range;
}

void range_del(object* self){}

object* range_next(object* self){
    if (CAST_RANGE(self)->idx+1>CAST_RANGE(self)->end){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }
    return new_int_fromint(CAST_RANGE(self)->idx++);
}

object* range_cmp(object* self, object* other, uint8_t type){
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