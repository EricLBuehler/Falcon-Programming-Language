void list_resize(ListObject* obj, size_t size){
    object** buf=(object**)fpl_malloc(obj->capacity * sizeof(struct object*));
    memcpy(buf, obj->array, obj->capacity * sizeof(struct object*));
    size_t oldcap=obj->capacity;

    obj->capacity=(size + (size >> 3) + 6) & ~3;
    
    obj->array=(object**)realloc(obj->array, obj->capacity * sizeof(struct object*));
    memcpy(obj->array, buf, oldcap * sizeof(struct object*));
    free(buf);
}

object* new_list(){
    object_var* obj=new_object_var(&ListType, 0);
    CAST_LIST(obj)->capacity=2; //Start with 2
    CAST_LIST(obj)->size=0;
    CAST_LIST(obj)->array=(object**)fpl_malloc((CAST_LIST(obj)->capacity * sizeof(struct object*)));
    
    return (object*)obj;
}

object* list_new(object* type, object* args, object* kwargs){
    if (CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()==0){
        object_var* obj=new_object_var(CAST_TYPE(type), sizeof(ListObject)+2*sizeof(object*));
        CAST_LIST(obj)->capacity=2; //Start with 2
        CAST_LIST(obj)->size=0;
        CAST_LIST(obj)->array=(object**)fpl_malloc((CAST_LIST(obj)->capacity * sizeof(struct object*)));
        
        return (object*)obj;
    }
    if (CAST_INT(args->type->slot_mappings->slot_len(kwargs))->val->to_int()>0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    if (object_istype(list_index_int(args, 0)->type, &ListType)){
        return INCREF(list_index_int(args, 0));
    }
    if (list_index_int(args, 0)->type->slot_iter!=NULL){
        object* o=list_index_int(args, 0);
        object* iter=o->type->slot_iter(o);

        object_var* obj=new_object_var(CAST_TYPE(type), sizeof(ListObject)+2*sizeof(object*));
        CAST_LIST(obj)->capacity=2; //Start with 2
        CAST_LIST(obj)->size=0;
        CAST_LIST(obj)->array=(object**)fpl_malloc((CAST_LIST(obj)->capacity * sizeof(struct object*)));
        
        o=iter->type->slot_next(iter);
        while (vm->exception==NULL){
            if (o->type->slot_mappings->slot_len!=NULL && *CAST_INT(o->type->slot_mappings->slot_len(o))->val!=1){
                DECREF((object*)obj);
                vm_add_err(&ValueError, vm, "Expected 1 value for list update, got '%d'",CAST_INT(o->type->slot_mappings->slot_len(o))->val->to_int());
                return NULL;
            }
            list_append((object*)obj, o);
            
            o=iter->type->slot_next(iter);
        }
        if (vm->exception!=NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        return (object*)obj;
    }

    //Append
    object_var* obj=new_object_var(CAST_TYPE(type), sizeof(ListObject)+2*sizeof(object*));
    CAST_LIST(obj)->capacity=2; //Start with 2
    CAST_LIST(obj)->size=0;
    CAST_LIST(obj)->array=(object**)fpl_malloc((CAST_LIST(obj)->capacity * sizeof(struct object*)));

    size_t length=(size_t)CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    for (size_t i=0; i<length; i++){
        list_append((object*)obj, INCREF(list_index_int(args, i)));
    }
    
    return (object*)obj;
}

object* list_bool(object* self){
    return new_bool_true();
}

object* list_len(object* self){
    return new_int_fromint(CAST_LIST(self)->size);
}

object* list_slice(object* self, object* idx){
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
        end_v=CAST_LIST(self)->size-1;
    }
    else{
        end_v=CAST_INT(end)->val->to_int();
    }
    if (start<0){
        start_v=0;
    }
    if (end_v>=CAST_LIST(self)->size){
        end_v=CAST_LIST(self)->size-1;
    }
    for (int i=start_v; i<=end_v; i++){
        list_append(result, list_index_int(self, i));
    }
    return result;
}

void list_store_slice(object* self, object* idx, object* val){
    object* start=CAST_SLICE(idx)->start;
    object* end=CAST_SLICE(idx)->end;
    
    
    int n=0;
    int start_v;
    int end_v;
    if (object_istype(start->type, &NoneType)){
        start_v=0;
    }
    else{
        start_v=CAST_INT(start)->val->to_int();
    }
    if (object_istype(end->type, &NoneType)){
        end_v=CAST_LIST(self)->size-1;
    }
    else{
        end_v=CAST_INT(end)->val->to_int();
    }
    

    if (start_v<0){
        start_v=0;
    }
    if (end_v>CAST_LIST(self)->size-1){
        end_v=CAST_LIST(self)->size-1;
    }
    
    if (end_v-start_v+1>CAST_LIST(val)->size){
        int numpop=end_v-start_v+1-CAST_LIST(val)->size;
        for (int i=0; i<numpop; i++){
            list_pop(self);
        }
    }
    for (int i=start_v; i<=end_v; i++){
        if (n>CAST_LIST(val)->size-1){
            break;
        }
        if (i>CAST_LIST(self)->size-1){
            list_append(self, list_index_int(val, n++));
            continue;
        }
        object* intv=new_int_fromint(i);
        list_set(self, intv, list_index_int(val, n++));
        DECREF(intv);
    }
    
}

object* list_pop(object* self){
    if (CAST_LIST(self)->size-1 < 0){ //Alloc more space
        vm_add_err(&IndexError, vm, "Pop from empty list");
        //Error
        return NULL;
    }

