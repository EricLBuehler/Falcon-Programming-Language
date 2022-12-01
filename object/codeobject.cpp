object* code_init(object* self, object* args, object* kwargs){
    FPLINCREF(self);
    return self;
}

object* code_new_fromargs(object* args){
    object* obj=new_object(&CodeType);
    if (CAST_LIST(args)->size!=8){
        //Error
        return NULL;
    }

    object* o=list_index_int(args, 0);
    FPLINCREF(o);
    CAST_CODE(obj)->co_names=o;
    o=list_index_int(args, 1);
    FPLINCREF(o);
    CAST_CODE(obj)->co_consts=o;
    o=list_index_int(args, 2);
    FPLINCREF(o);
    CAST_CODE(obj)->co_code=o;
    o=list_index_int(args, 3);
    FPLINCREF(o);
    CAST_CODE(obj)->co_file=o;
    o=list_index_int(args, 4);
    FPLINCREF(o);
    CAST_CODE(obj)->co_lines=o;
    o=list_index_int(args, 5);
    FPLINCREF(o);
    CAST_CODE(obj)->co_detailed_lines=o;
    o=list_index_int(args, 6);
    FPLINCREF(o);
    CAST_CODE(obj)->co_stack_size=o;
    o=list_index_int(args, 7);
    FPLINCREF(o);
    CAST_CODE(obj)->co_blockstack_size=o;

    const uint32_t len=CAST_LIST(CAST_CODE(obj)->co_code)->size;
    CAST_CODE(obj)->code=new uint32_t[len];
    for (int i=0; i<len; i++){
        CAST_CODE(obj)->code[i]=CAST_INT(list_index_int(CAST_CODE(obj)->co_code, i))->val->to_int();
    }
    
    return obj;
}

object* code_new(object* type, object* args, object* kwargs){
    object* obj=new_object(&CodeType);
    if (args==NULL){
        return obj;
    }
    if (CAST_LIST(args)->size!=8){
        vm_add_err(&ValueError, vm, "Expected 8 arguments, got %d", CAST_LIST(args)->size);
        return NULL;
    }
    
    object* o=list_index_int(args, 0);
    FPLINCREF(o);
    CAST_CODE(obj)->co_names=o;
    o=list_index_int(args, 1);
    FPLINCREF(o);
    CAST_CODE(obj)->co_consts=o;
    o=list_index_int(args, 2);
    FPLINCREF(o);
    CAST_CODE(obj)->co_code=o;
    o=list_index_int(args, 3);
    FPLINCREF(o);
    CAST_CODE(obj)->co_file=o;
    o=list_index_int(args, 4);
    FPLINCREF(o);
    CAST_CODE(obj)->co_lines=o;
    o=list_index_int(args, 5);
    FPLINCREF(o);
    CAST_CODE(obj)->co_detailed_lines=o;
    o=list_index_int(args, 6);
    FPLINCREF(o);
    CAST_CODE(obj)->co_stack_size=o;
    o=list_index_int(args, 7);
    FPLINCREF(o);
    CAST_CODE(obj)->co_blockstack_size=o;
    
    const uint32_t len=CAST_LIST(CAST_CODE(obj)->co_code)->size;
    CAST_CODE(obj)->code=new uint32_t[len];
    for (int i=0; i<len; i++){
        CAST_CODE(obj)->code[i]=CAST_INT(list_index_int(CAST_CODE(obj)->co_code, i))->val->to_int();
    }

    return obj;
}

void code_del(object* obj){
    FPLDECREF(CAST_CODE(obj)->co_names);
    FPLDECREF(CAST_CODE(obj)->co_consts);
    FPLDECREF(CAST_CODE(obj)->co_code);
    FPLDECREF(CAST_CODE(obj)->co_lines);
    FPLDECREF(CAST_CODE(obj)->co_file);
    FPLDECREF(CAST_CODE(obj)->co_detailed_lines);
    FPLDECREF(CAST_CODE(obj)->co_stack_size);
    FPLDECREF(CAST_CODE(obj)->co_blockstack_size);
    delete CAST_CODE(obj)->code;
}

object* code_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%p", self);

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
        istrue(object_cmp(CAST_CODE(self)->co_code, CAST_CODE(other)->co_code, type)) && \
        istrue(object_cmp(CAST_CODE(self)->co_detailed_lines, CAST_CODE(other)->co_detailed_lines, type)) && \
        istrue(object_cmp(CAST_CODE(self)->co_stack_size, CAST_CODE(other)->co_stack_size, type)) && \
        istrue(object_cmp(CAST_CODE(self)->co_stack_size, CAST_CODE(other)->co_blockstack_size, type))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_CODE(self)->co_file, CAST_CODE(other)->co_file, type)) || \
        !istrue(object_cmp(CAST_CODE(self)->co_consts, CAST_CODE(other)->co_consts, type)) || \
        !istrue(object_cmp(CAST_CODE(self)->co_names, CAST_CODE(other)->co_names, type)) || \
        !istrue(object_cmp(CAST_CODE(self)->co_code, CAST_CODE(other)->co_code, type)) || \
        !istrue(object_cmp(CAST_CODE(self)->co_detailed_lines, CAST_CODE(other)->co_detailed_lines, type)) || \
        !istrue(object_cmp(CAST_CODE(self)->co_stack_size, CAST_CODE(other)->co_stack_size, type)) || \
        !istrue(object_cmp(CAST_CODE(self)->co_stack_size, CAST_CODE(other)->co_blockstack_size, type))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

object* code_bool(object* self){
    return new_bool_true();
}