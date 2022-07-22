typedef object* (*builtinfunc)(object*, object*);
object* new_builtin(builtinfunc function, object* name, object* args, object* kwargs, uint32_t argc);

void setup_builtins(){
    object* printargs=new_tuple();
    printargs->type->slot_append(printargs, str_new_fromstr(new string("object")));
    printargs->type->slot_append(printargs, str_new_fromstr(new string("end")));
    object* printkwargs=new_tuple();
    printkwargs->type->slot_append(printkwargs, str_new_fromstr(new string("\n")));
    builtins[0]=new_builtin((builtinfunc)builtin_print, str_new_fromstr(new string("print")), printargs, printkwargs, CAST_INT(printargs->type->slot_len(printargs))->val->to_int());


    object* buildclassargs=new_tuple();
    buildclassargs->type->slot_append(buildclassargs, str_new_fromstr(new string("bases")));
    buildclassargs->type->slot_append(buildclassargs, str_new_fromstr(new string("name")));
    buildclassargs->type->slot_append(buildclassargs, str_new_fromstr(new string("func")));
    object* buildclasskwargs=new_tuple();
    builtins[1]=new_builtin((builtinfunc)builtin___build_class__, str_new_fromstr(new string("__build_class__")), buildclassargs, buildclasskwargs, CAST_INT(buildclassargs->type->slot_len(buildclassargs))->val->to_int());
}

object* new_builtin(builtinfunc function, object* name, object* args, object* kwargs, uint32_t argc){
    object* obj=new_object(&BuiltinType);
    CAST_BUILTIN(obj)->function=function;
    CAST_BUILTIN(obj)->name=name;
    CAST_BUILTIN(obj)->args=args;
    CAST_BUILTIN(obj)->kwargs=kwargs;
    CAST_BUILTIN(obj)->argc=argc;
    return obj;
}

object* builtin_call(object* self, object* args, object* kwargs){
    object* builtinargs=new_dict();
    return CAST_BUILTIN(self)->function(self, setup_args(builtinargs, CAST_BUILTIN(self)->argc, CAST_BUILTIN(self)->args, CAST_BUILTIN(self)->kwargs, args, kwargs));
}

object* builtin_repr(object* self){
    return str_new_fromstr(new string("<builtin function or method "+object_cstr(CAST_BUILTIN(self)->name)+">" ));
}

object* builtin_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return new_bool_false();
    }
    if (CAST_BUILTIN(self)->function!=CAST_BUILTIN(other)->function){
        return new_bool_false();
    }
    return new_bool_true();
}


void builtin_del(object* obj){
    delete CAST_BUILTIN(obj)->name;
}