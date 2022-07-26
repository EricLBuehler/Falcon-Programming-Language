object* newtp_init(object* self, object* args, object* kwargs){
    return new_none();
}
object* newtp_new(object* self, object* args, object* kwargs){
    object* o=new_object((TypeObject*)self);
    CAST_NEWTYPE(o)->dict=new_dict();
    o->type->dict_offset=offsetof(NewTypeObject, dict);

    object_setattr(o, str_new_fromstr(new string("__class__")), self);
    return o;
}
void newtp_del(object* self){
    return;
}
object* newtp_next(object* self){
    return new_none();
}
object* newtp_get(object* self, object* idx){
    return new_none();
}
object* newtp_len(object* self){
    return new_none();
}
void newtp_set(object* self, object* idx, object* val){
    return;
}
void newtp_append(object* self, object* val){
    return;
}
object* newtp_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="<";
    s+=self->type->name->c_str();
    s+=" object @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(new string(s));
}
object* newtp_str(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="<";
    s+=self->type->name->c_str();
    s+=" object @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(new string(s));
}
object* newtp_call(object* self, object* args, object* kwargs){
    return new_none();
}
object* newtp_cmp(object* self, object* other, uint8_t type){
    return new_none();
}

object* newtp_add(object* self, object* other){
    return new_none();
}
object* newtp_sub(object* self, object* other){
    return new_none();
}
object* newtp_mul(object* self, object* other){
    return new_none();
}
object* newtp_div(object* self, object* other){
    return new_none();
}

object* newtp_neg(object* self){
    return new_none();
}