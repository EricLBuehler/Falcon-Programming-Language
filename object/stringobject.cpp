string trim(string s);

object* str_new_fromstr(string val){
    if(str_map.find(val)!=str_map.end()){
        FPLINCREF(str_map[val]);
        return str_map[val];
    }

    object* obj=new_object(&StrType);
    
    ((StrObject*)obj)->val=new string(val);

    str_map[val]=obj;
    return obj;
}


object* str_int(object* self){
    BigInt* b;
    try{
        b=new BigInt(trim((*CAST_STRING(self)->val)));
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
    if (CAST_LIST(args)->size==0){ 
        if(str_map.find("")!=str_map.end()){
            FPLINCREF(str_map[""]);
            return str_map[""];
        }

        object* obj=new_object(CAST_TYPE(type));
        ((StrObject*)obj)->val=new string("");

        str_map[""]=obj;
        return obj;
    }
    int len=CAST_DICT(kwargs)->val->size();
    if (len>0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", len);
        return NULL;
    }
    object* val=list_index_int(args, 0);
    string s=object_cstr(val);
    if(str_map.find(s)!=str_map.end()){
        FPLINCREF(str_map[s]);
        return str_map[s];
    }

    object* obj=new_object(CAST_TYPE(type));
    ((StrObject*)obj)->val=new string(s);

    str_map[s]=obj;
    return obj;
}

object* str_slice(object* self, object* idx){
    if (STRING_LENGTH(self)==0){
        FPLINCREF(self);
        return self;
    }
    object* start=CAST_SLICE(idx)->start;
    object* end=CAST_SLICE(idx)->end;
    object* step=CAST_SLICE(idx)->step;

