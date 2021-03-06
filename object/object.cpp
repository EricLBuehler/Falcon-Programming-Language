

bool DECREF(struct object* object){
    object->refcnt--;
    if (object->refcnt==0){
        if (object->type->slot_del!=NULL){
            object->type->slot_del(object);
        }
        //GC collect it later.... or...
        if (!object->type->gc_trackable){
            if (object->ob_next!=NULL){
                object->ob_next->ob_prev=object->ob_prev;
            }
            if (object->ob_prev!=NULL){
                object->ob_prev->ob_next=object->ob_next;
            }
            else{
                immutable_objs=object->ob_next;
            }
            free(object);
            immutable_size--;
        }
        return true;
    }
    return false;
} 



object* INCREF(struct object* object){
    object->refcnt++;
    return object;
}

object* in_immutables(object* obj){
    object* o=immutable_objs->ob_next;
    if (obj->refcnt==0){
        return NULL;
    }
    while (o){
        if (o->refcnt==0){
            o=o->ob_next;
            continue;
        }
        if ((*o->type->name)==(*obj->type->name)){
            if (o->type->name==StrType.name){
                if ((*CAST_STRING(o)->val)==(*CAST_STRING(obj)->val)){
                    INCREF(o);
                    break;
                }
            }
            if (o->type->name==IntType.name){
                if ((*CAST_INT(o)->val)==(*CAST_INT(obj)->val)){
                    INCREF(o);
                    break;
                }
            }
        }
        o=o->ob_next;
    }
    return o;
}

object* new_object(TypeObject* type){
    object* object = (struct object*) malloc(type->size);
    object->refcnt=1;
    object->type=type;
    object->gen=0;

    if (object->type->gc_trackable){
        object->ob_next=gc.gen0;
        if (gc.gen0!=NULL){
            gc.gen0->ob_prev=object;
        }
        object->ob_prev=NULL;
        gc.gen0=object;

        gc.gen0_n++;
    }
    else{
        if (immutable_objs!=NULL){
            immutable_objs->ob_prev=object;
        }
        object->ob_prev=NULL;
        object->ob_next=immutable_objs;
        immutable_objs=object;
        immutable_size++;
    }

    if (gc.gen0_n==gc.gen0_thresh){
        gc_collect(0);
    }
    
    if (gc.gen1_n>=gc.gen1_thresh){
        gc_collect(1);
    }
    
    return object;
}

object_var* new_object_var(TypeObject* type, size_t size){
    object_var* object = (object_var*) malloc(sizeof(object_var)+size+1);
    object->refcnt=1;
    object->gc_ref=0;
    object->type=type;
    object->var_size=sizeof(struct object_var)+size;
    object->gen=0;

    if (object->type->gc_trackable){
        object->ob_next=gc.gen0;
        if (gc.gen0!=NULL){
            gc.gen0->ob_prev=(struct object*)object;
        }
        object->ob_prev=NULL;
        gc.gen0=(struct object*)object;

        gc.gen0_n++;
    }
    else{
        if (immutable_objs!=NULL){
            immutable_objs->ob_prev=(struct object*)object;
        }
        object->ob_prev=NULL;
        object->ob_next=immutable_objs;
        immutable_objs=(struct object*)object;
        immutable_size++;
    }

    if (gc.gen0_n==gc.gen0_thresh){
        gc_collect(0);
    }

    if (gc.gen1_n>=gc.gen1_thresh){
        gc_collect(1);
    }

    return object;
}



