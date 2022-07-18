#define BUILTIN_PRINT builtins[0]
#define BUILTIN_BUILD_CLASS builtins[1]

object* builtin_print(object* self, object* args){
    printf("%s%s",object_cstr(args->type->slot_get(args, str_new_fromstr(new string("object")))).c_str(), object_cstr(args->type->slot_get(args, str_new_fromstr(new string("end")))).c_str());
    return new_none();
}

object* builtin___build_class__(object* self, object* args){
    object* function=args->type->slot_get(args, str_new_fromstr(new string("func")));
    object* dict;

    if (!object_istype(function->type, &FuncType)){
        vm_add_err(vm, "TypeError: expected function");
        return NULL;
    }

    add_callframe(vm->callstack, INCREF(new_int_fromint(0)), CAST_STRING(object_repr(function))->val, INCREF(CAST_FUNC(function)->code));

    vm->callstack->head->locals=dict_new(NULL, NULL);
    dict=INCREF(vm->callstack->head->locals);
    object* ret=function->type->slot_call(function, tuple_new(NULL, NULL), dict_new(NULL, NULL));
    pop_callframe(vm->callstack);
    
    object* c=class_new(NULL, NULL);
    CAST_CLASS(c)->dict=dict;
    CAST_CLASS(c)->name=INCREF(CAST_FUNC(function)->name);
    return c;
}