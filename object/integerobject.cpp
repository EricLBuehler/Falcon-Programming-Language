object* new_int_fromint(int v){
    BigInt b=BigInt(v);
    if(int_map.find(b)!=int_map.end()){
        FPLINCREF(int_map[b]);
        return int_map[b];
    }

    object* obj=new_object(&IntType);
    ((IntObject*)obj)->val=new BigInt(v);

    int_map[b]=obj;
    
    return obj;
}

string trim(string s);

object* new_int_fromstr(string v){
    string v_=trim(v);
    
    BigInt b;
    try{
        b=BigInt(v_);
    }
    catch (std::invalid_argument){
        if (::vm!=NULL){
            vm_add_err(&ValueError, vm, "Invalid literal '%s'", v.c_str());
        }
        return NULL;
    }
    if(int_map.find(b)!=int_map.end()){
        FPLINCREF(int_map[b]);
        return int_map[b];
    }

    object* obj=new_object(&IntType);
    try{
        CAST_INT(obj)->val=new BigInt(v_);
    }
    catch (std::invalid_argument){
        if (::vm!=NULL){
            vm_add_err(&ValueError, vm, "Invalid literal '%s'", v.c_str());
        }
        return NULL;
    }

    int_map[b]=obj;

    return obj;
}

BigInt* new_int_frombase(string s, int base){
    BigInt i=0;
    int pwr=0;
    for (int idx=s.size()-1; idx>=0; idx--){
        int v;
        if (isdigit(s[idx])){
            v=stoi(string(1,s[idx]));
        }
        else if (isalpha(s[idx])){
            v=(int)'a'-tolower(s[idx]);
        }
        else{
            vm_add_err(&ValueError, vm, "Invalid literal for base %d: '%c'", base, s[idx]);
            return NULL;
        }

        if (v>=base){
            vm_add_err(&ValueError, vm, "Invalid literal for base %d: '%c'", base, s[idx]);
            return NULL;
        }

        i+=(pow(base,pwr++))*v;
    }
    return new BigInt(i);
}

//Return '\0' if normal, otherwise the character on which the error occured
char int_base_check(string s, int base){
    for (int idx=s.size()-1; idx>=0; idx--){
        int v;
        if (isdigit(s[idx])){
            v=stoi(string(1,s[idx]));
        }
        else if (isalpha(s[idx])){
            v=(int)'a'-tolower(s[idx]);
        }
        else{
            return s[idx];
        }

        if (v>=base){
            return s[idx];
        }
    }
    return '\0';
}

object* new_int_frombin(string v_){
    string v=trim(v_);
    
    BigInt* b=new_int_frombase(v, 2);
    if (b==NULL){
        return NULL;
    }

    return new_int_frombigint(b);
}

object* new_int_fromoctal(string v_){
    string v=trim(v_);
    
    BigInt* b=new_int_frombase(v, 8);
    if (b==NULL){
        return NULL;
    }

    return new_int_frombigint(b);
}

object* new_int_fromhex(string v_){
    string v=trim(v_);
    
    BigInt* b=new_int_frombase(v, 16);
    if (b==NULL){
        return NULL;
    }

    return new_int_frombigint(b);
}



object* new_int_fromstr(string* v){
    string v_=trim(*v);
    
    BigInt b;
    try{
        b=BigInt(v_);
    }
    catch (std::invalid_argument){
        if (::vm!=NULL){
            vm_add_err(&ValueError, vm, "Invalid literal '%s'", v->c_str());
        }
        return NULL;
    }
    if(int_map.find(b)!=int_map.end()){
        FPLINCREF(int_map[b]);
        return int_map[b];
    }

    object* obj=new_object(&IntType);
    try{
        CAST_INT(obj)->val=new BigInt(v_);
    }
    catch (std::invalid_argument){
        if (::vm!=NULL){
            vm_add_err(&ValueError, vm, "Invalid literal '%s'", v->c_str());
        }
        return NULL;
    }

    int_map[b]=obj;

    return obj;
}

object* new_int_frombigint(BigInt* v){
    if(int_map.find(*v)!=int_map.end()){
        FPLINCREF(int_map[*v]);
        return int_map[*v];
    }
    

