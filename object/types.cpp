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
object* int_int(object* self);
object* int_float(object* self);

object* int_wrapper_add(object* args, object* kwargs);
object* int_wrapper_sub(object* args, object* kwargs);
object* int_wrapper_mul(object* args, object* kwargs);
object* int_wrapper_div(object* args, object* kwargs);
object* int_wrapper_neg(object* args, object* kwargs);
object* int_wrapper_repr(object* args, object* kwargs);
object* int_wrapper_bool(object* args, object* kwargs);
object* int_wrapper_new(object* args, object* kwargs);
object* int_wrapper_eq(object* args, object* kwargs);
object* int_wrapper_ne(object* args, object* kwargs);
object* int_wrapper_gt(object* args, object* kwargs);
object* int_wrapper_gte(object* args, object* kwargs);
object* int_wrapper_lt(object* args, object* kwargs);
object* int_wrapper_lte(object* args, object* kwargs);

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
    (unaryfunc)int_int, //slot_int
    (unaryfunc)int_float, //slot_float
};

static Mappings int_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method int_methods[]={{NULL,NULL}};
GetSets int_getsets[]={{NULL,NULL}};

TypeObject IntType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("int"), //name
    sizeof(IntObject)+sizeof(BigInt), //size
    0, //var_base_size
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
    0, //slot_iter

    (reprfunc)int_repr, //slot_repr
    (reprfunc)int_repr, //slot_str
    0, //slot_call

    &int_num_methods, //slot_number
    &int_mappings, //slot_mapping

    int_methods, //slot_methods
    int_getsets, //slot_getsets

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
object* str_int(object* self);
object* str_float(object* self);

object* str_wrapper_new(object* args, object* kwargs);
object* str_wrapper_len(object* args, object* kwargs);
object* str_wrapper_repr(object* args, object* kwargs);
object* str_wrapper_str(object* args, object* kwargs);
object* str_wrapper_bool(object* args, object* kwargs);
object* str_wrapper_ne(object* args, object* kwargs);
object* str_wrapper_eq(object* args, object* kwargs);

object* str_new_fromstr(string val);

typedef struct StrObject{
    OBJHEAD_EXTRA
    string* val;
}StrObject;

static NumberMethods str_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)str_bool, //slot_bool
    (unaryfunc)str_int, //slot_int
    (unaryfunc)str_float, //slot_float
};

static Mappings str_mappings{
    0, //slot_get
    0, //slot_set
    str_len, //slot_len
};

Method str_methods[]={{NULL,NULL}};
GetSets str_getsets[]={{NULL,NULL}};

TypeObject StrType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("str"), //name
    sizeof(StrObject), //size
    0, //var_base_size
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
    0, //slot_iter

    (reprfunc)str_repr, //slot_repr
    (reprfunc)str_str, //slot_str
    0, //slot_call

    &str_num_methods, //slot_number
    &str_mappings, //slot_mapping

    str_methods, //slot_methods
    str_getsets, //slot_getsets

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
object* list_append_meth(object* args, object* kwargs);
object* list_iter(object* self);

typedef struct ListObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
}ListObject;

Method list_methods[]={{"append", (cwrapperfunc)list_append_meth}, {NULL,NULL}};
GetSets list_getsets[]={{NULL,NULL}};

static NumberMethods list_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)list_bool, //slot_bool
};

static Mappings list_mappings{
    list_get, //slot_get
    list_set, //slot_set
    list_len, //slot_len
    list_append, //slot_append
};

TypeObject ListType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("list"), //name
    0, //size
    sizeof(ListObject), //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    object_genericgetattr, //slot_getattr
    object_genericsetattr, //slot_setattr

    0, //slot_init
    (newfunc)list_new, //slot_new
    (delfunc)list_del, //slot_del

    0, //slot_next
    (unaryfunc)list_iter, //slot_iter

    (reprfunc)list_repr, //slot_repr
    (reprfunc)list_repr, //slot_str
    0, //slot_call

    &list_num_methods, //slot_number
    &list_mappings, //slot_mapping

    list_methods, //slot_methods
    list_getsets, //slot_getsets

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
object* dict_str(object* self);
object* dict_cmp(object* self, object* other, uint8_t type);
object* dict_bool(object* self);


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

