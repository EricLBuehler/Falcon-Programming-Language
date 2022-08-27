object* func_new_code(object* code, object* args, object* kwargs, uint32_t argc, object* name){
    object* obj=new_object(&FuncType);
    CAST_FUNC(obj)->code=code;
    CAST_FUNC(obj)->dict=new_dict();
    CAST_FUNC(obj)->args=args;
    CAST_FUNC(obj)->kwargs=kwargs;
    CAST_FUNC(obj)->argc=argc;
    CAST_FUNC(obj)->name=name;
    
    return obj;
}

object* func_new(object* type, object* args, object* kwargs){
    object* obj=new_object(CAST_TYPE(type));
    if (CAST_LIST(args)->size!=5){
        //Error
        return NULL;
    }
    CAST_FUNC(obj)->code=INCREF(args->type->slot_mappings->slot_get(args, new_int_fromint(0)));
    CAST_FUNC(obj)->args=INCREF(args->type->slot_mappings->slot_get(args, new_int_fromint(1)));
    CAST_FUNC(obj)->kwargs=INCREF(args->type->slot_mappings->slot_get(args, new_int_fromint(2)));
    CAST_FUNC(obj)->argc=CAST_INT(args->type->slot_mappings->slot_get(args, new_int_fromint(3)))->val->to_int();
    CAST_FUNC(obj)->name=INCREF(args->type->slot_mappings->slot_get(args, new_int_fromint(4)));
    CAST_FUNC(obj)->dict=new_dict();
    
    return obj;
}

object* func_call(object* self, object* args, object* kwargs){
    uint32_t argc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    uint32_t posargc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    uint32_t kwargc=argc-posargc;
    
    add_callframe(vm->callstack, new_int_fromint(0),  CAST_STRING(CAST_FUNC(self)->name)->val, CAST_FUNC(self)->code);
    vm->callstack->head->locals=new_dict();

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
    string s="";
    s+="<function ";
    s+=object_cstr(CAST_FUNC(self)->name);
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
}