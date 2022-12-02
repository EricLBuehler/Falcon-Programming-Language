#define BUILTIN_PRINT builtins[0]
#define BUILTIN_BUILD_CLASS builtins[1]

object* builtin_print(object* self, object* args){
    object* tupargs=dict_get_opti_deref(args, str_new_fromstr("args"));
    object* sep_=dict_get_opti_deref(args, str_new_fromstr("sep"));
    object* end=dict_get_opti_deref(args, str_new_fromstr("end"));
    string sep=object_cstr(sep_);
    for (int n=0; n<CAST_TUPLE(tupargs)->size; n++){
        cout<<object_cstr(tuple_index_int(tupargs, n));
        if (n+1!=CAST_TUPLE(tupargs)->size){
            cout<<sep;
        }
    }
    printf("%s",object_cstr(end).c_str());
    FPLDECREF(sep_);
    FPLDECREF(end);
    FPLDECREF(tupargs);
    return new_none();
}

object* builtin_repr(object* self, object* args){
    object* o=dict_get_opti_deref(args, str_new_fromstr("object"));
    object* s=object_repr(o);
    FPLDECREF(o);
    return s;
}

object* builtin___build_class__(object* self, object* args){
    object* function=dict_get_opti_deref(args, str_new_fromstr("func"));
    object* name=dict_get_opti_deref(args, str_new_fromstr("name"));
    object* bases=dict_get_opti_deref(args, str_new_fromstr("bases"));
    object* doc=dict_get_opti_deref(args, str_new_fromstr("doc"));
    object* dict;    

    if (!object_istype(function->type, &FuncType)){
        vm_add_err(&TypeError, vm, "expected function");
        FPLDECREF(function);
        FPLDECREF(name);
        FPLDECREF(bases);
        return NULL;
    }
    if (!object_istype(name->type, &StrType)){
        vm_add_err(&TypeError, vm, "expected str");
        FPLDECREF(function);
        FPLDECREF(name);
        FPLDECREF(bases);
        return NULL;
    }
    if (!object_istype(bases->type, &ListType) && !object_istype(bases->type, &TupleType)){
        vm_add_err(&TypeError, vm, "expected list or tuple");
        FPLDECREF(function);
        FPLDECREF(name);
        FPLDECREF(bases);
        return NULL;
    }
    
    uint32_t ip=0;
    FPLINCREF(CAST_FUNC(function)->code);
    add_callframe(vm->callstack, new_int_fromint(0),  CAST_STRING(CAST_FUNC(function)->name)->val, CAST_FUNC(function)->code, function, &ip);
    callstack_head(vm->callstack).locals=new_dict();
    
    object* kwargs=new_dict();
    object* fargs=new_list();   
    object* ret=func_call_nostack(function, fargs, kwargs);
    FPLDECREF(kwargs);
    FPLDECREF(fargs);

    dict=callstack_head(vm->callstack).locals;
    FPLINCREF(callstack_head(vm->callstack).locals);
    pop_callframe(vm->callstack);
    ERROR_RET(ret);
    
    object* t=new_type(CAST_STRING(name)->val, bases, dict, doc);
    FPLDECREF(function);
    FPLDECREF(doc);
    FPLDECREF(name);
    return t;
}

object* builtin_id(object* self, object* args){
    object* obj=dict_get_opti_deref(args, str_new_fromstr("object"));
    char buf[32];
    sprintf(buf, "%ld", (size_t)obj);
    FPLDECREF(obj);
    return new_int_fromstr(new string(buf));
}

object* builtin_input(object* self, object* args){
    object* obj=dict_get_opti_deref(args, str_new_fromstr("object"));
    cout<<object_cstr(obj);
    string s="";
    getline(cin,s);
    if(!cin){
        cin.clear();
        while (!hit_sigint){}
        hit_sigint=false;
        return NULL;
    }
    FPLDECREF(obj);
    return str_new_fromstr(s);
}

object* builtin_iter(object* self, object* args){
    object* obj=dict_get_opti_deref(args, str_new_fromstr("object"));
    if (obj->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not an iterable", obj->type->name->c_str());
        return NULL;
    }
    object* it=obj->type->slot_iter(obj);
    FPLDECREF(obj);
    return it;
}

object* builtin_next(object* self, object* args){
    object* obj=dict_get_opti_deref(args, str_new_fromstr("object"));
    if (obj->type->slot_next==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not an iterable", obj->type->name->c_str());
        return NULL;
    }
    object* n=obj->type->slot_next(obj);
    FPLDECREF(obj);
    return n;
}

object* builtin_round(object* self, object* args){
    object* obj=dict_get_opti_deref(args, str_new_fromstr("object"));
    object* digits=dict_get_opti_deref(args, str_new_fromstr("digits"));

