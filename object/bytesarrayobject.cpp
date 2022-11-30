object* new_bytesarray(){
    object* obj=new_object(&BytesarrayType);
    CAST_BYTESARRAY(obj)->size=0;
    CAST_BYTESARRAY(obj)->array=(char*)fpl_malloc(1);
    
    return obj;
}

object* bytesarray_new(object* type, object* args, object* kwargs){
    if (CAST_LIST(args)->size==0){
        return new_bytesarray();
    }
    if (CAST_DICT(kwargs)->val->size()>0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (object_istype(list_index_int(args, 0)->type, &BytesarrayType)){
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

        object* obj=new_bytesarray();
        
        o=iter->type->slot_next(iter);
        while (vm->exception==NULL){
            object* ob=object_int(o);
            if (ob==NULL || !object_istype(ob->type, &IntType)){
                vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", o->type->name->c_str());
                return NULL;
            }

            if (*CAST_INT(ob)->val<INT_MIN || *CAST_INT(ob)->val>INT_MAX){
                vm_add_err(&OverflowError, vm, "Value out of range of C int");
                return NULL; 
            }

            int i=CAST_INT(ob)->val->to_int();
            FPLDECREF(ob);
            FPLDECREF(o);
                    
            if (i>255 || i<0){
                vm_add_err(&ValueError, vm, "Expected value in range(0, 256), got %d", i);
                return NULL;
            }

            bytesarray_append(obj, i);
            
            o=iter->type->slot_next(iter);
        }
        if (vm->exception!=NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        FPLDECREF(iter);
        return obj;
    }

    //Append
    object* obj=new_bytesarray();

    for (size_t i=0; i<CAST_LIST(args)->size; i++){
        object* o=list_index_int(args, i);
        object* ob=object_int(o);
        if (ob==NULL || !object_istype(ob->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", o->type->name->c_str());
            return NULL;
        }

        if (*CAST_INT(ob)->val<INT_MIN || *CAST_INT(ob)->val>INT_MAX){
            vm_add_err(&OverflowError, vm, "Value out of range of C int");
            return NULL; 
        }

        int v=CAST_INT(ob)->val->to_int();
        FPLDECREF(ob);
                
        if (v>255 || v<0){
            vm_add_err(&ValueError, vm, "Expected value in range(0, 256), got %d", v);
            return NULL;
        }
            
        bytesarray_append(obj, v);
    }
    
    return obj;
}

object* bytesarray_bool(object* self){
    return new_bool_true();
}

object* bytesarray_len(object* self){
    return new_int_fromint(CAST_BYTESARRAY(self)->size);
}

object* bytesarray_slice(object* self, object* idx){
    if (CAST_BYTESARRAY(self)->size==0){
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
        end_v=CAST_BYTESARRAY(self)->size-1;
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
        start_v=CAST_BYTESARRAY(self)->size+start_v;
        if (start_v<0){
            vm_add_err(&IndexError, vm, "Bytesarray index out of range");
            return NULL;
        }
    }
    if (start_v>=CAST_BYTESARRAY(self)->size){
        start_v=CAST_BYTESARRAY(self)->size-1;
    }
    if (end_v<0){
        end_v=CAST_BYTESARRAY(self)->size+start_v;
        if (end_v<0){
            vm_add_err(&IndexError, vm, "Bytesarray index out of range");
            return NULL;
        }
    }
    if (end_v>=CAST_BYTESARRAY(self)->size){
        end_v=CAST_BYTESARRAY(self)->size-1;
    }

    object* result=new_bytesarray();

    for (int i=start_v; i<=end_v; i+=step_v){
        bytesarray_append(result, bytesarr_index_int(self, i));
    }
    return result;
}

object* bytesarray_store_slice(object* self, object* idx, object* val){
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
        end_v=CAST_BYTESARRAY(self)->size-1;
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
        start_v=CAST_BYTESARRAY(self)->size+start_v;
        if (start_v<0){
            vm_add_err(&IndexError, vm, "Bytesarray index out of range");
            return NULL;
        }
    }
    if (start_v>=CAST_BYTESARRAY(self)->size){
        start_v=CAST_BYTESARRAY(self)->size-1;
    }
    if (end_v<0){
        end_v=CAST_BYTESARRAY(self)->size+start_v;
        if (end_v<0){
            vm_add_err(&IndexError, vm, "Bytesarray index out of range");
            return NULL;
        }
    }
    if (end_v>=CAST_BYTESARRAY(self)->size){
        end_v=CAST_BYTESARRAY(self)->size-1;
    }
    
    if (end_v-start_v+1>CAST_BYTESARRAY(val)->size){
        int numpop=end_v-start_v+1-CAST_BYTESARRAY(val)->size;
        for (int i=0; i<numpop; i++){
            bytesarray_pop(self);
        }
    }
    
    if (val->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", val->type->name->c_str());
        return NULL;
    }
    
    object* iter=val->type->slot_iter(val);
    
    if (iter!=NULL && iter->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
        return NULL;
    }
    
    object* o=iter->type->slot_next(iter);

    for (int i=start_v; i<=end_v; i+=step_v){
        if (i>CAST_BYTESARRAY(val)->size-1){
            break;
        }
        if (i>CAST_BYTESARRAY(self)->size-1){
            object* ob=object_int(o);
            if (ob==NULL || !object_istype(ob->type, &IntType)){
                vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", o->type->name->c_str());
                return NULL;
            }

            if (*CAST_INT(ob)->val<INT_MIN || *CAST_INT(ob)->val>INT_MAX){
                vm_add_err(&OverflowError, vm, "Value out of range of C int");
                return NULL; 
            }

            int i=CAST_INT(ob)->val->to_int();
            FPLDECREF(ob);
            FPLDECREF(o);
                    
            if (i>255 || i<0){
                vm_add_err(&ValueError, vm, "Expected value in range(0, 256), got %d", i);
                return NULL;
            }
                
            bytesarray_append(self, i);
            o=iter->type->slot_next(iter);
            if (vm->exception!=NULL){
                FPLDECREF(vm->exception);
                vm->exception=NULL;
                break;
            }
            continue;
        }
        object* intv=new_int_fromint(i);
        bytesarray_set(self, intv, o);
        FPLDECREF(o);
        FPLDECREF(intv);
        o=iter->type->slot_next(iter);
        if (vm->exception!=NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
            break;
        }
    }

    FPLDECREF(iter);

    return SUCCESS;    
}

object* bytesarray_pop(object* self){
    if (CAST_BYTESARRAY(self)->size-1 < 0){ //Alloc more space
        vm_add_err(&IndexError, vm, "Pop from empty bytearray");
        //Error
        return NULL;
    }

    CAST_BYTESARRAY(self)->size--;
    char o=CAST_BYTESARRAY(self)->array[CAST_BYTESARRAY(self)->size];

    //Dealloc space
    char* newarr=(char*)fpl_malloc(CAST_BYTESARRAY(self)->size);
    memcpy(newarr, CAST_BYTESARRAY(self)->array, CAST_BYTESARRAY(self)->size);
    fpl_free(CAST_BYTESARRAY(self)->array);
    CAST_BYTESARRAY(self)->array=newarr;
    return new_int_fromint(o);
}

object* bytesarray_get(object* self, object* idx){
    if (object_istype(idx->type, &SliceType)){
        return bytesarray_slice(self, idx);
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
        lidx=lidx+CAST_BYTESARRAY(self)->size;
    }
    if (CAST_BYTESARRAY(self)->size<=lidx || lidx<0){
        vm_add_err(&IndexError, vm, "Bytesarray index out of range");
        FPLDECREF(idx_);
        return NULL;
    }
    FPLDECREF(idx_);
    
    return new_int_fromint(CAST_BYTESARRAY(self)->array[lidx]);
}

object* bytesarray_del_slice(object* self, object* index){
    object* start=CAST_SLICE(index)->start;
    object* end=CAST_SLICE(index)->end;
    object* step=CAST_SLICE(index)->step;

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
        end_v=CAST_BYTESARRAY(self)->size-1;
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
        start_v=CAST_BYTESARRAY(self)->size+start_v;
        if (start_v<0){
            vm_add_err(&IndexError, vm, "Bytesarray index out of range");
            return NULL;
        }
    }
    if (start_v>=CAST_BYTESARRAY(self)->size){
        start_v=CAST_BYTESARRAY(self)->size-1;
    }
    if (end_v<0){
        end_v=CAST_BYTESARRAY(self)->size+start_v;
        if (end_v<0){
            vm_add_err(&IndexError, vm, "Bytesarray index out of range");
            return NULL;
        }
    }
    if (end_v>=CAST_BYTESARRAY(self)->size){
        end_v=CAST_BYTESARRAY(self)->size-1;
    }

    int index_v=0;
    
    for (uint32_t idx=start_v; idx<=end_v; idx+=step_v){
        index_v++;
        
        for(int i = idx; i < CAST_BYTESARRAY(self)->size-1; i++){
            CAST_BYTESARRAY(self)->array[i] = CAST_BYTESARRAY(self)->array[i + 1];
        }
    }
    CAST_BYTESARRAY(self)->size=CAST_BYTESARRAY(self)->size-index_v;

    char* newarr=(char*)fpl_malloc(CAST_BYTESARRAY(self)->size);
    memcpy(newarr, CAST_BYTESARRAY(self)->array, CAST_BYTESARRAY(self)->size);
    fpl_free(CAST_BYTESARRAY(self)->array);
    CAST_BYTESARRAY(self)->array=newarr;

    return SUCCESS;
}

void bytesarray_del_item(object* self, long idx){
    for(int i = idx; i < CAST_BYTESARRAY(self)->size-1; i++){
        CAST_BYTESARRAY(self)->array[i] = CAST_BYTESARRAY(self)->array[i + 1];
    }
    char* newarr=(char*)fpl_malloc(CAST_BYTESARRAY(self)->size);
    memcpy(newarr, CAST_BYTESARRAY(self)->array, CAST_BYTESARRAY(self)->size);
    fpl_free(CAST_BYTESARRAY(self)->array);
    CAST_BYTESARRAY(self)->array=newarr;
}

object* bytesarray_set(object* self, object* idx, object* val){
    if (object_istype(idx->type, &SliceType)){
        if (val==NULL){
            if (bytesarray_del_slice(self, idx)==NULL){
                return NULL;
            }
            return new_none();
        }
        if (bytesarray_store_slice(self, idx,val)==NULL){
            return NULL;
        }
        return new_none();
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
        lidx=lidx+CAST_BYTESARRAY(self)->size;
    }
    if (CAST_BYTESARRAY(self)->size<=lidx || lidx<0){
        vm_add_err(&IndexError, vm, "Bytesarray index out of range");
        FPLDECREF(idx_);
        return NULL;
    }
    FPLDECREF(idx_);

