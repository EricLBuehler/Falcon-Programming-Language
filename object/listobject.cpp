void list_resize(ListObject* obj, size_t size){
    object** buf=(object**)malloc(obj->capacity * sizeof(struct object*));
    memcpy(buf, obj->array, obj->capacity * sizeof(struct object*));
    size_t oldcap=obj->capacity;

    obj->capacity=(size + (size >> 3) + 6) & ~3;
    
    obj->array=(object**)realloc(obj->array, obj->capacity * sizeof(struct object*));
    memcpy(obj->array, buf, oldcap * sizeof(struct object*));
    free(buf);
}

object* new_list(){
    object_var* obj=new_object_var(&ListType, sizeof(ListObject)+2*sizeof(object*));
    CAST_LIST(obj)->capacity=2; //Start with 2
    CAST_LIST(obj)->size=0;
    CAST_LIST(obj)->idx=0;
    CAST_LIST(obj)->array=(object**)malloc((CAST_LIST(obj)->capacity * sizeof(struct object*)));
    
    return (object*)obj;
}

object* list_new(object* type, object* args, object* kwargs){
    if (CAST_INT(args->type->slot_len(args))->val->to_int()==0){
        object_var* obj=new_object_var(&ListType, sizeof(ListObject)+2*sizeof(object*));
        CAST_LIST(obj)->capacity=2; //Start with 2
        CAST_LIST(obj)->size=0;
        CAST_LIST(obj)->idx=0;
        CAST_LIST(obj)->array=(object**)malloc((CAST_LIST(obj)->capacity * sizeof(struct object*)));
        
        return (object*)obj;
    }
    if (object_istype(args->type->slot_get(args, new_int_fromint(0))->type, &ListType)){
        return INCREF(args->type->slot_get(args, new_int_fromint(0)));
    }
    //Append
    object_var* obj=new_object_var(&ListType, sizeof(ListObject)+2*sizeof(object*));
    CAST_LIST(obj)->capacity=2; //Start with 2
    CAST_LIST(obj)->size=0;
    CAST_LIST(obj)->idx=0;
    CAST_LIST(obj)->array=(object**)malloc((CAST_LIST(obj)->capacity * sizeof(struct object*)));

    size_t len=(size_t)CAST_INT(args->type->slot_len(args))->val->to_int();
    for (size_t i=0; i<len; i++){
        list_append((object*)obj, INCREF(args->type->slot_get(args, new_int_fromint(i))));
    }
    
    return (object*)obj;
}

object* list_bool(object* self){
    return new_bool_true();
}

object* list_len(object* self){
    return new_int_fromint(CAST_LIST(self)->size);
}

object* list_get(object* self, object* idx){
    if (!object_istype(idx->type, &IntType)){
        vm_add_err(&TypeError, vm, "List must be indexed by int not '%s'",idx->type->name->c_str());
        return (object*)0x1;
    }
    if (CAST_LIST(self)->size<=CAST_INT(idx)->val->to_long_long()){
        vm_add_err(&IndexError, vm, "List index out of range");
        return (object*)0x1;
    }
    
    return CAST_LIST(self)->array[CAST_INT(idx)->val->to_long_long()];
}

void list_set(object* self, object* idx, object* val){
    if (!object_istype(idx->type, &IntType)){
        vm_add_err(&TypeError, vm, "List must be indexed by int not '%s'",idx->type->name->c_str());
        //Error
        return;
    }
    if (CAST_LIST(self)->size<=CAST_INT(idx)->val->to_long_long()){
        vm_add_err(&IndexError, vm, "List index out of range");
        //Error
        return;
    }
    
    DECREF(CAST_LIST(self)->array[CAST_INT(idx)->val->to_long_long()]);

    CAST_LIST(self)->array[CAST_INT(idx)->val->to_long_long()]=INCREF(val);
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

object* list_next(object* self){
    if (CAST_LIST(self)->idx+1>CAST_LIST(self)->size){
        CAST_LIST(self)->idx=0;
        return NULL;
    }
    return CAST_LIST(self)->array[CAST_LIST(self)->idx++];
}

object* list_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
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
    return new_bool_false();
}

object* list_append_meth(object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_len(args))->val->to_long();
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    
    object* self=args->type->slot_get(args, new_int_fromint(0));
    list_append(self, args->type->slot_get(args, new_int_fromint(1)));
    return new_none();
}