    object* res=object_round(obj, digits);
    if (res==NULL){
        vm_add_err(&TypeError, vm, "'%s' object does not define __round__", obj->type->name->c_str());
        return NULL;
    }
    FPLDECREF(obj);
    FPLDECREF(digits);    
    return res;
}

object* builtin_globals(object* self, object* args){
    FPLINCREF(vm->globals);
    return vm->globals;
}
    
object* builtin_locals(object* self, object* args){
    FPLINCREF(callstack_head(vm->callstack).locals);
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
    return str_new_fromstr(FPL_LICENSE);
}
    
object* builtin_len(object* self, object* args){
    object* arg=dict_get_opti_deref(args, str_new_fromstr("object"));

    if (arg->type->slot_mappings->slot_len==NULL){
        vm_add_err(&TypeError, vm, "'%s' object has no __len__", arg->type->name->c_str());
        return NULL;
    }
    object* len=arg->type->slot_mappings->slot_len(arg);
    return len;
}

object* builtin_issubclass(object* self, object* args){
    object* ob=dict_get_opti_deref(args, str_new_fromstr("object"));
    object* type=dict_get_opti_deref(args, str_new_fromstr("type"));

    if (!object_istype(type->type, &TypeType)){
        vm_add_err(&TypeError, vm, "Expected type object, got '%s' object", type->type->name->c_str());
        return NULL;
    }

    bool is=object_issubclass(ob, CAST_TYPE(type));
    FPLDECREF(ob);
    FPLDECREF(type);
    if (is){
        return new_bool_true();
    }
    return new_bool_false();
}

object* builtin_eval(object* self, object* args);

object* builtin_getattr(object* self, object* args){
    object* ob=dict_get_opti_deref(args, str_new_fromstr("object"));
    object* attr=dict_get_opti_deref(args, str_new_fromstr("attr"));
    
    if (!object_istype(attr->type, &StrType)){
        vm_add_err(&TypeError, vm, "Expected str object, got '%s' object", attr->type->name->c_str());
        return NULL;
    }
    object* v=object_getattr(ob, attr);
    FPLDECREF(ob);
    FPLDECREF(attr);
    return v;
}

object* builtin_setattr(object* self, object* args){
    object* ob=dict_get_opti_deref(args, str_new_fromstr("object"));
    object* attr=dict_get_opti_deref(args, str_new_fromstr("attr"));
    object* val=dict_get_opti_deref(args, str_new_fromstr("val"));

    if (!object_istype(attr->type, &StrType)){
        vm_add_err(&TypeError, vm, "Expected str object, got '%s' object", attr->type->name->c_str());
        return NULL;
    }
    
    object* v=object_setattr(ob, attr, val);
    FPLDECREF(ob);
    FPLDECREF(attr);
    FPLDECREF(val);
    if (v!=SUCCESS){
        return NULL;
    }

    return new_none();
}

object* builtin_abs(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("self")); 
    
    object* retval=object_abs(val);
    FPLDECREF(val);
    if (retval==NULL){
        vm_add_err(&TypeError, vm, "'%s' object has no absolute value", val->type->name->c_str());
        return NULL; 
    }
    return retval;
}
    
object* builtin_iscallable(object* self, object* args){
    object* o=dict_get_opti_deref(args, str_new_fromstr("object"));
    object* v=object_iscallable(o);
    FPLDECREF(o);
    return v;
}
    
object* builtin_reverse(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("object"));
    if (val->type->slot_mappings->slot_get==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not subscriptable", val->type->name->c_str());
        FPLDECREF(val);
        return NULL;
    }
    if (val->type->slot_mappings->slot_len==NULL){
        vm_add_err(&TypeError, vm, "'%s' object has no __len__", val->type->name->c_str());
        FPLDECREF(val);
        return NULL;
    }
    object* list=new_list();
    object* len=val->type->slot_mappings->slot_len(val);
    for (size_t i=CAST_INT(len)->val->to_long(); i>0; i--){
        object* idx=new_int_fromint(i-1);
        object* v=val->type->slot_mappings->slot_get(val, idx);
        if (v==NULL){
            FPLDECREF(len);
            FPLDECREF(val);
            FPLDECREF(idx);
            return NULL;
        }
        FPLDECREF(idx);
        tuple_append_noinc(list,v);
        FPLDECREF(v);
    }
    FPLDECREF(len);
    FPLDECREF(val);
    return list;
}
    
object* builtin_isiter(object* self, object* args){
    object* o=dict_get_opti_deref(args, str_new_fromstr("object"));

    if (o->type->slot_iter==NULL){
        FPLDECREF(o);
        return new_bool_false();
    }
    FPLDECREF(o);
    return new_bool_true();
}
    