static Mappings dict_mappings{
    dict_get, //slot_get
    dict_set, //slot_set
    dict_len, //slot_len
};

Method dict_methods[]={{NULL,NULL}};
GetSets dict_getsets[]={{NULL,NULL}};

static TypeObject DictType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("dict"), //name
    0, //size
    sizeof(DictObject), //var_base_size
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
    0, //slot_iter

    (reprfunc)dict_repr, //slot_repr
    (reprfunc)dict_str, //slot_str
    0, //slot_call

    &dict_num_methods, //slot_number
    &dict_mappings, //slot_mapping

    dict_methods, //slot_methods
    dict_getsets, //slot_getsets

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
    uint32_t co_instructions;
}CodeObject;

static NumberMethods code_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)code_bool, //slot_bool
};

static Mappings code_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method code_methods[]={{NULL,NULL}};
GetSets code_getsets[]={{NULL,NULL}};

TypeObject CodeType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("code"), //name
    sizeof(CodeObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    (initfunc)code_init, //slot_init
    (newfunc)code_new, //slot_new
    (delfunc)code_del, //slot_del

    0, //slot_next
    0, //slot_iter

    (reprfunc)code_repr, //slot_repr
    (reprfunc)code_repr, //slot_str
    0, //slot_call

    &code_num_methods, //slot_number
    &code_mappings, //slot_mapping

    code_methods, //slot_methods
    code_getsets, //slot_getsets

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
object* bool_int(object* self);
object* bool_float(object* self);

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
    (unaryfunc)bool_int, //slot_int
    (unaryfunc)bool_float, //slot_float
};

static Mappings bool_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method bool_methods[]={{NULL,NULL}};
GetSets bool_getsets[]={{NULL,NULL}};

TypeObject BoolType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("bool"), //name
    sizeof(BoolObject), //size
    0, //var_base_size
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
    0, //slot_iter
    
    (reprfunc)bool_repr, //slot_repr
    (reprfunc)bool_repr, //slot_str
    0, //slot_call

    &bool_num_methods, //slot_number
    &bool_mappings, //slot_mapping

    bool_methods, //slot_methods
    bool_getsets, //slot_getsets

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
object* tuple_iter(object* self);

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


static Mappings tuple_mappings{
    tuple_get, //slot_get
    0, //slot_set
    tuple_len, //slot_len
    list_append, //slot_append
};

Method tuple_methods[]={{NULL,NULL}};
GetSets tuple_getsets[]={{NULL,NULL}};

TypeObject TupleType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("tuple"), //name
    0, //size
    sizeof(TupleObject), //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)tuple_new, //slot_new
    (delfunc)tuple_del, //slot_del

    0, //slot_next
    (unaryfunc)tuple_iter, //slot_iter

    (reprfunc)tuple_repr, //slot_repr
    (reprfunc)tuple_repr, //slot_str
    0, //slot_call

    &tuple_num_methods, //slot_number
    &tuple_mappings, //slot_mapping

    tuple_methods, //slot_methods
    tuple_getsets, //slot_getsets

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
object* func_call_nostack(object* self, object* args, object* kwargs);


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

static Mappings func_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method func_methods[]={{NULL,NULL}};
GetSets func_getsets[]={{NULL,NULL}};

TypeObject FuncType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("function"), //name
    sizeof(FuncObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    offsetof(FuncObject, dict), //dict_offset
    NULL, //dict
    (getattrfunc)object_genericgetattr, //slot_getattr
    (setattrfunc)object_genericsetattr, //slot_setattr

    0, //slot_init
    (newfunc)func_new, //slot_new
    (delfunc)func_del, //slot_del

    0, //slot_next
    0, //slot_iter

    (reprfunc)func_repr, //slot_repr
    (reprfunc)func_repr, //slot_str
    (callfunc)func_call, //slot_call

    &func_num_methods, //slot_number
    &func_mappings, //slot_mapping

    func_methods, //slot_methods
    func_getsets, //slot_getsets

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

static Mappings none_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method none_methods[]={{NULL,NULL}};
GetSets none_getsets[]={{NULL,NULL}};

TypeObject NoneType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("NoneType"), //name
    sizeof(NoneObject), //size
    0, //var_base_size
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
    0, //slot_iter

    (reprfunc)none_repr, //slot_repr
    (reprfunc)none_repr, //slot_str
    0, //slot_call

    &none_num_methods, //slot_number
    &none_mappings, //slot_mapping

    none_methods, //slot_methods
    none_getsets, //slot_getsets

    (compfunc)none_cmp, //slot_cmp
};

