object* func_new_code(object* code, object* args, object* kwargs, uint32_t argc, object* name){
    object* obj=new_object(&FuncType);
    CAST_FUNC(obj)->code=code;
    CAST_FUNC(obj)->dict=dict_new(NULL, NULL);
    CAST_FUNC(obj)->args=args;
    CAST_FUNC(obj)->kwargs=kwargs;
    CAST_FUNC(obj)->argc=argc;
    CAST_FUNC(obj)->name=name;
    
    return obj;
}

object* func_new(object* args, object* kwargs){
    object* obj=new_object(&FuncType);
    if (CAST_LIST(args)->size!=5){
        //Error
        return NULL;
    }
    CAST_FUNC(obj)->code=args->type->slot_get(args, new_int_fromint(0));
    CAST_FUNC(obj)->args=args->type->slot_get(args, new_int_fromint(1));
    CAST_FUNC(obj)->kwargs=args->type->slot_get(args, new_int_fromint(2));
    CAST_FUNC(obj)->argc=CAST_INT(args->type->slot_get(args, new_int_fromint(3)))->val->to_int();
    CAST_FUNC(obj)->name=args->type->slot_get(args, new_int_fromint(4));
    CAST_FUNC(obj)->dict=dict_new(NULL, NULL);
    
    if (args!=NULL){
        DECREF(args);
    }
    if (kwargs!=NULL){
        DECREF(kwargs);
    }
    return obj;
}

object* func_call(object* self, object* args, object* kwargs){
    //Argument initialization

    uint32_t argn=0;
    uint32_t argsnum=CAST_FUNC(self)->argc-CAST_INT(CAST_FUNC(self)->kwargs->type->slot_len(CAST_FUNC(self)->kwargs))->val->to_int();   

    //Positional
    object* key=args->type->slot_next(args);
    
    while (key){
        vm->callstack->head->locals->type->slot_set(vm->callstack->head->locals, CAST_FUNC(self)->args->type->slot_get(CAST_FUNC(self)->args, new_int_fromint(argn)), key);
        argn++;
        key=args->type->slot_next(args);
    }
    //


    //Keyword    
    key=CAST_FUNC(self)->kwargs->type->slot_next(CAST_FUNC(self)->kwargs);
    uint32_t argn_tmp=argsnum;
    while (key){
        vm->callstack->head->locals->type->slot_set(vm->callstack->head->locals, CAST_FUNC(self)->args->type->slot_get(CAST_FUNC(self)->args, new_int_fromint(argn_tmp)), key);
        argn_tmp++;
        key=CAST_FUNC(self)->kwargs->type->slot_next(CAST_FUNC(self)->kwargs);
    }
    
    for (auto k: (*CAST_DICT(kwargs)->val)){
        //Check if k.first in self.args
        if (!object_find_bool(CAST_FUNC(self)->args, k.first)){
            //Error
            return NULL;
        }
        //

        vm->callstack->head->locals->type->slot_set(vm->callstack->head->locals, k.first, k.second);
        argn++;
        kwargs=args->type->slot_next(kwargs);
    }

    //
    uint32_t ip=0;
    return run_vm(CAST_FUNC(self)->code, &ip);
}

object* func_repr(object* self){
    string s="";
    s+="<function ";
    s+=object_cstr(CAST_FUNC(self)->name);
    s+=">";
    return str_new_fromstr(new string(s));
}

object* func_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_FUNC(self)->code, CAST_FUNC(other)->code, type)) && \
        istrue(object_cmp(CAST_FUNC(self)->name, CAST_FUNC(other)->name, type))){
            return new_bool_true();
        }
    }
    return new_bool_false();
}

void func_del(object* obj){}