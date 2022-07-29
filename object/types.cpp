object* int_new(object* type, object* args, object* kwargs);
void int_del(object* self);
object* int_add(object* self, object* other);
object* int_sub(object* self, object* other);
object* int_mul(object* self, object* other);
object* int_div(object* self, object* other);
object* int_neg(object* self);
object* int_repr(object* self);
object* int_cmp(object* self, object* other, uint8_t type);
object* int_bool(object* self);

object* new_int_fromint(int v);
object* new_int_fromstr(string* v);
object* new_int_frombigint(BigInt* v);

typedef struct IntObject{
    OBJHEAD_EXTRA
    BigInt* val;
}IntObject;

static NumberMethods int_num_methods{
    (binopfunc)int_add, //slot_add
    (binopfunc)int_sub, //slot_sub
    (binopfunc)int_mul, //slot_mul
    (binopfunc)int_div, //slot_div

    (unaryfunc)int_neg, //slot_neg

    (unaryfunc)int_bool, //slot_bool
};

TypeObject IntType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("int"), //name
    sizeof(IntObject)+sizeof(BigInt), //size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)int_new, //slot_new
    (delfunc)int_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append
    0, //slot_subscr

    (reprfunc)int_repr, //slot_repr
    (reprfunc)int_repr, //slot_str
    0, //slot_call

    &int_num_methods, //slot_number

    (compfunc)int_cmp, //slot_cmp
};

void setup_int_type(){
    IntType=(*(TypeObject*)finalize_type(&IntType));
}



object* str_new(object* type, object* args, object* kwargs);
void str_del(object* self);
object* str_len(object* self);
object* str_repr(object* self);
object* str_str(object* self);
object* str_cmp(object* self, object* other, uint8_t type);
object* str_bool(object* self);

object* str_new_fromstr(string* val);

typedef struct StrObject{
    OBJHEAD_VAR
    string* val;
}StrObject;

static NumberMethods str_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)str_bool, //slot_bool
};

TypeObject StrType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("str"), //name
    0, //size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)str_new, //slot_new
    (delfunc)str_del, //slot_del

    0, //slot_next
    0, //slot_get
    (lenfunc)str_len, //slot_len
    0, //slot_set
    0, //slot_append
    0, //slot_subscr

    (reprfunc)str_repr, //slot_repr
    (reprfunc)str_str, //slot_str
    0, //slot_call

    &str_num_methods, //slot_number

    (compfunc)str_cmp, //slot_cmp
};

void setup_str_type(){
    StrType=(*(TypeObject*)finalize_type(&StrType));
}




object* list_init(object* self, object* args, object* kwargs);
void list_del(object* self);
object* list_len(object* self);
object* list_get(object* self, object* idx);
void list_append(object* self, object* obj);
object* list_new(object* type, object* args, object* kwargs);
void list_set(object* self, object* idx, object* val);
object* list_repr(object* self);
object* list_next(object* self);
object* list_cmp(object* self, object* other, uint8_t type);
object* list_bool(object* self);
object* list_subscr(object* self, object* other);


typedef struct ListObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
    size_t idx;
}ListObject;

static NumberMethods list_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)list_bool, //slot_bool
};

TypeObject ListType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("list"), //name
    0, //size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)list_new, //slot_new
    (delfunc)list_del, //slot_del

    (iternextfunc)list_next, //slot_next
    (getfunc)list_get, //slot_get
    (lenfunc)list_len, //slot_len
    (setfunc)list_set, //slot_set
    (appendfunc)list_append, //slot_append
    (binopfunc)list_subscr, //slot_subscr

    (reprfunc)list_repr, //slot_repr
    (reprfunc)list_repr, //slot_str
    0, //slot_call

    &list_num_methods, //slot_number

    (compfunc)list_cmp, //slot_cmp
};

void setup_list_type(){
    ListType=(*(TypeObject*)finalize_type(&ListType));
}



object* dict_new(object* type, object* args, object* kwargs);
void dict_del(object* self);
object* dict_len(object* self);
object* dict_get(object* self, object* idx);
void dict_set(object* self, object* key, object* val);
object* dict_repr(object* self);
object* dict_cmp(object* self, object* other, uint8_t type);
object* dict_bool(object* self);
object* dict_subscr(object* self, object* other);


typedef struct DictObject{
    OBJHEAD_VAR
    map<object*, object*>* val;
}DictObject;

static NumberMethods dict_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)dict_bool, //slot_bool
};