void setup_none_type(){
    NoneType=(*(TypeObject*)finalize_type(&NoneType));
}


void builtin_del(object* self);
object* builtin_repr_slot(object* self);
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

static Mappings builtin_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method builtin_methods[]={{NULL,NULL}};
GetSets builtin_getsets[]={{NULL,NULL}};

TypeObject BuiltinType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("builtin"), //name
    sizeof(BuiltinObject), //size
    0, //var_base_size
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
    0, //slot_iter

    (reprfunc)builtin_repr_slot, //slot_repr
    (reprfunc)builtin_repr_slot, //slot_str
    (callfunc)builtin_call, //slot_call

    &builtin_num_methods, //slot_number
    &builtin_mappings, //slot_mapping

    builtin_methods, //slot_methods
    builtin_getsets, //slot_getsets

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

Method object_methods[]={{NULL,NULL}};
GetSets object_getsets[]={{NULL,NULL}};

static Mappings object_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

TypeObject ObjectType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("object"), //name
    sizeof(ObjectObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    object_genericgetattr, //slot_getattr
    object_genericsetattr, //slot_setattr
    (initfunc)object_init, //slot_init
    (newfunc)object_new, //slot_new
    0, //slot_del

    0, //slot_next
    0, //slot_iter

    (reprfunc)object_repr_, //slot_repr
    (reprfunc)object_repr_, //slot_str
    0, //slot_call

    &object_num_methods, //slot_number
    &object_mappings, //slot_mapping

    object_methods, //slot_methods
    object_getsets, //slot_getsets

    (compfunc)object_cmp_, //slot_cmp
};

void setup_object_type(){
    ObjectType=(*(TypeObject*)finalize_type(&ObjectType));
}


object* exception_new(object* type, object* args, object* kwargs);
void exception_del(object* self);
object* exception_repr(object* self);
object* exception_str(object* self);
object* exception_cmp(object* self, object* other, uint8_t type);

typedef struct ExceptionObject{
    OBJHEAD_EXTRA
    object* err;
}ExceptionObject;

static NumberMethods exception_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    0, //slot_bool
};
static Mappings exception_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method exception_methods[]={{NULL,NULL}};
GetSets exception_getsets[]={{NULL,NULL}};

TypeObject ExceptionType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("Exception"), //name
    sizeof(ExceptionObject), //size
    0, //var_base_size
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
    0, //slot_iter

    (reprfunc)exception_repr, //slot_repr
    (reprfunc)exception_str, //slot_str
    0, //slot_call

    &exception_num_methods, //slot_number
    &exception_mappings, //slot_mapping

    exception_methods, //slot_methods
    exception_getsets, //slot_getsets

    (compfunc)exception_cmp, //slot_cmp
};

object* new_type_exception(string* name, object* bases, object* dict){
    list_append(bases, (object*)&ExceptionType);
    TypeObject newtype={
        0, //refcnt
        0, //ob_prev
        0, //ob_next
        0, //gen
        &TypeType, //type
        name, //name
        sizeof(ExceptionObject), //size
        0, //var_base_size
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
        0, //slot_iter

        0, //slot_repr
        0, //slot_str
        0, //slot_call

        0, //slot_number
        0, //slot_mappings
        
        0, //slot_methods
        0, //slot_getsets

        0, //slot_cmp
    };
        
    return finalize_type(&newtype);
}