    object* obj=new_object(&IntType);
    ((IntObject*)obj)->val=v;

    int_map[*v]=obj;
    
    return obj;
}

object* int_int(object* self){
    FPLINCREF(self);
    return self;
}

object* int_float(object* self){
    return new_float_fromdouble(stod(CAST_INT(self)->val->to_string()));
}

object* int_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size+CAST_DICT(kwargs)->val->size();
    if (len==0){
        if(int_map.find(BigInt(0))!=int_map.end()){
            FPLINCREF(int_map[BigInt(0)]);
            return int_map[BigInt(0)];
        }

        object* obj=new_object(CAST_TYPE(type));
        ((IntObject*)obj)->val=new BigInt(0);

        int_map[BigInt(0)] = obj;

        return obj;
    }
    if ((len!=1 && len!=2) || CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }

    int base=10;

    if (len==2){     
        object* val=list_index_int(args, 1);
        
        object* obj=object_int(val);
        if (obj==NULL && !object_istype(val->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", val->type->name->c_str());
            return NULL;
        }

        if (!object_istype(list_index_int(args, 0)->type, &StrType)){
            vm_add_err(&TypeError, vm, "Cannot convert non-string with explicit base");
            return NULL;
        }

        if (*CAST_INT(obj)->val>36 && *CAST_INT(obj)->val<2 && *CAST_INT(obj)->val!=0){
            vm_add_err(&ValueError, vm, "Base must be >=2 and <=36, or 0");
            return NULL;            
        }

        base=CAST_INT(obj)->val->to_int();
        FPLDECREF(obj);
        if (base==0){
            return object_int(list_index_int(args, 0));
        }

        BigInt* b=new_int_frombase(object_cstr(list_index_int(args, 0)), base);
        if (b==NULL){
            return NULL;
        }

        return new_int_frombigint(b);
    }

    object* val=list_index_int(args, 0);
    
    if (object_istype(val->type, CAST_TYPE(type))){
        FPLINCREF(val);
        return val;
    }
    
    object* obj=object_int(val);
    if (obj==NULL && !object_istype(val->type, &StrType)){
        vm_add_err(&ValueError, vm, "Int argument must be str or a number, not '%s'",val->type->name->c_str());
        return NULL;
    }
    else if(obj==NULL || !object_istype(obj->type, &IntType)){
        vm_add_err(&ValueError, vm, "Could not convert string '%s' to int",object_cstr(val).c_str());
        return NULL;
    }
    
    if(int_map.find(*CAST_INT(obj)->val)!=int_map.end()){
        FPLINCREF(int_map[*CAST_INT(obj)->val]);
        FPLDECREF(obj);
        return int_map[*CAST_INT(obj)->val];
    }

    int_map[*CAST_INT(obj)->val]=obj;

    return obj;
}