static TypeObject DictType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("dict"), //name
    0, //size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)dict_new, //slot_new
    (delfunc)dict_del, //slot_del

    0, //slot_next
    (getfunc)dict_get, //slot_get
    (lenfunc)dict_len, //slot_len
    (setfunc)dict_set, //slot_set
    0, //slot_appends
    (binopfunc)dict_subscr, //slot_subscr

    (reprfunc)dict_repr, //slot_repr
    (reprfunc)dict_repr, //slot_str
    0, //slot_call

    &dict_num_methods, //slot_number

    (compfunc)dict_cmp, //slot_cmp
};

void setup_dict_type(){
    DictType=(*(TypeObject*)finalize_type(&DictType));
}



object* code_init(object* self, object* args, object* kwargs);
void code_del(object* self);
object* code_new(object* type, object* args, object* kwargs);
object* code_repr(object* self);
object* code_cmp(object* self, object* other, uint8_t type);
object* code_bool(object* self);

typedef struct CodeObject{
    OBJHEAD_EXTRA
    object* co_names;
    object* co_consts;
    object* co_code;
    object* co_file;
    object* co_lines;
}CodeObject;

static NumberMethods code_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)code_bool, //slot_bool
};

TypeObject CodeType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("code"), //name
    sizeof(CodeObject), //size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    (initfunc)code_init, //slot_init
    (newfunc)code_new, //slot_new
    (delfunc)code_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append
    0, //slot_subscr

    (reprfunc)code_repr, //slot_repr
    (reprfunc)code_repr, //slot_str
    0, //slot_call

    &code_num_methods, //slot_number

    (compfunc)code_cmp, //slot_cmp
};

void setup_code_type(){
    CodeType=(*(TypeObject*)finalize_type(&CodeType));
}



object* bool_new(object* type, object* args, object* kwargs);
void bool_del(object* self);
object* bool_add(object* self, object* other);
object* bool_sub(object* self, object* other);
object* bool_mul(object* self, object* other);
object* bool_div(object* self, object* other);
object* bool_neg(object* self);
object* bool_repr(object* self);
object* bool_cmp(object* self, object* other, uint8_t type);
object* bool_bool(object* self);

object* new_bool_true();
object* new_bool_false();


typedef struct BoolObject{
    OBJHEAD_EXTRA
    uint8_t val;
}BoolObject;

bool istrue(object* boolean){
    if (CAST_BOOL(boolean)->val==1){
        return true;
    }
    return false;
}

static NumberMethods bool_num_methods{
    (binopfunc)bool_add, //slot_add
    (binopfunc)bool_sub, //slot_sub
    (binopfunc)bool_mul, //slot_mul
    (binopfunc)bool_div, //slot_div

    (unaryfunc)bool_neg, //slot_neg

    (unaryfunc)bool_bool, //slot_bool
};

TypeObject BoolType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("bool"), //name
    sizeof(BoolObject), //size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)bool_new, //slot_new
    (delfunc)bool_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append
    0, //slot_subscr

    (reprfunc)bool_repr, //slot_repr
    (reprfunc)bool_repr, //slot_str
    0, //slot_call

    &bool_num_methods, //slot_number

    (compfunc)bool_cmp, //slot_cmp
};

void setup_bool_type(){
    BoolType=(*(TypeObject*)finalize_type(&BoolType));
}


object* tuple_init(object* self, object* args, object* kwargs);
void tuple_del(object* self);
object* tuple_len(object* self);
object* tuple_get(object* self, object* idx);
void tuple_append(object* self, object* obj);
object* tuple_new(object* type, object* args, object* kwargs);
object* tuple_repr(object* self);
object* tuple_next(object* self);
object* tuple_cmp(object* self, object* other, uint8_t type);
object* tuple_bool(object* self);
object* tuple_subscr(object* self, object* other);

typedef struct TupleObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
    size_t idx;
}TupleObject;

static NumberMethods tuple_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)tuple_bool, //slot_bool
};

TypeObject TupleType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("tuple"), //name
    0, //size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)tuple_new, //slot_new
    (delfunc)list_del, //slot_del

    (iternextfunc)tuple_next, //slot_next
    (getfunc)tuple_get, //slot_get
    (lenfunc)tuple_len, //slot_len
    0, //slot_set
    (appendfunc)tuple_append, //slot_append
    (binopfunc)tuple_subscr, //slot_append

    (reprfunc)tuple_repr, //slot_repr
    (reprfunc)tuple_repr, //slot_str
    0, //slot_call

    &tuple_num_methods, //slot_number

    (compfunc)tuple_cmp, //slot_cmp
};

void setup_tuple_type(){
    TupleType=(*(TypeObject*)finalize_type(&TupleType));
}