object* builtin_min(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("object"));

    if (val->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", val->type->name->c_str());
        FPLDECREF(val);
        return NULL;
    }

    if (val->type->slot_mappings->slot_len==NULL){
        vm_add_err(&TypeError, vm, "'%s' object has no __len__", val->type->name->c_str());
        FPLDECREF(val);
        return NULL;
    }
    
    object* len=val->type->slot_mappings->slot_len(val);
    ERROR_RET(len);
    if (!object_istype(len->type, &IntType)){
        vm_add_err(&TypeError, vm, "Expected int, got '%s' object", len->type->name->c_str());
        FPLDECREF(val);
        FPLDECREF(len);
        return NULL;
    }
    if (*CAST_INT(len)->val==0){
        vm_add_err(&ValueError, vm, "Got empty sequence");
        FPLDECREF(val);
        FPLDECREF(len);
        return NULL;
    }

    object* iter=val->type->slot_iter(val);
    
    if (iter!=NULL && iter->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
        FPLDECREF(val);
        FPLDECREF(len);
        FPLDECREF(iter);
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
            FPLDECREF(val);
            FPLDECREF(len);
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
                FPLDECREF(min);
                min=v;
            }
        }
        o=iter->type->slot_next(iter);
    }
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }
    FPLDECREF(val);
    FPLDECREF(len);
    FPLDECREF(iter);
    FPLDECREF(one);
    return min;
}
    
object* builtin_max(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("object"));

    if (val->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", val->type->name->c_str());
        FPLDECREF(val);
        return NULL;
    }

    if (val->type->slot_mappings->slot_len==NULL){
        vm_add_err(&TypeError, vm, "'%s' object has no __len__", val->type->name->c_str());
        FPLDECREF(val);
        return NULL;
    }
    
    object* len=val->type->slot_mappings->slot_len(val);
    ERROR_RET(len);
    if (!object_istype(len->type, &IntType)){
        vm_add_err(&TypeError, vm, "Expected int, got '%s' object", len->type->name->c_str());
        FPLDECREF(val);
        FPLDECREF(len);
        return NULL;
    }
    if (*CAST_INT(len)->val==0){
        vm_add_err(&ValueError, vm, "Got empty sequence");
        FPLDECREF(val);
        FPLDECREF(len);
        return NULL;
    }

    object* iter=val->type->slot_iter(val);
    
    if (iter!=NULL && iter->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
        FPLDECREF(val);
        FPLDECREF(len);
        FPLDECREF(iter);
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
            FPLDECREF(val);
            FPLDECREF(len);
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
                FPLDECREF(max);
                max=v;
            }
        }
        o=iter->type->slot_next(iter);
    }
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }
    FPLDECREF(val);
    FPLDECREF(len);
    FPLDECREF(iter);
    FPLDECREF(one);
    return max;
}
    
object* builtin_getannotation(object* self, object* args){
    object* nm=dict_get_opti_deref(args, str_new_fromstr("name"));
    object* anno=callstack_head(vm->callstack).annotations;

    object* tp=dict_get(anno, nm);
    FPLDECREF(nm);
    FPLDECREF(anno);
    if (tp==NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
        vm_add_err(&NameError, vm, "No annotation for name %s found", object_cstr(object_repr(nm)).c_str());
        return NULL;
    }
    return tp;
}
    
object* builtin_sum(object* self, object* args){
    object* o=dict_get_opti_deref(args, str_new_fromstr("object"));
    
    if (o->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", o->type->name->c_str());
        FPLDECREF(o);
        return NULL;
    }

    object* iter=o->type->slot_iter(o);
    
    if (iter!=NULL && iter->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
        FPLDECREF(o);
        FPLDECREF(iter);
        return NULL;
    }

    object* ob=iter->type->slot_next(iter);
    object* val=new_float_fromdouble(0);
    
    while (vm->exception==NULL){
        object* v=object_add(val, ob);
        if (v==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for +: '%s', and '%s'.", val->type->name->c_str(), ob->type->name->c_str());
            FPLDECREF(o);
            FPLDECREF(val);
            FPLDECREF(ob);
            FPLDECREF(iter);
            return NULL;
        }
        
        FPLDECREF(val);
        val=v;
        ob=iter->type->slot_next(iter);
    }
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }
    FPLDECREF(o);
    FPLDECREF(iter);
    return val;
}

object* builtin_hasattr(object* self, object* args){
    object* ob=dict_get_opti_deref(args, str_new_fromstr("object"));
    object* attr=dict_get_opti_deref(args, str_new_fromstr("attr"));
    
    if (!object_istype(attr->type, &StrType)){
        vm_add_err(&TypeError, vm, "Expected str object, got '%s' object", attr->type->name->c_str());
        return NULL;
    }
    attr=object_getattr(ob, attr);
    FPLDECREF(ob);
    FPLDECREF(attr);
    if (attr==NULL && vm->exception!=NULL && object_issubclass(vm->exception, &ExceptionType)){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
        return new_bool_false();
    }
    return new_bool_true();
}

