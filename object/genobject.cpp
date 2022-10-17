//Expects strong reference for locals!
object* new_generator_impl(object* func, object* locals){
    object* obj=new_object(&GeneratorType);
    CAST_GEN(obj)->func=FPLINCREF(func);
    CAST_GEN(obj)->locals=locals;
    CAST_GEN(obj)->ip=0;
    CAST_GEN(obj)->done=false;
    
    CAST_GEN(obj)->objstack=new_datastack();
    CAST_GEN(obj)->callstack=new_callstack();
    CAST_GEN(obj)->blockstack=new_blockstack();

    return obj;
}

void gen_del(object* self){
    FPLDECREF(CAST_GEN(self)->func);
    FPLDECREF(CAST_GEN(self)->locals);
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

    if (CAST_GEN(self)->objstack->size>0){
        while (CAST_GEN(self)->objstack->size>0){
            add_dataframe(vm, vm->objstack, pop_dataframe(CAST_GEN(self)->objstack));
        }
    }

    if (CAST_GEN(self)->callstack->size>0){
        while (CAST_GEN(self)->callstack->size>0){
            add_callframe(vm->callstack, CAST_GEN(self)->callstack->head->line, CAST_GEN(self)->callstack->head->name,\
                CAST_GEN(self)->callstack->head->code, CAST_GEN(self)->callstack->head->callable);
            FPLDECREF(vm->callstack->head->annontations);
            vm->callstack->head->annontations=FPLINCREF(CAST_GEN(self)->callstack->head->annontations);
            pop_callframe(CAST_GEN(self)->callstack);
        }
    }

    if (CAST_GEN(self)->blockstack->size>0){
        while (CAST_GEN(self)->blockstack->size>0){
            add_blockframe(&(CAST_GEN(self)->blockstack->head->start_ip), vm, vm->blockstack, CAST_GEN(self)->blockstack->head->arg, CAST_GEN(self)->blockstack->head->type);
            pop_blockframe(CAST_GEN(self)->blockstack);
        }
    }

    add_callframe(vm->callstack, tuple_index_int(list_index_int(CAST_CODE(CAST_FUNC(CAST_GEN(self)->func)->code)->co_lines, 0),2),  CAST_STRING(CAST_FUNC(CAST_GEN(self)->func)->name)->val, CAST_FUNC(CAST_GEN(self)->func)->code, self);
    vm->callstack->head->locals=CAST_GEN(self)->locals;

    uint32_t ip_=CAST_GEN(self)->ip;
    uint32_t datastack_size=vm->objstack->size;
    uint32_t callstack_size=vm->callstack->size;
    uint32_t blockstack_size=vm->blockstack->size;
    uint32_t realip=0;

    object* ret=run_vm(CAST_FUNC(CAST_GEN(self)->func)->code, &(CAST_GEN(self)->ip));

    
    uint32_t datastack_size_=vm->objstack->size;
    uint32_t callstack_size_=vm->callstack->size;
    uint32_t blockstack_size_=vm->blockstack->size;

    if (datastack_size_>datastack_size){
        while (vm->objstack->size>datastack_size){
            add_dataframe(vm, CAST_GEN(self)->objstack, pop_dataframe(vm->objstack));
        }
    }
    if (callstack_size_>callstack_size){
        while (vm->callstack->size>callstack_size){
            add_callframe(CAST_GEN(self)->callstack, vm->callstack->head->line, vm->callstack->head->name,\
                vm->callstack->head->code, vm->callstack->head->callable);
            FPLDECREF(CAST_GEN(self)->callstack->head->annontations);
            CAST_GEN(self)->callstack->head->annontations=FPLINCREF(vm->callstack->head->annontations);
            pop_callframe(vm->callstack);
        }
    }
    if (blockstack_size_>blockstack_size){
        while (vm->blockstack->size>blockstack_size){
            add_blockframe(&(vm->blockstack->head->start_ip), vm, CAST_GEN(self)->blockstack, vm->blockstack->head->arg, vm->blockstack->head->type);
            pop_blockframe(vm->blockstack);
        }
    }
    
    if (*CAST_INT(list_index_int(CAST_CODE(CAST_FUNC(CAST_GEN(self)->func)->code)->co_code, CAST_GEN(self)->ip-2))->val == RETURN_VAL){
        CAST_GEN(self)->done=true;
    }
    
    if (CAST_GEN(self)->done){
        CAST_GEN(self)->done=true;
        vm_add_err(&StopIteration, vm, "Iterator out of data");

        pop_callframe(vm->callstack);
        return NULL;
    }

    pop_callframe(vm->callstack);
    return ret;
}