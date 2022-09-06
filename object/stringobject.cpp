object* str_new_fromstr(string val){
    object* obj=new_object(&StrType);
    
    ((StrObject*)obj)->val=new string(val);
    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    
    DECREF((object*)obj);
    return o;
}


object* str_int(object* self){
    BigInt* b;
    try{
        b=new BigInt(CAST_STRING(self)->val->c_str());
    }
    catch (std::invalid_argument){
        if (vm!=NULL){
            vm_add_err(&ValueError, vm, "Invalid literal '%s'", CAST_STRING(self)->val->c_str());
        }
        return NULL;
    }
    return new_int_frombigint(b);
}

object* str_float(object* self){
    double d;
    try{
        d=stod(CAST_STRING(self)->val->c_str());
    }
    catch (std::invalid_argument){
        if (vm!=NULL){
            vm_add_err(&ValueError, vm, "Invalid literal '%s'", CAST_STRING(self)->val->c_str());
        }
        return NULL;
    }
    return new_float_fromdouble(d);
}


object* str_new(object* type, object* args, object* kwargs){
    if (CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()==0){
        object* obj=new_object(CAST_TYPE(type));
        ((StrObject*)obj)->val=new string("");

        object* o = in_immutables((struct object*)obj);
        if (o==NULL){
            return (object*)obj;
        }
        DECREF((struct object*)obj);
        return o;
    }
    object* val=INCREF(list_index_int(args, 0));
    string s=object_cstr(val);

    object* obj=new_object(CAST_TYPE(type));
    ((StrObject*)obj)->val=new string(s);

    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* str_slice(object* self, object* idx){
    object* start=CAST_SLICE(idx)->start;
    object* end=CAST_SLICE(idx)->end;

    string s="";
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
    if (end_v>=CAST_STRING(self)->val->size()){
        end_v=CAST_STRING(self)->val->size()-1;
    }
    for (int i=start_v; i<=end_v; i++){
        s+=CAST_STRING(self)->val->at(i);
    }
    return str_new_fromstr(s);
}

object* str_get(object* self, object* idx){
    if (object_istype(idx->type, &SliceType)){
        return str_slice(self, idx);
    }
    if (!object_istype(idx->type, &IntType)){
        vm_add_err(&TypeError, vm, "String must be indexed by int not '%s'",idx->type->name->c_str());
        return (object*)0x1;
    }
    if (CAST_STRING(self)->val->size()<=CAST_INT(idx)->val->to_long_long()){
        vm_add_err(&IndexError, vm, "String index out of range");
        return (object*)0x1;
    }
    