    if (val==NULL){
        bytesarray_del_item(self, lidx);
        return new_none();
    }

    object* ob=object_int(val);
    if (ob==NULL || !object_istype(ob->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", val->type->name->c_str());
        return NULL;
    }

    if (*CAST_INT(ob)->val<INT_MIN || *CAST_INT(ob)->val>INT_MAX){
        vm_add_err(&OverflowError, vm, "Value out of range of C int");
        return NULL; 
    }

    int v=CAST_INT(ob)->val->to_int();
    FPLDECREF(val);
    FPLDECREF(ob);
            
    if (v>255 || v<0){
        vm_add_err(&ValueError, vm, "Expected value in range(0, 256), got %d", v);
        return NULL;
    }

    CAST_BYTESARRAY(self)->array[lidx]=v;
    return new_none();
}

void bytesarray_append(object* self, int val){
    char* newarr=(char*)fpl_malloc(CAST_BYTESARRAY(self)->size++);
    memcpy(newarr, CAST_BYTESARRAY(self)->array, CAST_BYTESARRAY(self)->size-1);
    fpl_free(CAST_BYTESARRAY(self)->array);
    CAST_BYTESARRAY(self)->array=newarr;

    CAST_BYTESARRAY(self)->array[CAST_BYTESARRAY(self)->size-1]=val;
}

void bytesarray_append_obj(object* self, object* val){  // NO ERR CHECKING!!
    char* newarr=(char*)fpl_malloc(CAST_BYTESARRAY(self)->size++);
    memcpy(newarr, CAST_BYTESARRAY(self)->array, CAST_BYTESARRAY(self)->size-1);
    fpl_free(CAST_BYTESARRAY(self)->array);
    CAST_BYTESARRAY(self)->array=newarr;

    CAST_BYTESARRAY(self)->array[CAST_BYTESARRAY(self)->size-1]=CAST_INT(val)->val->to_int();
}

void bytesarray_del(object* obj){
    fpl_free(((BytesarrayObject*)obj)->array);
}

object* bytesarray_repr(object* self){
    string s="";
    for (int i=0; i<CAST_BYTESARRAY(self)->size; i++){
        s+=string(1, CAST_BYTESARRAY(self)->array[i]);
    }
    return str_new_fromstr(s);
}


object* bytesarray_str(object* self){
    string s="b'";
    for (int i=0; i<CAST_BYTESARRAY(self)->size; i++){
        s+=_byte_repr(CAST_BYTESARRAY(self)->array[i]);
        if (i==128){
            s+="... ";
            break;
        }
        if (i!=CAST_BYTESARRAY(self)->size-1){
            s+=", ";
        }
    }
    s+="'";
    return str_new_fromstr(s);
}

object* bytesarray_cmp(object* self, object* other, uint8_t type){
    if (type==CMP_IN){
        return object_in_iter(other, self);
    }
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_BYTESARRAY(self)->size != CAST_BYTESARRAY(other)->size){
            return new_bool_false();
        }
        
