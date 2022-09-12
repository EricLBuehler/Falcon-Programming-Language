#define BUILTIN_PRINT builtins[0]
#define BUILTIN_BUILD_CLASS builtins[1]

object* builtin_print(object* self, object* args){
    object* tupargs=args->type->slot_mappings->slot_get(args, str_new_fromstr("args"));
    for (int n=0; n<CAST_TUPLE(tupargs)->size; n++){
        printf("%s",object_cstr(tuple_index_int(tupargs, n)).c_str());
        if (n+1!=CAST_TUPLE(tupargs)->size){
            cout<<" ";
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
    
    add_callframe(vm->callstack, new_int_fromint(0),  CAST_STRING(CAST_FUNC(function)->name)->val, INCREF(CAST_FUNC(function)->code));
    vm->callstack->head->locals=new_dict();
    
    object* kwargs=new_dict();
    object* fargs=new_list();
    object* ret=func_call_nostack(function, fargs, kwargs);
    DECREF(kwargs);
    DECREF(fargs);

    dict=INCREF(vm->callstack->head->locals);
    pop_callframe(vm->callstack);
    if (ret==NULL){
        return NULL;
    }
    
    object* t=new_type(CAST_STRING(name)->val, args->type->slot_mappings->slot_get(args, str_new_fromstr("bases")), dict);
    return t;
}

object* builtin_id(object* self, object* args){
    object* obj=args->type->slot_mappings->slot_get(args, str_new_fromstr("object"));
    char buf[32];
    sprintf(buf, "%d", obj);
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
    if (floatval==NULL){
        return NULL;
    }
    object* ndigits=object_int(digits);
    if (ndigits==NULL){
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
        return new_int_fromint(round(stod(substr)));
    }
    substr=orig_val.substr(0, pos+2+CAST_INT(ndigits)->val->to_int());
    
    return new_float_fromdouble(round_double(stod(substr), CAST_INT(ndigits)->val->to_int()));
}

object* builtin_globals(object* self, object* args){
    return vm->globals;
}
    
object* builtin_locals(object* self, object* args){
    return vm->callstack->head->locals;
}
    
object* builtin_alllocals(object* self, object* args){
    object* locals=new_list();
    struct callframe* c=vm->callstack->head;
    while(c){
        list_append(locals, c->locals);
        c=c->next;
    }
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