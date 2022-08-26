#define BUILTIN_PRINT builtins[0]
#define BUILTIN_BUILD_CLASS builtins[1]

object* builtin_print(object* self, object* args){
    printf("%s%s",object_cstr(args->type->slot_mappings->slot_get(args, str_new_fromstr("object"))).c_str(), object_cstr(dict_get(args, str_new_fromstr("end"))).c_str());
    return new_none();
}

object* builtin_repr(object* self, object* args){
    return object_repr(args->type->slot_mappings->slot_get(args, str_new_fromstr("object")));
}

object* builtin___build_class__(object* self, object* args){
    object* function=args->type->slot_mappings->slot_get(args, str_new_fromstr("func"));
    object* name=args->type->slot_mappings->slot_get(args, str_new_fromstr("name"));
    object* dict;
    

    if (!object_istype(function->type, &FuncType)){
        vm_add_err(&TypeError, vm, "expected function");
        return NULL;
    }
    
    add_callframe(vm->callstack, new_int_fromint(0),  CAST_STRING(CAST_FUNC(function)->name)->val, INCREF(CAST_FUNC(function)->code));
    vm->callstack->head->locals=new_dict();
    
    object* kwargs=new_dict();
    object* fargs=new_list();
    object* ret=func_call_nostack(function, fargs, kwargs);
    DECREF(kwargs);
    DECREF(fargs);

    dict=INCREF(vm->callstack->head->locals);
    pop_callframe(vm->callstack);
    if (ret==NULL){
        return NULL;
    }
    
    object* t=new_type(CAST_STRING(name)->val, args->type->slot_mappings->slot_get(args, str_new_fromstr("bases")), dict);
    return t;
}

object* builtin_id(object* self, object* args){
    object* obj=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    char buf[32];
    sprintf(buf, "%d", obj);
    return new_int_fromstr(new string(buf));
}

object* builtin_input(object* self, object* args){
    object* obj=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    cout<<object_cstr(obj);
    string s="";
    cin>>s;
    return str_new_fromstr(s);
}

object* builtin_iter(object* self, object* args){
    object* obj=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    if (obj->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not an iterable", obj->type->name->c_str());
        return NULL;
    }
    return obj->type->slot_iter(obj);
}

object* builtin_next(object* self, object* args){
    object* obj=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    if (obj->type->slot_next==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not an iterable", obj->type->name->c_str());
        return NULL;
    }
    return obj->type->slot_next(obj);
}