object* int_pow(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        BigInt out=1;
        if (otherv>0){
            for (BigInt i=0; i<otherv; i++){
                out*=selfv;
            }
            return new_int_frombigint(new BigInt(out));
        }
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    double res=pow(selfv,otherval);
    FPLDECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_mod(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        return new_int_frombigint(new BigInt(selfv%otherv));
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    double res=fmod(selfv,otherval);
    FPLDECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_add(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        return new_int_frombigint(new BigInt(selfv+otherv));
    }
    
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv+otherval;
    FPLDECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_sub(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        return new_int_frombigint(new BigInt(selfv-otherv));
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv-otherval;
    FPLDECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_mul(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        return new_int_frombigint(new BigInt(selfv*otherv));
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    double res=selfv*otherval;
    FPLDECREF(otherv);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_div(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        if (otherv==0){
            vm_add_err(&ZeroDivisionError, vm, "Divison by zero");
            return NULL;
        }
        if (selfv%otherv==0){
            return new_int_frombigint(new BigInt(selfv/otherv));
        }
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    FPLDECREF(otherv);
    if (otherval==0){
        vm_add_err(&ZeroDivisionError, vm, "Divison by zero");
        return NULL;
    }
    double res=selfv/otherval;
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

object* int_fldiv(object* self, object* other){
    if (object_istype(other->type, &IntType)){
        BigInt selfv =*CAST_INT(self)->val;
        BigInt otherv=*CAST_INT(other)->val;
        if (selfv%otherv==0){
            return new_int_frombigint(new BigInt(selfv/otherv));
        }
    }
    object* otherv=object_float(other);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    object* selfv_=object_float(self);
    double selfv =CAST_FLOAT(selfv_)->val;
    FPLDECREF(selfv_);
    double otherval=CAST_FLOAT(otherv)->val;
    FPLDECREF(otherv);
    if (otherval==0){
        vm_add_err(&ZeroDivisionError, vm, "Divison by zero");
        return NULL;
    }
    double res=floor(selfv/otherval);
    int ires=(int)res;
    if (res-ires==0){
        return new_int_fromint(ires);
    }
    return new_float_fromdouble(res);
}

const int len_uint=to_string(UINT_MAX).size();

vector<long long> convert_str_to_intarr(string num){
    vector<long long> arr;
    arr.clear();

    string current_chunk="";
    for (long long i=num.size()-1; i>=0; i--){
        current_chunk+=num[i];
        if (current_chunk.size()==len_uint){
            string new_str="";
            for (int i = current_chunk.size() - 1; i >= 0; i--){
                new_str+=current_chunk[i];
            }            
            arr.push_back(stoll(new_str));
            current_chunk="";
        }
    }

    if (current_chunk.size()>0){
        string new_str="";
        for (int i = current_chunk.size() - 1; i >= 0; i--){
            new_str+=current_chunk[i];
        }            
        arr.push_back(stoll(new_str));
    }

    return arr;
}

object* int_and(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &IntType)){
        return NULL;
    }
    if (*CAST_INT(self)->val<LLONG_MAX && *CAST_INT(other)->val<LLONG_MAX){
        object* res=new_int_fromint(CAST_INT(self)->val->to_long_long() & CAST_INT(otherv)->val->to_long_long());
        FPLDECREF(otherv);
        return res;        
    }
    string c="";
    vector<long long> a=convert_str_to_intarr(CAST_INT(self)->val->to_string());
    vector<long long> b=convert_str_to_intarr(CAST_INT(other)->val->to_string());
    if (a.size()<=b.size()){ //a is limitation
        for (int i=0; i<a.size(); i++){
            c+=to_string(a[i]&b[i]);
        }
    }
    else{ //a.size()>b.size(), so b is limitation
        for (int i=0; i<b.size(); i++){
            c+=to_string(a[i]&b[i]);
        }
    }
    
    FPLDECREF(otherv);
    return new_int_fromstr(c);
}

object* int_or(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &IntType)){
        return NULL;
    }
    if (*CAST_INT(self)->val<LLONG_MAX && *CAST_INT(other)->val<LLONG_MAX){
        object* res=new_int_fromint(CAST_INT(self)->val->to_long_long() | CAST_INT(otherv)->val->to_long_long());
        FPLDECREF(otherv);
        return res;        
    }
    string c="";
    vector<long long> a=convert_str_to_intarr(CAST_INT(self)->val->to_string());
    vector<long long> b=convert_str_to_intarr(CAST_INT(other)->val->to_string());
    if (a.size()<=b.size()){ //a is limitation
        for (int i=0; i<a.size(); i++){
            c+=to_string(a[i]|b[i]);
        }
    }
    else{ //a.size()>b.size(), so b is limitation
        for (int i=0; i<b.size(); i++){
            c+=to_string(a[i]|b[i]);
        }
    }
    
    FPLDECREF(otherv);
    return new_int_fromstr(c);
}

object* int_xor(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &IntType)){
        return NULL;
    }
    if (*CAST_INT(self)->val<LLONG_MAX && *CAST_INT(other)->val<LLONG_MAX){
        object* res=new_int_fromint(CAST_INT(self)->val->to_long_long() ^ CAST_INT(otherv)->val->to_long_long());
        FPLDECREF(otherv);
        return res;        
    }
    string c="";
    vector<long long> a=convert_str_to_intarr(CAST_INT(self)->val->to_string());
    vector<long long> b=convert_str_to_intarr(CAST_INT(other)->val->to_string());
    if (a.size()<=b.size()){ //a is limitation
        for (int i=0; i<a.size(); i++){
            c+=to_string(a[i]^b[i]);
        }
    }
    else{ //a.size()>b.size(), so b is limitation
        for (int i=0; i<b.size(); i++){
            c+=to_string(a[i]^b[i]);
        }
    }
    
    FPLDECREF(otherv);
    return new_int_fromstr(c);
}