void func_del(object* self);
object* func_new(object* type, object* args, object* kwargs);
object* func_repr(object* self);
object* func_cmp(object* self, object* other, uint8_t type);
object* func_call(object* self, object* args, object* callfunc);
object* func_bool(object* self);


typedef struct FuncObject{
    OBJHEAD_EXTRA
    object* code;
    object* dict;
    object* args; //Tuple
    object* kwargs; //Tuple
    uint32_t argc;
    object* name;
}FuncObject;

static NumberMethods func_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)func_bool, //slot_bool
};

TypeObject FuncType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("function"), //name
    sizeof(FuncObject), //size
    true, //gc_trackable
    NULL, //bases
    offsetof(FuncObject, dict), //dict_offset
    NULL, //dict
    (getattrfunc)object_genericgetattr, //slot_getattr
    (setattrfunc)object_genericsetattr, //slot_setattr

    0, //slot_init
    (newfunc)func_new, //slot_new
    (delfunc)func_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append
    0, //slot_subscr

    (reprfunc)func_repr, //slot_repr
    (reprfunc)func_repr, //slot_str
    (callfunc)func_call, //slot_call

    &func_num_methods, //slot_number

    (compfunc)func_cmp, //slot_cmp
};

void setup_func_type(){
    FuncType=(*(TypeObject*)finalize_type(&FuncType));
}


object* none_new( object* args, object* kwargs);
void none_del(object* self);
object* none_repr(object* self);
object* none_cmp(object* self, object* other, uint8_t type);
object* none_bool(object* self);


typedef struct NoneObject{
    OBJHEAD_EXTRA
}NoneObject;

static NumberMethods none_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)none_bool, //slot_bool
};

TypeObject NoneType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("NoneType"), //name
    sizeof(NoneObject), //size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)none_new, //slot_new
    (delfunc)none_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append
    0, //slot_subscr

    (reprfunc)none_repr, //slot_repr
    (reprfunc)none_repr, //slot_str
    0, //slot_call

    &none_num_methods, //slot_number

    (compfunc)none_cmp, //slot_cmp
};

void setup_none_type(){
    NoneType=(*(TypeObject*)finalize_type(&NoneType));
}


void builtin_del(object* self);
object* builtin_repr(object* self);
object* builtin_cmp(object* self, object* other, uint8_t type);
object* builtin_call(object* self, object* args, object* kwargs);
object* builtin_bool(object* self);


typedef struct BuiltinObject{
    OBJHEAD_EXTRA
    object* (*function)(object*, object*);
    object* name;
    object* args; //Tuple
    object* kwargs; //Tuple
    uint32_t argc;
}BuiltinObject;

static NumberMethods builtin_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)builtin_bool, //slot_bool
};

TypeObject BuiltinType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("builtin_function_or_method"), //name
    sizeof(BuiltinObject), //size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset    
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    0, //slot_new
    (delfunc)builtin_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append
    0, //slot_subscr

    (reprfunc)builtin_repr, //slot_repr
    (reprfunc)builtin_repr, //slot_str
    (callfunc)builtin_call, //slot_call

    &builtin_num_methods, //slot_number

    (compfunc)builtin_cmp, //slot_cmp
};

void setup_builtin_type(){
    BuiltinType=(*(TypeObject*)finalize_type(&BuiltinType));
}


object* object_new(object* type, object* args, object* kwargs);
object* object_repr_(object* self);
object* object_init(object* self, object* args, object* kwargs);
object* object_cmp_(object* self, object* other, uint8_t type);
object* object_bool(object* self);


typedef struct ObjectObject{
    OBJHEAD_EXTRA
}ObjectObject;

static NumberMethods object_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)object_bool, //slot_bool
};

TypeObject ObjectType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("object"), //name
    sizeof(ObjectObject), //size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr
    (initfunc)object_init, //slot_init
    (newfunc)object_new, //slot_new
    (delfunc)object_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append
    0, //slot_subscr

    (reprfunc)object_repr_, //slot_repr
    (reprfunc)object_repr_, //slot_str
    0, //slot_call

    &object_num_methods, //slot_number

    (compfunc)object_cmp_, //slot_cmp
};

void setup_object_type(){
    ObjectType=(*(TypeObject*)finalize_type(&ObjectType));
}


object* exception_new(object* type, object* args, object* kwargs);
void exception_del(object* self);
object* exception_repr(object* self);
object* exception_cmp(object* self, object* other, uint8_t type);
object* exception_call(object* type, object* args, object* kwargs);

typedef struct ExceptionObject{
    OBJHEAD_EXTRA
    vector<string*>* headers;
    vector<string*>* snippets;
    string* err;
}ExceptionObject;

