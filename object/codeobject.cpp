object* code_init(object* self, object* args, object* kwargs){
    return self;
}

object* code_new_fromargs(object* args){
    object* obj=new_object(&CodeType);
    if (CAST_LIST(args)->size!=5){
        //Error
        return NULL;
    }
    CAST_CODE(obj)->co_names=INCREF(list_index_int(args, 0));
    CAST_CODE(obj)->co_consts=INCREF(list_index_int(args, 1));
    CAST_CODE(obj)->co_code=INCREF(list_index_int(args, 2));
    CAST_CODE(obj)->co_file=INCREF(list_index_int(args, 3));
    CAST_CODE(obj)->co_lines=INCREF(list_index_int(args, 4));
    
    return obj;
}

object* code_new(object* type, object* args, object* kwargs){
    object* obj=new_object(&CodeType);
    if (args==NULL){
        return obj;
    }
    if (CAST_LIST(args)->size!=5){
        vm_add_err(&ValueError, vm, "Expected 5 arguments, got %d", CAST_LIST(args)->size);
        return NULL;
    }
    CAST_CODE(obj)->co_names=INCREF(list_index_int(args, 0));
    CAST_CODE(obj)->co_consts=INCREF(list_index_int(args, 1));
    CAST_CODE(obj)->co_code=INCREF(list_index_int(args, 2));
    CAST_CODE(obj)->co_file=INCREF(list_index_int(args, 3));
    CAST_CODE(obj)->co_lines=INCREF(list_index_int(args, 4));
    return obj;
}

void code_del(object* obj){
    DECREF(CAST_CODE(obj)->co_names);
    DECREF(CAST_CODE(obj)->co_consts);
    DECREF(CAST_CODE(obj)->co_code);
    DECREF(CAST_CODE(obj)->co_lines);
    DECREF(CAST_CODE(obj)->co_file);
}

object* code_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="<code ";
    s+=" @ ";
    s+=buf;
    s+=", file ";
    s+=object_cstr(CAST_CODE(self)->co_file);
    s+=", line ";
    object* first=list_index_int(CAST_CODE(self)->co_lines, 0);
    s+=CAST_INT(tuple_index_int(first, 2))->val->operator+(1).to_string();
    s+=">";
    return str_new_fromstr(s);
}

object* code_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_CODE(self)->co_file, CAST_CODE(other)->co_file, type)) && \
        istrue(object_cmp(CAST_CODE(self)->co_consts, CAST_CODE(other)->co_consts, type)) && \
        istrue(object_cmp(CAST_CODE(self)->co_names, CAST_CODE(other)->co_names, type)) && \
        istrue(object_cmp(CAST_CODE(self)->co_code, CAST_CODE(other)->co_code, type))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_CODE(self)->co_file, CAST_CODE(other)->co_file, type)) || \
        !istrue(object_cmp(CAST_CODE(self)->co_consts, CAST_CODE(other)->co_consts, type)) || \
        !istrue(object_cmp(CAST_CODE(self)->co_names, CAST_CODE(other)->co_names, type)) || \
        !istrue(object_cmp(CAST_CODE(self)->co_code, CAST_CODE(other)->co_code, type))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

object* code_bool(object* self){
    return new_bool_true();
}