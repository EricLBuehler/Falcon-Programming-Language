object* _new_none(){
    object* obj=new_object(&NoneType);
    return obj;
}

object* new_none(){
    FPLINCREF(noneobj);
    return noneobj;
}

object* none_repr(object* self){
    return str_new_fromstr("None");
}

object* none_bool(object* self){
    return new_bool_false();
}

void none_del(object* obj){}