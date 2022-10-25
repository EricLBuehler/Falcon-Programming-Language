typedef object* (*builtinfunc)(object*, object*);
object* new_builtin(builtinfunc function, object* name, object* args, object* kwargs, uint32_t argc, bool nargs);

void setup_builtins(){
    object* printargs=new_tuple();
    printargs->type->slot_mappings->slot_append(printargs, str_new_fromstr("end"));
    printargs->type->slot_mappings->slot_append(printargs, str_new_fromstr("sep"));
    object* printkwargs=new_tuple();
    printkwargs->type->slot_mappings->slot_append(printkwargs, str_new_fromstr("\n"));
    printkwargs->type->slot_mappings->slot_append(printkwargs, str_new_fromstr(" "));
    builtins[0]=new_builtin((builtinfunc)builtin_print, str_new_fromstr("print"), printargs, printkwargs, CAST_INT(printargs->type->slot_mappings->slot_len(printargs))->val->to_int(), true);

    object* buildclassargs=new_tuple();
    buildclassargs->type->slot_mappings->slot_append(buildclassargs, str_new_fromstr("bases"));
    buildclassargs->type->slot_mappings->slot_append(buildclassargs, str_new_fromstr("name"));
    buildclassargs->type->slot_mappings->slot_append(buildclassargs, str_new_fromstr(("func")));
    object* buildclasskwargs=new_tuple();
    builtins[1]=new_builtin((builtinfunc)builtin___build_class__, str_new_fromstr("__build_class__"), buildclassargs, buildclasskwargs, CAST_INT(buildclassargs->type->slot_mappings->slot_len(buildclassargs))->val->to_int(), false);

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
    builtins[16]=new_builtin((builtinfunc)builtin_id, str_new_fromstr("id"), idargs, idkwargs, 1, false);

    object* inputargs=new_tuple();
    inputargs->type->slot_mappings->slot_append(inputargs, str_new_fromstr("object"));
    object* inputkwargs=new_tuple();
    inputkwargs->type->slot_mappings->slot_append(inputkwargs, str_new_fromstr(""));
    builtins[17]=new_builtin((builtinfunc)builtin_input, str_new_fromstr("input"), inputargs, inputkwargs, 1, false);
    
    builtins[18]=(object*)&FileType;

    object* reprargs=new_tuple();
    reprargs->type->slot_mappings->slot_append(reprargs, str_new_fromstr("object"));
    object* reprkwargs=new_tuple();
    reprkwargs->type->slot_mappings->slot_append(reprkwargs, str_new_fromstr(""));
    builtins[19]=new_builtin((builtinfunc)builtin_repr, str_new_fromstr("repr"), reprargs, reprkwargs, CAST_INT(reprargs->type->slot_mappings->slot_len(reprargs))->val->to_int(), false);

    builtins[20]=(object*)&FloatType;

    builtins[21]=(object*)&StopIteration;

    object* iterargs=new_tuple();
    iterargs->type->slot_mappings->slot_append(iterargs, str_new_fromstr("object"));
    object* iterkwargs=new_tuple();
    iterkwargs->type->slot_mappings->slot_append(iterkwargs, new_none());
    builtins[22]=new_builtin((builtinfunc)builtin_iter, str_new_fromstr("iter"), iterargs, iterkwargs, CAST_INT(iterargs->type->slot_mappings->slot_len(iterargs))->val->to_int(), false);

    object* nextargs=new_tuple();
    nextargs->type->slot_mappings->slot_append(nextargs, str_new_fromstr("object"));
    object* nextkwargs=new_tuple();
    nextkwargs->type->slot_mappings->slot_append(nextkwargs, new_none());
    builtins[23]=new_builtin((builtinfunc)builtin_next, str_new_fromstr("next"), nextargs, nextkwargs, CAST_INT(nextargs->type->slot_mappings->slot_len(nextargs))->val->to_int(), false);

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
    builtins[31]=new_builtin((builtinfunc)builtin_round, str_new_fromstr("round"), roundargs, roundkwargs, CAST_INT(roundargs->type->slot_mappings->slot_len(roundargs))->val->to_int(), false);
    
    builtins[32]=(object*)&EnumType;
    builtins[33]=(object*)&RangeType;
    builtins[34]=(object*)&ZipType;

    object* globalsargs=new_tuple();
    object* globalskwargs=new_tuple();
    builtins[35]=new_builtin((builtinfunc)builtin_globals, str_new_fromstr("globals"), globalsargs, globalskwargs, 0, false);

    object* localsargs=new_tuple();
    object* localskwargs=new_tuple();
    builtins[36]=new_builtin((builtinfunc)builtin_locals, str_new_fromstr("locals"), localsargs, localskwargs, 0, false);

    object* alllocalsargs=new_tuple();
    object* alllocalskwargs=new_tuple();
    builtins[37]=new_builtin((builtinfunc)builtin_alllocals, str_new_fromstr("alllocals"), alllocalsargs, alllocalskwargs, 0, false);

    object* exitargs=new_tuple();
    object* exitkwargs=new_tuple();
    builtins[38]=new_builtin((builtinfunc)builtin_exit, str_new_fromstr("exit"), exitargs, exitkwargs, 0, false);
    
    object* copyrightargs=new_tuple();
    object* copyrightkwargs=new_tuple();
    builtins[39]=new_builtin((builtinfunc)builtin_copyright, str_new_fromstr("copyright"), copyrightargs, copyrightkwargs, 0, false);
        
    object* lenargs=new_tuple();
    lenargs->type->slot_mappings->slot_append(lenargs, str_new_fromstr("object"));
    object* lenkwargs=new_tuple();
    builtins[40]=new_builtin((builtinfunc)builtin_len, str_new_fromstr("len"), lenargs, lenkwargs, 1, false);

    builtins[41]=(object*)&KeyboardInterrupt;
    builtins[42]=(object*)&AssertionError;

    object* isinstargs=new_tuple();
    isinstargs->type->slot_mappings->slot_append(isinstargs, str_new_fromstr("object"));
    isinstargs->type->slot_mappings->slot_append(isinstargs, str_new_fromstr("type"));
    object* isinstkwargs=new_tuple();
    builtins[43]=new_builtin((builtinfunc)builtin_issubclass, str_new_fromstr("isinstance"), isinstargs, isinstkwargs, 2, false);

    object* evalargs=new_tuple();
    evalargs->type->slot_mappings->slot_append(evalargs, str_new_fromstr("string"));
    evalargs->type->slot_mappings->slot_append(evalargs, str_new_fromstr("globals"));
    evalargs->type->slot_mappings->slot_append(evalargs, str_new_fromstr("locals"));
    object* evalkwargs=new_tuple();
    evalkwargs->type->slot_mappings->slot_append(evalkwargs, new_dict());
    evalkwargs->type->slot_mappings->slot_append(evalkwargs, new_dict());
    builtins[44]=new_builtin((builtinfunc)builtin_eval, str_new_fromstr("eval"), evalargs, evalkwargs, 3, false);

    builtins[45]=(object*)&SuperType;
    
    object* getattrargs=new_tuple();
    getattrargs->type->slot_mappings->slot_append(getattrargs, str_new_fromstr("object"));
    getattrargs->type->slot_mappings->slot_append(getattrargs, str_new_fromstr("attr"));
    object* getattrkwargs=new_tuple();
    builtins[46]=new_builtin((builtinfunc)builtin_getattr, str_new_fromstr("getattr"), getattrargs, getattrkwargs, 2, false);
    
    object* setattrargs=new_tuple();
    setattrargs->type->slot_mappings->slot_append(setattrargs, str_new_fromstr("object"));
    setattrargs->type->slot_mappings->slot_append(setattrargs, str_new_fromstr("attr"));
    setattrargs->type->slot_mappings->slot_append(setattrargs, str_new_fromstr("val"));
    object* setattrkwargs=new_tuple();
    builtins[47]=new_builtin((builtinfunc)builtin_setattr, str_new_fromstr("setattr"), setattrargs, setattrkwargs, 3, false);

    object* absargs=new_tuple();
    absargs->type->slot_mappings->slot_append(absargs, str_new_fromstr("self"));
    object* abskwargs=new_tuple();
    builtins[48]=new_builtin((builtinfunc)builtin_abs, str_new_fromstr("abs"), absargs, abskwargs, 1, false);
    
    builtins[49]=(object*)&ZeroDivisionError;
    
    object* iscallableargs=new_tuple();
    iscallableargs->type->slot_mappings->slot_append(iscallableargs, str_new_fromstr("object"));
    object* iscallablekwargs=new_tuple();
    builtins[50]=new_builtin((builtinfunc)builtin_iscallable, str_new_fromstr("iscallable"), iscallableargs, iscallablekwargs, 1, false);
    
    object* reverseargs=new_tuple();
    reverseargs->type->slot_mappings->slot_append(reverseargs, str_new_fromstr("object"));
    object* reversekwargs=new_tuple();
    builtins[51]=new_builtin((builtinfunc)builtin_reverse, str_new_fromstr("reverse"), reverseargs, reversekwargs, 1, false);
    
    object* isiterargs=new_tuple();
    isiterargs->type->slot_mappings->slot_append(isiterargs, str_new_fromstr("object"));
    object* isiterkwargs=new_tuple();
    builtins[52]=new_builtin((builtinfunc)builtin_isiter, str_new_fromstr("isiter"), isiterargs, isiterkwargs, 1, false);
    
    object* maxargs=new_tuple();
    maxargs->type->slot_mappings->slot_append(maxargs, str_new_fromstr("object"));
    object* maxkwargs=new_tuple();
    builtins[53]=new_builtin((builtinfunc)builtin_max, str_new_fromstr("max"), maxargs, maxkwargs, 1, false);
    
    object* minargs=new_tuple();
    minargs->type->slot_mappings->slot_append(minargs, str_new_fromstr("object"));
    object* minkwargs=new_tuple();
    builtins[54]=new_builtin((builtinfunc)builtin_min, str_new_fromstr("min"), minargs, minkwargs, 1, false);
    
    builtins[55]=(object*)&MapType;
    builtins[56]=(object*)&MethodType;
    builtins[57]=(object*)&ClassMethodType;
    builtins[58]=(object*)&StaticMethodType;
    builtins[59]=(object*)&PropertyType;
    
    object* getannotationargs=new_tuple();
    getannotationargs->type->slot_mappings->slot_append(getannotationargs, str_new_fromstr("name"));
    object* getannotationkwargs=new_tuple();
    builtins[60]=new_builtin((builtinfunc)builtin_getannotation, str_new_fromstr("getannotation"), getannotationargs, getannotationkwargs, 1, false);

    object* sumargs=new_tuple();
    sumargs->type->slot_mappings->slot_append(sumargs, str_new_fromstr("object"));
    object* sumkwargs=new_tuple();
    builtins[61]=new_builtin((builtinfunc)builtin_sum, str_new_fromstr("sum"), sumargs, sumkwargs, 1, false);

    builtins[62]=(object*)&SetType;    
    
    object* hasattrargs=new_tuple();
    hasattrargs->type->slot_mappings->slot_append(hasattrargs, str_new_fromstr("object"));
    hasattrargs->type->slot_mappings->slot_append(hasattrargs, str_new_fromstr("attr"));
    object* hasattrkwargs=new_tuple();
    builtins[63]=new_builtin((builtinfunc)builtin_sum, str_new_fromstr("hasattr"), hasattrargs, hasattrkwargs, 2, false);
}

