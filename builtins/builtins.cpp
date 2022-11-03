#define BUILTIN_PRINT builtins[0]
#define BUILTIN_BUILD_CLASS builtins[1]

object* builtin_print(object* self, object* args){
    object* tupargs=args->type->slot_mappings->slot_get(args, str_new_fromstr("args"));
    string sep=object_cstr(dict_get(args, str_new_fromstr("sep")));
    for (int n=0; n<CAST_TUPLE(tupargs)->size; n++){
        cout<<object_cstr(tuple_index_int(tupargs, n));
        if (n+1!=CAST_TUPLE(tupargs)->size){
            cout<<sep;
        }
    }
    printf("%s",object_cstr(dict_get(args, str_new_fromstr("end"))).c_str());
    return new_none();
}

object* builtin_repr(object* self, object* args){
    return object_repr(args->type->slot_mappings->slot_get(args, str_new_fromstr("object")));
}

object* builtin___build_class__(object* self, object* args){
    object* function=args->type->slot_mappings->slot_get(args, str_new_fromstr("func"));
    object* name=args->type->slot_mappings->slot_get(args, str_new_fromstr("name"));
    object* dict;
    

    if (!object_istype(function->type, &FuncType)){
        vm_add_err(&TypeError, vm, "expected function");
        return NULL;
    }
    if (!object_istype(name->type, &StrType)){
        vm_add_err(&TypeError, vm, "expected str");
        return NULL;
    }
    
    uint32_t ip=0;
    add_callframe(vm->callstack, new_int_fromint(0),  CAST_STRING(CAST_FUNC(function)->name)->val, FPLINCREF(CAST_FUNC(function)->code), function, &ip);
    callstack_head(vm->callstack).locals=new_dict();
    
    object* kwargs=new_dict();
    object* fargs=new_list();   
    object* ret=func_call_nostack(function, fargs, kwargs);
    FPLDECREF(kwargs);
    FPLDECREF(fargs);

    dict=FPLINCREF(callstack_head(vm->callstack).locals);
    pop_callframe(vm->callstack);
    ERROR_RET(ret);
    
    object* t=new_type(CAST_STRING(name)->val, args->type->slot_mappings->slot_get(args, str_new_fromstr("bases")), dict);
    return t;
}

object* builtin_id(object* self, object* args){
    object* obj=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    char buf[32];
    sprintf(buf, "%ld", (size_t)obj);
    return new_int_fromstr(new string(buf));
}

object* builtin_input(object* self, object* args){
    object* obj=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    cout<<object_cstr(obj);
    string s="";
    getline(cin,s);
    if(!cin){
        cin.clear();
        while (!hit_sigint){}
        hit_sigint=false;
        return NULL;
    }
    return str_new_fromstr(s);
}

object* builtin_iter(object* self, object* args){
    object* obj=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    if (obj->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not an iterable", obj->type->name->c_str());
        return NULL;
    }
    return obj->type->slot_iter(obj);
}

object* builtin_next(object* self, object* args){
    object* obj=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    if (obj->type->slot_next==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not an iterable", obj->type->name->c_str());
        return NULL;
    }
    return obj->type->slot_next(obj);
}

object* builtin_round(object* self, object* args){
    object* obj=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    object* digits=args->type->slot_mappings->slot_get(args, str_new_fromstr("digits"));
    