object* builtin_dir(object* self, object* args){
    object* o=dict_get_opti_deref(args, str_new_fromstr("object"));

    object* dict;
    if (object_istype(o->type, &NoneType)){
        FPLINCREF(callstack_head(vm->callstack).locals);
        dict=callstack_head(vm->callstack).locals;
    }
    else{
        object* v=str_new_fromstr("__dict__");
        dict=object_getattr(o, v);
        ERROR_RET(dict);
        FPLDECREF(v);
    }
    
    object*  list=new_list();
    for (object* o: *CAST_DICT(dict)->keys){
        list_append(list, o);
    }
    
    FPLDECREF(o);
    FPLDECREF(dict);
    return list;
}

object* builtin_bin(object* self, object* args){
    object* o=dict_get_opti_deref(args, str_new_fromstr("object"));
    object* intv=object_int(o);
    if (intv==NULL || !object_istype(intv->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", o->type->name->c_str());
        FPLDECREF(o);
        return NULL; 
    }
    FPLDECREF(o);
    
    string s="0b";

    string s_="";

    vector<long long> num=convert_str_to_intarr(CAST_INT(intv)->val->to_string());
    long long mask;
    for (int i=0; i<num.size(); i++){
        string binary=bitset<sizeof(long long)>(num[i]).to_string();
        s_+=binary;
    }
    s_.erase(0, s_.find_first_not_of('0'));

    s+=s_;

    FPLDECREF(intv);
    return str_new_fromstr(s);
}

object* builtin_hex(object* self, object* args){
    object* o=dict_get_opti_deref(args, str_new_fromstr("object"));
    object* intv=object_int(o);
    if (intv==NULL || !object_istype(intv->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", o->type->name->c_str());
        FPLDECREF(o);
        return NULL; 
    }
    FPLDECREF(o);
    
    string s="0x";

    vector<long long> num=convert_str_to_intarr(CAST_INT(intv)->val->to_string());
    long long mask;
    for (int i=0; i<num.size(); i++){
        std::stringstream stream;
        stream << std::hex << num[i];
        s+=stream.str();
    }

    FPLDECREF(intv);
    return str_new_fromstr(s);
}

object* builtin_chr(object* self, object* args){
    object* o=dict_get_opti_deref(args, str_new_fromstr("object"));
    object* intv=object_int(o);
    if (intv==NULL || !object_istype(intv->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", o->type->name->c_str());
        FPLDECREF(o);
        return NULL; 
    }
    FPLDECREF(o);

    if (*CAST_INT(intv)->val>INT_MAX){
        vm_add_err(&ValueError, vm, "Code point value too large");
        return NULL; 
    }

    if (*CAST_INT(intv)->val<INT_MIN){
        vm_add_err(&ValueError, vm, "Code point value too small");
        return NULL; 
    }

    if (*CAST_INT(intv)->val>=_cp_names_rev.size()){
        vm_add_err(&ValueError, vm, "Code point value too large");
        return NULL; 
    }

    string s=codept_to_str(CAST_INT(intv)->val->to_int());
    FPLDECREF(intv);
    return str_new_fromstr(s);
}

object* builtin_ord(object* self, object* args){
    object* o=dict_get_opti_deref(args, str_new_fromstr("object"));
    uint32_t size;
    object* res=NULL;
    if (object_issubclass(o, &BytesType)){
        size=CAST_BYTES(o)->len;
        if (size==1){
            res = new_int_fromint(CAST_BYTES(o)->val[0]);
        }
    }
    else {
        object* s_=object_str(o);
        size=STRING_LENGTH(s_);
        if (size==1){
            string s=object_cstr(s_);
            FPLDECREF(s_);
            res = new_int_fromint(string_get_uval(s.c_str(), 0));
        }
    }
    FPLDECREF(o);
    if (size!=1){
        vm_add_err(&ValueError, vm, "Expected string of length 1, got string of length %d", size);
        return NULL; 
    }
    return res;
}

object* builtin_unicode_name(object* self, object* args){
    object* o=dict_get_opti_deref(args, str_new_fromstr("object"));
    object* intv=object_int(o);
    if (intv==NULL || !object_istype(intv->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", o->type->name->c_str());
        FPLDECREF(o);
        return NULL; 
    }
    FPLDECREF(o);    

    if (*CAST_INT(intv)->val>=_cp_names_rev.size()){
        vm_add_err(&ValueError, vm, "Code point value too large");
        return NULL; 
    }

    if (*CAST_INT(intv)->val<0){
        vm_add_err(&ValueError, vm, "Code point value too small");
        return NULL; 
    }
    
    int idx=CAST_INT(intv)->val->to_int();

    string s=get_name_from_cp(idx);

    FPLDECREF(intv);
    return str_new_fromstr(s);
}

object* builtin_compile(object* self, object* args);