void setup_exception_type(){
    ExceptionType=(*(TypeObject*)finalize_type(&ExceptionType));

    TypeError=(*(TypeObject*)new_type_exception(new string("TypeError"), new_tuple(), new_dict()));
    
    ValueError=(*(TypeObject*)new_type_exception(new string("ValueError"), new_tuple(), new_dict()));
    
    AttributeError=(*(TypeObject*)new_type_exception(new string("AttributeError"), new_tuple(), new_dict()));
    
    object* indexerr_bases=new_tuple();
    indexerr_bases->type->slot_mappings->slot_append(indexerr_bases, (object*)&ValueError);
    IndexError=(*(TypeObject*)new_type_exception(new string("IndexError"), indexerr_bases, new_dict()));
    
    KeyError=(*(TypeObject*)new_type_exception(new string("KeyError"), new_tuple(), new_dict()));
    
    NameError=(*(TypeObject*)new_type_exception(new string("NameError"), new_tuple(), new_dict()));
    
    MemoryError=(*(TypeObject*)new_type_exception(new string("MemoryError"), new_tuple(), new_dict()));
    
    object* recursionerr_bases=new_tuple();
    recursionerr_bases->type->slot_mappings->slot_append(recursionerr_bases, (object*)&MemoryError);
    RecursionError=(*(TypeObject*)new_type_exception(new string("RecursionError"), recursionerr_bases, new_dict()));

    object* stopiter_bases=new_tuple();
    stopiter_bases->type->slot_mappings->slot_append(stopiter_bases, (object*)&ValueError);
    StopIteration=(*(TypeObject*)new_type_exception(new string("StopIteration"), stopiter_bases, new_dict()));
}


typedef struct StringStreamObject{
    OBJHEAD_EXTRA
    object* file;
    object* mode;
    object* data;
}StringStreamObject;

static NumberMethods stringstream_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    0, //slot_bool
};
static Mappings stringstream_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method stringstream_methods[]={{NULL,NULL}};
GetSets stringstream_getsets[]={{NULL,NULL}};

TypeObject StringStreamType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("StringStream"), //name
    sizeof(StringStreamObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    0, //slot_new
    0, //slot_del

    0, //slot_next
    0, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &stringstream_num_methods, //slot_number
    &stringstream_mappings, //slot_mapping

    stringstream_methods, //slot_methods
    stringstream_getsets, //slot_getsets

    0, //slot_cmp
};

void setup_stringstream_type(){
    StringStreamType=(*(TypeObject*)finalize_type(&StringStreamType));
}

object* cwrapper_call(object* self, object* args, object* kwargs);
object* cwrapper_new_fromfunc(cwrapperfunc func, string name);
object* cwrapper_repr(object* self);

typedef struct CWrapperObject{
    OBJHEAD_EXTRA
    cwrapperfunc function;
    string* name;
}CWrapperObject;

Method cwrapper_methods[]={{NULL,NULL}};
GetSets cwrapper_getsets[]={{NULL,NULL}};

TypeObject CWrapperType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("CWrapperType"), //name
    sizeof(CWrapperObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    0, //slot_new
    0, //slot_del

    0, //slot_next
    0, //slot_iter

    (reprfunc)cwrapper_repr, //slot_repr
    (reprfunc)cwrapper_repr, //slot_str
    (callfunc)cwrapper_call, //slot_call

    0, //slot_number
    0, //slot_mapping

    cwrapper_methods, //slot_methods
    cwrapper_getsets, //slot_getsets

    0, //slot_cmp
};

void setup_cwrapper_type(){
    CWrapperType=(*(TypeObject*)finalize_type(&CWrapperType));
}

object* slotwrapper_call(object* self, object* args, object* kwargs);
object* slotwrapper_new_fromfunc(getsetfunc func, getfunc get, setfunc set, lenfunc len,string name, TypeObject* type);
object* slotwrapper_repr(object* self);
object* slotwrapper_str(object* self);
object* slotwrapper_get(object* self, object* key);
void slotwrapper_set(object* self, object* key, object* val);
object* slotwrapper_len(object* self);

typedef struct SlotWrapperObject{
    OBJHEAD_EXTRA
    getsetfunc function;
    getfunc get;
    setfunc set;
    lenfunc len;
    string* name;
    TypeObject* basetype;
}SlotWrapperObject;


TypeObject SlotWrapperType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("SlotWrapperType"), //name
    sizeof(SlotWrapperObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    0, //slot_new
    0, //slot_del

    0, //slot_next
    0, //slot_iter

    (reprfunc)slotwrapper_repr, //slot_repr
    (reprfunc)slotwrapper_str, //slot_str
    (callfunc)slotwrapper_call, //slot_call

    0, //slot_number
    0, //slot_mapping

    0, //slot_methods
    0, //slot_getsets

    0, //slot_cmp
};