object* object_add(object* left, object* right){
    if (left->type->slot_number==NULL || right->type->slot_number==NULL || left->type->slot_number->slot_add==NULL || right->type->slot_number->slot_add==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_add(left, right);
}

object* object_sub(object* left, object* right){
    if (left->type->slot_number==NULL || right->type->slot_number==NULL || left->type->slot_number->slot_sub==NULL || right->type->slot_number->slot_sub==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_sub(left, right);
}

object* object_mul(object* left, object* right){
    if (left->type->slot_number==NULL || right->type->slot_number==NULL || left->type->slot_number->slot_mul==NULL || right->type->slot_number->slot_mul==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_mul(left, right);
}

object* object_div(object* left, object* right){
    if (left->type->slot_number==NULL || right->type->slot_number==NULL || left->type->slot_number->slot_div==NULL || right->type->slot_number->slot_div==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_div(left, right);
}

object* object_neg(object* right){
    if (right->type->slot_number==NULL || right->type->slot_number->slot_neg==NULL){
        return NULL;
    }
    return right->type->slot_number->slot_neg(right);
}

object* object_str(object* obj){
    return obj->type->slot_str(obj);
}

object* object_repr(object* obj){
    return obj->type->slot_repr(obj);
}

void object_print(object* obj){
    cout<<(*CAST_STRING(object_str(obj))->val);
}

string object_cstr(object* obj){
    return (*CAST_STRING(object_str(obj))->val);
}

string object_crepr(object* obj){
    return (*CAST_STRING(object_repr(obj))->val);
}

object* object_cmp(object* self, object* other, uint8_t type){
    return self->type->slot_cmp(self, other, type);
}

size_t object_find(object* iter, object* needle){
    object* o=iter->type->slot_next(iter);
    int i=0;
    while (o){
        if (istrue(object_cmp(o,needle, CMP_EQ))){
            while (o){o=iter->type->slot_next(iter);}
            return i;
        }
        i++;
        o=iter->type->slot_next(iter);
    }
    return -1;
}

bool object_find_bool(object* iter, object* needle){
    object* o=iter->type->slot_next(iter);
    int i=0;
    while (o){
        if (istrue(object_cmp(o,needle, CMP_EQ))){
            while (o){o=iter->type->slot_next(iter);}
            return true;
        }
        i++;
        o=iter->type->slot_next(iter);
    }
    return false;
}

bool object_find_bool_dict_keys(object* dict, object* needle){
    for (auto k: (*CAST_DICT(dict)->val)){
        if (istrue(object_cmp(k.first,needle, CMP_EQ))){
            return true;
        }
    }
    return false;
}

object* object_find_dict_keys(object* dict, object* needle){
    for (auto k: (*CAST_DICT(dict)->val)){
        if (istrue(object_cmp(k.first,needle, CMP_EQ))){
            return k.first;
        }
    }
    return NULL;
}

object* setup_args(object* dict, uint32_t argc, object* selfargs, object* selfkwargs, object* args, object* kwargs){
    uint32_t argn=0;
    uint32_t argsnum=argc-CAST_INT(selfkwargs->type->slot_len(selfkwargs))->val->to_int();

    //Positional
    object* key=args->type->slot_next(args);
    object* names=new_list();
    
    while (key){
        dict->type->slot_set(dict, selfargs->type->slot_get(selfargs, new_int_fromint(argn)), key);
        argn++;
        names->type->slot_append(names, selfargs->type->slot_get(selfargs, new_int_fromint(argn-1)));
        key=args->type->slot_next(args);
    }
    //

    
    
    //Keyword    
    key=selfkwargs->type->slot_next(selfkwargs);
    uint32_t argn_tmp=argsnum;
    while (key){
        if (!object_find_bool(names, selfargs->type->slot_get(selfargs, new_int_fromint(argn_tmp)))){
            dict->type->slot_set(dict, selfargs->type->slot_get(selfargs, new_int_fromint(argn_tmp)), key);
        }
        argn_tmp++;
        key=selfkwargs->type->slot_next(selfkwargs);
    }
    
    for (auto k: (*CAST_DICT(kwargs)->val)){
        //Check if k.first in self.args
        if (!object_find_bool(selfargs, k.first)){
            //Error
            return NULL;
        }
        //

        dict->type->slot_set(dict, k.first, k.second);
        argn++;
    }
    
