typedef object* (*builtinfunc)(object*, object*);
object* new_builtin(builtinfunc function, object* name, object* args, object* kwargs, uint32_t argc);

void setup_builtins(){
    object* printargs=new_tuple();
    printargs->type->slot_mappings->slot_append(printargs, str_new_fromstr("object"));
    printargs->type->slot_mappings->slot_append(printargs, str_new_fromstr("end"));
    object* printkwargs=new_tuple();
    printkwargs->type->slot_mappings->slot_append(printkwargs, str_new_fromstr(""));
    printkwargs->type->slot_mappings->slot_append(printkwargs, str_new_fromstr("\n"));
    builtins[0]=new_builtin((builtinfunc)builtin_print, str_new_fromstr("print"), printargs, printkwargs, CAST_INT(printargs->type->slot_mappings->slot_len(printargs))->val->to_int());

    object* buildclassargs=new_tuple();
    buildclassargs->type->slot_mappings->slot_append(buildclassargs, str_new_fromstr("bases"));
    buildclassargs->type->slot_mappings->slot_append(buildclassargs, str_new_fromstr("name"));
    buildclassargs->type->slot_mappings->slot_append(buildclassargs, str_new_fromstr(("func")));
    object* buildclasskwargs=new_tuple();
    builtins[1]=new_builtin((builtinfunc)builtin___build_class__, str_new_fromstr("__build_class__"s), buildclassargs, buildclasskwargs, CAST_INT(buildclassargs->type->slot_mappings->slot_len(buildclassargs))->val->to_int());

    builtins[2]=(object*)&TypeType;
    builtins[3]=(object*)&IntType;
    builtins[4]=(object*)&StrType;
    builtins[5]=(object*)&DictType;
    builtins[6]=(object*)&ListType;
    builtins[7]=(object*)&TupleType;
    builtins[8]=(object*)&ObjectType;
    builtins[9]=(object*)&TypeError;
    builtins[10]=(object*)&ValueError;
    builtins[11]=(object*)&AttributeError;
    builtins[12]=(object*)&KeyError;
    builtins[13]=(object*)&NameError;
    builtins[14]=(object*)&IndexError;
    builtins[15]=(object*)&ExceptionType;

    object* idargs=new_tuple();
    idargs->type->slot_mappings->slot_append(idargs, str_new_fromstr("object"));
    object* idkwargs=new_tuple();
    builtins[16]=new_builtin((builtinfunc)builtin_id, str_new_fromstr("id"), idargs, idkwargs, 1);

    object* inputargs=new_tuple();
    inputargs->type->slot_mappings->slot_append(inputargs, str_new_fromstr("object"));
    object* inputkwargs=new_tuple();
    builtins[17]=new_builtin((builtinfunc)builtin_input, str_new_fromstr("input"), inputargs, inputkwargs, 1);
    
    builtins[18]=(object*)&FileType;

    object* reprargs=new_tuple();
    reprargs->type->slot_mappings->slot_append(reprargs, str_new_fromstr("object"));
    object* reprkwargs=new_tuple();
    reprkwargs->type->slot_mappings->slot_append(reprkwargs, str_new_fromstr(""));
    builtins[19]=new_builtin((builtinfunc)builtin_repr, str_new_fromstr("repr"), reprargs, reprkwargs, CAST_INT(reprargs->type->slot_mappings->slot_len(reprargs))->val->to_int());

    builtins[20]=(object*)&FloatType;

    builtins[21]=(object*)&StopIteration;

    object* iterargs=new_tuple();
    iterargs->type->slot_mappings->slot_append(iterargs, str_new_fromstr("object"));
    object* iterkwargs=new_tuple();
    iterkwargs->type->slot_mappings->slot_append(iterkwargs, new_none());
    builtins[22]=new_builtin((builtinfunc)builtin_iter, str_new_fromstr("iter"), iterargs, iterkwargs, CAST_INT(iterargs->type->slot_mappings->slot_len(iterargs))->val->to_int());

    object* nextargs=new_tuple();
    nextargs->type->slot_mappings->slot_append(nextargs, str_new_fromstr("object"));
    object* nextkwargs=new_tuple();
    nextkwargs->type->slot_mappings->slot_append(nextkwargs, new_none());
    builtins[23]=new_builtin((builtinfunc)builtin_next, str_new_fromstr("next"), nextargs, nextkwargs, CAST_INT(nextargs->type->slot_mappings->slot_len(nextargs))->val->to_int());

    builtins[24]=(object*)&RecursionError;
    builtins[25]=(object*)&MemoryError;
    builtins[26]=(object*)&FileNotFoundError;
    builtins[27]=(object*)&InvalidOperationError;
    builtins[28]=(object*)&ImportError;
    builtins[29]=(object*)&SliceType;

    builtins[30]=(object*)&BoolType;
    
    object* roundargs=new_tuple();
    roundargs->type->slot_mappings->slot_append(roundargs, str_new_fromstr("object"));
    roundargs->type->slot_mappings->slot_append(roundargs, str_new_fromstr("digits"));
    object* roundkwargs=new_tuple();
    roundkwargs->type->slot_mappings->slot_append(roundkwargs, new_int_fromint(0));
    builtins[31]=new_builtin((builtinfunc)builtin_round, str_new_fromstr("round"), roundargs, roundkwargs, CAST_INT(roundargs->type->slot_mappings->slot_len(roundargs))->val->to_int());
    
    builtins[32]=(object*)&EnumType;
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
    uint32_t argc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    uint32_t posargc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    uint32_t kwargc=argc-posargc;

    if (CAST_BUILTIN(self)->argc-CAST_INT(CAST_BUILTIN(self)->kwargs->type->slot_mappings->slot_len(CAST_BUILTIN(self)->kwargs))->val->to_int()>posargc \
    || CAST_INT(CAST_BUILTIN(self)->kwargs->type->slot_mappings->slot_len(CAST_BUILTIN(self)->kwargs))->val->to_int()<kwargc \
    || CAST_BUILTIN(self)->argc<argc){
        if (CAST_INT(CAST_BUILTIN(self)->kwargs->type->slot_mappings->slot_len(CAST_BUILTIN(self)->kwargs))->val->to_int()==0){
            vm_add_err(&ValueError, vm, "expected %d argument(s).",CAST_INT(CAST_BUILTIN(self)->args->type->slot_mappings->slot_len(CAST_BUILTIN(self)->args))->val->to_int());
            return NULL;
        }
        vm_add_err(&ValueError, vm, "expected %d to %d arguments, got %d.",CAST_INT(CAST_BUILTIN(self)->args->type->slot_mappings->slot_len(CAST_BUILTIN(self)->args))->val->to_int()-CAST_INT(CAST_BUILTIN(self)->kwargs->type->slot_mappings->slot_len(CAST_BUILTIN(self)->kwargs))->val->to_int(), CAST_BUILTIN(self)->argc, argc);
        return NULL;
    }
    

    object* builtinargs=new_dict();
    setup_args(builtinargs, CAST_BUILTIN(self)->argc, CAST_BUILTIN(self)->args, CAST_BUILTIN(self)->kwargs, args, kwargs);
    return CAST_BUILTIN(self)->function(self, builtinargs);
}

object* builtin_repr_slot(object* self){
    return str_new_fromstr("<builtin "+object_cstr(CAST_BUILTIN(self)->name)+">");
}

object* builtin_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_BUILTIN(self)->function==CAST_BUILTIN(other)->function){
            return new_bool_true();
        }
        return new_bool_false();
    }
   if (type==CMP_NE){
        if (CAST_BUILTIN(self)->function!=CAST_BUILTIN(other)->function){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

object* builtin_bool(object* self){
    return new_bool_true();
}

void builtin_del(object* obj){
    delete CAST_BUILTIN(obj)->name;
}