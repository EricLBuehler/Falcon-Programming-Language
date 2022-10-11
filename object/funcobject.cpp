object* func_new_code(object* code, object* args, object* kwargs, uint32_t argc, object* name, object* closure, int type, int flags\
    , object* stargs, object* stkwargs, object* annotations, bool isgen){
    object* obj=new_object(&FuncType);
    CAST_FUNC(obj)->code=code;
    CAST_FUNC(obj)->dict=new_dict();
    CAST_FUNC(obj)->args=args;
    CAST_FUNC(obj)->kwargs=kwargs;
    CAST_FUNC(obj)->argc=argc;
    CAST_FUNC(obj)->name=name;
    CAST_FUNC(obj)->closure=closure;
    CAST_FUNC(obj)->functype=type;
    CAST_FUNC(obj)->flags=flags;
    CAST_FUNC(obj)->stargs=stargs;
    CAST_FUNC(obj)->stkwargs=stkwargs;
    CAST_FUNC(obj)->annotations=annotations;
    CAST_FUNC(obj)->isgen=isgen;

    return obj;
}

object* func_call(object* self, object* args, object* kwargs){
    uint32_t argc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    uint32_t posargc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    uint32_t kwargc=argc-posargc;

    add_callframe(vm->callstack, tuple_index_int(list_index_int(CAST_CODE(CAST_FUNC(self)->code)->co_lines, 0),2),  CAST_STRING(CAST_FUNC(self)->name)->val, CAST_FUNC(self)->code, self);
    vm->callstack->head->locals=new_dict();
    dict_set(vm->callstack->head->locals, str_new_fromstr("__annotations__"), vm->callstack->head->annontations);
    
    
    int flags=CAST_FUNC(self)->flags;

    if (flags!=FUNC_STAR && CAST_FUNC(self)->argc-CAST_INT(CAST_FUNC(self)->kwargs->type->slot_mappings->slot_len(CAST_FUNC(self)->kwargs))->val->to_int()>posargc \
    || CAST_INT(CAST_FUNC(self)->kwargs->type->slot_mappings->slot_len(CAST_FUNC(self)->kwargs))->val->to_int()<kwargc \
    || CAST_FUNC(self)->argc<argc){
        //If we are starargs and positonal differs, no error
        if (flags==FUNC_STARARGS && CAST_FUNC(self)->argc-CAST_INT(CAST_FUNC(self)->kwargs->type->slot_mappings->slot_len(CAST_FUNC(self)->kwargs))->val->to_int()!=posargc){
            goto noerror;
        }
        //If we are starkwargs and positonal differs, no error
        if (flags==FUNC_STARKWARGS && CAST_INT(CAST_FUNC(self)->kwargs->type->slot_mappings->slot_len(CAST_FUNC(self)->kwargs))->val->to_int()!=kwargc){
            goto noerror;
        }
        if (CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()==0){
            vm_add_err(&ValueError, vm, "expected %d argument(s), got %d.",CAST_FUNC(self)->argc, argc);
            return NULL;
        }
    }
    noerror:
    
    setup_args_stars(vm->callstack->head->locals, CAST_FUNC(self)->argc, CAST_FUNC(self)->args, CAST_FUNC(self)->kwargs, args, kwargs, flags, CAST_FUNC(self)->stargs, CAST_FUNC(self)->stkwargs);
    uint32_t ip=0;
    object* ret;
    if (CAST_FUNC(self)->isgen){
        goto make_gen;
    }
    
    ret=run_vm(CAST_FUNC(self)->code, &ip);

    for (auto k: (*CAST_DICT(vm->callstack->head->locals)->val)){
        DECREF(k.first);
        if (k.second->type->size==0){
            ((object_var*)k.second)->gc_ref--;
        }
        DECREF(k.second);
    }

    make_gen:
    if (CAST_FUNC(self)->isgen){
        INCREF(vm->callstack->head->locals);
        pop_callframe(vm->callstack);
        return new_generator_impl(self, vm->callstack->head->locals);
    }
    pop_callframe(vm->callstack);
    return ret;
}

object* func_call_nostack(object* self, object* args, object* kwargs){
    uint32_t argc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    uint32_t posargc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    uint32_t kwargc=argc-posargc;

    if (CAST_FUNC(self)->argc-CAST_INT(CAST_FUNC(self)->kwargs->type->slot_mappings->slot_len(CAST_FUNC(self)->kwargs))->val->to_int()>posargc \
    || CAST_INT(CAST_FUNC(self)->kwargs->type->slot_mappings->slot_len(CAST_FUNC(self)->kwargs))->val->to_int()<kwargc \
    || CAST_FUNC(self)->argc<argc){
        if (CAST_INT(CAST_FUNC(self)->kwargs->type->slot_mappings->slot_len(CAST_FUNC(self)->kwargs))->val->to_int()-CAST_INT(CAST_FUNC(self)->kwargs->type->slot_mappings->slot_len(CAST_FUNC(self)->kwargs))->val->to_int()==0){
            vm_add_err(&ValueError, vm, "expected %d argument(s), got %d.",CAST_INT(CAST_FUNC(self)->args->type->slot_mappings->slot_len(CAST_FUNC(self)->args))->val->to_int(), argc);
            return NULL;
        }
        vm_add_err(&ValueError, vm, "expected %d to %d arguments, got %d.",CAST_INT(CAST_FUNC(self)->args->type->slot_mappings->slot_len(CAST_FUNC(self)->args))->val->to_int()-CAST_INT(CAST_FUNC(self)->kwargs->type->slot_mappings->slot_len(CAST_FUNC(self)->kwargs))->val->to_int(), CAST_FUNC(self)->argc, argc);
        return NULL;
    }

    setup_args(vm->callstack->head->locals, CAST_FUNC(self)->argc, CAST_FUNC(self)->args, CAST_FUNC(self)->kwargs, args, kwargs);
    uint32_t ip=0;

    object* ret=run_vm(CAST_FUNC(self)->code, &ip);
    return ret;
}

object* func_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="";
    s+="<function ";
    s+=object_cstr(CAST_FUNC(self)->name);
    s+=" @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(s);
}

object* func_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_FUNC(self)->code, CAST_FUNC(other)->code, type)) && \
        istrue(object_cmp(CAST_FUNC(self)->name, CAST_FUNC(other)->name, type))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_FUNC(self)->code, CAST_FUNC(other)->code, type)) || \
        !istrue(object_cmp(CAST_FUNC(self)->name, CAST_FUNC(other)->name, type))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

object* func_bool(object* self){
    return new_bool_true();
}

void func_del(object* obj){
    DECREF(CAST_FUNC(obj)->code);
    DECREF(CAST_FUNC(obj)->name);
    DECREF(CAST_FUNC(obj)->args);
    DECREF(CAST_FUNC(obj)->kwargs);
    DECREF(CAST_FUNC(obj)->dict);
    if (CAST_FUNC(obj)->closure!=NULL){
        DECREF(CAST_FUNC(obj)->closure);
    }
}

object* func_descrget(object* obj, object* self){
    return method_new_impl(self, obj);
}