object* int_lshift(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &IntType)){
        return NULL;
    }
    
    if (*CAST_INT(otherv)->val<0){
        vm_add_err(&ValueError, vm, "Cannot left shift by negative number of bits");
        FPLDECREF(otherv);  
        return NULL;
    } 
    if (*CAST_INT(otherv)->val==0){
        FPLINCREF(self);
        FPLDECREF(otherv);  
        return self;
    }

    if (*CAST_INT(otherv)->val>LLONG_MAX){
        vm_add_err(&ValueError, vm, "Cannot left shift by value greater than %d", LLONG_MAX);
        FPLDECREF(otherv);  
        return NULL;
    }

    object* o=new_int_frombigint(new BigInt( (*CAST_INT(self)->val)*pow(2, CAST_INT(otherv)->val->to_long_long())));
    FPLDECREF(otherv); 
    return o;
}

object* int_rshift(object* self, object* other){
    if (!object_issubclass(other, &IntType) && !object_issubclass(other, &BoolType)){
        return NULL;
    }
    object* otherv=object_int(other);
    if (otherv==NULL || !object_istype(otherv->type, &IntType)){
        return NULL;
    }
    
    if (*CAST_INT(otherv)->val<0){
        vm_add_err(&ValueError, vm, "Cannot right shift by negative number of bits");
        FPLDECREF(otherv);  
        return NULL;
    } 
    if (*CAST_INT(otherv)->val==0){
        FPLINCREF(self);
        FPLDECREF(otherv);  
        return self;
    }
    if (*CAST_INT(otherv)->val>LLONG_MAX){
        vm_add_err(&ValueError, vm, "Cannot right shift by value greater than %d", LLONG_MAX);
        FPLDECREF(otherv);  
        return NULL;
    }

    object* o=new_int_frombigint(new BigInt( (*CAST_INT(self)->val)/pow(2, CAST_INT(otherv)->val->to_long_long())));
    FPLDECREF(otherv); 
    return o;
}

object* int_abs(object* self){
    if (*CAST_INT(self)->val<0){
        return new_int_frombigint(new BigInt(*CAST_INT(self)->val*-1));
    }
    FPLINCREF(self);
    return self;
}

object* int_neg(object* self){
    return new_int_frombigint(new BigInt((*CAST_INT(self)->val)*-1));
}

object* int_not(object* self){
    return new_int_frombigint(new BigInt( ((*CAST_INT(self)->val)+1)*-1 ));
}

object* int_repr(object* self){
    return str_new_fromstr(CAST_INT(self)->val->to_string());
}

object* int_cmp(object* self, object* other, uint8_t type){
    object* otherv=object_float(other);
    object* selfv=object_float(self);
    if (otherv==NULL || !object_istype(otherv->type, &FloatType)){
        return NULL;
    }
    
    //Other type is int
    if (type==CMP_EQ){
        if (CAST_FLOAT(selfv)->val==CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }
    else if (type==CMP_NE){
        if (CAST_FLOAT(selfv)->val!=CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }
    else if (type==CMP_GT){
        if (CAST_FLOAT(selfv)->val>CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }
    else if (type==CMP_GTE){
        if (CAST_FLOAT(selfv)->val>=CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }
    else if (type==CMP_LT){
        if (CAST_FLOAT(selfv)->val<CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }
    else if (type==CMP_LTE){
        if (CAST_FLOAT(selfv)->val<=CAST_FLOAT(otherv)->val){
            FPLDECREF(otherv);
            FPLDECREF(selfv);
            return new_bool_true();
        }
        FPLDECREF(otherv);
        FPLDECREF(selfv);
        return new_bool_false();
    }

    return NULL;
}

object* int_bool(object* self){
    if ((*CAST_INT(self)->val)>0){
        return new_bool_true();
    }
    return new_bool_false();
}

void int_del(object* obj){
    int_map.erase(*((IntObject*)obj)->val);
    delete ((IntObject*)obj)->val;
}