    return dict;
}

object* object_genericgetattr(object* obj, object* attr){
    //Check dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        if (object_find_bool_dict_keys(dict, attr)){
            return dict->type->slot_get(dict, attr);
        }
    }
    //Check type dict
    if (obj->type->dict!=0){
        object* dict = obj->type->dict;
        if (object_find_bool_dict_keys(dict, attr)){
            return dict->type->slot_get(dict, attr);
        }
    }
    //Check bases
    uint32_t total_bases = CAST_INT(obj->type->bases->type->slot_len(obj->type->bases))->val->to_long_long();
    for (uint32_t i=0; i<total_bases; i++){
        TypeObject* base_tp=CAST_TYPE(obj->type->bases->type->slot_get(obj->type->bases, new_int_fromint(i)));
        //Check type dict
        if (base_tp->dict!=0){
            object* dict = base_tp->dict;
            if (object_find_bool_dict_keys(dict, attr)){
                return dict->type->slot_get(dict, attr);
            }
        }
    }
    vm_add_err(&AttributeError, vm, "%s has no attribute '%s'",obj->type->name->c_str(), object_cstr(attr).c_str());
    return NULL;
}

object* object_getattr(object* obj, object* attr){
    if (obj->type->slot_getattr!=NULL){
        return obj->type->slot_getattr(obj, attr);
    }
    vm_add_err(&AttributeError, vm, "%s has no attribute '%s'",obj->type->name->c_str(), object_cstr(attr).c_str());
    return NULL;
}

void object_genericsetattr(object* obj, object* attr, object* val){
     //Try instance dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        dict->type->slot_set(dict, attr, val);
        return;
    }
    //Check type dict 
    if (obj->type->dict!=0){
        object* dict = obj->type->dict;
        if (object_find_bool_dict_keys(dict, attr)){
            dict->type->slot_set(dict, attr, val);
            return;
        }
    }
    //Check bases
    uint32_t total_bases = CAST_INT(obj->type->bases->type->slot_len(obj->type->bases))->val->to_long_long();
    for (uint32_t i=0; i<total_bases; i++){
        TypeObject* base_tp=CAST_TYPE(obj->type->bases->type->slot_get(obj->type->bases, new_int_fromint(i-1)));

        //Check type dict
        if (base_tp->dict!=0){
            object* dict = base_tp->dict;
            if (object_find_bool_dict_keys(dict, attr)){
                dict->type->slot_set(dict, attr, val);
                return;
            }
        }
    }

    vm_add_err(&AttributeError, vm, "%s is read only",obj->type->name->c_str());
    return;
}

void object_setattr(object* obj, object* attr, object* val){
    if (obj->type->slot_setattr!=NULL){
        obj->type->slot_setattr(obj, attr,val);
        return;
    }
    vm_add_err(&AttributeError, vm, "%s is read only",obj->type->name->c_str());
    return;
}

object* object_call(object* obj, object* args, object* kwargs){
    return obj->type->slot_call(obj, args,kwargs);
}

object* object_istruthy(object* obj){
    if (obj->type->slot_number==NULL || obj->type->slot_number->slot_bool==NULL){
        return new_bool_false();
    }
    return obj->type->slot_number->slot_bool(obj);
}

object* object_get(object* base, object* idx){
    if (base->type->slot_get==NULL){
        vm_add_err(&TypeError, vm, "Type '%s' is not subscriptable",base->type->name->c_str());
        return NULL;
    }
    object* obj=base->type->slot_get(base, idx);
    return obj;
}

void object_set(object* base, object* idx, object* val){
    if (base->type->slot_set==NULL){
        vm_add_err(&TypeError, vm, "Type '%s' does not support item assignment",base->type->name->c_str());
        return;
    }
    base->type->slot_set(base, idx, val);
}

bool object_issubclass(object* obj, TypeObject* t){
    if ((void*)obj->type==(void*)t){
        return true;
    }
    object* base=obj->type->bases->type->slot_next(obj->type->bases);
    while (base){
        if ((void*)base==(void*)t){
            return true;
        }
        base=obj->type->bases->type->slot_next(obj->type->bases);
    }
    return false;
}