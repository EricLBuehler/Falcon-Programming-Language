
object* property_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if ((len!=0 && len!=1 && len!=2 && len!=3) || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 0, 1, 2 or 3 arguments, got %d", len);
        return NULL;
    }

    object* property=new_object(CAST_TYPE(type));
    CAST_PROPERTY(property)->get=NULL;
    CAST_PROPERTY(property)->set=NULL;
    CAST_PROPERTY(property)->del=NULL;
    if (len>=1){
        CAST_PROPERTY(property)->get=FPLINCREF(list_index_int(args, 0));
        if (!istrue(object_iscallable(CAST_PROPERTY(property)->get))){
            vm_add_err(&TypeError, vm, "Expected callable, got '%s' object", CAST_PROPERTY(property)->get->type->name->c_str());
            return NULL;            
        }
    }
    if (len>=2){
        CAST_PROPERTY(property)->set=FPLINCREF(list_index_int(args, 1));
        if (!istrue(object_iscallable(CAST_PROPERTY(property)->set))){
            vm_add_err(&TypeError, vm, "Expected callable, got '%s' object", CAST_PROPERTY(property)->set->type->name->c_str());
            return NULL;            
        }
    }
    if (len>=3){
        CAST_PROPERTY(property)->del=FPLINCREF(list_index_int(args, 2));
        if (!istrue(object_iscallable(CAST_PROPERTY(property)->del))){
            vm_add_err(&TypeError, vm, "Expected callable, got '%s' object", CAST_PROPERTY(property)->del->type->name->c_str());
            return NULL;            
        }
    }
    return property;
}

bool non_null(object* a, object* b){
    return ((a!=NULL) && (b!=NULL))? true: false;
}

object* property_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if ( (non_null(CAST_PROPERTY(self)->get, CAST_PROPERTY(other)->get) && istrue(object_cmp(CAST_PROPERTY(self)->get,CAST_PROPERTY(other)->get, CMP_EQ)))\
        && (non_null(CAST_PROPERTY(self)->set, CAST_PROPERTY(other)->set) && istrue(object_cmp(CAST_PROPERTY(self)->set,CAST_PROPERTY(other)->set, CMP_EQ)))\
        && (non_null(CAST_PROPERTY(self)->del, CAST_PROPERTY(other)->del) && istrue(object_cmp(CAST_PROPERTY(self)->del,CAST_PROPERTY(other)->del, CMP_EQ))) ){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if ( !(non_null(CAST_PROPERTY(self)->get, CAST_PROPERTY(other)->get) && istrue(object_cmp(CAST_PROPERTY(self)->get,CAST_PROPERTY(other)->get, CMP_EQ)))\
        && !(non_null(CAST_PROPERTY(self)->set, CAST_PROPERTY(other)->set) && istrue(object_cmp(CAST_PROPERTY(self)->set,CAST_PROPERTY(other)->set, CMP_EQ)))\
        && !(non_null(CAST_PROPERTY(self)->del, CAST_PROPERTY(other)->del) && istrue(object_cmp(CAST_PROPERTY(self)->del,CAST_PROPERTY(other)->del, CMP_EQ))) ){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}
object* property_repr(object* self){
    char buf[32];
    sprintf(buf, "0x%x", self);

    string s="<property ";
    s+=buf;
    s+=">";
    return str_new_fromstr(s);
}
object* property_descrget(object* instance, object* self, object* owner){
    if (owner==NULL || object_istype(owner->type, &NoneType)){
        return FPLINCREF(self);
    }
    if (CAST_PROPERTY(self)->get==NULL){
        vm_add_err(&AttributeError, vm, "property of '%s' object has no getter", object_cstr(instance).c_str());
        return NULL;
    }

    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, instance);
        
    object* val=object_call_nokwargs(CAST_PROPERTY(self)->get, args);
    ERROR_RET(val);
    return val;
}
object* property_descrset(object* instance, object* self, object* value){
    if (value!=NULL){
        if (CAST_PROPERTY(self)->set==NULL){
            vm_add_err(&AttributeError, vm, "property of '%s' object has no setter", object_cstr(instance).c_str());
            return NULL;
        }

        object* args=new_tuple();
        args->type->slot_mappings->slot_append(args, instance);
        args->type->slot_mappings->slot_append(args, value);
            
        object* val=object_call_nokwargs(CAST_PROPERTY(self)->set, args);
        ERROR_RET(val);
        return val;
    }
    
    if (CAST_PROPERTY(self)->del==NULL){
        vm_add_err(&AttributeError, vm, "property of '%s' object has no deleter", object_cstr(instance).c_str());
        return NULL;
    }

    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, instance);
        
    object* val=object_call_nokwargs(CAST_PROPERTY(self)->del, args);
    ERROR_RET(val);
    return val;
}
object* property_getter(object* selftp, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }

    object* self=list_index_int(args, 0);
    
    CAST_PROPERTY(self)->get=FPLINCREF(list_index_int(args, 1));
    if (!istrue(object_iscallable(CAST_PROPERTY(self)->get))){
        vm_add_err(&TypeError, vm, "Expected callable, got '%s' object", CAST_PROPERTY(self)->get->type->name->c_str());
        return NULL;            
    }
    return self;
}
object* property_setter(object* selftp, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }

    object* self=list_index_int(args, 0);
    
    CAST_PROPERTY(self)->set=FPLINCREF(list_index_int(args, 1));
    if (!istrue(object_iscallable(CAST_PROPERTY(self)->set))){
        vm_add_err(&TypeError, vm, "Expected callable, got '%s' object", CAST_PROPERTY(self)->set->type->name->c_str());
        return NULL;            
    }
    return self;
}
object* property_deleter(object* selftp, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }

    object* self=list_index_int(args, 0);
    
    CAST_PROPERTY(self)->del=FPLINCREF(list_index_int(args, 1));
    if (!istrue(object_iscallable(CAST_PROPERTY(self)->del))){
        vm_add_err(&TypeError, vm, "Expected callable, got '%s' object", CAST_PROPERTY(self)->del->type->name->c_str());
        return NULL;            
    }
    return self;
}
void property_del(object* self){
    if (CAST_PROPERTY(self)->get!=NULL){
        FPLDECREF(CAST_PROPERTY(self)->get);
    }
    if (CAST_PROPERTY(self)->set!=NULL){
        FPLDECREF(CAST_PROPERTY(self)->set);
    }
    if (CAST_PROPERTY(self)->del!=NULL){
        FPLDECREF(CAST_PROPERTY(self)->del);
    }
}