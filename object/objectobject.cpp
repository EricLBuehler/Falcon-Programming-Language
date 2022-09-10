object* object_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (len!=0){
        vm_add_err(&ValueError, vm, "Expected 0 arguments, got %d", len);
        return NULL;
    }

    return new_object(CAST_TYPE(type));
}
object* object_repr_(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="<";
    s+=self->type->name->c_str();
    s+=" @ ";
    s+=buf;
    s+=">";
    return str_new_fromstr(s);
}
object* object_init(object* self, object* args, object* kwargs){
    return self;
}

object* object_bool(object* self){
    return new_bool_true();
}

object* object_cmp_(object* self, object* other, uint8_t type){
    if (type==CMP_EQ){
        if (self==other){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (self!=other){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}