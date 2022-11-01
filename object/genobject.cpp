//Expects strong reference for locals!
object* new_generator_impl(object* func, object* locals){
    object* obj=new_object(&GeneratorType);
    CAST_GEN(obj)->func=FPLINCREF(func);
    CAST_GEN(obj)->locals=locals;
    CAST_GEN(obj)->ip=0;
    CAST_GEN(obj)->done=false;
    
    CAST_GEN(obj)->objstack=new_datastack(CAST_INT(CAST_CODE(CAST_FUNC(func)->code)->co_stack_size)->val->to_int());
    CAST_GEN(obj)->callstack=new_callstack();
    CAST_GEN(obj)->blockstack=new_blockstack(CAST_INT(CAST_CODE(CAST_FUNC(func)->code)->co_blockstack_size)->val->to_int());

    return obj;
}

void gen_del(object* self){
    FPLDECREF(CAST_GEN(self)->func);
    FPLDECREF(CAST_GEN(self)->locals);
}

object* gen_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%p", self);

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
            add_callframe(vm->callstack, callstack_head(CAST_GEN(self)->callstack).line, callstack_head(CAST_GEN(self)->callstack).name,\
                callstack_head(CAST_GEN(self)->callstack).code, callstack_head(CAST_GEN(self)->callstack).callable, callstack_head(CAST_GEN(self)->callstack).ip);
            FPLDECREF(callstack_head(vm->callstack).annotations);
            callstack_head(vm->callstack).annotations=FPLINCREF(callstack_head(CAST_GEN(self)->callstack).annotations);
            pop_callframe(CAST_GEN(self)->callstack);
        }
    }

    if (CAST_GEN(self)->blockstack->size>0){
        while (CAST_GEN(self)->blockstack->size>0){
            add_blockframe(&(blockstack_head(CAST_GEN(self)->blockstack).start_ip), vm, vm->blockstack, blockstack_head(CAST_GEN(self)->blockstack).arg, blockstack_head(CAST_GEN(self)->blockstack).type);
            blockstack_head(vm->blockstack).obj=blockstack_head(CAST_GEN(self)->blockstack).obj;
            pop_blockframe(CAST_GEN(self)->blockstack);
        }
    }

    add_callframe(vm->callstack, tuple_index_int(list_index_int(CAST_CODE(CAST_FUNC(CAST_GEN(self)->func)->code)->co_lines, 0),2),  CAST_STRING(CAST_FUNC(CAST_GEN(self)->func)->name)->val, CAST_FUNC(CAST_GEN(self)->func)->code, self, &(CAST_GEN(self)->ip));
    callstack_head(vm->callstack).locals=CAST_GEN(self)->locals;

    uint32_t ip_=CAST_GEN(self)->ip;
    uint32_t datastack_size=vm->objstack->size;
    uint32_t callstack_size=vm->callstack->size;
    uint32_t blockstack_size=vm->blockstack->size;
    uint32_t realip=0;

    object* ret=run_vm(CAST_FUNC(CAST_GEN(self)->func)->code, &(CAST_GEN(self)->ip));

    
    uint32_t datastack_size_=vm->objstack->size;
    uint32_t callstack_size_=vm->callstack->size;
    uint32_t blockstack_size_=vm->blockstack->size;

    
    
    if (*CAST_INT(list_index_int(CAST_CODE(CAST_FUNC(CAST_GEN(self)->func)->code)->co_code, CAST_GEN(self)->ip-2))->val == RETURN_VAL){
        CAST_GEN(self)->done=true;
    }
    
    if (CAST_GEN(self)->done){
        if (datastack_size_>datastack_size){
            while (vm->objstack->size>datastack_size){
                pop_dataframe(vm->objstack);
            }
        }
        if (callstack_size_>callstack_size){
            while (vm->callstack->size>callstack_size){
                pop_callframe(vm->callstack);
            }
        }
        if (blockstack_size_>blockstack_size){
            while (vm->blockstack->size>blockstack_size){
                if (blockstack_head(vm->blockstack).type==WITH_BLOCK){
                    object_exit_with(blockstack_head(vm->blockstack).obj);
                }
                pop_blockframe(vm->blockstack);
            }
        }
        
        CAST_GEN(self)->done=true;
        vm_add_err(&StopIteration, vm, "Iterator out of data");

        pop_callframe(vm->callstack);
        return NULL;
    }

    if (datastack_size_>datastack_size){
        while (vm->objstack->size>datastack_size){
            add_dataframe(vm, CAST_GEN(self)->objstack, pop_dataframe(vm->objstack));
        }
    }
    if (callstack_size_>callstack_size){
        while (vm->callstack->size>callstack_size){
            add_callframe(CAST_GEN(self)->callstack, callstack_head(vm->callstack).line, callstack_head(vm->callstack).name,\
                callstack_head(vm->callstack).code, callstack_head(vm->callstack).callable, callstack_head(vm->callstack).ip);
            FPLDECREF(callstack_head(CAST_GEN(self)->callstack).annotations);
            callstack_head(CAST_GEN(self)->callstack).annotations=FPLINCREF(callstack_head(vm->callstack).annotations);
            pop_callframe(vm->callstack);
        }
    }
    if (blockstack_size_>blockstack_size){
        while (vm->blockstack->size>blockstack_size){
            add_blockframe(&(blockstack_head(vm->blockstack).start_ip), vm, CAST_GEN(self)->blockstack, blockstack_head(vm->blockstack).arg, blockstack_head(vm->blockstack).type);
            blockstack_head(CAST_GEN(self)->blockstack).obj=blockstack_head(vm->blockstack).obj;
            pop_blockframe(vm->blockstack);
        }
    }

    pop_callframe(vm->callstack);
    return ret;
}