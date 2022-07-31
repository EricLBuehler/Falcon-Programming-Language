void tuple_resize(TupleObject* obj, size_t size){
    object** buf=(object**)malloc(obj->capacity * sizeof(struct object*));
    memcpy(buf, obj->array, obj->capacity * sizeof(struct object*));
    size_t oldcap=obj->capacity;

    obj->capacity=(size + (size >> 3) + 6) & ~3;
    
    obj->array=(object**)realloc(obj->array, obj->capacity * sizeof(struct object*));
    memcpy(obj->array, buf, oldcap * sizeof(struct object*));
    free(buf);
}


object* new_tuple(){
    object_var* obj=new_object_var(&TupleType, sizeof(TupleObject)+2*sizeof(object*));
    CAST_TUPLE(obj)->capacity=2; //Start with 2
    CAST_TUPLE(obj)->size=0;
    CAST_TUPLE(obj)->idx=0;
    CAST_TUPLE(obj)->array=(object**)malloc((CAST_TUPLE(obj)->capacity * sizeof(struct object*)));
    
    return (object*)obj;
}

object* tuple_new(object* type, object* args, object* kwargs){
    if (CAST_INT(args->type->slot_len(args))->val->to_int()==0){
        object_var* obj=new_object_var(&TupleType, sizeof(TupleObject)+2*sizeof(object*));
        CAST_TUPLE(obj)->capacity=2; //Start with 2
        CAST_TUPLE(obj)->size=0;
        CAST_TUPLE(obj)->idx=0;
        CAST_TUPLE(obj)->array=(object**)malloc((CAST_TUPLE(obj)->capacity * sizeof(struct object*)));
        
        return (object*)obj;
    }
    if (object_istype(args->type->slot_get(args, new_int_fromint(0))->type, &TupleType)){
        return INCREF(args->type->slot_get(args, new_int_fromint(0)));
    }

    //Append
    object_var* obj=new_object_var(&TupleType, sizeof(TupleObject)+2*sizeof(object*));
    CAST_TUPLE(obj)->capacity=2; //Start with 2
    CAST_TUPLE(obj)->size=0;
    CAST_TUPLE(obj)->idx=0;
    CAST_TUPLE(obj)->array=(object**)malloc((CAST_TUPLE(obj)->capacity * sizeof(struct object*)));

    for (size_t i=0; i<CAST_INT(args->type->slot_len(args))->val->to_int(); i++){
        tuple_append((object*)obj, INCREF(args->type->slot_get(args, new_int_fromint(i))));
    }
    
    return (object*)obj;
}

object* tuple_bool(object* self){
    return new_bool_true();
}

object* tuple_len(object* self){
    return new_int_fromint(CAST_TUPLE(self)->size);
}

object* tuple_get(object* self, object* idx){
    if (!object_istype(idx->type, &IntType)){
        vm_add_err(&TypeError, vm, "List must be indexed by int not '%s'",idx->type->name->c_str());
        return (object*)0x1;
    }
    if (CAST_TUPLE(self)->size<=CAST_INT(idx)->val->to_long_long()){
        vm_add_err(&IndexError, vm, "List index out of range");
        return (object*)0x1;
    }

    return CAST_TUPLE(self)->array[CAST_INT(idx)->val->to_long_long()];
}

void tuple_append(object* self, object* obj){
    if (CAST_TUPLE(self)->size+1 == CAST_TUPLE(self)->capacity){ //Alloc more space
        tuple_resize(CAST_TUPLE(self), CAST_TUPLE(self)->size+1);
    }

    CAST_TUPLE(self)->array[CAST_TUPLE(self)->size++]=INCREF(obj);

    ((object_var*)self)->var_size=sizeof(TupleObject)+CAST_TUPLE(self)->size;
}

void tuple_del(object* obj){
    for (size_t i=0; i<CAST_TUPLE(obj)->size; i++){
        DECREF(CAST_TUPLE(obj)->array[i]);
    }
    free(((TupleObject*)obj)->array);
}

object* tuple_repr(object* self){
    string s="";
    s+="(";
    for (int i=0; i<CAST_TUPLE(self)->size; i++){
        s+=object_crepr(CAST_TUPLE(self)->array[i]);
        if (i!=CAST_TUPLE(self)->size-1){
            s+=", ";
        }
    }
    s+=")";
    return str_new_fromstr(new string(s));
}

object* tuple_next(object* self){
    if (CAST_TUPLE(self)->idx+1>CAST_TUPLE(self)->size){
        CAST_TUPLE(self)->idx=0;
        return NULL;
    }
    return CAST_TUPLE(self)->array[CAST_TUPLE(self)->idx++];
}

object* tuple_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
    }
    if (type==CMP_EQ){
        if (CAST_TUPLE(self)->size != CAST_TUPLE(other)->size){
            return new_bool_false();
        }
        
        for (size_t i=0; i<CAST_TUPLE(self)->size; i++){
            if (!istrue(object_cmp(CAST_TUPLE(self)->array[i], CAST_TUPLE(other)->array[i], type))){
                return new_bool_false(); 
            }
        }
        return new_bool_true();
    }
    return new_bool_false();
}