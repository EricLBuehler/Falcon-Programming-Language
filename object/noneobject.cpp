object* none_new(object* args, object* kwargs){
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
    return str_new_fromstr(new string("None"));
}

object* none_bool(object* self){
    return new_bool_false();
}

object* none_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
    }
    return new_bool_true();
}


void none_del(object* obj){}