void setup_slotwrapper_type(){
    SlotWrapperType.bases=new_list();
    list_append(SlotWrapperType.bases, INCREF((object*)&CWrapperType));
    list_append(SlotWrapperType.bases, INCREF((object*)&ObjectType));

    SlotWrapperType=(*(TypeObject*)finalize_type(&SlotWrapperType));
}


object* float_new(object* type, object* args, object* kwargs);
void float_del(object* self);
object* float_add(object* self, object* other);
object* float_sub(object* self, object* other);
object* float_mul(object* self, object* other);
object* float_div(object* self, object* other);
object* float_neg(object* self);
object* float_repr(object* self);
object* float_cmp(object* self, object* other, uint8_t type);
object* float_bool(object* self);
object* float_int(object* self);
object* float_float(object* self);

object* float_wrapper_add(object* args, object* kwargs);
object* float_wrapper_sub(object* args, object* kwargs);
object* float_wrapper_mul(object* args, object* kwargs);
object* float_wrapper_div(object* args, object* kwargs);
object* float_wrapper_neg(object* args, object* kwargs);
object* float_wrapper_repr(object* args, object* kwargs);
object* float_wrapper_bool(object* args, object* kwargs);
object* float_wrapper_new(object* args, object* kwargs);
object* float_wrapper_eq(object* args, object* kwargs);
object* float_wrapper_ne(object* args, object* kwargs);
object* float_wrapper_gt(object* args, object* kwargs);
object* float_wrapper_gte(object* args, object* kwargs);
object* float_wrapper_lt(object* args, object* kwargs);
object* float_wrapper_lte(object* args, object* kwargs);

object* new_float_fromdouble(double v);
object* new_float_fromstr(string* v);
object* new_float_frombigfloat(BigFloat v);

typedef struct FloatObject{
    OBJHEAD_EXTRA
    BigFloat* val;
}FloatObject;

static NumberMethods float_num_methods{
    (binopfunc)float_add, //slot_add
    (binopfunc)float_sub, //slot_sub
    (binopfunc)float_mul, //slot_mul
    (binopfunc)float_div, //slot_div

    (unaryfunc)float_neg, //slot_neg

    (unaryfunc)float_bool, //slot_bool
    (unaryfunc)float_int, //slot_int
    (unaryfunc)float_float, //slot_float
};

static Mappings float_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method float_methods[]={{NULL,NULL}};
GetSets float_getsets[]={{NULL,NULL}};

TypeObject FloatType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("float"), //name
    sizeof(FloatObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)float_new, //slot_new
    (delfunc)float_del, //slot_del

    0, //slot_next
    0, //slot_iter

    (reprfunc)float_repr, //slot_repr
    (reprfunc)float_repr, //slot_str
    0, //slot_call

    &float_num_methods, //slot_number
    &float_mappings, //slot_mapping

    float_methods, //slot_methods
    float_getsets, //slot_getsets

    (compfunc)float_cmp, //slot_cmp
};

void setup_float_type(){
    FloatType=(*(TypeObject*)finalize_type(&FloatType));
}

void list_iter_del(object* self);
object* list_iter_repr(object* self);
object* list_iter_next(object* self);
object* list_iter_cmp(object* self, object* other, uint8_t type);
object* list_iter_bool(object* self);

typedef struct ListIterObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
    size_t idx;
}ListIterObject;

Method list_iter_methods[]={{NULL,NULL}};
GetSets list_iter_getsets[]={{NULL,NULL}};

static NumberMethods list_iter_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)list_iter_bool, //slot_bool
};

static Mappings list_iter_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject ListIterType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("list_iter"), //name
    0, //size
    sizeof(ListIterObject), //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    object_genericgetattr, //slot_getattr
    object_genericsetattr, //slot_setattr

    0, //slot_init
    0, //slot_new
    (delfunc)list_iter_del, //slot_del

    (iternextfunc)list_iter_next, //slot_next
    (unaryfunc)generic_iter_iter, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &list_iter_num_methods, //slot_number
    &list_iter_mappings, //slot_mapping

    list_iter_methods, //slot_methods
    list_iter_getsets, //slot_getsets

    (compfunc)list_iter_cmp, //slot_cmp
};

