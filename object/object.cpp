inline bool DECREF(struct object* object){
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



        //Delete now
        else if (object->type->gc_trackable || no_outside_refs(object) ){ //No outside references, so we can free
            if (((object_var*)object)->gc_ref - ((struct object*)object)->refcnt<0){
                if (object->type->slot_del!=NULL){
                    object->type->slot_del(object);
                }
            }
            if (object->gen!=2){                
                if (object->gen==1){
                    if (object->ob_prev!=NULL){
                        object->ob_prev->ob_next=object->ob_next;
                    }
                    else{
                        gc.gen1=object->ob_next;
                    }                    
                    if (object->ob_next!=NULL){
                        object->ob_next->ob_prev=object->ob_prev;
                    }
                    gc.gen1_n--;
                }
                else{
                    if (object->ob_prev!=NULL){
                        object->ob_prev->ob_next=object->ob_next;
                    }
                    else{
                        gc.gen0=object->ob_next;
                    }   
                    if (object->ob_next!=NULL){
                        object->ob_next->ob_prev=object->ob_prev;
                    }
                    gc.gen0_n--;
                }

                free(object);
            }        
        }
        return true;
    }
    return false;
} 



inline object* INCREF(struct object* object){
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
                    return INCREF(o);
                }
            }
            if (o->type->name==IntType.name){
                if ((*CAST_INT(o)->val)==(*CAST_INT(obj)->val)){
                    return INCREF(o);
                }
            }
            if (o->type->name==FloatType.name){
                if (CAST_FLOAT(o)->val==CAST_FLOAT(obj)->val){
                    return INCREF(o);
                }
            }
        }
        o=o->ob_next;
    }
    return NULL;
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
    return (*CAST_STRING(object_str(obj))->val);
}

string object_crepr(object* obj){
    return (*CAST_STRING(object_repr(obj))->val);
}

object* object_cmp(object* self, object* other, uint8_t type){
    return self->type->slot_cmp(self, other, type);
}

size_t object_find(object* iter, object* needle){
    for (int i=0; i<CAST_INT(iter->type->slot_mappings->slot_len(iter))->val->to_int(); i++){
        bool b=list_index_int(iter, i)->type==needle->type && istrue(object_cmp(list_index_int(iter, i),needle, CMP_EQ));
        if (b){
            return i;
        }
    }
    return -1;
}

bool object_find_bool(object* iter, object* needle){
    for (int i=0; i<CAST_INT(iter->type->slot_mappings->slot_len(iter))->val->to_int(); i++){
        if (list_index_int(iter, i)->type==needle->type && istrue(object_cmp(list_index_int(iter, i),needle, CMP_EQ))){
            return true;
        }
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
    uint32_t argsnum=argc-CAST_INT(selfargs->type->slot_mappings->slot_len(selfkwargs))->val->to_int();

    //Positional
    object* names=new_list();
    for (int i=0; i<CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int(); i++){
        object* o=list_index_int(selfargs, argn);
        dict->type->slot_mappings->slot_set(dict, o, list_index_int(args, i));
        names->type->slot_mappings->slot_append(names, o);
        argn++;
    }
    
    //

    
    
    //Keyword
    uint32_t argn_tmp=argsnum;
    for (int i=0; i<CAST_INT(selfkwargs->type->slot_mappings->slot_len(selfkwargs))->val->to_int(); i++){
        object* o=list_index_int(selfargs, argn_tmp);
        
        if (!object_find_bool(names, o)){
            dict->type->slot_mappings->slot_set(dict, o, list_index_int(selfkwargs, i));
        }
        argn_tmp++;
    }
    //Setup user kwargs
    for (auto k: (*CAST_DICT(kwargs)->val)){
        //Check if k.first in self.args
        if (!object_find_bool(selfargs, k.first)){
            vm_add_err(&NameError, vm, "Got unexpected keyword argument %s", object_cstr(k.first).c_str());
            DECREF(names);
            return NULL;
        }
        //

        dict->type->slot_mappings->slot_set(dict, k.first, k.second);
        argn++;
    }
    DECREF(names);
    
    return dict;
}

object* setup_args_allargs(object* dict, uint32_t argc, object* selfargs, object* selfkwargs, object* args, object* kwargs){
    uint32_t argn=0;
    uint32_t argsnum=argc-CAST_INT(selfargs->type->slot_mappings->slot_len(selfkwargs))->val->to_int();

    //Positional

    dict->type->slot_mappings->slot_set(dict, str_new_fromstr("args"), args);
    
    //

    
    
    //Keyword
    uint32_t argn_tmp=argsnum;
    for (int i=0; i<CAST_INT(selfkwargs->type->slot_mappings->slot_len(selfkwargs))->val->to_int(); i++){
        object* o=list_index_int(selfargs, argn_tmp);
        
        dict->type->slot_mappings->slot_set(dict, o, list_index_int(selfkwargs, i));
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

        dict->type->slot_mappings->slot_set(dict, k.first, k.second);
        argn++;
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

object* object_genericgetattr_notype(object* obj, object* attr){
    object* res=NULL;
    //Check dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        if (object_find_bool_dict_keys(dict, attr)){
            res=dict_get(dict, attr);
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
                if (object_find_bool_dict_keys(dict, attr)){
                    res=dict_get(dict, attr);
                    break;
                }
            }
        }
    }
    if (res==NULL){
        vm_add_err(&AttributeError, vm, "%s has no attribute '%s'",obj->type->name->c_str(), object_cstr(attr).c_str());
    }
    else{
        if (res->type->slot_descrget!=NULL){
            object* r=res->type->slot_descrget(obj, res);
            return r;
        }
    }
    return res;
}


