object* str_new_fromstr(string val){
    object* obj=new_object(&StrType);
    
    ((StrObject*)obj)->val=new string(val);
    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    
    DECREF((object*)obj);
    return o;
}


object* str_int(object* self){
    BigInt* b;
    try{
        b=new BigInt(CAST_STRING(self)->val->c_str());
    }
    catch (std::invalid_argument){
        return NULL;
    }
    return new_int_frombigint(b);
}

object* str_float(object* self){
    double d;
    try{
        d=stod(CAST_STRING(self)->val->c_str());
    }
    catch (std::invalid_argument){
        return NULL;
    }
    return new_float_fromdouble(d);
}


object* str_new(object* type, object* args, object* kwargs){
    if (CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()==0){
        object* obj=new_object(CAST_TYPE(type));
        ((StrObject*)obj)->val=new string("");

        object* o = in_immutables((struct object*)obj);
        if (o==NULL){
            return (object*)obj;
        }
        DECREF((struct object*)obj);
        return o;
    }
    int len=CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (len>0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", len);
        return NULL;
    }
    object* val=INCREF(list_index_int(args, 0));
    string s=object_cstr(val);

    object* obj=new_object(CAST_TYPE(type));
    ((StrObject*)obj)->val=new string(s);

    object* o = in_immutables((struct object*)obj);
    if (o==NULL){
        return (object*)obj;
    }
    DECREF((struct object*)obj);
    return o;
}

object* str_slice(object* self, object* idx){
    object* start=CAST_SLICE(idx)->start;
    object* end=CAST_SLICE(idx)->end;

    string s="";
    int start_v;
    int end_v;
    if (object_istype(start->type, &NoneType)){
        start_v=0;
    }
    else{
        start_v=CAST_INT(start)->val->to_int();
    }
    if (object_istype(end->type, &NoneType)){
        end_v=CAST_LIST(self)->size-1;
    }
    else{
        end_v=CAST_INT(end)->val->to_int();
    }
    
    if (start<0){
        start_v=0;
    }
    if (end_v>=CAST_STRING(self)->val->size()){
        end_v=CAST_STRING(self)->val->size()-1;
    }
    for (int i=start_v; i<=end_v; i++){
        s+=CAST_STRING(self)->val->at(i);
    }
    return str_new_fromstr(s);
}

object* str_get(object* self, object* idx){
    if (object_istype(idx->type, &SliceType)){
        return str_slice(self, idx);
    }
    if (!object_istype(idx->type, &IntType)){
        vm_add_err(&TypeError, vm, "String must be indexed by int not '%s'",idx->type->name->c_str());
        return NULL;
    }
    if (CAST_STRING(self)->val->size()<=CAST_INT(idx)->val->to_long_long()){
        vm_add_err(&IndexError, vm, "String index out of range");
        return NULL;
    }
    
    return str_new_fromstr(string(1,CAST_STRING(self)->val->at(CAST_INT(idx)->val->to_long_long())));
}

object* str_len(object* self){
    return new_int_fromint(CAST_STRING(self)->val->size());
}

object* str_repr(object* self){
    return str_new_fromstr("'"+replace_newlines((*CAST_STRING(self)->val))+"'");
}

object* str_str(object* self){
    return self; //str_new_fromstr(new string((*CAST_STRING(self)->val)));
}

object* str_bool(object* self){
    return new_bool_true();
}

object* str_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_STRING(self)->val->size()!=CAST_STRING(other)->val->size()){
            return new_bool_false();
        }
        if ((*CAST_STRING(self)->val)==(*CAST_STRING(other)->val)){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (CAST_STRING(self)->val->size()==CAST_STRING(other)->val->size()){
            return new_bool_false();
        }
        if ((*CAST_STRING(self)->val)!=(*CAST_STRING(other)->val)){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

void str_del(object* obj){
    delete ((StrObject*)obj)->val;
}

object* str_iter(object* self){
    //Make an iterator
    object* iter=new_object(&StrIterType);
    CAST_STRITER(iter)->val=new string(*CAST_STRING(self)->val);
    CAST_STRITER(iter)->idx=0;
    return iter;
}

void str_iter_del(object* self){
    delete CAST_STRITER(self)->val;
}

object* str_iter_next(object* self){
    if (CAST_STRITER(self)->idx+1>CAST_STRITER(self)->val->size()){
        vm_add_err(&StopIteration, vm, "Iterator out of data");
        return NULL;
    }
    return str_new_fromstr(string(1,CAST_STRITER(self)->val->at(CAST_STRITER(self)->idx++)));
}

object* str_iter_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if ((*CAST_STRITER(self)->val) == (*CAST_STRITER(other)->val)){
            return new_bool_true();
        }
    }
    if (type==CMP_NE){
        if ((*CAST_STRITER(self)->val) != (*CAST_STRITER(other)->val)){
            return new_bool_true();
        }
    }
    return NULL;
}

