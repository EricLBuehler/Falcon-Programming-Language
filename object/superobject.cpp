object* super_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (len!=1|| CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",len);
        return NULL;
    }

    object* ob=tuple_index_int(args, 0);

    object* super=new_object(CAST_TYPE(type));
    CAST_SUPER(super)->ob=INCREF(ob);
    
    return super;
}

object* super_getattr(object* self, object* attr){
    object* obj=CAST_SUPER(self)->ob;
    if (!object_istype(obj->type, &TypeType)){
        obj=(object*)obj->type;
    }
    object* bases=CAST_TYPE(obj)->bases;
    long len=CAST_TUPLE(bases)->size;
    for (long i=0; i<len; i++){
        object* ob=object_getattr_type(tuple_index_int(bases, i), attr);
        if (ob!=NULL){
            if (ob->type->slot_descrget!=NULL){
                ob=ob->type->slot_descrget(CAST_SUPER(self)->ob, ob);
            }
            return ob;
        }
    }

    vm_add_err(&AttributeError, vm, "'%s' object has no attribute '%s'",CAST_TYPE(CAST_SUPER(self)->ob)->name->c_str(), object_cstr(attr).c_str());
    return NULL;
}
void super_del(object* self){
    DECREF(CAST_SUPER(self)->ob);
}

object* super_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_SUPER(self)->ob,CAST_SUPER(other)->ob, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_SUPER(self)->ob,CAST_SUPER(other)->ob, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}