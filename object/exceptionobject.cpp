object* exception_new(object* type, object* args, object* kwargs){
    object* tp = new_object((TypeObject*)type);
    CAST_EXCEPTION(tp)->err=new_none();
    if (CAST_INT(args->type->slot_len(args))->val->to_long()==1){
        DECREF(tp);
        return vm_setup_err((TypeObject*)type, vm, object_cstr(args->type->slot_get(args, new_int_fromint(0)) ).c_str() );
    }

    //CAST_EXCEPTION(tp)->err=new_none();
    return tp;
}

void exception_del(object* self){
    DECREF(CAST_EXCEPTION(self)->err);
}

object* exception_repr(object* self){
    string s="";
    s+=self->type->name->c_str();
    s+="(";
    s+=object_crepr(CAST_EXCEPTION(self)->err)+")";
    return str_new_fromstr(new string(s));
}

object* exception_str(object* self){
    return CAST_EXCEPTION(self)->err;
}
object* exception_bool(object* self){
    return new_bool_true();
}

object* exception_cmp(object* self, object* other, uint8_t type){
    return new_bool_true();
}