object* str_iter_bool(object* self){
    return new_bool_true();
}

object* str_add(object* self, object* other){
    if (other->type!=&StrType){
        vm_add_err(&TypeError, vm, "Invalid operand types for +: '%s', and '%s'.", self->type->name->c_str(), other->type->name->c_str());
        return NULL;
    }
        
    return str_new_fromstr(*CAST_STRING(self)->val+*CAST_STRING(other)->val);
}


object* string_join_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);
    object* arg=tuple_index_int(args, 1);

    if (arg->type->slot_iter==NULL){
        vm_add_err(&ValueError, vm, "Expected iterable");
        return NULL; 
    }

    object* iter=arg->type->slot_iter(arg);

    string selfstr=object_cstr(self);
    
    string s="";
    int i=0;
    len=CAST_INT(arg->type->slot_mappings->slot_len(arg))->val->to_long()-1;
    while (vm->exception==NULL){
        object* o=iter->type->slot_next(iter);
        if (o==NULL){
            DECREF(vm->exception);
            break;
        }
        s+=object_cstr(o);
        if (i<len){
            s+=selfstr;
        }
        i++;
    }
    vm->exception=NULL;
    return str_new_fromstr(s);
}

string repeat(string input, unsigned num){
    std::string ret;
    ret.reserve(input.size() * num);
    while (num--)
        ret += input;
    return ret;
}

object* str_mul(object* self, object* other){
    if (other->type!=&IntType){
        vm_add_err(&TypeError, vm, "Invalid operand types for *: '%s', and '%s'.", self->type->name->c_str(), other->type->name->c_str());
        return NULL;
    }
        
    return str_new_fromstr(repeat(*CAST_STRING(self)->val,(CAST_INT(other)->val->to_long())));
}



object* string_replace_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=3 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 3 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);
    object* replace_=tuple_index_int(args, 1);
    object* substr_=tuple_index_int(args, 2);

    if (!object_istype(replace_->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str, got '%s'", replace_->type->name->c_str());
        return NULL; 
    }  
    if (!object_istype(substr_->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str, got '%s'", substr_->type->name->c_str());
        return NULL; 
    }  
    

    string s=*CAST_STRING(self)->val;
    string substr=*CAST_STRING(substr_)->val;
    string replace=*CAST_STRING(replace_)->val;

    string newstr(s);
    
    if (replace.size()==0 || substr.size()==0){
        return self;
    }

    size_t pos=newstr.find(replace);

    while (pos != std::string::npos){
        newstr.replace(pos, replace.size(), substr);

        pos=newstr.find(replace, pos+substr.size());
    }
    
    return str_new_fromstr(newstr);
}

object* string_find_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);  
    object* val=tuple_index_int(args, 1);   

    if (!object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str, got '%s'", val->type->name->c_str());
        return NULL; 
    }  

    string s=*CAST_STRING(self)->val;
    string v=*CAST_STRING(val)->val;
    
    size_t idx=s.find(v);
    
    return new_int_fromint(idx);
}


vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

object* string_split_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);  
    object* val=tuple_index_int(args, 1);   

    if (!object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str, got '%s'", val->type->name->c_str());
        return NULL; 
    }  

    string s(*CAST_STRING(self)->val);
    string delimiter=*CAST_STRING(val)->val;

    object* list=new_list();
    
        
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        list_append(list, str_new_fromstr(token));
        s.erase(0, pos + delimiter.length());
    }
        
    return list;
}