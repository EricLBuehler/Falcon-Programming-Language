object* method_new_impl(object* func, object* instance){
    object* method=new_object(&MethodType);
    FPLINCREF(func);
    CAST_METHOD(method)->function=func;
    if (instance!=NULL){
        FPLINCREF(instance);
    }
    CAST_METHOD(method)->instance=instance;
    return method;
}

object* method_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }

    object* method=new_object(CAST_TYPE(type));
    object* o=list_index_int(args, 0);
    FPLINCREF(o);
    CAST_METHOD(method)->function=o;
    o=list_index_int(args, 1);
    FPLINCREF(o);
    CAST_METHOD(method)->instance=o;
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
        tuple_append_noinc(args_, CAST_METHOD(self)->instance);
    }
    for (int i=0; i<CAST_LIST(args)->size; i++){
        tuple_append_noinc(args_, list_index_int(args, i));
    }
    object* val=object_call(CAST_METHOD(self)->function, args_, kwargs);
    FPLDECREF(args_);
    return val;
}