object* type_wrapper_add(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_number->slot_add(self, list_index_int(args, 1));
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for +: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_number->slot_add(self, list_index_int(args, 1));
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for +: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_sub(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_number->slot_sub(self, list_index_int(args, 1));
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for -: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_number->slot_sub(self, list_index_int(args, 1));
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for -: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_mul(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_number->slot_mul(self, list_index_int(args, 1));
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for *: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_number->slot_mul(self, list_index_int(args, 1));
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for *: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_div(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_number->slot_div(self, list_index_int(args, 1));
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for /: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_number->slot_div(self, list_index_int(args, 1));
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for /: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_bool(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    
    
    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_number->slot_bool(self);
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to bool", self->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_number->slot_bool(self);
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to bool", self->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_neg(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);

    
    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_number->slot_bool(self);
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid unary operand -: '%s'.", self->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_number->slot_bool(self);
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid unary operand -: '%s'.", self->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_int(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    
    
    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_number->slot_int(self);
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", self->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_number->slot_int(self);
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", self->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_float(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    
    
    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_number->slot_float(self);
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", self->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_number->slot_float(self);
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", self->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_pow(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_number->slot_pow(self, list_index_int(args, 1));
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for **: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_number->slot_pow(self, list_index_int(args, 1));
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for **: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_mod(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_number->slot_mod(self, list_index_int(args, 1));
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for %: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_number->slot_mod(self, list_index_int(args, 1));
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for %: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}



object* type_wrapper_eq(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);    


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_cmp(self, list_index_int(args, 1), CMP_EQ);
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for ==: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_cmp(self, list_index_int(args, 1), CMP_EQ);
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for ==: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_ne(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);  


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_cmp(self, list_index_int(args, 1), CMP_NE);
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for !=: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_cmp(self, list_index_int(args, 1), CMP_NE);
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for !=: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_gt(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);  


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_cmp(self, list_index_int(args, 1), CMP_GT);
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for >: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_cmp(self, list_index_int(args, 1), CMP_GT);
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for >: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_lt(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);  


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_cmp(self, list_index_int(args, 1), CMP_LT);
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for <: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_cmp(self, list_index_int(args, 1), CMP_LT);
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for <: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_gte(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);  


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_cmp(self, list_index_int(args, 1), CMP_GTE);
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for >=: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_cmp(self, list_index_int(args, 1), CMP_GTE);
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for >=: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_lte(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=2 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);  


    if (object_istype(self->type, &TypeType)){
        object* ret=CAST_TYPE(self)->slot_cmp(self, list_index_int(args, 1), CMP_LTE);
        if (ret==NULL){
            vm_add_err(&TypeError, vm, "Invalid operand types for <=: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        return ret;
    }

    object* ret=self->type->slot_cmp(self, list_index_int(args, 1), CMP_LTE);
    if (ret==NULL){
        vm_add_err(&TypeError, vm, "Invalid operand types for <=: '%s', and '%s'.", self->type->name->c_str(), list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    return ret;
}

object* type_wrapper_call(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val==0 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected at least 1 argument, got 0");
        return NULL;
    }
    object* self=list_index_int(args, 0);

    if (object_istype(self->type, &TypeType)){
        return object_call(self, args, kwargs);
    }    
    return object_call(self, args, kwargs);
}

object* type_wrapper_init(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val==0 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected at least 1 argument, got 0");
        return NULL;
    }
    object* self=list_index_int(args, 0);

    if (object_istype(self->type, &TypeType)){
        return CAST_TYPE(self)->slot_init(self, args, kwargs);
    }    
    
    return self->type->slot_init(self, args, kwargs);
}

object* type_wrapper_iter(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val==1 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected at least 1 argument, got  %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    
    if (object_istype(self->type, &TypeType)){
        return CAST_TYPE(self)->slot_iter(self);
    }
    
    return self->type->slot_iter(self);
}

object* type_wrapper_new(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val==0 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected at least 1 argument, got 0");
        return NULL;
    }
    object* self=list_index_int(args, 0);
    
    if (object_istype(self->type, &TypeType)){
        return CAST_TYPE(self)->slot_new(self, args, kwargs);
    }
    
    return self->type->slot_new(self, args, kwargs);
}

object* type_wrapper_next(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val==0 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected at least 1 argument, got  %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    
    if (object_istype(self->type, &TypeType)){
        return CAST_TYPE(self)->slot_next(self);
    }
    
    return self->type->slot_next(self);
}

object* type_wrapper_repr(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    
    if (object_istype(self->type, &TypeType)){
        return CAST_TYPE(self)->slot_repr(self);
    }
    return self->type->slot_repr(self);
}

object* type_wrapper_str(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    
    if (object_istype(self->type, &TypeType)){
        return CAST_TYPE(self)->slot_str(self);
    }
    return self->type->slot_str(self);
}

object* type_wrapper_del(object* args, object* kwargs){
    if (*CAST_INT(args->type->slot_mappings->slot_len(args))->val!=1 && *CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d",CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int());
        return NULL;
    }
    object* self=list_index_int(args, 0);
    
    if (object_istype(self->type, &TypeType)){
        CAST_TYPE(self)->slot_iter(self);
        return new_none();
    }
    self->type->slot_del(self);
    return new_none();
}