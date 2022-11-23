inline void _FPLDECREF(struct object* object){
    object->refcnt--;
    if (object->refcnt==0){
        if (object->type->slot_del!=NULL){
            object->type->slot_del(object);
            if (object->refcnt>0){
                return;
            }
        }
        
        //If it is immutable, free it now
        if (!object->type->gc_trackable){
            fpl_free(object);
        }
        return;
    }
    return;
} 

inline object* _FPLINCREF(struct object* object){
    object->refcnt++;
    return object;
}


object* new_object(TypeObject* type){
    object* object = (struct object*) fpl_malloc(type->size);
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

    if (gc.gen0_n==gc.gen0_thresh){
        gc_collect(0);
    }
    else if (gc.gen1_n>=gc.gen1_thresh){
        gc_collect(1);
    }
    
    return object;
}

object_var* new_object_var(TypeObject* type, size_t size){
    object_var* object = (object_var*) fpl_malloc(sizeof(struct object_var)+type->var_base_size);
    object->refcnt=1;
    object->gc_ref=0;
    if (vm==NULL){
        object->gc_ref++;
    }
    object->type=type;
    object->var_size=type->var_base_size;
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

    if (gc.gen0_n==gc.gen0_thresh){
        gc_collect(0);
    }
    else if (gc.gen1_n>=gc.gen1_thresh){
        gc_collect(1);
    }

    return object;
}