    object* floatval=object_float(obj);
    if (floatval==NULL || !object_istype(floatval->type, &FloatType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", obj->type->name->c_str());
        return NULL; 
    }
    object* ndigits=object_int(digits);
    if (ndigits==NULL || !object_istype(ndigits->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", digits->type->name->c_str());
        return NULL; 
    }

    char buf[to_string(CAST_FLOAT(floatval)->val).size()];
    sprintf(buf, "%g", CAST_FLOAT(floatval)->val);

    string orig_val(buf);
    size_t pos=orig_val.find('.');
    
    string substr;
    if (pos==-1){
        return new_int_fromint(stod(orig_val));
    }
    if (CAST_INT(ndigits)->val->to_int()==0){
        substr=orig_val.substr(0, pos+2);
        FPLDECREF(ndigits);
        FPLDECREF(floatval);
        return new_int_fromint(round(stod(substr)));
    }
    substr=orig_val.substr(0, pos+2+CAST_INT(ndigits)->val->to_int());
    FPLDECREF(ndigits);
    FPLDECREF(floatval);
    
    return new_float_fromdouble(round_double(stod(substr), CAST_INT(ndigits)->val->to_int()));
}

object* builtin_globals(object* self, object* args){
    return vm->globals;
}
    
object* builtin_locals(object* self, object* args){
    return callstack_head(vm->callstack).locals;
}
    
object* builtin_alllocals(object* self, object* args){
    object* locals=new_list();
    for (int i=0; i<vm->callstack->size-1; i++){
        list_append(locals, vm->callstack->data[vm->callstack->size].locals);
    }
    list_append(locals, vm->globals);
    return locals;
}
    
object* builtin_exit(object* self, object* args){
    return TERM_PROGRAM;
}
    
object* builtin_copyright(object* self, object* args){
    cout<<FPL_LICENSE<<endl;
    return str_new_fromstr(FPL_LICENSE);
}
    
object* builtin_len(object* self, object* args){
    object* arg=dict_get(args, str_new_fromstr("object"));

    if (arg->type->slot_mappings->slot_len==NULL){
        vm_add_err(&TypeError, vm, "'%s' object has no __len__", arg->type->name->c_str());
        return NULL;
    }

    return arg->type->slot_mappings->slot_len(arg);
}

object* builtin_issubclass(object* self, object* args){
    object* ob=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    object* type=args->type->slot_mappings->slot_get(args, str_new_fromstr("type"));

    if (!object_istype(type->type, &TypeType)){
        vm_add_err(&TypeError, vm, "Expected type object, got '%s' object", type->type->name->c_str());
        return NULL;
    }

    bool is=object_issubclass(ob, CAST_TYPE(type));
    if (is){
        return new_bool_true();
    }
    return new_bool_false();
}

object* builtin_eval(object* self, object* args);

object* builtin_getattr(object* self, object* args){
    object* ob=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    object* attr=args->type->slot_mappings->slot_get(args, str_new_fromstr("attr"));
    
    if (!object_istype(attr->type, &StrType)){
        vm_add_err(&TypeError, vm, "Expected str object, got '%s' object", attr->type->name->c_str());
        return NULL;
    }

    return object_getattr(ob, attr);
}

object* builtin_setattr(object* self, object* args){
    object* ob=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    object* attr=args->type->slot_mappings->slot_get(args, str_new_fromstr("attr"));
    object* val=args->type->slot_mappings->slot_get(args, str_new_fromstr("val"));

    if (!object_istype(attr->type, &StrType)){
        vm_add_err(&TypeError, vm, "Expected str object, got '%s' object", attr->type->name->c_str());
        return NULL;
    }
    
    object* v=object_setattr(ob, attr, val);
    if (v!=SUCCESS){
        return NULL;
    }

    return new_none();
}

object* builtin_abs(object* self, object* args){
    object* val=args->type->slot_mappings->slot_get(args, str_new_fromstr("self")); 
    
    object* retval=object_abs(val);
    if (retval==NULL){
        vm_add_err(&TypeError, vm, "'%s' object has no absolute value", val->type->name->c_str());
        return NULL; 
    }
    return retval;
}
    
object* builtin_iscallable(object* self, object* args){
    return object_iscallable(args->type->slot_mappings->slot_get(args, str_new_fromstr("object")));
}
    
object* builtin_reverse(object* self, object* args){
    object* val=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    if (val->type->slot_mappings->slot_get==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not subscriptable", val->type->name->c_str());
        return NULL;
    }
    if (val->type->slot_mappings->slot_len==NULL){
        vm_add_err(&TypeError, vm, "'%s' object has no __len__", val->type->name->c_str());
        return NULL;
    }
    object* list=new_list();
    object* len=val->type->slot_mappings->slot_len(val);
    for (size_t i=CAST_INT(len)->val->to_long(); i>0; i--){
        object* idx=new_int_fromint(i-1);
        object* v=val->type->slot_mappings->slot_get(val, idx);
        if (v==NULL){
            return NULL;
        }
        FPLDECREF(idx);
        list_append(list,v);
    }
    FPLDECREF(len);

    return list;
}
    
object* builtin_isiter(object* self, object* args){
    object* o=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));

    if (o->type->slot_iter==NULL){
        return new_bool_false();
    }
    return new_bool_true();
}
    