    return str_new_fromstr(string(1,CAST_STRING(self)->val->at(CAST_INT(idx)->val->to_long_long())));
}

object* str_len(object* self){
    return new_int_fromint(CAST_STRING(self)->val->size());
}

object* str_repr(object* self){
    return str_new_fromstr("'"+replace_newlines((*CAST_STRING(self)->val))+"'");
}

object* str_str(object* self){
    return self; //str_new_fromstr(new string((*CAST_STRING(self)->val)));
}

object* str_bool(object* self){
    return new_bool_true();
}

object* str_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_STRING(self)->val->size()!=CAST_STRING(other)->val->size()){
            return new_bool_false();
        }
        if ((*CAST_STRING(self)->val)==(*CAST_STRING(other)->val)){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (CAST_STRING(self)->val->size()==CAST_STRING(other)->val->size()){
            return new_bool_false();
        }
        if ((*CAST_STRING(self)->val)!=(*CAST_STRING(other)->val)){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

void str_del(object* obj){
    delete ((StrObject*)obj)->val;
}

object* str_wrapper_new(object* args, object* kwargs){
    if (CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()==0){
        object* obj=new_object(&StrType);
        ((StrObject*)obj)->val=new string("");

        object* o = in_immutables((struct object*)obj);
        if (o==NULL){
            return (object*)obj;
        }
        DECREF((struct object*)obj);
        return o;
    }
    object* val=INCREF(list_index_int(args, 1));
    string s=object_cstr(val);

    object* obj=new_object(&StrType);
    ((StrObject*)obj)->val=new string(s);

    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* str_wrapper_len(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=list_index_int(args, 0)->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    return self->type->slot_mappings->slot_len(self);
}

object* str_wrapper_repr(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=list_index_int(args, 0)->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    return self->type->slot_repr(self);
}

object* str_wrapper_str(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=list_index_int(args, 0)->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    return self->type->slot_str(self);
}

object* str_wrapper_bool(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=list_index_int(args, 0)->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    return self->type->slot_number->slot_bool(self);
}

object* str_wrapper_ne(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=list_index_int(args, 0)->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
    }
    object* self=list_index_int(args, 0);
    object* other=list_index_int(args, 1);

    if (self->type!=other->type){
        return NULL;
    }
    if (CAST_STRING(self)->val->size()!=CAST_STRING(other)->val->size()){
        return new_bool_true();
    }
    if ((*CAST_STRING(self)->val)==(*CAST_STRING(other)->val)){
        return new_bool_false();
    }
    return new_bool_true();
}

object* str_wrapper_eq(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2){
        if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val>=1){
            TypeObject* tp=list_index_int(args, 0)->type;
            if (!object_istype(tp, &StrType)){
                vm_add_err(&TypeError, vm, "Expected 'str' object, got '%s' object",tp->name->c_str());
                return NULL;
            }
        }
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int());
    }
    object* self=list_index_int(args, 0);
    object* other=list_index_int(args, 1);

    if (self->type!=other->type){
        return NULL;
    }
    if (CAST_STRING(self)->val->size()!=CAST_STRING(other)->val->size()){
        return new_bool_false();
    }
    if ((*CAST_STRING(self)->val)==(*CAST_STRING(other)->val)){
        return new_bool_true();
    }
    return new_bool_false();
}


object* str_iter(object* self){
    //Make an iterator
    object* iter=new_object(&StrIterType);
    CAST_STRITER(iter)->val=new string(*CAST_STRING(self)->val);
    CAST_STRITER(iter)->idx=0;
    return iter;
}

void str_iter_del(object* self){
    delete CAST_STRITER(self)->val;
}

object* str_iter_next(object* self){
    if (CAST_STRITER(self)->idx+1>CAST_STRITER(self)->val->size()){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }
    return str_new_fromstr(string(1,CAST_STRITER(self)->val->at(CAST_STRITER(self)->idx++)));
}

object* str_iter_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if ((*CAST_STRITER(self)->val) == (*CAST_STRITER(other)->val)){
            return new_bool_true();
        }
    }
    if (type==CMP_NE){
        if ((*CAST_STRITER(self)->val) != (*CAST_STRITER(other)->val)){
            return new_bool_true();
        }
    }
    return NULL;
}

object* str_iter_bool(object* self){
    return new_bool_true();
}

object* str_add(object* self, object* other){
    if (other->type!=&StrType){
        vm_add_err(&TypeError, vm, "Invalid operand type for +: '%s', and '%s'.", self->type->name->c_str(), other->type->name->c_str());
        return NULL;
    }
        
    return str_new_fromstr(*CAST_STRING(self)->val+*CAST_STRING(other)->val);
}


object* string_join_meth(object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long();
    if (len!=2 && CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() == 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);
    object* arg=tuple_index_int(args, 1);

    if (arg->type->slot_iter==NULL){
        vm_add_err(&ValueError, vm, "Expected iterable");
        return NULL; 
    }

    object* iter=arg->type->slot_iter(arg);

    string selfstr=object_cstr(self);
    
    string s="";
    int i=0;
    len=CAST_INT(arg->type->slot_mappings->slot_len(arg))->val->to_long()-1;
    while (vm->exception==NULL){
        object* o=iter->type->slot_next(iter);
        if (o==NULL){
            DECREF(vm->exception);
            break;
        }
        s+=object_cstr(o);
        if (i<len){
            s+=selfstr;
        }
        i++;
    }
    vm->exception=NULL;
    return str_new_fromstr(s);
}