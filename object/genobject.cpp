//Expects strong reference for locals!
object* new_generator_impl(object* func, object* locals){
    object* obj=new_object(&GeneratorType);
    CAST_GEN(obj)->func=INCREF(func);
    CAST_GEN(obj)->locals=locals;
    CAST_GEN(obj)->ip=0;
    CAST_GEN(obj)->done=false;

    return obj;
}

void gen_del(object* self){
    DECREF(CAST_GEN(self)->func);
    DECREF(CAST_GEN(self)->locals);
}

object* gen_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="";
    s+="<generator ";
    s+=object_cstr(CAST_FUNC(CAST_GEN(self)->func)->name);
    s+=" @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(s);
}

object* gen_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_GEN(self)->func, CAST_GEN(other)->func, type)) && \
        istrue(object_cmp(CAST_GEN(self)->locals, CAST_GEN(other)->locals, type))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_GEN(self)->func, CAST_GEN(other)->func, type)) || \
        !istrue(object_cmp(CAST_GEN(self)->locals, CAST_GEN(other)->locals, type))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

object* gen_next(object* self){
    if (CAST_GEN(self)->done){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }

    add_callframe(vm->callstack, tuple_index_int(list_index_int(CAST_CODE(CAST_FUNC(CAST_GEN(self)->func)->code)->co_lines, 0),2),  CAST_STRING(CAST_FUNC(CAST_GEN(self)->func)->name)->val, CAST_FUNC(CAST_GEN(self)->func)->code, self);
    vm->callstack->head->locals=CAST_GEN(self)->locals;

    uint32_t ip_=CAST_GEN(self)->ip;
    object* ret=run_vm(CAST_FUNC(CAST_GEN(self)->func)->code, &(CAST_GEN(self)->ip));
    if (*CAST_INT(list_index_int(CAST_CODE(CAST_FUNC(CAST_GEN(self)->func)->code)->co_code, CAST_GEN(self)->ip-2))->val == RETURN_VAL){
        CAST_GEN(self)->done=true;
    }
    if (CAST_GEN(self)->ip==ip_){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }

    pop_callframe(vm->callstack);
    return ret;
}