    CAST_LIST(self)->size--;
    object* o=CAST_LIST(self)->array[CAST_LIST(self)->size]; //No need to decref, we return a reference

    ((object_var*)self)->var_size=sizeof(ListObject)+CAST_LIST(self)->size;

    if (CAST_LIST(self)->size == CAST_LIST(self)->capacity){ //DEalloc space
        list_resize(CAST_LIST(self), CAST_LIST(self)->size);
    }
    return o;
}

object* list_get(object* self, object* idx){
    if (object_istype(idx->type, &SliceType)){
        return list_slice(self, idx);
    }
    if (!object_istype(idx->type, &IntType)){
        vm_add_err(&TypeError, vm, "List must be indexed by int not '%s'",idx->type->name->c_str());
        return (object*)0x1;
    }
    if (CAST_LIST(self)->size<=CAST_INT(idx)->val->to_long_long() || CAST_INT(idx)->val->to_long_long()<0){
        vm_add_err(&IndexError, vm, "List index out of range");
        return (object*)0x1;
    }
    
    return CAST_LIST(self)->array[CAST_INT(idx)->val->to_long_long()];
}

void list_del_slice(object* self, object* index){
    uint32_t start=CAST_INT(CAST_SLICE(index)->start)->val->to_int();
    uint32_t end=CAST_INT(CAST_SLICE(index)->end)->val->to_int();

    int start_v;
    int end_v;
    if (start<0){
        start_v=0;
    }
    if (end_v>=CAST_LIST(self)->size){
        end_v=CAST_LIST(self)->size-1;
    }

    for (uint32_t idx=start_v; idx<end_v; idx++){
        DECREF(CAST_LIST(self)->array[idx]);
        
        for(int i = start_v; i < CAST_INT(self->type->slot_mappings->slot_len(self))->val->to_int()-1; i++){
            CAST_LIST(self)->array[i] = CAST_LIST(self)->array[i + 1];
        }
    }
    CAST_LIST(self)->size=CAST_LIST(self)->size-(end_v-start_v);
    
    list_resize(CAST_LIST(self), CAST_LIST(self)->size);
}

void list_del_item(object* self, object* index){
    uint32_t idx=CAST_INT(index)->val->to_int();
    DECREF(CAST_LIST(self)->array[idx]);
    for(int i = idx; i < CAST_INT(self->type->slot_mappings->slot_len(self))->val->to_int()-1; i++){
        CAST_LIST(self)->array[i] = CAST_LIST(self)->array[i + 1];
    }
    list_resize(CAST_LIST(self), CAST_LIST(self)->size--);
}

object* list_set(object* self, object* idx, object* val){
    if (object_istype(idx->type, &SliceType)){
        if (val==NULL){
            list_del_slice(self, idx);
            return new_none();
        }
        list_store_slice(self, idx,val);
        return new_none();
    }
    if (!object_istype(idx->type, &IntType)){
        vm_add_err(&TypeError, vm, "List must be indexed by int not '%s'",idx->type->name->c_str());
        //Error
        return NULL;
    }
    if (CAST_LIST(self)->size<=CAST_INT(idx)->val->to_long_long() || CAST_INT(idx)->val->to_long_long()<0){
        vm_add_err(&IndexError, vm, "List index out of range");
        //Error
        return NULL;
    }