object* builtin_min(object* self, object* args){
    object* val=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));

    if (val->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", val->type->name->c_str());
        return NULL;
    }

    if (val->type->slot_mappings->slot_len==NULL){
        vm_add_err(&TypeError, vm, "'%s' object has no __len__", val->type->name->c_str());
        return NULL;
    }
    
    object* len=val->type->slot_mappings->slot_len(val);
    ERROR_RET(len);
    if (!object_istype(len->type, &IntType)){
        vm_add_err(&TypeError, vm, "Expected int, got '%s' object", len->type->name->c_str());
        return NULL;
    }
    if (*CAST_INT(len)->val==0){
        vm_add_err(&ValueError, vm, "Got empty sequence");
        return NULL;
    }

    object* iter=val->type->slot_iter(val);
    
    if (iter!=NULL && iter->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
        return NULL;
    }

    object* one=new_int_fromint(0);
    object* res=NULL;
    
    object* o=iter->type->slot_next(iter);
    object* v;
    object* min=NULL;
    while (vm->exception==NULL){
        if (o->type->slot_mappings->slot_len==NULL){
            v=o;
            goto cont;
        }
        if (o->type->slot_mappings==NULL || o->type->slot_mappings->slot_get==NULL){
            FPLDECREF(iter);
            FPLDECREF(one);
            vm_add_err(&TypeError, vm, "'%s' object is not subscriptable", o->type->name->c_str());
            return NULL;
        }
        v=o->type->slot_mappings->slot_get(o, one);
        ERROR_RET(v);
        
        cont:
        if (min==NULL){
            min=v;
        }
        else{
            if (istrue(object_gt(v, min))){
                min=v;
            }
        }
        o=iter->type->slot_next(iter);
    }
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }
    return min;
}
    
object* builtin_max(object* self, object* args){
    object* val=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));

    if (val->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", val->type->name->c_str());
        return NULL;
    }

    if (val->type->slot_mappings->slot_len==NULL){
        vm_add_err(&TypeError, vm, "'%s' object has no __len__", val->type->name->c_str());
        return NULL;
    }
    
    object* len=val->type->slot_mappings->slot_len(val);
    ERROR_RET(len);
    if (!object_istype(len->type, &IntType)){
        vm_add_err(&TypeError, vm, "Expected int, got '%s' object", len->type->name->c_str());
        return NULL;
    }
    if (*CAST_INT(len)->val==0){
        vm_add_err(&ValueError, vm, "Got empty sequence");
        return NULL;
    }

    object* iter=val->type->slot_iter(val);
    
    if (iter!=NULL && iter->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
        return NULL;
    }

    object* one=new_int_fromint(0);
    object* res=NULL;
    
    object* o=iter->type->slot_next(iter);
    object* v;
    object* max=NULL;
    while (vm->exception==NULL){
        if (o->type->slot_mappings->slot_len==NULL){
            v=o;
            goto cont;
        }
        if (o->type->slot_mappings==NULL || o->type->slot_mappings->slot_get==NULL){
            FPLDECREF(iter);
            FPLDECREF(one);
            vm_add_err(&TypeError, vm, "'%s' object is not subscriptable", o->type->name->c_str());
            return NULL;
        }
        v=o->type->slot_mappings->slot_get(o, one);
        ERROR_RET(v);
        
        cont:
        if (max==NULL){
            max=v;
        }
        else{
            if (istrue(object_lt(v, max))){
                max=v;
            }
        }
        o=iter->type->slot_next(iter);
    }
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }
    return max;
}
    
object* builtin_getannotation(object* self, object* args){
    object* nm=args->type->slot_mappings->slot_get(args, str_new_fromstr("name"));
    object* anno=callstack_head(vm->callstack).annotations;

    object* tp=dict_get(anno, nm);
    if (tp==NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
        vm_add_err(&NameError, vm, "No annotation for name %s found", object_cstr(object_repr(nm)).c_str());
        return NULL;
    }
    return tp;
}
    
object* builtin_sum(object* self, object* args){
    object* o=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    
    if (o->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", o->type->name->c_str());
        return NULL;
    }

    object* iter=o->type->slot_iter(o);
    
    if (iter!=NULL && iter->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
        return NULL;
    }

    object* ob=iter->type->slot_next(iter);
    object* val=new_float_fromdouble(0);
    
    while (vm->exception==NULL){
        object* v=object_add(val, ob);
        if (v==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for +: '%s', and '%s'.", val->type->name->c_str(), ob->type->name->c_str());
            return NULL;
        }
        val=v;
        ob=iter->type->slot_next(iter);
    }
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }
    return val;
}

object* builtin_hasattr(object* self, object* args){
    object* ob=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    object* attr=args->type->slot_mappings->slot_get(args, str_new_fromstr("attr"));
    
    if (!object_istype(attr->type, &StrType)){
        vm_add_err(&TypeError, vm, "Expected str object, got '%s' object", attr->type->name->c_str());
        return NULL;
    }
    attr=object_getattr(ob, attr);
    if (attr==NULL && vm->exception!=NULL && object_issubclass(vm->exception, &ExceptionType)){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
        return new_bool_false();
    }
    FPLDECREF(attr);
    return new_bool_true();
}