object* object_pow(object* left, object* right){
    if (left->type->slot_number==NULL || right->type->slot_number==NULL || left->type->slot_number->slot_pow==NULL || right->type->slot_number->slot_pow==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_pow(left, right);
}

object* object_mod(object* left, object* right){
    if (left->type->slot_number==NULL || right->type->slot_number==NULL || left->type->slot_number->slot_mod==NULL || right->type->slot_number->slot_mod==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_mod(left, right);
}

object* object_fldiv(object* left, object* right){
    if (left->type->slot_number==NULL || right->type->slot_number==NULL || left->type->slot_number->slot_fldiv==NULL || right->type->slot_number->slot_fldiv==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_fldiv(left, right);
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

object* object_not(object* right){
    if (right->type->slot_number==NULL || right->type->slot_number->slot_not==NULL){
        return NULL;
    }
    return right->type->slot_number->slot_not(right);
}

object* object_and(object* left, object* right){
    if (right->type->slot_number==NULL || right->type->slot_number->slot_and==NULL || left->type->slot_number==NULL || left->type->slot_number->slot_and==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_and(left, right);
}

object* object_or(object* left, object* right){
    if (right->type->slot_number==NULL || right->type->slot_number->slot_or==NULL || left->type->slot_number==NULL || left->type->slot_number->slot_or==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_or(left, right);
}

object* object_xor(object* left, object* right){
    if (right->type->slot_number==NULL || right->type->slot_number->slot_or==NULL || left->type->slot_number==NULL || left->type->slot_number->slot_or==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_xor(left, right);
}

object* object_lshift(object* left, object* right){
    if (right->type->slot_number==NULL || right->type->slot_number->slot_lshift==NULL || left->type->slot_number==NULL || left->type->slot_number->slot_lshift==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_lshift(left, right);
}

object* object_rshift(object* left, object* right){
    if (right->type->slot_number==NULL || right->type->slot_number->slot_rshift==NULL || left->type->slot_number==NULL || left->type->slot_number->slot_rshift==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_rshift(left, right);
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
    object* str=object_str(obj);
    string s = (*CAST_STRING(str)->val);
    FPLDECREF(str);
    return s;
}

string object_crepr(object* obj){
    object* str=object_repr(obj);
    string s = (*CAST_STRING(str)->val);
    FPLDECREF(str);
    return s;
}

object* object_cmp(object* self, object* other, uint8_t type){
    return self->type->slot_cmp(self, other, type);
}

size_t object_find(object* iter, object* needle){
    object* len=iter->type->slot_mappings->slot_len(iter);
    for (int i=0; i<CAST_INT(len)->val->to_int(); i++){
        bool b=list_index_int(iter, i)->type==needle->type && istrue(object_cmp(list_index_int(iter, i),needle, CMP_EQ));
        if (b){
            FPLDECREF(len);
            FPLDECREF(needle);
            return i;
        }
    }
    FPLDECREF(len);
    FPLDECREF(needle);
    return -1;
}

bool object_find_bool(object* iter, object* needle){
    object* len=iter->type->slot_mappings->slot_len(iter);
    for (int i=0; i<CAST_INT(len)->val->to_int(); i++){
        if (list_index_int(iter, i)->type==needle->type && istrue(object_cmp(list_index_int(iter, i),needle, CMP_EQ))){
            FPLDECREF(len);
            return true;
        }
    }
    FPLDECREF(len);
    return false;
}

bool object_find_bool_dict_keys(object* dict, object* needle){
    if (CAST_DICT(dict)->val->find(needle)!=CAST_DICT(dict)->val->end()){
        return true;
    }
    return false;
}

object* object_find_dict_keys(object* dict, object* needle){
    if (CAST_DICT(dict)->val->find(needle)!=CAST_DICT(dict)->val->end()){
        return needle;
    }
    return NULL;
}

object* setup_args(object* dict, uint32_t argc, object* selfargs, object* selfkwargs, object* args, object* kwargs){
    if (argc==0){
        return dict;
    }
    uint32_t argn=0;
    uint32_t argsnum=argc-CAST_LIST(selfkwargs)->size;

    //Positional
    object* names=new_list();
    for (int i=0; i<CAST_LIST(args)->size; i++){
        object* o=list_index_int(selfargs, argn);
        object* res=dict->type->slot_mappings->slot_set(dict, o, list_index_int(args, i));
        if (res!=NULL && res!=TERM_PROGRAM){
            FPLDECREF(res);
        }
        names->type->slot_mappings->slot_append(names, o);
        argn++;
    }
    if (argc==argn){
        return dict;
    }
    
    //
  
    
    //Keyword
    uint32_t argn_tmp=argsnum;
    for (int i=0; i<CAST_LIST(selfkwargs)->size; i++){
        object* o=list_index_int(selfargs, argn_tmp);
        
        if (!object_find_bool(names, o)){
            object* res=dict->type->slot_mappings->slot_set(dict, o, list_index_int(selfkwargs, i));
            if (res!=NULL && res!=TERM_PROGRAM){
                FPLDECREF(res);
            }
        }
        argn_tmp++;
    }
    //Setup user kwargs
    for (auto k: (*CAST_DICT(kwargs)->val)){
        //Check if k.first in self.args
        if (!object_find_bool(selfargs, k.first)){
            vm_add_err(&NameError, vm, "Got unexpected keyword argument '%s'", object_cstr(k.first).c_str());
            FPLDECREF(names);
            return NULL;
        }
        //

        object* res=dict->type->slot_mappings->slot_set(dict, k.first, k.second);
        if (res!=NULL && res!=TERM_PROGRAM){
            FPLDECREF(res);
        }
        argn++;
    }
    FPLDECREF(names);
    
    return dict;
}

object* setup_args_allargs(object* dict, uint32_t argc, object* selfargs, object* selfkwargs, object* args, object* kwargs){
    uint32_t argn=0;
    uint32_t argsnum=argc-CAST_LIST(selfkwargs)->size;

    //Positional
    dict_set_noinc_noret(dict, str_new_fromstr("args"), args);
    FPLINCREF(args);
    //

    
    
    //Keyword
    uint32_t argn_tmp=argsnum;
    for (int i=0; i<CAST_LIST(selfkwargs)->size; i++){
        object* o=list_index_int(selfargs, argn_tmp);
        
        object* res=dict->type->slot_mappings->slot_set(dict, o, list_index_int(selfkwargs, i));
        if (res!=NULL && res!=TERM_PROGRAM){
            FPLDECREF(res);
        }
        argn_tmp++;
    }
    //Setup user kwargs
    for (auto k: (*CAST_DICT(kwargs)->val)){
        //Check if k.first in self.args
        if (!object_find_bool(selfargs, k.first)){
            vm_add_err(&NameError, vm, "Got unexpected keyword argument '%s'", object_cstr(k.first).c_str());
            return NULL;
        }
        //

        object* res=dict->type->slot_mappings->slot_set(dict, k.first, k.second);
        if (res!=NULL && res!=TERM_PROGRAM){
            FPLDECREF(res);
        }
        argn++;
    }
    
    return dict;
}

object* setup_args_stars(object* dict, uint32_t argc, object* selfargs, object* selfkwargs, object* args, object* kwargs, int flags, \
                        object* stargs, object* stkwargs){
    
    if (argc==0){
        return dict;
    }
    uint32_t argn=0;
    uint32_t argsnum=argc-CAST_LIST(selfkwargs)->size;
    //Positional
    object* names=new_list();
    int selfarglen=CAST_LIST(selfargs)->size;
    int arglen=CAST_LIST(args)->size;
    if (arglen>=selfarglen){
        for (int i=0; i<selfarglen; i++){
            object* o=list_index_int(selfargs, argn);
            object* res=dict->type->slot_mappings->slot_set(dict, o, list_index_int(args, i));
            if (res!=NULL && res!=TERM_PROGRAM){
                FPLDECREF(res);
            }
            names->type->slot_mappings->slot_append(names, o);
            argn++;
        } 
        if (argc==argn){
            return dict;
        }
        if (flags==FUNC_STAR || flags==FUNC_STARARGS){
            //Star positional
            object* new_tup=new_tuple();
            for (int i=argn; i<CAST_LIST(args)->size; i++){
                new_tup->type->slot_mappings->slot_append(new_tup, list_index_int(args, i));
                argn++;
            }
            if (stargs!=NULL){
                object* res=dict->type->slot_mappings->slot_set(dict, stargs, new_tup);
                if (res!=NULL && res!=TERM_PROGRAM){
                    FPLDECREF(res);
                }
            }
            FPLDECREF(new_tup);
        }
    }
    else{
        for (int i=0; i<arglen; i++){
            object* o=list_index_int(selfargs, argn);
            object* res=dict->type->slot_mappings->slot_set(dict, o, list_index_int(args, i));
            if (res!=NULL && res!=TERM_PROGRAM){
                FPLDECREF(res);
            }
            names->type->slot_mappings->slot_append(names, list_index_int(selfargs, argn));
            argn++;
        }
    }
    
    //

    
    
    //Keyword
    uint32_t argn_tmp=argsnum;
    for (int i=0; i<CAST_LIST(selfkwargs)->size; i++){
        object* o=list_index_int(selfargs, argn_tmp);
        
        if (!object_find_bool(names, o)){
            object* res=dict->type->slot_mappings->slot_set(dict, o, list_index_int(selfkwargs, i));
            if (res!=NULL && res!=TERM_PROGRAM){
                FPLDECREF(res);
            }
        }
        argn_tmp++;
    }
    //Setup user kwargs
    object* stdict;
    if (flags==FUNC_STAR || flags==FUNC_STARKWARGS){
        stdict=new_dict();
    }
    else{
        stdict=NULL;
    }
    for (auto k: (*CAST_DICT(kwargs)->val)){
        //Check if k.first in self.args
        if (!object_find_bool(selfargs, k.first)){ 
            if (flags==FUNC_STAR || flags==FUNC_STARKWARGS){
                dict_set_noret(stdict, k.first, k.second);
                argn++;
                continue; 
            }
            else{
                vm_add_err(&NameError, vm, "Got unexpected keyword argument '%s'", object_cstr(k.first).c_str());
                FPLDECREF(names);
                return NULL;
            }
        }
        //

        if (object_find_bool(names, k.first)){
            vm_add_err(&NameError, vm, "Got multiple values for keyword argument '%s'", object_cstr(k.first).c_str());
            FPLDECREF(names);
            return NULL;
        }

        object* res=dict->type->slot_mappings->slot_set(dict, k.first, k.second);
        if (res!=NULL && res!=TERM_PROGRAM){
            FPLDECREF(res);
        }
        argn++;
    }
    FPLDECREF(names);
    
    

    if (flags==FUNC_STAR || flags==FUNC_STARKWARGS){
        //Star keyword
        if (stkwargs!=NULL){
            object* res=dict->type->slot_mappings->slot_set(dict, stkwargs, stdict);
            if (res!=NULL && res!=TERM_PROGRAM){
                FPLDECREF(res);
            }
        }
        FPLDECREF(stdict);
    }
    
    return dict;
}

object* object_getattr_type(object* obj, object* attr){
    //Check type dict
    if (CAST_TYPE(obj)->dict!=0){
        object* dict = CAST_TYPE(obj)->dict;
        if (object_find_bool_dict_keys(dict, attr)){
            return dict_get(dict, attr);
        }
    }

    return NULL;
}

object* object_getattr_self(object* obj, object* attr){
    //Check dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        if (object_find_bool_dict_keys(dict, attr)){
            return dict_get(dict, attr);
        }
    }
    //Check type dict
    if (obj->type->dict!=0){
        object* dict = obj->type->dict;
        if (object_find_bool_dict_keys(dict, attr)){
            return dict_get(dict, attr);
        }
    }

    return NULL;
}

object* object_genericgetattr_notype_deref(object* obj, object* attr){
    object* v=object_genericgetattr_notype(obj, attr);
    FPLDECREF(attr);
    return v;
}

object* object_genericgetattr_notype(object* obj, object* attr){
    object* res=NULL;
    //Check dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        if (object_find_bool_dict_keys(dict, attr)){
            res = dict_get(dict, attr);
            goto done;
        }
    }
    //Check type dict
    if (obj->type->dict!=0){
        object* dict = obj->type->dict;
        if (object_find_bool_dict_keys(dict, attr)){
            res = dict_get(dict, attr);
            goto done;
        }
    }

    if (res==NULL){
        vm_add_err(&AttributeError, vm, "%s has no attribute '%s'",obj->type->name->c_str(), object_cstr(attr).c_str());
    }
    else{
        done:
        if (res->type->slot_descrget!=NULL){
            object* r=res->type->slot_descrget(obj, res, (object*)(obj->type));
            FPLDECREF(res);
            return r;
        }
    }
    return res;
}

object* object_genericgetattr_deref(object* obj, object* attr){
    object* v=object_genericgetattr(obj, attr);
    FPLDECREF(attr);
    return v;
}

object* object_genericgetattr(object* obj, object* attr){
    object* res=NULL;   
    //Check dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        if (CAST_DICT(dict)->val->find(attr)!=CAST_DICT(dict)->val->end()){
            res=CAST_DICT(dict)->val->at(attr);
            FPLINCREF(res);
            goto done;
        }
    }
    //Check type dict
    if (obj->type->dict!=0){
        object* dict = obj->type->dict;
        if (CAST_DICT(dict)->val->find(attr)!=CAST_DICT(dict)->val->end()){
            res=CAST_DICT(dict)->val->at(attr);
            FPLINCREF(res);
            goto done;
        }
    }

    if (res==NULL){
        //Check bases
        uint32_t total_bases = CAST_INT(list_len(obj->type->bases))->val->to_long_long();
        for (uint32_t i=0; i<total_bases; i++){
            TypeObject* base_tp=CAST_TYPE(list_index_int(obj->type->bases, i));
            //Check type dict
            if (base_tp->dict!=0){
                object* dict = base_tp->dict;
                if (CAST_DICT(dict)->val->find(attr)!=CAST_DICT(dict)->val->end()){
                    res=CAST_DICT(dict)->val->at(attr);
                    FPLINCREF(res);
                    goto done;
                }
            }
        }
    }
    if (res==NULL){
        vm_add_err(&AttributeError, vm, "%s has no attribute '%s'",obj->type->name->c_str(), object_cstr(attr).c_str());
    }
    else{
        done:
        if (res->type->slot_descrget!=NULL){
            object* r=res->type->slot_descrget(obj, res, (object*)(obj->type));
            FPLDECREF(res);
            return r;
        }
    }
    return res;
}

object* object_getattr_noerror(object* obj, object* attr){
    if (obj->type->slot_getattr!=NULL){
        return obj->type->slot_getattr(obj, attr);
    }
    