static NumberMethods exception_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    0, //slot_bool
};

TypeObject ExceptionType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("Exception"), //name
    sizeof(ExceptionObject), //size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)exception_new, //slot_new
    (delfunc)exception_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append
    0, //slot_subscr

    (reprfunc)exception_repr, //slot_repr
    (reprfunc)exception_repr, //slot_str
    0, //slot_call

    &exception_num_methods, //slot_number

    (compfunc)exception_cmp, //slot_cmp
};

object* new_type_exception(string* name, object* bases, object* dict){
    bases->type->slot_append(bases, (object*)&ExceptionType);
    TypeObject newtype={
        0, //refcnt
        0, //ob_prev
        0, //ob_next
        0, //gen
        &TypeType, //type
        name, //name
        sizeof(ExceptionObject), //size
        true, //gc_trackable
        bases, //bases
        0, //dict_offset
        dict, //dict
        0, //slot_getattr
        0, //slot_setattr

        0, //slot_init
        0, //slot_new
        0, //slot_del

        0, //slot_next
        0, //slot_get
        0, //slot_len
        0, //slot_set
        0, //slot_append
        0, //slot_subscr

        0, //slot_repr
        0, //slot_str
        0, //slot_call

        0, //slot_number

        0, //slot_cmp
    };
        
    return finalize_type(&newtype);
}

void setup_exception_type(){
    ExceptionType=(*(TypeObject*)finalize_type(&ExceptionType));
    TypeError=new_type_exception(new string("TypeError"), new_tuple(), new_dict());
    ValueError=new_type_exception(new string("ValueError"), new_tuple(), new_dict());
    AttributeError=new_type_exception(new string("AttributeError"), new_tuple(), new_dict());
    IndexError=new_type_exception(new string("IndexError"), new_tuple(), new_dict());
    KeyError=new_type_exception(new string("KeyError"), new_tuple(), new_dict());
    NameError=new_type_exception(new string("NameError"), new_tuple(), new_dict());
}




object* new_type(string* name, object* bases, object* dict);

object* type_new(object* type, object* args, object* kwargs){
    //Argument size checking
    if (CAST_INT(args->type->slot_len(args))->val->to_long()!=3){
        vm_add_err(ValueError, vm, "Expected 3 arguments, got %d",CAST_INT(args->type->slot_len(args))->val->to_long());
        return NULL;
    }
    //
    if (!object_istype(args->type->slot_get(args, new_int_fromint(0))->type, &StrType)){
        vm_add_err(ValueError, vm, "Expected first argument to be string, got type '%s'",args->type->slot_get(args, new_int_fromint(0))->type->name->c_str());
        return NULL;
    }
    if (!object_istype(args->type->slot_get(args, new_int_fromint(1))->type, &ListType) || \
    !object_istype(args->type->slot_get(args, new_int_fromint(1))->type, &TupleType)){
        vm_add_err(ValueError, vm, "Expected first argument to be list or tuple, got type '%s'",args->type->slot_get(args, new_int_fromint(0))->type->name->c_str());
        return NULL;
    }
    if (!object_istype(args->type->slot_get(args, new_int_fromint(2))->type, &DictType)){
        vm_add_err(ValueError, vm, "Expected first argument to be dict, got type '%s'",args->type->slot_get(args, new_int_fromint(0))->type->name->c_str());
        return NULL;
    }
    //
    string* name=CAST_STRING(args->type->slot_get(args, new_int_fromint(0)))->val;
    object* bases=args->type->slot_get(args, new_int_fromint(1));
    object* dict=args->type->slot_get(args, new_int_fromint(2));
    return new_type(name, bases, dict);
}

void type_del(object* self){}

object* type_repr(object* self){
    string s="<class '"+(*CAST_TYPE(self)->name)+"'>";
    return str_new_fromstr(new string(s));
}

object* type_call(object* self, object* args, object* kwargs){
    //Special case
    if (object_istype(CAST_TYPE(self), &TypeType)){
        if (CAST_INT(args->type->slot_len(args))->val->to_long()==1){
            return (object*)(args->type->slot_get(args, new_int_fromint(0))->type);
        }
    }
    object* o=CAST_TYPE(self)->slot_new(self, args, kwargs);
    if (o != NULL && o->type->slot_init!=NULL){
        o->type->slot_init(o, args, kwargs);
    }
    return o;
}

object* type_get(object* self, object* attr){
    //ESSENTIALLY the same as object_genericgetattr, but this checks metatype

    // metatype should alwaye be TypeType.? right??
    
    //Check us and then our bases

