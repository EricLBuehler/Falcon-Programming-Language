object* method_new_impl(object* func, object* instance){
    object* method=new_object(&MethodType);
    CAST_METHOD(method)->function=FPLINCREF(func);
    if (instance!=NULL){
        FPLINCREF(instance);
    }
    CAST_METHOD(method)->instance=instance;
    return method;
}

object* method_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }

    object* method=new_object(CAST_TYPE(type));
    CAST_METHOD(method)->function=FPLINCREF(list_index_int(args, 0));
    CAST_METHOD(method)->instance=FPLINCREF(list_index_int(args, 1));
    return method;
}

void method_del(object* self){
    FPLDECREF(CAST_METHOD(self)->function);
    if (CAST_METHOD(self)->instance!=NULL){
        FPLDECREF(CAST_METHOD(self)->instance);
    }
}

object* method_repr(object* self){
    string s="<method ";
    s+=object_cstr(CAST_METHOD(self)->function);
    s+=">";
    return str_new_fromstr(s);
}

object* method_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_METHOD(self)->function,CAST_METHOD(other)->function, CMP_EQ)) &&\
        !( (CAST_METHOD(self)->instance!=NULL && CAST_METHOD(other)->instance!=NULL) && istrue(object_cmp(CAST_METHOD(self)->instance,CAST_METHOD(other)->instance, CMP_EQ)))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_METHOD(self)->function,CAST_METHOD(other)->function, CMP_EQ)) &&\
        !( (CAST_METHOD(self)->instance!=NULL && CAST_METHOD(other)->instance!=NULL) && istrue(object_cmp(CAST_METHOD(self)->instance,CAST_METHOD(other)->instance, CMP_EQ)))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

object* method_call(object* self, object* args, object* kwargs){
    object* args_=new_tuple();
    if (CAST_METHOD(self)->instance!=NULL){
        args_->type->slot_mappings->slot_append(args_, CAST_METHOD(self)->instance);
    }
    for (int i=0; i<CAST_LIST(args)->size; i++){
        args_->type->slot_mappings->slot_append(args_, list_index_int(args, i));
    }
    object* val=object_call(CAST_METHOD(self)->function, args_, kwargs);
    return val;
}