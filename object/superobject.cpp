object* super_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if ((len!=1 && len!=2) || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 or 2 arguments, got %d",len);
        return NULL;
    }

    object* ob=tuple_index_int(args, 0);
    if (object_istype(ob->type, &SuperType)){
        return super_call(type, args, kwargs);
    }

    if (!object_istype(ob->type, &TypeType)){
        ob=(object*)ob->type;
    }

    object* super=new_object(CAST_TYPE(type));
    CAST_SUPER(super)->ob=INCREF(ob);
    CAST_SUPER(super)->attr=NULL;
    if (len==2){
        CAST_SUPER(super)->attr=INCREF(tuple_index_int(args, 0));
    }
    
    return super;
}

object* super_getattr(object* self, object* attr){
    object* bases=CAST_TYPE(CAST_SUPER(self)->ob)->bases;
    long len=CAST_TUPLE(bases)->size;
    for (long i=0; i<len; i++){
        object* ob=object_getattr_type(tuple_index_int(bases, i), attr);
        if (ob!=NULL){
            if (ob->type->slot_offsetget!=NULL){
                ob=ob->type->slot_offsetget(tuple_index_int(bases, i), ob);
            }
            CAST_SUPER(self)->attr=INCREF(ob);
            return self;
        }
    }

    vm_add_err(&AttributeError, vm, "'%s' object has no attribute '%s'",CAST_TYPE(CAST_SUPER(self)->ob)->name->c_str(), object_cstr(attr).c_str());
    return NULL;
}

object* super_call(object* self, object* args, object* kwargs){
    return CAST_SUPER(self)->attr->type->slot_call(CAST_SUPER(self)->attr, args, kwargs);
}

void super_del(object* self){
    DECREF(CAST_SUPER(self)->ob);
    if (CAST_SUPER(self)->attr!=NULL){
        DECREF(CAST_SUPER(self)->attr);
    }
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