    //Check instance dict
    if ( self->type->dict_offset!=0){
        object* dict= (*(object**)((char*)self + self->type->dict_offset));
        if (object_find_bool_dict_keys(dict, attr)){
            return dict->type->slot_get(dict, attr);
        }
    }
    //Check type dict
    if (self->type->dict!=0){
        object* dict = self->type->dict;
        if (object_find_bool_dict_keys(dict, attr)){
            return dict->type->slot_get(dict, attr);
        }
    }

    uint32_t total_bases = CAST_INT(self->type->bases->type->slot_len(self->type->bases))->val->to_long_long();
    for (uint32_t i=total_bases; i>0; i--){
        TypeObject* base_tp=CAST_TYPE(self->type->bases->type->slot_get(self->type->bases, new_int_fromint(i-1)));

        //Check type dict
        if (base_tp->dict!=0){
            object* dict = base_tp->dict;
            if (object_find_bool_dict_keys(dict, attr)){
                return dict->type->slot_get(dict, attr);
            }
        }
    }


    vm_add_err(AttributeError, vm, "%s has no attribute '%s'",self->type->name->c_str(), object_cstr(attr).c_str());
    return NULL;
}

void type_set(object* obj, object* attr, object* val){
    //Check dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        dict->type->slot_set(dict, attr, val);
        return;
    }
    vm_add_err(AttributeError, vm, "%s is read only",obj->type->name->c_str());
    return;
}

void _inherit_number_slots(NumberMethods* m, TypeObject* base_tp){
    if (base_tp->slot_number==NULL){
        return;
    }
    
    m->slot_add=base_tp->slot_number->slot_add;
    m->slot_sub=base_tp->slot_number->slot_sub;
    m->slot_mul=base_tp->slot_number->slot_mul;
    m->slot_div=base_tp->slot_number->slot_div;

    m->slot_neg=base_tp->slot_number->slot_neg;

    m->slot_bool=base_tp->slot_number->slot_bool;
}

void _inherit_slots(TypeObject* tp_tp, TypeObject* base_tp, NumberMethods* m){
    SETSLOT(tp_tp, base_tp, slot_getattr);
    SETSLOT(tp_tp, base_tp, slot_setattr);
    SETSLOT(tp_tp, base_tp, slot_init);
    SETSLOT(tp_tp, base_tp, slot_new);
    SETSLOT(tp_tp, base_tp, slot_del);    
    SETSLOT(tp_tp, base_tp, slot_next);
    SETSLOT(tp_tp, base_tp, slot_get);
    SETSLOT(tp_tp, base_tp, slot_len);
    SETSLOT(tp_tp, base_tp, slot_set);
    SETSLOT(tp_tp, base_tp, slot_append);
    SETSLOT(tp_tp, base_tp, slot_repr);
    SETSLOT(tp_tp, base_tp, slot_str);
    SETSLOT(tp_tp, base_tp, slot_call);
    SETSLOT(tp_tp, base_tp, slot_cmp);
    SETSLOT(tp_tp, base_tp, slot_subscr);

    //Inheritance of number methods could be updated to be special
    _inherit_number_slots(m, base_tp);
}

object* finalize_type(TypeObject* newtype){
    object* tp=(object*)malloc(sizeof(TypeObject));
    memcpy(tp, newtype, sizeof(TypeObject));
        
    TypeObject* tp_tp=CAST_TYPE(tp);

    //tp is the what we'll copy to...

    //Clean out bases
    if (tp_tp->bases==NULL){
        tp_tp->bases=new_list();
    }
    tp_tp->bases->type->slot_append(tp_tp->bases, INCREF((object*)&ObjectType));
    uint32_t total_bases = CAST_INT(tp_tp->bases->type->slot_len(tp_tp->bases))->val->to_long_long();

    //This is a slower method than could theoritically be done.
    //I could just use implied list indexing (uses my internal knowledge of ListObject), but this
    //also breaks fewer rules...
    
    

    NumberMethods* m=(NumberMethods*)malloc(sizeof(NumberMethods));
    memset(m, 0, sizeof(NumberMethods));
    for (uint32_t i=total_bases; i>0; i--){
        TypeObject* base_tp=CAST_TYPE(tp_tp->bases->type->slot_get(tp_tp->bases, new_int_fromint(i-1)));
        //Dirty inheritance here... go over each
        _inherit_slots(tp_tp, base_tp, m);
        //
    }
    //Hack to ensure retention of original slots
    _inherit_slots(tp_tp, newtype, m);
    

    tp_tp->slot_number=m;

    tp_tp->refcnt=1;

    return tp;
}

object* type_bool(object* self){
    return new_bool_true();
}


#include "typeobject_newtp.cpp"