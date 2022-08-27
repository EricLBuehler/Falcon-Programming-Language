object* slice_new_fromnums(object* start, object* end){
    object* slice=new_object(&SliceType);
    CAST_SLICE(slice)->start=INCREF(start);
    CAST_SLICE(slice)->end=INCREF(end);
    return slice;
}

object* slice_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got '%d'",len);
        return NULL;
    }
    object* slice=new_object(CAST_TYPE(type));
    CAST_SLICE(slice)->start=INCREF(args->type->slot_mappings->slot_get(args, new_int_fromint(0)));
    CAST_SLICE(slice)->end=INCREF(args->type->slot_mappings->slot_get(args, new_int_fromint(1)));
    return slice;
}

void slice_del(object* self){
    DECREF(CAST_SLICE(self)->start);
    DECREF(CAST_SLICE(self)->end);
}

object* slice_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="";
    s+=self->type->name->c_str();
    s+="(";
    s+=object_cstr(CAST_SLICE(self)->start);
    s+=", ";
    s+=object_cstr(CAST_SLICE(self)->end);
    s+=")";
    return str_new_fromstr(s);
}

object* slice_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_SLICE(self)->start, CAST_SLICE(other)->start, CMP_EQ))\
        && istrue(object_cmp(CAST_SLICE(self)->end, CAST_SLICE(other)->end, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_SLICE(self)->start, CAST_SLICE(other)->start, CMP_EQ))\
        || !istrue(object_cmp(CAST_SLICE(self)->end, CAST_SLICE(other)->end, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}