void tuple_resize(TupleObject* obj, size_t size){
    object** buf=(object**)fpl_malloc(obj->capacity * sizeof(struct object*));
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
    CAST_TUPLE(obj)->array=(object**)fpl_malloc((CAST_TUPLE(obj)->capacity * sizeof(struct object*)));
    
    return (object*)obj;
}


object* tuple_new(object* type, object* args, object* kwargs){
    if (CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()==0){
        object_var* obj=new_object_var(CAST_TYPE(type), sizeof(TupleObject)+2*sizeof(object*));
        CAST_TUPLE(obj)->capacity=2; //Start with 2
        CAST_TUPLE(obj)->size=0;
        CAST_TUPLE(obj)->idx=0;
        CAST_TUPLE(obj)->array=(object**)fpl_malloc((CAST_TUPLE(obj)->capacity * sizeof(struct object*)));
        
        return (object*)obj;
    }
    if (CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()>0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    if (object_istype(list_index_int(args, 0)->type, &TupleType)){
        return INCREF(list_index_int(args, 0));
    }
    if (list_index_int(args, 0)->type->slot_iter!=NULL){
        object* o=list_index_int(args, 0);
        object* iter=o->type->slot_iter(o);

        object_var* obj=new_object_var(CAST_TYPE(type), sizeof(TupleObject)+2*sizeof(object*));
        CAST_TUPLE(obj)->capacity=2; //Start with 2
        CAST_TUPLE(obj)->size=0;
        CAST_TUPLE(obj)->idx=0;
        CAST_TUPLE(obj)->array=(object**)fpl_malloc((CAST_TUPLE(obj)->capacity * sizeof(struct object*)));

        o=iter->type->slot_next(iter);
        while (vm->exception==NULL){
            if (o->type->slot_mappings->slot_len!=NULL && *CAST_INT(o->type->slot_mappings->slot_len(o))->val!=1){
                DECREF((object*)obj);
                vm_add_err(&ValueError, vm, "Expected 1 value for list update, got '%d'",CAST_INT(o->type->slot_mappings->slot_len(o))->val->to_int());
                return NULL;
            }
            tuple_append((object*)obj, o);
            
            o=iter->type->slot_next(iter);
        }
        if (vm->exception!=NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        return (object*)obj;
    }

    //Append
    object_var* obj=new_object_var(CAST_TYPE(type), sizeof(TupleObject)+2*sizeof(object*));
    CAST_TUPLE(obj)->capacity=2; //Start with 2
    CAST_TUPLE(obj)->size=0;
    CAST_TUPLE(obj)->idx=0;
    CAST_TUPLE(obj)->array=(object**)fpl_malloc((CAST_TUPLE(obj)->capacity * sizeof(struct object*)));

    for (size_t i=0; i<CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int(); i++){
        tuple_append((object*)obj, INCREF(list_index_int(args, i)));
    }
    
    return (object*)obj;
}

object* tuple_bool(object* self){
    return new_bool_true();
}

object* tuple_len(object* self){
    return new_int_fromint(CAST_TUPLE(self)->size);
}

object* tuple_slice(object* self, object* idx){
    object* start=CAST_SLICE(idx)->start;
    object* end=CAST_SLICE(idx)->end;

    object* result=new_list();

    int start_v;
    int end_v;
    if (object_istype(start->type, &NoneType)){
        start_v=0;
    }
    else{
        start_v=CAST_INT(start)->val->to_int();
    }
    if (object_istype(end->type, &NoneType)){
        end_v=CAST_TUPLE(self)->size-1;
    }
    else{
        end_v=CAST_INT(end)->val->to_int();
    }
    if (start<0){
        start_v=0;
    }
    if (end_v>=CAST_TUPLE(self)->size){
        end_v=CAST_TUPLE(self)->size-1;
    }
    for (int i=start_v; i<=end_v; i++){
        list_append(result, tuple_index_int(self, i));
    }
    return result;
}

object* tuple_get(object* self, object* idx){
    if (object_istype(idx->type, &SliceType)){
        return list_slice(self, idx);
    }
    if (!object_istype(idx->type, &IntType)){
        vm_add_err(&TypeError, vm, "List must be indexed by int not '%s'",idx->type->name->c_str());
        return NULL;
    }
    if (CAST_TUPLE(self)->size<=CAST_INT(idx)->val->to_long_long() || CAST_INT(idx)->val->to_long_long()<0){
        vm_add_err(&IndexError, vm, "List index out of range");
        return NULL;
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
    return str_new_fromstr(s);
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
        return NULL;
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
    if (type==CMP_NE){
        if (CAST_TUPLE(self)->size != CAST_TUPLE(other)->size){
            return new_bool_true();
        }

        bool t=false;
        for (size_t i=0; i<CAST_TUPLE(self)->size; i++){
            if (!istrue(object_cmp(CAST_TUPLE(self)->array[i], CAST_TUPLE(other)->array[i], type))){
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


object* tuple_iter(object* self){
    //Make an iterator
    object* iter=(object*)new_object_var(&ListIterType, sizeof(ListIterObject)+CAST_TUPLE(self)->capacity*sizeof(object*));
    CAST_TUPLEITER(iter)->capacity=CAST_TUPLE(self)->capacity;
    CAST_TUPLEITER(iter)->size=CAST_TUPLE(self)->size;
    CAST_TUPLEITER(iter)->idx=0;
    CAST_TUPLEITER(iter)->array=(object**)fpl_malloc(CAST_TUPLEITER(iter)->capacity * sizeof(struct object*));
    for (size_t i=0; i<CAST_TUPLE(self)->size; i++){
        CAST_TUPLEITER(iter)->array[i]=INCREF(CAST_TUPLE(self)->array[i]);
    }
    return iter;
}

void tuple_iter_del(object* self){
    for (size_t i=0; i<CAST_TUPLEITER(self)->size; i++){
        DECREF(CAST_TUPLEITER(self)->array[i]);
    }
    free(CAST_TUPLEITER(self)->array);
}

object* tuple_iter_next(object* self){
    if (CAST_TUPLEITER(self)->idx+1>CAST_TUPLEITER(self)->size){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }
    return CAST_TUPLEITER(self)->array[CAST_TUPLEITER(self)->idx++];
}

object* tuple_iter_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_TUPLEITER(self)->size != CAST_TUPLEITER(other)->size){
            return new_bool_false();
        }
        for (size_t i=0; i<CAST_TUPLEITER(self)->size; i++){
            if (!istrue(object_cmp(CAST_TUPLEITER(self)->array[i], CAST_TUPLEITER(other)->array[i], type))){
                return new_bool_false(); 
            }
        }
        return new_bool_true();
    }
    if (type==CMP_NE){
        bool t=false;
        for (size_t i=0; i<CAST_TUPLEITER(self)->size; i++){
            if (!istrue(object_cmp(CAST_TUPLEITER(self)->array[i], CAST_TUPLEITER(other)->array[i], type))){
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

object* tuple_iter_bool(object* self){
    if (CAST_TUPLEITER(self)->idx+1>CAST_TUPLEITER(self)->size){
        return new_bool_false();
    }
    return new_bool_true();
}


object* tuple_mul(object* self, object* other){
    if (other->type!=&IntType){
        vm_add_err(&TypeError, vm, "Invalid operand types for *: '%s', and '%s'.", self->type->name->c_str(), other->type->name->c_str());
        return NULL;
    }
    long nrepeat=CAST_INT(other)->val->to_long();
    size_t orig_size=CAST_TUPLE(self)->size;
    CAST_TUPLE(self)->size*=nrepeat;
    
    tuple_resize(CAST_TUPLE(self), CAST_TUPLE(self)->size);
        
    for (int i=1; i<nrepeat; i++){
        for (int a=0; a<orig_size; a++){
            CAST_TUPLE(self)->array[a+(i*orig_size)]=INCREF(CAST_TUPLE(self)->array[a]);
        }
    }
    return self;
}

object* tuple_add(object* self, object* other){
    if (other->type!=&TupleType){
        vm_add_err(&TypeError, vm, "Invalid operand types for +: '%s', and '%s'.", self->type->name->c_str(), other->type->name->c_str());
        return NULL;
    }

    object* tup=new_tuple();
    CAST_TUPLE(tup)->size=CAST_TUPLE(self)->size+CAST_TUPLE(other)->size;
    tuple_resize(CAST_TUPLE(tup), CAST_TUPLE(tup)->size);

    for (size_t i=0; i<CAST_TUPLE(self)->size; i++){
        CAST_TUPLE(tup)->array[i]=INCREF(CAST_TUPLE(self)->array[i]);
    }
    for (size_t i=CAST_TUPLE(self)->size; i<CAST_TUPLE(tup)->size; i++){
        CAST_TUPLE(tup)->array[i]=INCREF(CAST_TUPLE(other)->array[i-CAST_TUPLE(self)->size]);
    }
    return tup;
}

object* tuple_find_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);  
    object* val=tuple_index_int(args, 1);  

    for (size_t i=0; i<CAST_TUPLE(self)->size; i++){
        if (istrue(object_cmp(CAST_TUPLE(self)->array[i], val, CMP_EQ))){
            return new_int_fromint(i);
        }
    }

    return new_int_fromint(-1);
}