object* exception_new(object* type, object* args, object* kwargs){
    object* tp = new_object((TypeObject*)type);
    CAST_EXCEPTION(tp)->headers=new vector<string*>;
    CAST_EXCEPTION(tp)->snippets=new vector<string*>;
    CAST_EXCEPTION(tp)->err=new string("");
    return tp;
}

void exception_del(object* self){
    CAST_EXCEPTION(self)->headers;
    CAST_EXCEPTION(self)->snippets;
    CAST_EXCEPTION(self)->err;
}

object* exception_repr(object* self){
    string s="";
    for (int i=CAST_EXCEPTION(vm->exception)->headers->size(); i>0; i--){
        if (CAST_EXCEPTION(vm->exception)->headers->at(i-1)==NULL){
            continue;
        }
        s+=(*CAST_EXCEPTION(vm->exception)->headers->at(i-1))+"\n";
        s+=(*CAST_EXCEPTION(vm->exception)->snippets->at(i-1))+"\n";
    }
    s+=self->type->name->c_str();
    s+=": ";
    s+=(*CAST_EXCEPTION(vm->exception)->err)+"\n";
    return str_new_fromstr(new string(s));
}

object* exception_bool(object* self){
    return new_bool_true();
}

object* exception_call(object* type, object* args, object* kwargs){
    return new_none();
}

object* exception_cmp(object* self, object* other, uint8_t type){
    return new_bool_true();
}