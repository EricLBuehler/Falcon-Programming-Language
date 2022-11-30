void setup_builtins(){
    object* printargs=new_tuple();
    tuple_append_noinc(printargs, str_new_fromstr("end"));
    tuple_append_noinc(printargs, str_new_fromstr("sep"));
    object* printkwargs=new_tuple();
    tuple_append_noinc(printkwargs, str_new_fromstr("\n"));
    tuple_append_noinc(printkwargs, str_new_fromstr(" "));
    builtins[0]=new_builtin((builtinfunc)builtin_print, str_new_fromstr("print"), printargs, printkwargs, 2, true);

    object* buildclassargs=new_tuple();
    tuple_append_noinc(buildclassargs, str_new_fromstr("doc"));
    tuple_append_noinc(buildclassargs, str_new_fromstr("bases"));
    tuple_append_noinc(buildclassargs, str_new_fromstr("name"));
    tuple_append_noinc(buildclassargs, str_new_fromstr(("func")));
    object* buildclasskwargs=new_tuple();
    builtins[1]=new_builtin((builtinfunc)builtin___build_class__, str_new_fromstr("__build_class__"), buildclassargs, buildclasskwargs, 4, false);

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
    tuple_append_noinc(idargs, str_new_fromstr("object"));
    object* idkwargs=new_tuple();
    builtins[16]=new_builtin((builtinfunc)builtin_id, str_new_fromstr("id"), idargs, idkwargs, 1, false);

    object* inputargs=new_tuple();
    tuple_append_noinc(inputargs, str_new_fromstr("object"));
    object* inputkwargs=new_tuple();
    tuple_append_noinc(inputkwargs, str_new_fromstr(""));
    builtins[17]=new_builtin((builtinfunc)builtin_input, str_new_fromstr("input"), inputargs, inputkwargs, 1, false);
    
    builtins[18]=(object*)&FileType;

    object* reprargs=new_tuple();
    tuple_append_noinc(reprargs, str_new_fromstr("object"));
    object* reprkwargs=new_tuple();
    tuple_append_noinc(reprkwargs, str_new_fromstr(""));
    builtins[19]=new_builtin((builtinfunc)builtin_repr, str_new_fromstr("repr"), reprargs, reprkwargs, 1, false);

    builtins[20]=(object*)&FloatType;

    builtins[21]=(object*)&StopIteration;

    object* iterargs=new_tuple();
    tuple_append_noinc(iterargs, str_new_fromstr("object"));
    object* iterkwargs=new_tuple();
    tuple_append_noinc(iterkwargs, new_none());
    builtins[22]=new_builtin((builtinfunc)builtin_iter, str_new_fromstr("iter"), iterargs, iterkwargs, 1, false);

    object* nextargs=new_tuple();
    tuple_append_noinc(nextargs, str_new_fromstr("object"));
    object* nextkwargs=new_tuple();
    tuple_append_noinc(nextkwargs, new_none());
    builtins[23]=new_builtin((builtinfunc)builtin_next, str_new_fromstr("next"), nextargs, nextkwargs, 1, false);

    builtins[24]=(object*)&RecursionError;
    builtins[25]=(object*)&MemoryError;
    builtins[26]=(object*)&FileNotFoundError;
    builtins[27]=(object*)&InvalidOperationError;
    builtins[28]=(object*)&ImportError;
    builtins[29]=(object*)&SliceType;

    builtins[30]=(object*)&BoolType;
    
    object* roundargs=new_tuple();
    tuple_append_noinc(roundargs, str_new_fromstr("object"));
    tuple_append_noinc(roundargs, str_new_fromstr("digits"));
    object* roundkwargs=new_tuple();
    tuple_append_noinc(roundkwargs, new_int_fromint(0));
    builtins[31]=new_builtin((builtinfunc)builtin_round, str_new_fromstr("round"), roundargs, roundkwargs, 2, false);
    
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
    tuple_append_noinc(lenargs, str_new_fromstr("object"));
    object* lenkwargs=new_tuple();
    builtins[40]=new_builtin((builtinfunc)builtin_len, str_new_fromstr("len"), lenargs, lenkwargs, 1, false);

    builtins[41]=(object*)&KeyboardInterrupt;
    builtins[42]=(object*)&AssertionError;

    object* isinstargs=new_tuple();
    tuple_append_noinc(isinstargs, str_new_fromstr("object"));
    tuple_append_noinc(isinstargs, str_new_fromstr("type"));
    object* isinstkwargs=new_tuple();
    builtins[43]=new_builtin((builtinfunc)builtin_issubclass, str_new_fromstr("isinstance"), isinstargs, isinstkwargs, 2, false);

    object* evalargs=new_tuple();
    tuple_append_noinc(evalargs, str_new_fromstr("string"));
    tuple_append_noinc(evalargs, str_new_fromstr("globals"));
    tuple_append_noinc(evalargs, str_new_fromstr("locals"));
    object* evalkwargs=new_tuple();
    tuple_append_noinc(evalkwargs, new_dict());
    tuple_append_noinc(evalkwargs, new_dict());
    builtins[44]=new_builtin((builtinfunc)builtin_eval, str_new_fromstr("eval"), evalargs, evalkwargs, 3, false);

    builtins[45]=(object*)&SuperType;
    
    object* getattrargs=new_tuple();
    tuple_append_noinc(getattrargs, str_new_fromstr("object"));
    tuple_append_noinc(getattrargs, str_new_fromstr("attr"));
    object* getattrkwargs=new_tuple();
    builtins[46]=new_builtin((builtinfunc)builtin_getattr, str_new_fromstr("getattr"), getattrargs, getattrkwargs, 2, false);
    
    object* setattrargs=new_tuple();
    tuple_append_noinc(setattrargs, str_new_fromstr("object"));
    tuple_append_noinc(setattrargs, str_new_fromstr("attr"));
    tuple_append_noinc(setattrargs, str_new_fromstr("val"));
    object* setattrkwargs=new_tuple();
    builtins[47]=new_builtin((builtinfunc)builtin_setattr, str_new_fromstr("setattr"), setattrargs, setattrkwargs, 3, false);

    object* absargs=new_tuple();
    tuple_append_noinc(absargs, str_new_fromstr("self"));
    object* abskwargs=new_tuple();
    builtins[48]=new_builtin((builtinfunc)builtin_abs, str_new_fromstr("abs"), absargs, abskwargs, 1, false);
    
    builtins[49]=(object*)&ZeroDivisionError;
    
    object* iscallableargs=new_tuple();
    tuple_append_noinc(iscallableargs, str_new_fromstr("object"));
    object* iscallablekwargs=new_tuple();
    builtins[50]=new_builtin((builtinfunc)builtin_iscallable, str_new_fromstr("iscallable"), iscallableargs, iscallablekwargs, 1, false);
    
    object* reverseargs=new_tuple();
    tuple_append_noinc(reverseargs, str_new_fromstr("object"));
    object* reversekwargs=new_tuple();
    builtins[51]=new_builtin((builtinfunc)builtin_reverse, str_new_fromstr("reverse"), reverseargs, reversekwargs, 1, false);
    
    object* isiterargs=new_tuple();
    tuple_append_noinc(isiterargs, str_new_fromstr("object"));
    object* isiterkwargs=new_tuple();
    builtins[52]=new_builtin((builtinfunc)builtin_isiter, str_new_fromstr("isiter"), isiterargs, isiterkwargs, 1, false);
    
    object* maxargs=new_tuple();
    tuple_append_noinc(maxargs, str_new_fromstr("object"));
    object* maxkwargs=new_tuple();
    builtins[53]=new_builtin((builtinfunc)builtin_max, str_new_fromstr("max"), maxargs, maxkwargs, 1, false);
    
    object* minargs=new_tuple();
    tuple_append_noinc(minargs, str_new_fromstr("object"));
    object* minkwargs=new_tuple();
    builtins[54]=new_builtin((builtinfunc)builtin_min, str_new_fromstr("min"), minargs, minkwargs, 1, false);
    
    builtins[55]=(object*)&MapType;
    builtins[56]=(object*)&MethodType;
    builtins[57]=(object*)&ClassMethodType;
    builtins[58]=(object*)&StaticMethodType;
    builtins[59]=(object*)&PropertyType;
    
    object* getannotationargs=new_tuple();
    tuple_append_noinc(getannotationargs, str_new_fromstr("name"));
    object* getannotationkwargs=new_tuple();
    builtins[60]=new_builtin((builtinfunc)builtin_getannotation, str_new_fromstr("getannotation"), getannotationargs, getannotationkwargs, 1, false);

    object* sumargs=new_tuple();
    tuple_append_noinc(sumargs, str_new_fromstr("object"));
    object* sumkwargs=new_tuple();
    builtins[61]=new_builtin((builtinfunc)builtin_sum, str_new_fromstr("sum"), sumargs, sumkwargs, 1, false);

    builtins[62]=(object*)&SetType;    
    
    object* hasattrargs=new_tuple();
    tuple_append_noinc(hasattrargs, str_new_fromstr("object"));
    tuple_append_noinc(hasattrargs, str_new_fromstr("attr"));
    object* hasattrkwargs=new_tuple();
    builtins[63]=new_builtin((builtinfunc)builtin_sum, str_new_fromstr("hasattr"), hasattrargs, hasattrkwargs, 2, false);

    builtins[64]=(object*)&BytesType;

    object* dirargs=new_tuple();
    tuple_append_noinc(dirargs, str_new_fromstr("object"));
    object* dirkwargs=new_tuple();
    builtins[65]=new_builtin((builtinfunc)builtin_dir, str_new_fromstr("dir"), dirargs, dirkwargs, 1, false);

    object* binargs=new_tuple();
    tuple_append_noinc(binargs, str_new_fromstr("object"));
    object* binkwargs=new_tuple();
    builtins[66]=new_builtin((builtinfunc)builtin_bin, str_new_fromstr("bin"), binargs, binkwargs, 1, false);

    object* hexargs=new_tuple();
    tuple_append_noinc(hexargs, str_new_fromstr("object"));
    object* hexkwargs=new_tuple();
    builtins[67]=new_builtin((builtinfunc)builtin_hex, str_new_fromstr("hex"), hexargs, hexkwargs, 1, false);

    object* chrargs=new_tuple();
    tuple_append_noinc(chrargs, str_new_fromstr("object"));
    object* chrkwargs=new_tuple();
    builtins[68]=new_builtin((builtinfunc)builtin_chr, str_new_fromstr("chr"), chrargs, chrkwargs, 1, false);

    object* ordargs=new_tuple();
    tuple_append_noinc(ordargs, str_new_fromstr("object"));
    object* ordkwargs=new_tuple();
    builtins[69]=new_builtin((builtinfunc)builtin_ord, str_new_fromstr("ord"), ordargs, ordkwargs, 1, false);

    object* unicode_nameargs=new_tuple();
    tuple_append_noinc(unicode_nameargs, str_new_fromstr("object"));
    object* unicode_namewargs=new_tuple();
    builtins[70]=new_builtin((builtinfunc)builtin_unicode_name, str_new_fromstr("unicode_name"), unicode_nameargs, unicode_namewargs, 1, false);

    builtins[71]=(object*)&OverflowError;
    builtins[72]=(object*)&NotImplementedError;
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
    uint32_t argc=CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    uint32_t posargc=CAST_LIST(args)->size;
    uint32_t kwargc=argc-posargc;

    if (CAST_BUILTIN(self)->argc!=argc){
        if (!CAST_BUILTIN(self)->nargs){
            if (CAST_BUILTIN(self)->argc-CAST_LIST(CAST_BUILTIN(self)->kwargs)->size>posargc \
            || CAST_LIST(CAST_BUILTIN(self)->kwargs)->size<kwargc \
            || CAST_BUILTIN(self)->argc<argc){
                if (CAST_DICT(kwargs)->val->size()==0){
                    vm_add_err(&ValueError, vm, "expected %d argument(s), got %d.",CAST_BUILTIN(self)->argc, argc);
                    return NULL;
                }
            }
        }
    }

    object* builtinargs=CAST_BUILTIN(self)->argc == 0 ? NULL : new_dict();
    if (CAST_BUILTIN(self)->argc != 0){
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
    }
    object* o=CAST_BUILTIN(self)->function(self, builtinargs);
    if (CAST_BUILTIN(self)->argc != 0){
        FPLDECREF(builtinargs);
    }
    return o;
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