    return NULL;
}

object* object_getattr_deref(object* obj, object* attr){
    object* v=object_getattr(obj, attr);
    FPLDECREF(attr);
    return v;
}

object* object_getattr(object* obj, object* attr){
    if (obj->type->slot_getattr!=NULL){
        return obj->type->slot_getattr(obj, attr);
    }
    vm_add_err(&AttributeError, vm, "%s has no attribute '%s'",obj->type->name->c_str(), object_cstr(attr).c_str());
    return NULL;
}

object* object_genericsetattr(object* obj, object* attr, object* val){
    object* d=NULL;
    object* dict_=NULL;
    
    //Try instance dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        dict_=dict;
        if (object_find_bool_dict_keys(dict, attr)){
            d=dict;
            goto done;
        }
    }
    
    //Check type dict 
    else if (d==NULL && obj->type->dict!=0){
        object* dict = obj->type->dict;
        dict_=dict;
        if (object_find_bool_dict_keys(dict, attr)){
            d=dict;
            goto done;
        }
    }

    else if (d==NULL){
        //Check bases
        uint32_t total_bases = CAST_INT(list_len(obj->type->bases))->val->to_long_long();
        for (uint32_t i=0; i<total_bases; i++){
            TypeObject* base_tp=CAST_TYPE(list_index_int(obj->type->bases, i));

            //Check type dict
            if (base_tp->dict!=0){
                object* dict = base_tp->dict;
                dict_=dict;
                if (object_find_bool_dict_keys(dict, attr)){
                    d=dict;
                    goto done;
                }
            }
        }
    }