void setup_listiter_type(){
    ListIterType=(*(TypeObject*)finalize_type(&ListIterType));
}


void tuple_iter_del(object* self);
object* tuple_iter_repr(object* self);
object* tuple_iter_next(object* self);
object* tuple_iter_cmp(object* self, object* other, uint8_t type);
object* tuple_iter_bool(object* self);

typedef struct TupleIterObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
    size_t idx;
}TupleIterObject;

Method tuple_iter_methods[]={{NULL,NULL}};
GetSets tuple_iter_getsets[]={{NULL,NULL}};

static NumberMethods tuple_iter_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)tuple_iter_bool, //slot_bool
};

static Mappings tuple_iter_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject TupleIterType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("tuple_iter"), //name
    0, //size
    sizeof(ListIterObject), //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    object_genericgetattr, //slot_getattr
    object_genericsetattr, //slot_setattr

    0, //slot_init
    0, //slot_new
    (delfunc)tuple_iter_del, //slot_del

    (iternextfunc)tuple_iter_next, //slot_next
    (unaryfunc)generic_iter_iter, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &tuple_iter_num_methods, //slot_number
    &tuple_iter_mappings, //slot_mapping

    tuple_iter_methods, //slot_methods
    tuple_iter_getsets, //slot_getsets

    (compfunc)tuple_iter_cmp, //slot_cmp
};

void setup_tupleiter_type(){
    TupleIterType=(*(TypeObject*)finalize_type(&TupleIterType));
}




object* new_type(string* name, object* bases, object* dict);

object* type_new(object* type, object* args, object* kwargs){
    //Argument size checking
    if (CAST_INT(list_len(args))->val->to_long()!=3){
        vm_add_err(&ValueError, vm, "Expected 3 arguments, got %d",CAST_INT(list_len(args))->val->to_long());
        return NULL;
    }
    //
    if (!object_istype(list_get(args, new_int_fromint(0))->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected first argument to be string, got type '%s'",list_get(args, new_int_fromint(0))->type->name->c_str());
        return NULL;
    }
    if (!object_istype(list_get(args, new_int_fromint(1))->type, &ListType) || \
    !object_istype(list_get(args, new_int_fromint(1))->type, &TupleType)){
        vm_add_err(&ValueError, vm, "Expected first argument to be list or tuple, got type '%s'",list_get(args, new_int_fromint(0))->type->name->c_str());
        return NULL;
    }
    if (!object_istype(list_get(args, new_int_fromint(2))->type, &DictType)){
        vm_add_err(&ValueError, vm, "Expected first argument to be dict, got type '%s'",list_get(args, new_int_fromint(0))->type->name->c_str());
        return NULL;
    }
    //
    string* name=CAST_STRING(list_get(args, new_int_fromint(0)))->val;
    object* bases=list_get(args, new_int_fromint(1));
    object* dict=list_get(args, new_int_fromint(2));
    return new_type(name, bases, dict);
}

void type_del(object* self){}

object* type_repr(object* self){
    string s="<class '"+(*CAST_TYPE(self)->name)+"'>";
    return str_new_fromstr(s);
}

object* type_call(object* self, object* args, object* kwargs){
    //Special case
    if (object_istype(CAST_TYPE(self), &TypeType)){
        if (CAST_INT(list_len(args))->val->to_long()==1){
            return (object*)(list_get(args, new_int_fromint(0))->type);
        }
    }
    if (CAST_TYPE(self)->slot_new==NULL){
        vm_add_err(&TypeError, vm, "Cannot create instances of type '%s'", CAST_TYPE(self)->name);
        return NULL;
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
            return dict_get(dict, attr);
        }
    }
    //Check type dict
    if (self->type->dict!=0){
        object* dict = self->type->dict;
        if (object_find_bool_dict_keys(dict, attr)){
            return dict_get(dict, attr);
        }
    }

    uint32_t total_bases = CAST_INT(list_len(self->type->bases))->val->to_long_long();
    for (uint32_t i=total_bases; i>0; i--){
        TypeObject* base_tp=CAST_TYPE(list_get(self->type->bases, new_int_fromint(i-1)));

        //Check type dict
        if (base_tp->dict!=0){
            object* dict = base_tp->dict;
            if (object_find_bool_dict_keys(dict, attr)){
                return dict_get(dict, attr);
            }
        }
    }