    string s="";
    int start_v;
    int end_v;
    int step_v;
    if (object_istype(start->type, &NoneType)){
        start_v=0;
    }
    else{
        object* start_=object_int(start);
        if (start_==NULL || !object_istype(start_->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int",start->type->name->c_str());
            return NULL;
        }
        if (*CAST_INT(start_)->val>LONG_MAX || *CAST_INT(start_)->val<LONG_MIN){
            vm_add_err(&OverflowError, vm, "Value out of range of C long");
            return NULL;
        }
        start_v=CAST_INT(start_)->val->to_long();
        FPLDECREF(start_);
    }
    if (object_istype(end->type, &NoneType)){
        end_v=CAST_STRING(self)->val->size()-1;
    }
    else{
        object* end_=object_int(end);
        if (end_==NULL || !object_istype(end_->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int",start->type->name->c_str());
            return NULL;
        }
        if (*CAST_INT(end_)->val>LONG_MAX || *CAST_INT(end_)->val<LONG_MIN){
            vm_add_err(&OverflowError, vm, "Value out of range of C long");
            return NULL;
        }
        int target=CAST_INT(end_)->val->to_long();
        FPLDECREF(end_);
        const char* arr=CAST_STRING(self)->val->c_str();
        int raw_len=CAST_STRING(self)->val->size();

        int res=get_index_fromtarget(target, raw_len, arr);
        
        end_v=(res>=0) ? res : STRING_LENGTH(self)-1;
    }
    
    if (object_istype(step->type, &NoneType)){
        step_v=1;
    }
    else{
        object* step_=object_int(step);
        if (step_==NULL || !object_istype(step_->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int",step_->type->name->c_str());
            return NULL;
        }
        if (*CAST_INT(step_)->val>LONG_MAX || *CAST_INT(step_)->val<LONG_MIN || *CAST_INT(step_)->val<0){
            vm_add_err(&IndexError, vm, "Value out of range of C long");
            return NULL;
        }
        step_v=CAST_INT(step_)->val->to_long();
        FPLDECREF(step_);
    }

    if (start_v<0){
        start_v=STRING_LENGTH(self)+start_v;
        if (start_v<0){
            vm_add_err(&IndexError, vm, "Index out of range");
            return NULL;
        }
    }
    if (start_v>=STRING_LENGTH(self)){
        start_v=STRING_LENGTH(self)-1;
    }
    if (end_v<0){
        end_v=STRING_LENGTH(self)+start_v;
        if (end_v<0){
            vm_add_err(&IndexError, vm, "Index out of range");
            return NULL;
        }
    }
    if (end_v>=STRING_LENGTH(self)){
        end_v=STRING_LENGTH(self)-1;
    }

    for (int i=start_v; i<=end_v; i+=step_v){
        object* intv=new_int_fromint(i);
        object* o=str_get(self, intv);
        s+=object_cstr(o);
        FPLDECREF(o);
        FPLDECREF(intv);
    }
    return str_new_fromstr(s);
}

object* str_get(object* self, object* idx){
    if (object_istype(idx->type, &SliceType)){
        return str_slice(self, idx);
    }
    
    object* idx_=object_int(idx);
    if (idx_==NULL || !object_istype(idx->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int",idx->type->name->c_str());
        FPLDECREF(idx_);
        return NULL;
    }
    if (*CAST_INT(idx_)->val>LONG_MAX || *CAST_INT(idx_)->val<LONG_MIN){
        vm_add_err(&OverflowError, vm, "Value out of range of C long");
        FPLDECREF(idx_);
        return NULL;
    }
    long lidx=CAST_INT(idx_)->val->to_long();
    if (lidx<0){
        lidx=lidx+CAST_STRING(self)->val->size();
    }
    if (STRING_LENGTH(self)<=lidx || lidx<0){
        vm_add_err(&IndexError, vm, "String index out of range");
        FPLDECREF(idx_);
        return NULL;
    }
    FPLDECREF(idx_);
    
    const char* arr=CAST_STRING(self)->val->c_str();
    int raw_len=CAST_STRING(self)->val->size();

    int res=get_index_fromtarget_min(lidx, raw_len, arr);

    char bytes[]={0, 0, 0, 0, 0};
    extract_uchar_bytes(CAST_STRING(self)->val->c_str(), bytes, res);

    string s(bytes);
    return str_new_fromstr(s);
}

object* str_len(object* self){
    return new_int_fromint(STRING_LENGTH(self));
}

object* str_repr(object* self){
    return str_new_fromstr("'"+replace_newlines((*CAST_STRING(self)->val))+"'");
}

object* str_str(object* self){
    FPLINCREF(self);
    return self; //str_new_fromstr(new string((*CAST_STRING(self)->val)));
}
     
object* str_bool(object* self){
    return new_bool_true();
}

object* str_cmp(object* self, object* other, uint8_t type){
    if (type==CMP_IN){
        return str_in(self, other);
    }
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (STRING_LENGTH(self)!=STRING_LENGTH(other)){
            return new_bool_false();
        }
        if ((*CAST_STRING(self)->val)==(*CAST_STRING(other)->val)){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (STRING_LENGTH(self)==STRING_LENGTH(other)){
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
    str_map.erase(*((StrObject*)obj)->val);
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
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
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
    
    if (iter==NULL){
        vm_add_err(&TypeError, vm, "Expected iterator, got '%s' object", arg->type->name->c_str());
        return NULL;
    }

    string selfstr=object_cstr(self);
    
    string s="";
    int i=0;
    object* len_=arg->type->slot_mappings->slot_len(arg);
    if (*CAST_INT(len_)->val<LONG_MIN || *CAST_INT(len_)->val>LONG_MAX){
        vm_add_err(&OverflowError, vm, "Value out of range of C long");
        return NULL; 
    }
    len=CAST_INT(len_)->val->to_long()-1;
    while (vm->exception==NULL){
        object* o=iter->type->slot_next(iter);
        if (o==NULL){
            FPLDECREF(vm->exception);
            break;
        }
        s+=object_cstr(o);
        if (i<len){
            s+=selfstr;
        }
        i++;
    }
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }
    FPLDECREF(iter);
    FPLDECREF(len_);
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
    if (*CAST_INT(other)->val>LONG_MAX || *CAST_INT(other)->val<LONG_MIN){
        vm_add_err(&OverflowError, vm, "Value out of range of C long");
        return NULL;
    }    
    return str_new_fromstr(repeat(*CAST_STRING(self)->val,(CAST_INT(other)->val->to_long())));
}



object* string_replace_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=3){
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
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
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

object* string_split_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if ((len!=2 && len!=1)){
        vm_add_err(&ValueError, vm, "Expected 1 or 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0); 
    object* val;
    if (len==2){ 
        val=tuple_index_int(args, 1);   
    }
    if (len==1){ 
        val=str_new_fromstr(""); 
    }

    object* v=object_str(val);

    if (v==NULL || !object_istype(v->type, &StrType)){
        vm_add_err(&ValueError, vm, "'%s' object cannot be coercerd to str", val->type->name->c_str());
        return NULL; 
    }  

    string s(*CAST_STRING(self)->val);
    string delimiter=*CAST_STRING(v)->val;

    object* list=new_list();
    
    
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos && delimiter.size()!=0) {
        token = s.substr(0, pos);
        tuple_append_noinc(list, str_new_fromstr(token));
        s.erase(0, pos + delimiter.length());
    }
    if (s.length()>=0){
        tuple_append_noinc(list, str_new_fromstr(s));
    }
        
    return list;
}

string to_upper_(string strToConvert){
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);

    return strToConvert;
}

string to_lower_(string strToConvert){
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::tolower);

    return strToConvert;
}

object* string_upper_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);  
    return str_new_fromstr(to_upper_((*CAST_STRING(self)->val)));
}

object* string_lower_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);  
    return str_new_fromstr(to_lower_((*CAST_STRING(self)->val)));
}