    if (val==NULL){
        list_del_item(self, idx);
        return new_none();
    }
    
    DECREF(CAST_LIST(self)->array[CAST_INT(idx)->val->to_long_long()]);

    CAST_LIST(self)->array[CAST_INT(idx)->val->to_long_long()]=INCREF(val);
    return new_none();
}

bool _list_contains(object* haystack, string* needle){
    for (size_t i=0; i<CAST_LIST(haystack)->size; i++){
        if ((*CAST_STRING(object_str(CAST_LIST(haystack)->array[i]))->val)==(*needle)){
            return true;
        }
    }
    return false;
}

void list_append(object* self, object* obj){
    if (CAST_LIST(self)->size+1 == CAST_LIST(self)->capacity){ //Alloc more space
        list_resize(CAST_LIST(self), CAST_LIST(self)->size+1);
    }

    CAST_LIST(self)->array[CAST_LIST(self)->size++]=INCREF(obj);

    ((object_var*)self)->var_size=sizeof(ListObject)+CAST_LIST(self)->size;
}

void list_del(object* obj){
    for (size_t i=0; i<CAST_LIST(obj)->size; i++){
        DECREF(CAST_LIST(obj)->array[i]);
    }
    free(((ListObject*)obj)->array);
}

object* list_repr(object* self){
    string s="";
    s+="[";
    for (size_t i=0; i<CAST_LIST(self)->size; i++){
        s+=object_crepr(CAST_LIST(self)->array[i]);
        if (i!=CAST_LIST(self)->size-1){
            s+=", ";
        }
    }
    s+="]";
    return str_new_fromstr(s);
}


object* list_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_LIST(self)->size != CAST_LIST(other)->size){
            return new_bool_false();
        }
        
        for (size_t i=0; i<CAST_LIST(self)->size; i++){
            if (!istrue(object_cmp(CAST_LIST(self)->array[i], CAST_LIST(other)->array[i], type))){
                return new_bool_false(); 
            }
        }
        return new_bool_true();
    }
    if (type==CMP_NE){
        if (CAST_LIST(self)->size != CAST_LIST(other)->size){
            return new_bool_true();
        }

        bool t=false;
        for (size_t i=0; i<CAST_LIST(self)->size; i++){
            if (!istrue(object_cmp(CAST_LIST(self)->array[i], CAST_LIST(other)->array[i], type))){
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

object* list_append_meth(object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long();
    if (len!=2 && CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() == 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    
    object* self=list_index_int(args, 0);
    list_append(self, list_index_int(args, 1));
    return new_none();
}

object* list_iter(object* self){
    //Make an iterator
    object* iter=(object*)new_object_var(&ListIterType, sizeof(ListIterObject)+CAST_LIST(self)->capacity*sizeof(object*));
    CAST_LISTITER(iter)->capacity=CAST_LIST(self)->capacity;
    CAST_LISTITER(iter)->size=CAST_LIST(self)->size;
    CAST_LISTITER(iter)->idx=0;
    CAST_LISTITER(iter)->array=(object**)fpl_malloc(CAST_LISTITER(iter)->capacity * sizeof(struct object*));
    for (size_t i=0; i<CAST_LIST(self)->size; i++){
        CAST_LISTITER(iter)->array[i]=INCREF(CAST_LIST(self)->array[i]);
    }
    return iter;
}

void list_iter_del(object* self){
    for (size_t i=0; i<CAST_LISTITER(self)->size; i++){
        DECREF(CAST_LISTITER(self)->array[i]);
    }
    free(CAST_LISTITER(self)->array);
}

object* list_iter_next(object* self){
    if (CAST_LISTITER(self)->idx+1>CAST_LISTITER(self)->size){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }
    return CAST_LISTITER(self)->array[CAST_LISTITER(self)->idx++];
}

object* list_iter_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_LISTITER(self)->size != CAST_LISTITER(other)->size){
            return new_bool_false();
        }
        for (size_t i=0; i<CAST_LISTITER(self)->size; i++){
            if (!istrue(object_cmp(CAST_LISTITER(self)->array[i], CAST_LISTITER(other)->array[i], type))){
                return new_bool_false(); 
            }
        }
        return new_bool_true();
    }
    if (type==CMP_NE){
        bool t=false;
        for (size_t i=0; i<CAST_LISTITER(self)->size; i++){
            if (!istrue(object_cmp(CAST_LISTITER(self)->array[i], CAST_LISTITER(other)->array[i], type))){
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

object* list_iter_bool(object* self){
    if (CAST_LISTITER(self)->idx+1>CAST_LISTITER(self)->size){
        return new_bool_false();
    }
    return new_bool_true();
}

object* list_pop_meth(object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long();
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* self=list_index_int(args, 0);


    uint32_t idx=CAST_INT(self->type->slot_mappings->slot_len(self))->val->to_int()-1;
    if (CAST_INT(self->type->slot_mappings->slot_len(self))->val->to_int()==0){
        vm_add_err(&ValueError, vm, "Cannot pop from empty list");
        return NULL; 
    }
    object* ob=CAST_LIST(self)->array[idx];
    for(int i = idx; i < CAST_INT(self->type->slot_mappings->slot_len(self))->val->to_int()-1; i++){
        CAST_LIST(self)->array[i] = CAST_LIST(self)->array[i + 1];
    }
    list_resize(CAST_LIST(self), CAST_LIST(self)->size--);
    return ob;
}


object* list_mul(object* self, object* other){
    if (other->type!=&IntType){
        vm_add_err(&TypeError, vm, "Invalid operand types for *: '%s', and '%s'.", self->type->name->c_str(), other->type->name->c_str());
        return NULL;
    }
    long nrepeat=CAST_INT(other)->val->to_long();
    size_t orig_size=CAST_LIST(self)->size;
    CAST_LIST(self)->size*=nrepeat;
    
    list_resize(CAST_LIST(self), CAST_LIST(self)->size);
        
    for (int i=1; i<nrepeat; i++){
        for (int a=0; a<orig_size; a++){
            CAST_LIST(self)->array[a+(i*orig_size)]=INCREF(CAST_LIST(self)->array[a]);
        }
    }
    return self;
}

object* list_add(object* self, object* other){
    if (other->type!=&ListType){
        vm_add_err(&TypeError, vm, "Invalid operand types for +: '%s', and '%s'.", self->type->name->c_str(), other->type->name->c_str());
        return NULL;
    }
    

    object* list=new_list();
    CAST_LIST(list)->size=CAST_LIST(self)->size+CAST_LIST(other)->size;

    for (size_t i=0; i<CAST_LIST(self)->size; i++){
        CAST_LIST(list)->array[i]=INCREF(CAST_LIST(self)->array[i]);
    }
    for (size_t i=CAST_LIST(self)->size; i<CAST_LIST(list)->size; i++){
        CAST_LIST(list)->array[i]=INCREF(CAST_LIST(other)->array[i-CAST_LIST(self)->size]);
    }
    return list;
}