object* newtp_init(object* self, object* args, object* kwargs){
    //Try to call __new__
    object* n=object_getattr(self, str_new_fromstr(new string("__init__")));
    if (n==NULL){
        vm->exception=NULL;
    }
    else{
        //args->type->slot_append(args, self); //do not need to
        object* val=object_call(n, args, kwargs);
        return val;
    }
    return new_none();
}
object* newtp_new(object* self, object* args, object* kwargs){
    object* o=new_object((TypeObject*)self);
    CAST_NEWTYPE(o)->dict=new_dict();
    o->type->dict_offset=offsetof(NewTypeObject, dict);

    //Setup dunder attributes
    object_setattr(o, str_new_fromstr(new string("__class__")), self);

    //Try to call __new__
    object* n=object_getattr(o, str_new_fromstr(new string("__new__")));
    if (n==NULL){
        vm->exception=NULL;
    }
    else{
        args->type->slot_append(args, o);
        object* val=object_call(n, args, kwargs);
        return val;
    }
    return o;
}
void newtp_del(object* self){
    object* n=object_getattr(self, str_new_fromstr(new string("__del__")));
    if (n==NULL){
        vm->exception=NULL;
    }
    else{
        object* args=new_tuple();
        args->type->slot_append(args, self);
        object_call(n, args, new_dict());
    }
}
object* newtp_next(object* self){
    return new_none();
}
object* newtp_get(object* self, object* idx){
    return new_none();
}
object* newtp_len(object* self){
    object* n=object_getattr(self, str_new_fromstr(new string("__len__")));
    if (n==NULL){
        vm->exception=NULL;
        return NULL;
    }
    else{
        object* args=new_tuple();
        args->type->slot_append(args, self);
        object* val=object_call(n, args, new_dict());
        return val;
    }
}
void newtp_set(object* self, object* idx, object* val){
    return;
}
void newtp_append(object* self, object* val){
    return;
}
object* newtp_repr(object* self){
    object* n=object_getattr(self, str_new_fromstr(new string("__repr__")));
    if (n==NULL){
        vm->exception=NULL;
    }
    else{
        object* args=new_tuple();
        args->type->slot_append(args, self);
        object* val=object_call(n, args, new_dict());
        return val;
    }

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
    object* n=object_getattr(self, str_new_fromstr(new string("__str__")));
    if (n==NULL){
        vm->exception=NULL;
    }
    else{
        object* args=new_tuple();
        args->type->slot_append(args, self);
        object* val=object_call(n, args, new_dict());
        return val;
    }

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
    //Try to call __call__
    object* function=object_getattr(self, str_new_fromstr(new string("__call__")));
    if (function==NULL){
        vm->exception=NULL;
    }
    else{
        uint32_t argc=CAST_INT(args->type->slot_len(args))->val->operator+((*CAST_INT(kwargs->type->slot_len(kwargs))->val)).to_int()+1;
        uint32_t posargc=CAST_INT(args->type->slot_len(args))->val->to_int()+1;
        uint32_t kwargc=argc-posargc;     

        if (function->type->slot_call==NULL){
            vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
            return NULL;
        }
        
        args->type->slot_append(args, self);
        object* val=object_call(function, args, kwargs);
        return val;
    }
    return new_none();
}
object* newtp_cmp(object* self, object* other, uint8_t type){
    return new_none();
}

object* newtp_add(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr(new string("__add__")));
    if (n==NULL){
        vm->exception=NULL;
        return NULL;
    }
    else{
        object* args=new_tuple();
        args->type->slot_append(args, self);
        object* val=object_call(n, args, new_dict());
        return val;
    }
}
object* newtp_sub(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr(new string("__sub__")));
    if (n==NULL){
        vm->exception=NULL;
        return NULL;
    }
    else{
        object* args=new_tuple();
        args->type->slot_append(args, self);
        object* val=object_call(n, args, new_dict());
        return val;
    }
}
object* newtp_mul(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr(new string("__mul__")));
    if (n==NULL){
        vm->exception=NULL;
        return NULL;
    }
    else{
        object* args=new_tuple();
        args->type->slot_append(args, self); //do not need to
        object* val=object_call(n, args, new_dict());
        return val;
    }
}
object* newtp_div(object* self, object* other){
    object* n=object_getattr(self, str_new_fromstr(new string("__div__")));
    if (n==NULL){
        vm->exception=NULL;
        return NULL;
    }
    else{
        object* args=new_tuple();
        args->type->slot_append(args, self);
        object* val=object_call(n, args, new_dict());
        return val;
    }
}

object* newtp_neg(object* self){
    object* n=object_getattr(self, str_new_fromstr(new string("__neg__")));
    if (n==NULL){
        vm->exception=NULL;
        return NULL;
    }
    else{
        object* args=new_tuple();
        args->type->slot_append(args, self);
        object* val=object_call(n, args, new_dict());
        return val;
    }
}

object* newtp_bool(object* self){
    object* n=object_getattr(self, str_new_fromstr(new string("__bool__")));
    if (n==NULL){
        vm->exception=NULL;
        return NULL;
    }
    else{
        object* args=new_tuple();
        args->type->slot_append(args, self);
        object* val=object_call(n, args, new_dict());
        return val;
    }
}
