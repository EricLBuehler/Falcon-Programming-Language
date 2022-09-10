object* none_new(object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (len!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }

    object* obj=new_object(&NoneType);
    return obj;
}

object* _new_none(){
    object* obj=new_object(&NoneType);
    return obj;
}

object* new_none(){
    return INCREF(noneobj);
}

object* none_repr(object* self){
    return str_new_fromstr("None");
}

object* none_bool(object* self){
    return new_bool_false();
}

void none_del(object* obj){}