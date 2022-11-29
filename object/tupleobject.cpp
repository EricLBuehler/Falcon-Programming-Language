void tuple_resize(TupleObject* obj, size_t size){
    obj->capacity=(size + (size >> 3) + 6) & ~3;
    
    obj->array=(object**)fpl_realloc(obj->array, obj->capacity * sizeof(struct object*));
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
    if (CAST_LIST(args)->size==0){
        object_var* obj=new_object_var(CAST_TYPE(type), sizeof(TupleObject)+2*sizeof(object*));
        CAST_TUPLE(obj)->capacity=2; //Start with 2
        CAST_TUPLE(obj)->size=0;
        CAST_TUPLE(obj)->idx=0;
        CAST_TUPLE(obj)->array=(object**)fpl_malloc((CAST_TUPLE(obj)->capacity * sizeof(struct object*)));
        
        return (object*)obj;
    }
    if (CAST_DICT(kwargs)->val->size()>0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (object_istype(list_index_int(args, 0)->type, &TupleType)){
        object* o=list_index_int(args, 0);
        FPLINCREF(o);
        return o;
    }
    if (list_index_int(args, 0)->type->slot_iter!=NULL){
        object* o=list_index_int(args, 0);
        object* iter=o->type->slot_iter(o);
        
        if (iter==NULL){
            vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
            return NULL;
        }

        object_var* obj=new_object_var(CAST_TYPE(type), sizeof(TupleObject)+2*sizeof(object*));
        CAST_TUPLE(obj)->capacity=2; //Start with 2
        CAST_TUPLE(obj)->size=0;
        CAST_TUPLE(obj)->idx=0;
        CAST_TUPLE(obj)->array=(object**)fpl_malloc((CAST_TUPLE(obj)->capacity * sizeof(struct object*)));
        object* one=new_int_fromint(0);
        
        o=iter->type->slot_next(iter);
        while (vm->exception==NULL){
            tuple_append_noinc((object*)obj, o);
            
            o=iter->type->slot_next(iter);
        }
        if (vm->exception!=NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        FPLDECREF(iter);
        FPLDECREF(one);
        return (object*)obj;
    }

    //Append
    object_var* obj=new_object_var(CAST_TYPE(type), sizeof(TupleObject)+2*sizeof(object*));
    CAST_TUPLE(obj)->capacity=2; //Start with 2
    CAST_TUPLE(obj)->size=0;
    CAST_TUPLE(obj)->idx=0;
    CAST_TUPLE(obj)->array=(object**)fpl_malloc((CAST_TUPLE(obj)->capacity * sizeof(struct object*)));

    for (size_t i=0; i<CAST_LIST(args)->size; i++){
        object* o=list_index_int(args, i);
        tuple_append((object*)obj, o);
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
    if (CAST_TUPLE(self)->size==0){
        FPLINCREF(self);
        return self;
    }
    object* start=CAST_SLICE(idx)->start;
    object* end=CAST_SLICE(idx)->end;
    object* step=CAST_SLICE(idx)->step;

    int start_v;
    int end_v;
    int step_v;
    if (object_istype(start->type, &NoneType)){
        start_v=0;
    }
    else{
        object* start_=object_int(start);
        if (start_==NULL || !object_istype(start_->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int",start->type->name->c_str());
            return NULL;
        }
        if (*CAST_INT(start_)->val>LONG_MAX || *CAST_INT(start_)->val<LONG_MIN){
            vm_add_err(&OverflowError, vm, "Value out of range of C long");
            return NULL;
        }
        start_v=CAST_INT(start_)->val->to_long();
        FPLDECREF(start_);
    }
    if (object_istype(end->type, &NoneType)){
        end_v=CAST_LIST(self)->size-1;
    }
    else{
        object* end_=object_int(end);
        if (end_==NULL || !object_istype(end_->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int",start->type->name->c_str());
            return NULL;
        }
        if (*CAST_INT(end_)->val>LONG_MAX || *CAST_INT(end_)->val<LONG_MIN){
            vm_add_err(&OverflowError, vm, "Value out of range of C long");
            return NULL;
        }
        end_v=CAST_INT(end_)->val->to_long();
        FPLDECREF(end_);
    }
    
    if (object_istype(step->type, &NoneType)){
        step_v=1;
    }
    else{
        object* step_=object_int(step);
        if (step_==NULL || !object_istype(step_->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int",step_->type->name->c_str());
            return NULL;
        }
        if (*CAST_INT(step_)->val>LONG_MAX || *CAST_INT(step_)->val<LONG_MIN || *CAST_INT(step_)->val<0){
            vm_add_err(&IndexError, vm, "Value out of range of C long");
            return NULL;
        }
        step_v=CAST_INT(step_)->val->to_long();
        FPLDECREF(step_);
    }

    if (start_v<0){
        start_v=CAST_LIST(self)->size+start_v;
        if (start_v<0){
            vm_add_err(&IndexError, vm, "Index out of range");
            return NULL;
        }
    }
    if (start_v>=CAST_LIST(self)->size){
        start_v=CAST_LIST(self)->size-1;
    }
    if (end_v<0){
        end_v=CAST_LIST(self)->size+start_v;
        if (end_v<0){
            vm_add_err(&IndexError, vm, "Index out of range");
            return NULL;
        }
    }
    if (end_v>=CAST_LIST(self)->size){
        end_v=CAST_LIST(self)->size-1;
    }

    object* result=new_list();
    
    for (int i=start_v; i<=end_v; i++){
        list_append(result, tuple_index_int(self, i));
    }
    return result;
}

object* tuple_get(object* self, object* idx){
    if (object_istype(idx->type, &SliceType)){
        return list_slice(self, idx);
    }
    object* idx_=object_int(idx);
    if (idx_==NULL || !object_istype(idx->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int",idx->type->name->c_str());
        FPLDECREF(idx_);
        return NULL;
    }
    if (*CAST_INT(idx_)->val>LONG_MAX || *CAST_INT(idx_)->val<LONG_MIN){
        vm_add_err(&OverflowError, vm, "Value out of range of C long");
        FPLDECREF(idx_);
        return NULL;
    }
    long lidx=CAST_INT(idx_)->val->to_long();
    if (lidx<0){
        lidx=lidx+CAST_TUPLE(self)->size;
    }
    if ((int)CAST_TUPLE(self)->size<=lidx || lidx<0){
        vm_add_err(&IndexError, vm, "Tuple index out of range");
        FPLDECREF(idx_);
        return NULL;
    }
    FPLDECREF(idx_);
    
    FPLINCREF(CAST_TUPLE(self)->array[lidx]);
    return CAST_TUPLE(self)->array[lidx];
}

void tuple_append(object* self, object* obj){
    if (CAST_TUPLE(self)->size+1 == CAST_TUPLE(self)->capacity){ //Alloc more space
        tuple_resize(CAST_TUPLE(self), CAST_TUPLE(self)->size+1);
    }

    FPLINCREF(obj);
    CAST_TUPLE(self)->array[CAST_TUPLE(self)->size++]=obj;

    ((object_var*)self)->var_size=sizeof(TupleObject)+CAST_TUPLE(self)->size;
}

void tuple_append_noinc(object* self, object* obj){
    if (CAST_TUPLE(self)->size+1 == CAST_TUPLE(self)->capacity){ //Alloc more space
        tuple_resize(CAST_TUPLE(self), CAST_TUPLE(self)->size+1);
    }

    CAST_TUPLE(self)->array[CAST_TUPLE(self)->size++]=obj;

    ((object_var*)self)->var_size=sizeof(TupleObject)+CAST_TUPLE(self)->size;
}

void tuple_del(object* obj){
    for (size_t i=0; i<CAST_TUPLE(obj)->size; i++){
        FPLDECREF(CAST_TUPLE(obj)->array[i]);
    }
    fpl_free(((TupleObject*)obj)->array);
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
    if (type==CMP_IN){
        return object_in_iter(other, self);
    }
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
        object* o=CAST_TUPLE(self)->array[i];
        FPLINCREF(o);
        CAST_TUPLEITER(iter)->array[i]=o;
    }
    return iter;
}

void tuple_iter_del(object* self){
    for (size_t i=0; i<CAST_TUPLEITER(self)->size; i++){
        FPLDECREF(CAST_TUPLEITER(self)->array[i]);
    }
    fpl_free(CAST_TUPLEITER(self)->array);
}

object* tuple_iter_next(object* self){
    if (CAST_TUPLEITER(self)->idx+1>CAST_TUPLEITER(self)->size){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }
    FPLINCREF(CAST_TUPLEITER(self)->array[CAST_TUPLEITER(self)->idx]);
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
    if (*CAST_INT(other)->val>LONG_MAX || *CAST_INT(other)->val<LONG_MIN){
        vm_add_err(&OverflowError, vm, "Value out of range of C long");
        return NULL;
    }    
    long nrepeat=CAST_INT(other)->val->to_long();
    size_t orig_size=CAST_TUPLE(self)->size;
    CAST_TUPLE(self)->size*=nrepeat;
    
    tuple_resize(CAST_TUPLE(self), CAST_TUPLE(self)->size);
    
    for (int i=1; i<nrepeat; i++){
        for (int a=0; a<orig_size; a++){
            object* o=CAST_TUPLE(self)->array[a];
            FPLINCREF(o);
            CAST_TUPLE(self)->array[a+(i*orig_size)]=o;
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
        object* o=CAST_TUPLE(self)->array[i];
        FPLINCREF(o);
        CAST_TUPLE(tup)->array[i]=o;
    }
    for (size_t i=CAST_TUPLE(self)->size; i<CAST_TUPLE(tup)->size; i++){
        object* o=CAST_TUPLE(other)->array[i-CAST_TUPLE(self)->size];
        FPLINCREF(o);
        CAST_TUPLE(tup)->array[i]=o;
    }
    return tup;
}

object* tuple_find_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
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