object* new_builtin(builtinfunc function, object* name, object* args, object* kwargs, uint32_t argc, bool nargs){
    object* obj=new_object(&BuiltinType);
    CAST_BUILTIN(obj)->function=function;
    CAST_BUILTIN(obj)->name=name;
    CAST_BUILTIN(obj)->args=args;
    CAST_BUILTIN(obj)->kwargs=kwargs;
    CAST_BUILTIN(obj)->argc=argc;
    CAST_BUILTIN(obj)->nargs=nargs;
    return obj;
}

object* builtin_call(object* self, object* args, object* kwargs){
    uint32_t argc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    uint32_t posargc=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    uint32_t kwargc=argc-posargc;

    if (!CAST_BUILTIN(self)->nargs){
        if (CAST_BUILTIN(self)->argc-CAST_INT(CAST_BUILTIN(self)->kwargs->type->slot_mappings->slot_len(CAST_BUILTIN(self)->kwargs))->val->to_int()>posargc \
        || CAST_INT(CAST_BUILTIN(self)->kwargs->type->slot_mappings->slot_len(CAST_BUILTIN(self)->kwargs))->val->to_int()<kwargc \
        || CAST_BUILTIN(self)->argc<argc){
            if (CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()==0){
                vm_add_err(&ValueError, vm, "expected %d argument(s), got %d.",CAST_BUILTIN(self)->argc, argc);
                return NULL;
            }
        }
    }

    object* builtinargs=new_dict();
    if (!CAST_BUILTIN(self)->nargs){
        if (setup_args(builtinargs, CAST_BUILTIN(self)->argc, CAST_BUILTIN(self)->args, CAST_BUILTIN(self)->kwargs, args, kwargs)\
        ==NULL){
            return NULL;
        }
    }
    else{
        if (setup_args_allargs(builtinargs, CAST_BUILTIN(self)->argc, CAST_BUILTIN(self)->args, CAST_BUILTIN(self)->kwargs, args, kwargs)\
        ==NULL){
            return NULL;
        }
    }
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