    vm_add_err(&AttributeError, vm, "%s has no attribute '%s'",self->type->name->c_str(), object_cstr(attr).c_str());
    return NULL;
}

void type_set(object* obj, object* attr, object* val){
    //Check dict
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        dict_set(dict, attr, val);
        return;
    }
    vm_add_err(&AttributeError, vm, "%s is read only",obj->type->name->c_str());
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
    m->slot_int=base_tp->slot_number->slot_int;
    m->slot_float=base_tp->slot_number->slot_float;
}

void _inherit_mapping_slots(Mappings* m, TypeObject* base_tp){
    if (base_tp->slot_mappings==NULL){
        return;
    }
    
    m->slot_get=base_tp->slot_mappings->slot_get;
    m->slot_set=base_tp->slot_mappings->slot_set;
    m->slot_len=base_tp->slot_mappings->slot_len;
    m->slot_append=base_tp->slot_mappings->slot_append;
}

void _inherit_slots(TypeObject* tp_tp, TypeObject* base_tp, NumberMethods* m, Mappings* ma){
    SETSLOT(tp_tp, base_tp, slot_getattr);
    SETSLOT(tp_tp, base_tp, slot_setattr);
    SETSLOT(tp_tp, base_tp, slot_init);
    SETSLOT(tp_tp, base_tp, slot_new);
    SETSLOT(tp_tp, base_tp, slot_del);    
    SETSLOT(tp_tp, base_tp, slot_next);
    SETSLOT(tp_tp, base_tp, slot_iter);
    SETSLOT(tp_tp, base_tp, slot_repr);
    SETSLOT(tp_tp, base_tp, slot_str);
    SETSLOT(tp_tp, base_tp, slot_call);
    SETSLOT(tp_tp, base_tp, slot_cmp);
    
    _inherit_number_slots(m, base_tp);
    _inherit_mapping_slots(ma, base_tp);
}

object* finalize_type(TypeObject* newtype){
    object* tp=(object*)malloc(sizeof(TypeObject));
    memcpy(tp, newtype, sizeof(TypeObject));
        
    TypeObject* tp_tp=CAST_TYPE(tp);

    //tp is the what we'll copy to...

    //Clean out bases
    if (tp_tp->bases==NULL){
        tp_tp->bases=new_list();
        list_append(tp_tp->bases, INCREF((object*)&ObjectType));
    }

    uint32_t total_bases = CAST_INT(list_len(tp_tp->bases))->val->to_long_long();

    //This is a slower method than could theoritically be done.
    //I could just use implied list indexing (uses my internal knowledge of ListObject), but this
    //also breaks fewer rules...
    
    

    NumberMethods* m=(NumberMethods*)malloc(sizeof(NumberMethods));
    memset(m, 0, sizeof(NumberMethods));

    Mappings* ma=(Mappings*)malloc(sizeof(Mappings));
    memset(ma, 0, sizeof(Mappings));

    for (uint32_t i=total_bases; i>0; i--){
        TypeObject* base_tp=CAST_TYPE(list_get(tp_tp->bases, new_int_fromint(i-1)));
        //Dirty inheritance here... go over each
        _inherit_slots(tp_tp, base_tp, m, ma);
        //      
    }
    //Hack to ensure retention of original slots
    _inherit_slots(tp_tp, newtype, m, ma);


    tp_tp->slot_number=m;
    tp_tp->slot_mappings=ma;

    tp_tp->refcnt=1;

    return tp;
}

void inherit_type_dict(TypeObject* tp){
    TypeObject* tp_tp=CAST_TYPE(tp);

    uint32_t total_bases = CAST_INT(list_len(tp_tp->bases))->val->to_long_long();

    
    //Setup type dict
    tp_tp->dict=new_dict();

    //This is a slower method than could theoritically be done.
    //I could just use implied list indexing (uses my internal knowledge of ListObject), but this
    //also breaks fewer rules...
    
    for (uint32_t i=total_bases; i>0; i--){
        TypeObject* base_tp=CAST_TYPE(list_get(tp_tp->bases, new_int_fromint(i-1)));
        object* dict=base_tp->dict;

        for (auto k: (*CAST_DICT(dict)->val)){
            dict_set(tp_tp->dict, k.first, k.second);
        }  
    }
}