bool is_number(const std::string& s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool is_alpha(const std::string& s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isalpha(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool is_space(const std::string& s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isspace(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool is_lower(const std::string& s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::islower(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool is_upper(const std::string& s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isupper(*it)) ++it;
    return !s.empty() && it == s.end();
}
    
object* string_isalpha_meth(object* self, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    string s=(*CAST_STRING(list_index_int(args, 0))->val);
    if (is_alpha(s)){
        return new_bool_true();
    }
    return new_bool_false();
}
    
object* string_isnumeric_meth(object* self, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    string s=(*CAST_STRING(list_index_int(args, 0))->val);
    if (is_number(s)){
        return new_bool_true();
    }
    return new_bool_false();
}
    
object* string_isspace_meth(object* self, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    string s=(*CAST_STRING(list_index_int(args, 0))->val);
    if (is_space(s)){
        return new_bool_true();
    }
    return new_bool_false();
}
    
object* string_islower_meth(object* self, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    string s=(*CAST_STRING(list_index_int(args, 0))->val);
    if (is_lower(s)){
        return new_bool_true();
    }
    return new_bool_false();
}
    
object* string_isupper_meth(object* self, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    string s=(*CAST_STRING(list_index_int(args, 0))->val);
    if (is_upper(s)){
        return new_bool_true();
    }
    return new_bool_false();
}
    
object* string_count_meth(object* self, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }

    string s=(*CAST_STRING(list_index_int(args, 0))->val);
    string target=(*CAST_STRING(list_index_int(args, 1))->val);

    int occurrences = 0;
    std::string::size_type pos = 0;
    while ((pos = s.find(target, pos )) != std::string::npos) {
        occurrences++;
        pos += target.length();
    }
    return new_int_fromint(occurrences);
}

const string WHITESPACE = " \n\r\t\f\v";
 
string lefttrim(string s){
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s.substr(start);
}

string righttrim(string s){
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}
 
string trim(string s) {
    return righttrim(lefttrim(s));
}
    
object* string_strip_meth(object* self, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    string s=(*CAST_STRING(list_index_int(args, 0))->val);
    
    return str_new_fromstr(trim(s));
}
    
object* string_rstrip_meth(object* self, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    string s=(*CAST_STRING(list_index_int(args, 0))->val);
    
    return str_new_fromstr(righttrim(s));
}
    
object* string_lstrip_meth(object* self, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    string s=(*CAST_STRING(list_index_int(args, 0))->val);
    
    return str_new_fromstr(lefttrim(s));
}

object* string_contains_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
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
    
    if (idx==-1){
        return new_bool_false();
    }
    return new_bool_true();
}

object* str_in(object* self, object* other){
    if (!object_istype(other->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str, got '%s'", other->type->name->c_str());
        return NULL; 
    }  

    string s=*CAST_STRING(self)->val;
    string v=*CAST_STRING(other)->val;
    
    size_t idx=s.find(v);
    if (idx!=(size_t)-1){
        return new_bool_true();
    }
    return new_bool_false();
}

object* string_encode_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* self=tuple_index_int(args, 0);  
    object* encoding=tuple_index_int(args, 1);

    if (!object_istype(encoding->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected str, got '%s'", encoding->type->name->c_str());
        return NULL; 
    }  

    string s=*CAST_STRING(self)->val;
    string enc=*CAST_STRING(encoding)->val;
    
    iconv_t cd = iconv_open(enc.c_str(), "UTF-8");
    if((int) cd == -1) {
        if (errno == EINVAL) {
            vm_add_err(&ValueError, vm, "Invalid conversion");
            return NULL; 
        }
    }

    size_t s_size=s.size();  
    size_t new_size=(s.size()+1)*sizeof(uint32_t);

    const char* orig_str=s.c_str();
    char* converted=(char*)fpl_calloc(new_size, sizeof(char));
    char* start=converted;
    
    int ret = iconv(cd, &orig_str, &s_size, &converted, &new_size);

    if((iconv_t)ret == (iconv_t)(-1)) {
        vm_add_err(&ValueError, vm, "Invalid multibyte sequence encountered");
        return NULL; 
    }
    iconv_close(cd);

    size_t real_size=abs(start-converted);
    if (real_size!=0){
        start=(char*)fpl_realloc(start, real_size);
    }

    return bytes_new_frombytearr(start, real_size);
}

object* string_startswith_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
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
    
    bool res=s.rfind(v,0)==0;
    
    if (!res){
        return new_bool_false();
    }
    return new_bool_true();
}

bool endsWith(std::string s, std::string suffix){
    return s.rfind(suffix) == std::abs((long)(s.size()-suffix.size()));
}

object* string_endswith_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
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
    
    bool res=endsWith(s, v);
    
    if (!res){
        return new_bool_false();
    }
    return new_bool_true();
}

object* string_rfind_meth(object* selftp, object* args, object* kwargs){
    long len= CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
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
    
    size_t idx=s.rfind(v);
    
    return new_int_fromint(idx);
}







//Unicode-specific macros and functions
//uchar means unicode character
//code point is the code for a unicode character
//uchar_length is the number of bytes a UTF-8 uchar takes up


//Get uchar code point at char** 
int decode_code_point(char** s){
    int k = **s ?__builtin_clz(~(**s << 24)) : 0;
    int mask = (1 << (8 - k)) - 1;
    int value = **s & mask;
    for (++(*s), --k; k>0 && **s; --k, ++(*s)) {
        value <<= 6;
        value += (**s & 0x3f);
    }
    return value;
}

void encode_code_point(char **s, char *end, int code) {
    char val[4];
    int lead_byte_max = 0x7F;
    int val_index = 0;
    while (code > lead_byte_max) {
        val[val_index++] = (code & 0x3F) | 0x80;
        code >>= 6;
        lead_byte_max >>= (val_index == 1 ? 2 : 1);
    }
    val[val_index++] = (code & lead_byte_max) | (~lead_byte_max << 1);
    while (val_index-- && *s < end) {
        **s = val[val_index];
        (*s)++;
    }
}

//Number of bytes UTF-8 uchar takes up
int get_uchar_length(unsigned char c){
    if (c>=0 && c<=127){
        return 1;
    }
    else if ((c & 0xe0) == 0xc0){
        return 2;
    }
    else if ((c & 0xf0) == 0xe0){
        return 3;
    }
    else if ((c & 0xf8) == 0xf0){
        return 4;
    }
    return -1;
}

//Extract uchar bytes into arr, which must be 5 char long and be null-initialized.
//Startidx is the offset to read from 
void extract_uchar_bytes(char* unicode, char* arr, int startidx){
    int len=get_uchar_length(unicode[startidx]);
    if (len>=1){
        arr[0]=unicode[0+startidx];
    }    
    if (len>=2){
        arr[1]=unicode[1+startidx];
    }    
    if (len>=3){
        arr[2]=unicode[2+startidx];
    }    
    if (len==4){
        arr[3]=unicode[3+startidx];
    }
}

//Extract uchar bytes into arr, which must be 5 char long and be null-initialized.
//Startidx is the offset to read from 
void extract_uchar_bytes(const char* unicode, char* arr, int startidx){
    int len=get_uchar_length(unicode[startidx]);
    if (len>=1){
        arr[0]=unicode[0+startidx];
    }    
    if (len>=2){
        arr[1]=unicode[1+startidx];
    }    
    if (len>=3){
        arr[2]=unicode[2+startidx];
    }    
    if (len==4){
        arr[3]=unicode[3+startidx];
    }
}

//Get unicode code point at index, no error checking performed.
//self is str and idx_ is int
int string_get_uval(object* self, object* idx_){
    const char* c=CAST_STRING(self)->val->c_str();
    int idx=CAST_INT(idx_)->val->to_int();
    char bytes[]={0, 0, 0, 0, 0};
    extract_uchar_bytes(c, bytes, idx);
    char* ptr=&bytes[0];
    return decode_code_point(&ptr);
}

//Get unicode code point at index, no error checking performed.
//c is str and idx is int
int string_get_uval(char* c, int idx){
    char bytes[]={0, 0, 0, 0, 0};
    extract_uchar_bytes(c, bytes, idx);
    char* ptr=&bytes[0];
    return decode_code_point(&ptr);
}

//Get unicode code point at index, no error checking performed.
//c is str and idx is int
int string_get_uval(const char* c, int idx){
    char bytes[]={0, 0, 0, 0, 0};
    extract_uchar_bytes(c, bytes, idx);
    char* ptr=&bytes[0];
    return decode_code_point(&ptr);
}
//Get unicode length of a string
inline int STRING_LENGTH(object* str){
    const char* arr=CAST_STRING(str)->val->c_str();
    int q,i,ix;
    for (q=0, i=0, ix=CAST_STRING(str)->val->size(); i<ix; q++){
        unsigned char c=(unsigned char) arr[i];
        i+=get_uchar_length(c);
    }
    return q;
}

//Get unicode length of a string
inline int STRING_LENGTH(char* arr, int raw_len){
    int q,i,ix;
    for (q=0, i=0, ix=raw_len; i<ix; q++){
        unsigned char c=(unsigned char) arr[i];
        i+=get_uchar_length(c);
    }
    return q;
}

//Get real index of target index
//Return >=0 if found, -1 if not
int get_index_fromtarget(int target, int raw_len, char* arr){
    int q,i,ix;
    for (q=0, i=0, ix=raw_len; i<ix; q++){
        unsigned char c=(unsigned char) arr[i];
        i+=get_uchar_length(c);
        if (q==target){
            return i;
        }
    }

    return -1;
}

//Get real index of target index
//Return >=0 if found, -1 if not
int get_index_fromtarget(int target, int raw_len, const char* arr){
    int q,i,ix;
    for (q=0, i=0, ix=raw_len; i<ix; q++){
        unsigned char c=(unsigned char) arr[i];
        i+=get_uchar_length(c);
        if (q==target){
            return i;
        }
    }

    return -1;
}

//Get real index of target index
//Return >=0 if found, -1 if not
int get_index_fromtarget_min(int target, int raw_len, char* arr){
    int q,i,ix;
    for (q=0, i=0, ix=raw_len; i<ix; q++){
        if (q==target){
            return i;
        }
        unsigned char c=(unsigned char) arr[i];
        i+=get_uchar_length(c);
    }

    return -1;
}

//Get real index of target index
//Return >=0 if found, -1 if not
int get_index_fromtarget_min(int target, int raw_len, const char* arr){
    int q,i,ix;
    for (q=0, i=0, ix=raw_len; i<ix; q++){
        if (q==target){
            return i;
        }
        unsigned char c=(unsigned char) arr[i];
        i+=get_uchar_length(c);
    }

    return -1;
}

//Convert code point to string representation
string codept_to_str(uint32_t num){    
    char c[]={0,0,0,0,0};
    char* ptr=&c[0];
    encode_code_point(&ptr, &c[4], num);
    string s(c);
    return s;
}