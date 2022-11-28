object* range_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size;
    if (len!=3 && len!=2 && len!=1){
        vm_add_err(&ValueError, vm, "Expected 1, 2, or 3 arguments, got %d", len);
        return NULL;
    }
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    object* range=new_object(CAST_TYPE(type));
    object* arg=object_int(list_index_int(args, 0));
    if (arg==NULL || !object_istype(arg->type, &IntType)){
        FPLDECREF(range);
        vm_add_err(&ValueError, vm, "%s object cannot be coerced to int", list_index_int(args, 0)->type->name->c_str());
        return arg;
    }

    if (*CAST_INT(arg)->val<INT_MIN || *CAST_INT(arg)->val<INT_MAX){
        vm_add_err(&OverflowError, vm, "Value out of range of C int");
        return NULL; 
    }
    CAST_RANGE(range)->start=CAST_INT(arg)->val->to_int();
    if (len==2){
        arg=object_int(list_index_int(args, 1));
        if (arg==NULL || !object_istype(arg->type, &IntType)){
            FPLDECREF(range);
            vm_add_err(&ValueError, vm, "%s object cannot be coerced to int", list_index_int(args, 1)->type->name->c_str());
            return arg;
        }

        if (*CAST_INT(arg)->val<LLONG_MIN || *CAST_INT(arg)->val<LLONG_MAX){
            vm_add_err(&OverflowError, vm, "Value out of range of C long long");
            return NULL; 
        }
        CAST_RANGE(range)->end=CAST_INT(arg)->val->to_long_long();
        CAST_RANGE(range)->step=1;
    }
    else if (len==3){
        arg=object_int(list_index_int(args, 1));
        if (arg==NULL || !object_istype(arg->type, &IntType)){
            FPLDECREF(range);
            vm_add_err(&ValueError, vm, "%s object cannot be coerced to int", list_index_int(args, 1)->type->name->c_str());
            return arg;
        }

        if (*CAST_INT(arg)->val<LLONG_MIN || *CAST_INT(arg)->val<LLONG_MAX){
            vm_add_err(&OverflowError, vm, "Value out of range of C long long");
            return NULL; 
        }
        CAST_RANGE(range)->end=CAST_INT(arg)->val->to_long_long();
        
        arg=object_int(list_index_int(args, 2));
        if (arg==NULL || !object_istype(arg->type, &IntType)){
            FPLDECREF(range);
            vm_add_err(&ValueError, vm, "%s object cannot be coerced to int", list_index_int(args, 2)->type->name->c_str());
            return arg;
        }

        if (*CAST_INT(arg)->val<LLONG_MIN || *CAST_INT(arg)->val<LLONG_MAX){
            vm_add_err(&OverflowError, vm, "Value out of range of C long long");
            return NULL; 
        }
        CAST_RANGE(range)->step=CAST_INT(arg)->val->to_long_long();
    }
    else{
        CAST_RANGE(range)->end=CAST_RANGE(range)->start;
        CAST_RANGE(range)->start=0;
        CAST_RANGE(range)->step=1;
    }

    CAST_RANGE(range)->idx=CAST_RANGE(range)->start;
    return range;
}

void range_del(object* self){}

object* range_next(object* self){
    if (CAST_RANGE(self)->idx+CAST_RANGE(self)->step>CAST_RANGE(self)->end){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }
    object* o = new_int_fromint(CAST_RANGE(self)->idx);
    CAST_RANGE(self)->idx+=CAST_RANGE(self)->step;
    return o;
}

object* range_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_RANGE(self)->start==CAST_RANGE(other)->start && CAST_RANGE(self)->end==CAST_RANGE(other)->end &&\
            CAST_RANGE(self)->step==CAST_RANGE(other)->step){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (CAST_RANGE(self)->start!=CAST_RANGE(other)->start || CAST_RANGE(self)->end!=CAST_RANGE(other)->end ||\
            CAST_RANGE(self)->step!=CAST_RANGE(other)->step){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}