object* inherit_type_methods(TypeObject* tp){
    TypeObject* tp_tp=CAST_TYPE(tp);

    //tp is the what we'll copy to...

    //Clean out bases
    uint32_t total_bases = CAST_INT(list_len(tp_tp->bases))->val->to_long_long();

    //This is a slower method than could theoritically be done.
    //I could just use implied list indexing (uses my internal knowledge of ListObject), but this
    //also breaks fewer rules...
    
    for (uint32_t i=total_bases; i>0; i--){
        TypeObject* base_tp=CAST_TYPE(list_get(tp_tp->bases, new_int_fromint(i-1)));
        //Inherit methods
        uint32_t idx=0;
        while (base_tp->slot_methods[idx].name!=NULL){
            dict_set(tp_tp->dict, str_new_fromstr(base_tp->slot_methods[idx].name), cwrapper_new_fromfunc((cwrapperfunc)base_tp->slot_methods[idx].function, base_tp->slot_methods[idx].name));
            idx++;
        }        
    }

    //Inherit methods
    uint32_t idx=0;
    while (tp_tp->slot_methods[idx].name!=NULL){
        dict_set(tp_tp->dict, str_new_fromstr(tp_tp->slot_methods[idx].name), cwrapper_new_fromfunc((cwrapperfunc)tp_tp->slot_methods[idx].function, tp_tp->slot_methods[idx].name));
        idx++;
    }

    return (object*)tp;
}

object* inherit_type_getsets(TypeObject* tp){
    TypeObject* tp_tp=CAST_TYPE(tp);

    //tp is the what we'll copy to...

    //Clean out bases
    uint32_t total_bases = CAST_INT(list_len(tp_tp->bases))->val->to_long_long();

    //This is a slower method than could theoritically be done.
    //I could just use implied list indexing (uses my internal knowledge of ListObject), but this
    //also breaks fewer rules...
    
    for (uint32_t i=total_bases; i>0; i--){
        TypeObject* base_tp=CAST_TYPE(list_get(tp_tp->bases, new_int_fromint(i-1)));
        //Inherit methods
        uint32_t idx=0;
        while (base_tp->slot_getsets[idx].name!=NULL){
            dict_set(tp_tp->dict, str_new_fromstr(base_tp->slot_getsets[idx].name), slotwrapper_new_fromfunc((getsetfunc)base_tp->slot_getsets[idx].function, (getfunc)base_tp->slot_getsets[idx].get, (setfunc)base_tp->slot_getsets[idx].set, (lenfunc)base_tp->slot_getsets[idx].len, base_tp->slot_getsets[idx].name, base_tp));
            idx++;
        }        
    }

    //Inherit methods
    uint32_t idx=0;
    while (tp_tp->slot_getsets[idx].name!=NULL){
        dict_set(tp_tp->dict, str_new_fromstr(tp_tp->slot_getsets[idx].name), slotwrapper_new_fromfunc((getsetfunc)tp_tp->slot_getsets[idx].function, (getfunc)tp_tp->slot_getsets[idx].get, (setfunc)tp_tp->slot_getsets[idx].set, (lenfunc)tp_tp->slot_getsets[idx].len, tp_tp->slot_getsets[idx].name, tp_tp));
        idx++;
    }

    return (object*)tp;
}

void type_set_cwrapper(TypeObject* tp, cwrapperfunc func, string name){
    object* f=cwrapper_new_fromfunc(func, name);
    dict_set(tp->dict, str_new_fromstr(*CAST_CWRAPPER(f)->name), f);
}

object* type_bool(object* self){
    return new_bool_true();
}

object* type_dict(object* type){
    return CAST_SLOTWRAPPER(type)->basetype->dict;
}

object* type_dictget(object* type, object* key){
    return dict_get(CAST_SLOTWRAPPER(type)->basetype->dict, key);
}
void type_dictset(object* type, object* key, object* val){
    dict_set(CAST_SLOTWRAPPER(type)->basetype->dict, key, val);
}
object* type_dictlen(object* type){
    return dict_len(CAST_SLOTWRAPPER(type)->basetype->dict);
}



#include "typeobject_newtp.cpp"
#include "object_dicts.cpp"