object* object_genericgetattr(object* obj, object* attr){
    object* res=object_getattr_self(obj, attr);

    if (res==NULL){
        //Check bases
        uint32_t total_bases = CAST_INT(list_len(obj->type->bases))->val->to_long_long();
        for (uint32_t i=0; i<total_bases; i++){
            TypeObject* base_tp=CAST_TYPE(list_index_int(obj->type->bases, i));
            //Check type dict
            if (base_tp->dict!=0){
                object* dict = base_tp->dict;
                if (object_find_bool_dict_keys(dict, attr)){
                    res=dict_get(dict, attr);
                    break;
                }
            }
        }
    }
    if (res==NULL){
        vm_add_err(&AttributeError, vm, "%s has no attribute '%s'",obj->type->name->c_str(), object_cstr(attr).c_str());
    }
    else{
        if (res->type->slot_descrget!=NULL){
            object* r=res->type->slot_descrget(obj, res);
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

object* object_getattr(object* obj, object* attr){
    if (obj->type->slot_getattr!=NULL){
        return obj->type->slot_getattr(obj, attr);
    }
    vm_add_err(&AttributeError, vm, "%s has no attribute '%s'",obj->type->name->c_str(), object_cstr(attr).c_str());
    return NULL;
}

void object_genericsetattr(object* obj, object* attr, object* val){
    object* d=NULL;
    
    //Try instance dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        if (object_find_bool_dict_keys(dict, attr)){
            d=dict;
        }
    }
    
    //Check type dict 
    if (d==NULL && obj->type->dict!=0){
        object* dict = obj->type->dict;
        if (object_find_bool_dict_keys(dict, attr)){
            d=dict;
        }
    }

    if (d==NULL){
        //Check bases
        uint32_t total_bases = CAST_INT(list_len(obj->type->bases))->val->to_long_long();
        for (uint32_t i=0; i<total_bases; i++){
            TypeObject* base_tp=CAST_TYPE(list_index_int(obj->type->bases, i-1));

            //Check type dict
            if (base_tp->dict!=0){
                object* dict = base_tp->dict;
                if (object_find_bool_dict_keys(dict, attr)){
                    d=dict;
                    break;
                }
            }
        }
    }

    if (d==NULL){
        vm_add_err(&AttributeError, vm, "%s object is read only",obj->type->name->c_str());
        return;
    }
    else{
        object* res=dict_get(d, attr);
        if (res!=NULL && !(res->type->slot_descrset==NULL && res->type->slot_descrget==NULL) ){
            if (res!=NULL && res->type->slot_descrset!=NULL){
                object* v=res->type->slot_descrset(obj, res, val);
                if (v!=NULL){
                    DECREF(v);
                }
                return;
            }
            if (res!=NULL && res->type->slot_descrget==NULL){
                vm_add_err(&AttributeError, vm, "%s object is read only",res->type->name->c_str());
                return;
            }
        }
        else{
            if (vm!=NULL && vm->exception!=NULL){
                DECREF(vm->exception);
                vm->exception=NULL;
            }
        }
    }
    
    dict_set(d, attr, val);
    return;
}

void object_setattr(object* obj, object* attr, object* val){
    if (obj->type->slot_setattr!=NULL){
        obj->type->slot_setattr(obj, attr,val);
        return;
    }
    vm_add_err(&AttributeError, vm, "%s object is read only",obj->type->name->c_str());
    return;
}

object* object_call(object* obj, object* args, object* kwargs){
    return obj->type->slot_call(obj, args,kwargs);
}

object* object_call_nokwargs(object* obj, object* args){
    object* kwargs=new_dict();
    object* res=obj->type->slot_call(obj, args,kwargs);
    DECREF(kwargs);
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
    base->type->slot_mappings->slot_set(base, idx, val);
}

bool object_issubclass(object* obj, TypeObject* t){
    if ((void*)obj->type==(void*)t){
        return true;
    }
    for (int i=0; i<CAST_INT(obj->type->bases->type->slot_mappings->slot_len(obj->type->bases))->val->to_int(); i++){
        if ((void*)list_index_int(obj->type->bases, i)==(void*)t){
            return true;
        }
    }
    
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

object* generic_iter_iter(object* self){
    return self;
}

void object_del_item(object* base, object* idx){
    base->type->slot_mappings->slot_set(base, idx, NULL);
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
        DECREF(exception);
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
            DECREF(exception);
        }
        goto done;
        
        quick_traceback:
        cout<<"MemoryError: Out of memory\n";
        goto done;
    }

    fatal:
    cout<<"Fatal memory error before necessary initialization.";

    done:
    exit(0);
}