    if (d==NULL && dict_==NULL){
        vm_add_err(&AttributeError, vm, "%s object is read only",obj->type->name->c_str());
        return NULL;
    }
    else if (d!=NULL){
        done:
        object* res=dict_get(d, attr);
        if (res!=NULL && res->type->slot_descrget!=NULL && res->type->slot_descrset==NULL){
            vm_add_err(&AttributeError, vm, "attribute '%s' of '%s' object is read only",object_cstr(attr).c_str() ,obj->type->name->c_str());
            return NULL;
        }
        if (res!=NULL && !(res->type->slot_descrset==NULL && res->type->slot_descrget==NULL) ){
            if (res!=NULL && res->type->slot_descrset!=NULL){
                object* v=res->type->slot_descrset(obj, res, val);
                FPLDECREF(res);
                FPLDECREF(val);
                if (v!=NULL && v!=TERM_PROGRAM){
                    FPLDECREF(v);
                    return SUCCESS;
                }
                return v;
            }
        }
        else{
            if (vm!=NULL && vm->exception!=NULL){
                FPLDECREF(vm->exception);
                vm->exception=NULL;
            }
        }
    }
    
    dict_set_noret(dict_, attr, val);
    return SUCCESS;
}

object* object_setattr_deref(object* obj, object* attr, object* val){
    object* v=object_setattr(obj, attr, val);
    FPLDECREF(attr);
    return v;
}

object* object_setattr(object* obj, object* attr, object* val){
    if (obj->type->slot_setattr!=NULL){
        return obj->type->slot_setattr(obj, attr,val);
    }
    vm_add_err(&AttributeError, vm, "%s object is read only",obj->type->name->c_str());
    return NULL;
}

object* object_call(object* obj, object* args, object* kwargs){
    return obj->type->slot_call(obj, args,kwargs);
}

object* object_call_nokwargs(object* obj, object* args){
    object* kwargs=new_dict();
    object* res=obj->type->slot_call(obj, args,kwargs);
    FPLDECREF(kwargs);
    return res;
}

object* object_istruthy(object* obj){
    if (obj->type->slot_number==NULL || obj->type->slot_number->slot_bool==NULL){
        return new_bool_false();
    }
    return obj->type->slot_number->slot_bool(obj);
}

object* object_get(object* base, object* idx){
    if (base->type->slot_mappings==NULL || base->type->slot_mappings->slot_get==NULL){
        vm_add_err(&TypeError, vm, "Type '%s' is not subscriptable",base->type->name->c_str());
        return NULL;
    }
    object* obj=base->type->slot_mappings->slot_get(base, idx);
    return obj;
}

void object_set(object* base, object* idx, object* val){
    if (base->type->slot_mappings==NULL || base->type->slot_mappings->slot_set==NULL){
        vm_add_err(&TypeError, vm, "Type '%s' does not support item assignment",base->type->name->c_str());
        return;
    }
    object* res=base->type->slot_mappings->slot_set(base, idx, val);
    if (res!=NULL && res!=TERM_PROGRAM){
        FPLDECREF(res);
    }
}

bool object_issubclass(object* obj, TypeObject* t){
    if ((void*)obj->type==(void*)t){
        return true;
    }
    object* len=obj->type->bases->type->slot_mappings->slot_len(obj->type->bases);
    for (int i=0; i<CAST_INT(len)->val->to_int(); i++){
        if ((void*)list_index_int(obj->type->bases, i)==(void*)t){
            FPLDECREF(len);
            return true;
        }
    }
    
    FPLDECREF(len);
    return false;
}

object* object_gt(object* left, object* right){
    if (left->type->slot_cmp==NULL || right->type->slot_cmp==NULL){
        return NULL;
    }
    return object_cmp(left, right, CMP_GT);
}

object* object_gte(object* left, object* right){
    if (left->type->slot_cmp==NULL || right->type->slot_cmp==NULL){
        return NULL;
    }
    return object_cmp(left, right, CMP_GTE);
}

object* object_lt(object* left, object* right){
    if (left->type->slot_cmp==NULL || right->type->slot_cmp==NULL){
        return NULL;
    }
    return object_cmp(left, right, CMP_LT);
}

object* object_lte(object* left, object* right){
    if (left->type->slot_cmp==NULL || right->type->slot_cmp==NULL){
        return NULL;
    }
    return object_cmp(left, right, CMP_LTE);
}

object* object_int(object* left){
    if (left->type->slot_number==NULL || left->type->slot_number->slot_int==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_int(left);
}

object* object_float(object* left){
    if (left->type->slot_number==NULL || left->type->slot_number->slot_float==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_float(left);
}

object* object_abs(object* left){
    if (left->type->slot_number==NULL || left->type->slot_number->slot_abs==NULL){
        return NULL;
    }
    return left->type->slot_number->slot_abs(left);
}

object* generic_iter_iter(object* self){
    FPLINCREF(self);
    return self;
}

void object_del_item(object* base, object* idx){
    object* res=base->type->slot_mappings->slot_set(base, idx, NULL);
    if (res!=NULL && res!=TERM_PROGRAM){
        FPLDECREF(res);
    }
}

object* object_iscallable(object* obj){
    if (obj->type->slot_call==NULL){
        return new_bool_false();
    }
    return new_bool_true();
}

object* object_bytes(object* obj){
    if (object_issubclass(obj, &BytesType)){
        return obj;
    }
    object* nm=str_new_fromstr("__bytes__");
    object* o=object_getattr(obj, nm);
    FPLDECREF(nm);
    if (o==NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }
    return o;
}

object* object_in(object* left, object* right){
    return right->type->slot_cmp(right, left, CMP_IN);
}

object* object_in_iter(object* left, object* right){
    if (right->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", right->type->name->c_str());
        return NULL;
    }
    
