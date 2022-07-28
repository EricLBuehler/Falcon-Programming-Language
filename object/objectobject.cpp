object* object_new(object* type, object* args, object* kwargs){
    return new_object(CAST_TYPE(type));
}
object* object_repr_(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="<";
    s+="object @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(new string(s));
}
object* object_init(object* self, object* args, object* kwargs){
    return self;
}
object* object_cmp_(object* self, object* other, uint8_t type){
    if (type==CMP_EQ){
        if (self==other){
            return new_bool_true();
        }
    }
    return new_bool_false();
}