        if (memcmp(CAST_BYTESARRAY(self)->array, CAST_BYTESARRAY(other)->array, CAST_BYTESARRAY(self)->size)==0){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (CAST_BYTESARRAY(self)->size != CAST_BYTESARRAY(other)->size){
            return new_bool_true();
        }
        
        if (memcmp(CAST_BYTESARRAY(self)->array, CAST_BYTESARRAY(other)->array, CAST_BYTESARRAY(self)->size)!=0){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

object* bytesarray_append_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2 ){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* val=list_index_int(args, 1);
    object* ob=object_int(val);
    if (ob==NULL || !object_istype(ob->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", val->type->name->c_str());
        return NULL;
    }

    if (*CAST_INT(ob)->val<INT_MIN || *CAST_INT(ob)->val>INT_MAX){
        vm_add_err(&OverflowError, vm, "Value out of range of C int");
        return NULL;
    }

    int v=CAST_INT(ob)->val->to_int();
    FPLDECREF(ob);
            
    if (v>255 || v<0){
        vm_add_err(&ValueError, vm, "Expected value in range(0, 256), got %d", v);
        return NULL;
    }
    
    object* self=list_index_int(args, 0);
    bytesarray_append(self, v);
    return new_none();
}

object* bytesarray_iter(object* self){
    //Make an iterator
    object* iter=new_object(&BytesIterType);
    CAST_BYTESARRAYITER(iter)->val=(char*)fpl_malloc(CAST_BYTESARRAY(self)->size);
    CAST_BYTESARRAYITER(iter)->len=CAST_BYTESARRAY(self)->size;
    CAST_BYTESARRAYITER(iter)->idx=0;
    memcpy(CAST_BYTESARRAYITER(iter)->val, CAST_BYTESARRAY(self)->array, CAST_BYTESARRAYITER(iter)->len);
    return iter;
}

void bytesarray_iter_del(object* self){
    fpl_free((void*)CAST_BYTESARRAYITER(self)->val);
}

object* bytesarray_iter_next(object* self){
    if (CAST_BYTESARRAYITER(self)->idx+1>CAST_BYTESARRAYITER(self)->len){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }
    return new_int_fromint((int)CAST_BYTESARRAYITER(self)->val[CAST_BYTESARRAYITER(self)->idx++]);
}

object* bytesarray_iter_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_BYTESARRAYITER(self)->len!=CAST_BYTESARRAYITER(other)->len){
            return new_bool_false();
        }
        if (memcmp(CAST_BYTESARRAYITER(self)->val, CAST_BYTESARRAYITER(other)->val, CAST_BYTESARRAYITER(self)->len)==0){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (CAST_BYTESARRAYITER(self)->len==CAST_BYTESARRAYITER(other)->len){
            return new_bool_false();
        }
        if (!memcmp(CAST_BYTESARRAYITER(self)->val, CAST_BYTESARRAYITER(other)->val, CAST_BYTESARRAYITER(self)->len)==0){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

object* bytesarray_iter_bool(object* self){
    return new_bool_true();
}

object* bytesarray_pop_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if ((len!=2 && len!=1)){
        vm_add_err(&ValueError, vm, "Expected 1 or 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=list_index_int(args, 0);
    if (len==2){
        object* idx=list_index_int(args, 1);
        
        object* val=bytesarray_get(self, idx);
        if (val==NULL){
            return NULL;
        }
        FPLINCREF(val);
        bytesarray_set(self, idx, NULL);
        return val;
    }

    return bytesarray_pop(self);
}


object* bytesarray_mul(object* self, object* other){
    if (other->type!=&IntType){
        vm_add_err(&TypeError, vm, "Invalid operand types for *: '%s', and '%s'.", self->type->name->c_str(), other->type->name->c_str());
        return NULL;
    }
    if (*CAST_INT(other)->val>LONG_MAX || *CAST_INT(other)->val<LONG_MIN){
        vm_add_err(&OverflowError, vm, "Value out of range of C long");
        return NULL;
    }    
    long nrepeat=CAST_INT(other)->val->to_long();
    size_t orig_size=CAST_BYTESARRAY(self)->size;
    CAST_BYTESARRAY(self)->size*=nrepeat;
    
    char* newarr=(char*)fpl_malloc(CAST_BYTESARRAY(self)->size);
    memcpy(newarr, CAST_BYTESARRAY(self)->array, orig_size);
    fpl_free(CAST_BYTESARRAY(self)->array);
    CAST_BYTESARRAY(self)->array=newarr;
        
    for (int i=1; i<nrepeat; i++){
        for (int a=0; a<orig_size; a++){
            char o=CAST_BYTESARRAY(self)->array[a];
            CAST_BYTESARRAY(self)->array[a+(i*orig_size)]=o;
        }
    }
    return self;
}

object* bytesarray_add(object* self, object* other){
    if (other->type!=&BytesarrayType){
        vm_add_err(&TypeError, vm, "Invalid operand types for +: '%s', and '%s'.", self->type->name->c_str(), other->type->name->c_str());
        return NULL;
    }

    for (size_t i=0; i<CAST_BYTESARRAY(other)->size; i++){
        bytesarray_append(self, bytesarr_index_int(CAST_BYTESARRAY(other)->array, i));
    }
    FPLINCREF(self);
    return self;
}

object* bytesarray_replace_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=3){
        vm_add_err(&ValueError, vm, "Expected 3 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);
    object* replace_=tuple_index_int(args, 1);
    object* replacewi_=tuple_index_int(args, 2);

    object* ob=object_int(replace_);
    if (ob==NULL || !object_istype(ob->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", replace_->type->name->c_str());
        return NULL;
    }

    if (*CAST_INT(ob)->val<INT_MIN || *CAST_INT(ob)->val>INT_MAX){
        vm_add_err(&OverflowError, vm, "Value out of range of C int");
        return NULL; 
    }

    int replace=CAST_INT(ob)->val->to_int();
    FPLDECREF(replace_);
    FPLDECREF(ob);
            
    if (replace>255 || replace<0){
        vm_add_err(&ValueError, vm, "Expected value in range(0, 256), got %d", replace);
        return NULL;
    }

    
    ob=object_int(replacewi_);
    if (ob==NULL || !object_istype(ob->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", replacewi_->type->name->c_str());
        return NULL;
    }

    if (*CAST_INT(ob)->val<INT_MIN || *CAST_INT(ob)->val>INT_MAX){
        vm_add_err(&OverflowError, vm, "Value out of range of C int");
        return NULL; 
    }

    int replacewi=CAST_INT(ob)->val->to_int();
    FPLDECREF(replacewi_);
    FPLDECREF(ob);
            
    if (replacewi>255 || replacewi<0){
        vm_add_err(&ValueError, vm, "Expected value in range(0, 256), got %d", replacewi);
        return NULL;
    }

    for (size_t i=0; i<CAST_BYTESARRAY(self)->size; i++){
        if (CAST_BYTESARRAY(self)->array[i]==replace){
            CAST_BYTESARRAY(self)->array[i]=replacewi;
            return self;
        }
    }
    
    vm_add_err(&ValueError, vm, "Cannot find item '%d'", replace);
    return NULL; 
}

object* bytesarray_find_meth(object* selftp, object* args, object* kwargs){
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

    
    object* ob=object_int(val);
    if (ob==NULL || !object_istype(ob->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", val->type->name->c_str());
        return NULL;
    }

    if (*CAST_INT(ob)->val<INT_MIN || *CAST_INT(ob)->val>INT_MAX){
        vm_add_err(&OverflowError, vm, "Value out of range of C int");
        return NULL; 
    }

    int val_=CAST_INT(ob)->val->to_int();
    FPLDECREF(val);
    FPLDECREF(ob);
            
    if (val_>255 || val_<0){
        vm_add_err(&ValueError, vm, "Expected value in range(0, 256), got %d", val_);
        return NULL;
    }

    for (size_t i=0; i<CAST_BYTESARRAY(self)->size; i++){
        if (CAST_BYTESARRAY(self)->array[i]==val_, CMP_EQ){
            return new_int_fromint(i);
        }
    }

    return new_int_fromint(-1);
}

object* bytesarray_remove_meth(object* selftp, object* args, object* kwargs){
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

    
    object* ob=object_int(val);
    if (ob==NULL || !object_istype(ob->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", val->type->name->c_str());
        return NULL;
    }

    if (*CAST_INT(ob)->val<INT_MIN || *CAST_INT(ob)->val>INT_MAX){
        vm_add_err(&OverflowError, vm, "Value out of range of C int");
        return NULL; 
    }

    int val_=CAST_INT(ob)->val->to_int();
    FPLDECREF(val);
    FPLDECREF(ob);
            
    if (val_>255 || val_<0){
        vm_add_err(&ValueError, vm, "Expected value in range(0, 256), got %d", val_);
        return NULL;
    }

    for (size_t idx=0; idx<CAST_BYTESARRAY(self)->size; idx++){
        if (CAST_BYTESARRAY(self)->array[idx]==val_){
            for(int i = idx; i < CAST_BYTESARRAY(self)->size-1; i++){
                CAST_BYTESARRAY(self)->array[i] = CAST_BYTESARRAY(self)->array[i + 1];
            }
            char* newarr=(char*)fpl_malloc(CAST_BYTESARRAY(self)->size--);
            memcpy(newarr, CAST_BYTESARRAY(self)->array, CAST_BYTESARRAY(self)->size);
            fpl_free(CAST_BYTESARRAY(self)->array);
            CAST_BYTESARRAY(self)->array=newarr;
            return new_none();
        }
    }
    
    vm_add_err(&ValueError, vm, "bytesarray.remove(self, x): x not in bytesarray");
    return NULL; 
}

object* bytesarray_insert_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=3){
        vm_add_err(&ValueError, vm, "Expected 3 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);  
    object* idx_=tuple_index_int(args, 1);   
    object* val=tuple_index_int(args, 2);  

    object* idx_new=object_int(idx_);
    if (idx_new==NULL || !object_istype(idx_new->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int",idx_->type->name->c_str());
        return NULL;
    }
    if (*CAST_INT(idx_new)->val>LLONG_MAX || *CAST_INT(idx_new)->val<LLONG_MIN){
        vm_add_err(&OverflowError, vm, "Value out of range of C long long");
        return NULL;
    }   

    size_t idx=CAST_INT(idx_new)->val->to_long_long();
    if (idx>CAST_BYTESARRAY(self)->size){
        vm_add_err(&IndexError, vm, "Bytesarray index out of range");
        return NULL;
    }
    char* newarr=(char*)fpl_malloc(CAST_BYTESARRAY(self)->size++);
    memcpy(newarr, CAST_BYTESARRAY(self)->array, CAST_BYTESARRAY(self)->size-1);
    fpl_free(CAST_BYTESARRAY(self)->array);
    CAST_BYTESARRAY(self)->array=newarr;

    for (int i=CAST_BYTESARRAY(self)->size-1; i>idx; i--){
        CAST_BYTESARRAY(self)->array[i] = CAST_BYTESARRAY(self)->array[i-1];
    }
    
     

    
    object* ob=object_int(val);
    if (ob==NULL || !object_istype(ob->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", val->type->name->c_str());
        return NULL;
    }

    if (*CAST_INT(ob)->val<INT_MIN || *CAST_INT(ob)->val>INT_MAX){
        vm_add_err(&OverflowError, vm, "Value out of range of C int");
        return NULL; 
    }

    int val_=CAST_INT(ob)->val->to_int();
    FPLDECREF(val);
    FPLDECREF(ob);
            
    if (val_>255 || val_<0){
        vm_add_err(&ValueError, vm, "Expected value in range(0, 256), got %d", val_);
        return NULL;
    }

    CAST_BYTESARRAY(self)->array[idx]=val_;


    return new_none();
}

object* bytesarray_extend_meth(object* selftp, object* args, object* kwargs){
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
    object* iter_=tuple_index_int(args, 1);

    if (iter_->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not iterable", iter_->type->name->c_str());
        return NULL; 
    }
    object* iter=iter_->type->slot_iter(iter_);

    if (iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter_->type->name->c_str());
        return NULL;
    }
    

    object* one=new_int_fromint(0);
    object* two=new_int_fromint(1);
    
    object* o=iter->type->slot_next(iter);
    while (vm->exception==NULL){
        object* ob=object_int(o);
        if (ob==NULL || !object_istype(ob->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", o->type->name->c_str());
            return NULL;
        }

        if (*CAST_INT(ob)->val<INT_MIN || *CAST_INT(ob)->val>INT_MAX){
            vm_add_err(&OverflowError, vm, "Value out of range of C int");
            return NULL; 
        }

        int val_=CAST_INT(ob)->val->to_int();
        FPLDECREF(o);
        FPLDECREF(ob);
                
        if (val_>255 || val_<0){
            vm_add_err(&ValueError, vm, "Expected value in range(0, 256), got %d", val_);
            return NULL;
        } 
        bytesarray_append(self, val_);        
        o=iter->type->slot_next(iter);
    }
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }


    return new_none();
}

object* bytesarray_hex_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* self=list_index_int(args, 0);
    
    string s="0x";
    
    for (int i=0; i<CAST_BYTESARRAY(self)->size; i++){
        char arr[]="\0\0\0";
        sprintf(arr, "%.2x", (int)(bytesarr_index_int(self, i)));
        s+=string(arr);
    }

    return str_new_fromstr(s);
}

object* bytesarray_decode_meth(object* selftp, object* args, object* kwargs){
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
    object* encoding=tuple_index_int(args, 1);   
    

    if (!object_istype(encoding->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str, got '%s'", encoding->type->name->c_str());
        return NULL; 
    }  

    string enc=*CAST_STRING(encoding)->val;
    
    iconv_t cd = iconv_open("UTF-8", enc.c_str());
    if(cd == (iconv_t)-1) {
        if (errno == EINVAL) {
            vm_add_err(&ValueError, vm, "Invalid conversion");
            return NULL; 
        }
    }

    size_t s_size=CAST_BYTESARRAY(self)->size;
    size_t new_size=s_size;
    size_t _new_size=new_size;

    const char* orig_str=CAST_BYTESARRAY(self)->array;
    char* converted=(char*)fpl_calloc(new_size, sizeof(char));
    char* start=converted;
    
    int ret = iconv(cd, (char**)&orig_str, &s_size, &converted, &new_size);
    
    if(ret == -1) {
        vm_add_err(&ValueError, vm, "Invalid multibyte sequence encountered");
        return NULL; 
    }
    iconv_close(cd);

    string s(start);

    fpl_free(start);

    return str_new_fromstr(s);
}