    object* iter=right->type->slot_iter(right);
    
    if (iter!=NULL && iter->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", iter->type->name->c_str());
        return NULL;
    }

    object* one=new_int_fromint(0);
    object* res=NULL;
    
    object* o=iter->type->slot_next(iter);
    while (vm->exception==NULL){        
        if (istrue(object_cmp(o, left, CMP_EQ))){
            FPLDECREF(one);
            FPLDECREF(iter);
            res=new_bool_true();
            break;
        }
        
        cont:
        o=iter->type->slot_next(iter);
    }
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }
    if (res==NULL){
        return new_bool_false();
    }
    return res;
}

object* object_enter_with(object* self){
    object* nm=str_new_fromstr("__enter__");
    object* o=object_getattr(self, nm);
    FPLDECREF(nm);
    if (o==NULL){
        return NULL;
    }

    object* args=new_tuple();

    return object_call_nokwargs(o, args);
}

object* object_exit_with(object* self){
    object* nm=str_new_fromstr("__exit__");
    object* o=object_getattr(self, nm);
    FPLDECREF(nm);
    if (o==NULL){
        return NULL;
    }

    object* args=new_tuple();

    return object_call_nokwargs(o, args);
}



//I have setup memory_error to be a fatal exception
void memory_error(){
    object* exception=NULL;
    struct blockframe* frame=NULL;
    if (!setup_memory_error){
        goto fatal;
    }
    if (hit_memory_error){
        goto quick_traceback;
    }
    hit_memory_error=true;
    exception=new_object(&MemoryError);
    if (exception==NULL){
        goto fatal;
    }
    CAST_EXCEPTION(exception)->err=str_new_fromstr("Out of memory");
    if (CAST_EXCEPTION(exception)->err->type==&MemoryError){
        FPLDECREF(exception);
        goto fatal;
    }

    if (vm!=NULL){
        frame=in_blockstack(vm->blockstack, TRY_BLOCK);
        if (frame!=NULL && frame->obj!=NULL && frame->arg!=3){
            print_traceback();
            
            cout<<frame->obj->type->name->c_str();
            if (CAST_EXCEPTION(frame->obj)->err!=NULL){
                cout<<": "<<*CAST_STRING(CAST_EXCEPTION(frame->obj)->err)->val;
            }
            cout<<endl;
            cout<<endl<<"While handling the above exception, another exception was raised."<<endl<<endl;
        }
        print_traceback();
        
        cout<<exception->type->name->c_str();
        if (CAST_EXCEPTION(exception)->err!=NULL){
            cout<<": "<<*CAST_STRING(CAST_EXCEPTION(exception)->err)->val;
        }
        cout<<endl;

        if (exception!=NULL){
            FPLDECREF(exception);
        }
        goto done;
        
        quick_traceback:
        cout<<"MemoryError: Out of memory\n";
        goto done;
    }

    fatal:
    cout<<"Fatal memory error before necessary initialization.";

    done:
    finalize_threads();
    exit(0);
}