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
object* int_mod(object* self, object* other);
object* int_pow(object* self, object* other);

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
    (binopfunc)int_mod, //slot_mod
    (binopfunc)int_pow, //slot_pow

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
OffsetMember int_offsets[]={{NULL}};

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
    int_offsets, //slot_offsests

    (compfunc)int_cmp, //slot_cmp
};

void setup_int_type(){
    IntType=(*(TypeObject*)finalize_type(&IntType));
    fplbases.push_back(&IntType);
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
object* str_iter(object* self);

object* str_wrapper_new(object* args, object* kwargs);
object* str_wrapper_len(object* args, object* kwargs);
object* str_wrapper_repr(object* args, object* kwargs);
object* str_wrapper_str(object* args, object* kwargs);
object* str_wrapper_bool(object* args, object* kwargs);
object* str_wrapper_ne(object* args, object* kwargs);
object* str_wrapper_eq(object* args, object* kwargs);

object* str_add(object* self, object* other);
object* str_get(object* self, object* idx);
object* str_mul(object* self, object* other);

object* str_new_fromstr(string val);
object* string_join_meth(object* args, object* kwargs);
object* string_replace_meth(object* args, object* kwargs);
object* string_find_meth(object* args, object* kwargs);

typedef struct StrObject{
    OBJHEAD_EXTRA
    string* val;
}StrObject;

static NumberMethods str_num_methods{
    (binopfunc)str_add, //slot_add
    0, //slot_sub
    (binopfunc)str_mul, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    (unaryfunc)str_bool, //slot_bool
    (unaryfunc)str_int, //slot_int
    (unaryfunc)str_float, //slot_float
};

static Mappings str_mappings{
    str_get, //slot_get
    0, //slot_set
    str_len, //slot_len
};

Method str_methods[]={{"find", (cwrapperfunc)string_find_meth}, {"replace", (cwrapperfunc)string_replace_meth}, {"join", (cwrapperfunc)string_join_meth}, {NULL,NULL}};
GetSets str_getsets[]={{NULL,NULL}};
OffsetMember str_offsets[]={{NULL}};

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
    (unaryfunc)str_iter, //slot_iter

    (reprfunc)str_repr, //slot_repr
    (reprfunc)str_str, //slot_str
    0, //slot_call

    &str_num_methods, //slot_number
    &str_mappings, //slot_mapping

    str_methods, //slot_methods
    str_getsets, //slot_getsets
    str_offsets, //slot_offsests

    (compfunc)str_cmp, //slot_cmp
};

void setup_str_type(){
    StrType=(*(TypeObject*)finalize_type(&StrType));
    fplbases.push_back(&StrType);
}



object* list_init(object* self, object* args, object* kwargs);
void list_del(object* self);
object* list_len(object* self);
object* list_get(object* self, object* idx);
void list_append(object* self, object* obj);
object* list_new(object* type, object* args, object* kwargs);
object* list_set(object* self, object* idx, object* val);
object* list_repr(object* self);
object* list_next(object* self);
object* list_cmp(object* self, object* other, uint8_t type);
object* list_bool(object* self);
object* list_append_meth(object* args, object* kwargs);
object* list_iter(object* self);
object* list_pop(object* self);
object* list_pop_meth(object* args, object* kwargs);
object* list_replace_meth(object* args, object* kwargs);
object* list_find_meth(object* args, object* kwargs);

object* list_add(object* self, object* other);
object* list_mul(object* self, object* other);

typedef struct ListObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
}ListObject;

Method list_methods[]={{"find", (cwrapperfunc)list_find_meth}, {"replace", (cwrapperfunc)list_replace_meth}, {"append", (cwrapperfunc)list_append_meth},\
                    {"pop", (cwrapperfunc)list_pop_meth}, {NULL,NULL}};
GetSets list_getsets[]={{NULL,NULL}};
OffsetMember list_offsets[]={{NULL}};

static NumberMethods list_num_methods{
    (binopfunc)list_add, //slot_add
    0, //slot_sub
    (binopfunc)list_mul, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

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
    list_offsets, //slot_offsests

    (compfunc)list_cmp, //slot_cmp
};

void setup_list_type(){
    ListType=(*(TypeObject*)finalize_type(&ListType));
    fplbases.push_back(&ListType);
}



object* dict_new(object* type, object* args, object* kwargs);
void dict_del(object* self);
object* dict_len(object* self);
object* dict_get(object* self, object* idx);
object* dict_set(object* self, object* key, object* val);
object* dict_repr(object* self);
object* dict_str(object* self);
object* dict_cmp(object* self, object* other, uint8_t type);
object* dict_bool(object* self);
object* dict_iter(object* self);
object* dict_keys_meth(object* args, object* kwargs);
object* dict_values_meth(object* args, object* kwargs);

typedef struct DictObject{
    OBJHEAD_VAR
    map<object*, object*>* val;
    vector<object*>* keys;
}DictObject;

static NumberMethods dict_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    (unaryfunc)dict_bool, //slot_bool
};

static Mappings dict_mappings{
    dict_get, //slot_get
    dict_set, //slot_set
    dict_len, //slot_len
};

Method dict_methods[]={{"keys", (cwrapperfunc)dict_keys_meth}, {"values", (cwrapperfunc)dict_values_meth},{NULL,NULL}};
GetSets dict_getsets[]={{NULL,NULL}};
OffsetMember dict_offsets[]={{NULL}};

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
    (unaryfunc)dict_iter, //slot_iter

    (reprfunc)dict_repr, //slot_repr
    (reprfunc)dict_str, //slot_str
    0, //slot_call

    &dict_num_methods, //slot_number
    &dict_mappings, //slot_mapping

    dict_methods, //slot_methods
    dict_getsets, //slot_getsets
    dict_offsets, //slot_offsests

    (compfunc)dict_cmp, //slot_cmp
};

void setup_dict_type(){
    DictType=(*(TypeObject*)finalize_type(&DictType));
    fplbases.push_back(&DictType);
}



object* code_init(object* self, object* args, object* kwargs);
void code_del(object* self);
object* code_new(object* type, object* args, object* kwargs);
object* code_repr(object* self);
object* code_cmp(object* self, object* other, uint8_t type);
object* code_bool(object* self);

object* code_co_names(object* code);
object* code_co_consts(object* code);
object* code_co_code(object* code);
object* code_co_file(object* code);
object* code_co_lines(object* code);

typedef struct CodeObject{
    OBJHEAD_EXTRA
    object* co_names;
    object* co_consts;
    object* co_code;
    object* co_file;
    object* co_lines;
    uint32_t co_instructions;
    string* filedata;
}CodeObject;

static NumberMethods code_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    (unaryfunc)code_bool, //slot_bool
};

static Mappings code_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method code_methods[]={{NULL,NULL}};
GetSets code_getsets[]={{"co_names", (getter)code_co_names}, {"co_consts", (getter)code_co_consts}, \
{"co_code", (getter)code_co_code}, {"co_file", (getter)code_co_file}, {"co_lines", (getter)code_co_lines}, \
{NULL,NULL}};
OffsetMember code_offsets[]={{NULL}};

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
    code_offsets, //slot_offsests

    (compfunc)code_cmp, //slot_cmp
};

void setup_code_type(){
    CodeType=(*(TypeObject*)finalize_type(&CodeType));
    CodeType.slot_new=NULL;
    fplbases.push_back(&CodeType);
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
object* bool_mod(object* self, object* other);
object* bool_pow(object* self, object* other);

object* new_bool_true();
object* new_bool_false();


typedef struct BoolObject{
    OBJHEAD_EXTRA
    uint8_t val;
}BoolObject;

bool istrue(object* boolean){
    if (boolean!=NULL && CAST_BOOL(boolean)->val==1){
        DECREF(boolean);
        return true;
    }
    if (boolean!=NULL){
        DECREF(boolean);
    }
    return false;
}

static NumberMethods bool_num_methods{
    (binopfunc)bool_add, //slot_add
    (binopfunc)bool_sub, //slot_sub
    (binopfunc)bool_mul, //slot_mul
    (binopfunc)bool_div, //slot_div
    (binopfunc)bool_mod, //slot_div
    (binopfunc)bool_pow, //slot_pow

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
OffsetMember bool_offsets[]={{NULL}};

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
    bool_offsets, //slot_offsests

    (compfunc)bool_cmp, //slot_cmp
};

void setup_bool_type(){
    BoolType=(*(TypeObject*)finalize_type(&BoolType));
    fplbases.push_back(&BoolType);
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

object* tuple_mul(object* self, object* other);
object* tuple_add(object* self, object* other);

object* tuple_find_meth(object* args, object* kwargs);

typedef struct TupleObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
    size_t idx;
}TupleObject;

static NumberMethods tuple_num_methods{
    (binopfunc)tuple_add, //slot_add
    0, //slot_sub
    (binopfunc)tuple_mul, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    (unaryfunc)tuple_bool, //slot_bool
};


static Mappings tuple_mappings{
    tuple_get, //slot_get
    0, //slot_set
    tuple_len, //slot_len
    list_append, //slot_append
};

Method tuple_methods[]={{"find", (cwrapperfunc)tuple_find_meth}, {NULL,NULL}};
GetSets tuple_getsets[]={{NULL,NULL}};
OffsetMember tuple_offsets[]={{NULL}};

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
    tuple_offsets, //slot_offsests

    (compfunc)tuple_cmp, //slot_cmp
};

void setup_tuple_type(){
    TupleType=(*(TypeObject*)finalize_type(&TupleType));
    fplbases.push_back(&TupleType);
}


void func_del(object* self);
object* func_repr(object* self);
object* func_cmp(object* self, object* other, uint8_t type);
object* func_call(object* self, object* args, object* callfunc);
object* func_bool(object* self);
object* func_call_nostack(object* self, object* args, object* kwargs);
object* func_run(object* self, object* args, object* kwargs);

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
    0, //slot_mod
    0, //slot_pow

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
OffsetMember func_offsets[]={{NULL}};

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
    0, //slot_new
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
    func_offsets, //slot_offsests

    (compfunc)func_cmp, //slot_cmp
};

void setup_func_type(){
    FuncType=(*(TypeObject*)finalize_type(&FuncType));
    fplbases.push_back(&FuncType);
}



void none_del(object* self);
object* none_repr(object* self);
object* none_bool(object* self);


typedef struct NoneObject{
    OBJHEAD_EXTRA
}NoneObject;

static NumberMethods none_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

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
OffsetMember none_offsets[]={{NULL}};

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
    0, //slot_new
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
    none_offsets, //slot_offsests

    0, //slot_cmp
};

void setup_none_type(){
    NoneType=(*(TypeObject*)finalize_type(&NoneType));
    fplbases.push_back(&NoneType);
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
    bool nargs;
}BuiltinObject;

static NumberMethods builtin_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

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
OffsetMember builtin_offsets[]={{NULL}};

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
    builtin_offsets, //slot_offsests

    (compfunc)builtin_cmp, //slot_cmp
};

void setup_builtin_type(){
    BuiltinType=(*(TypeObject*)finalize_type(&BuiltinType));
    BuiltinType.slot_new=NULL;
    fplbases.push_back(&BuiltinType);
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
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    (unaryfunc)object_bool, //slot_bool
};

Method object_methods[]={{NULL,NULL}};
GetSets object_getsets[]={{"__dict__", (getter)type_dict, 0}, {"__bases__", (getter)type_bases_get, 0},{NULL,NULL}};
OffsetMember object_offsets[]={{NULL}};

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
    object_offsets, //slot_offsests

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
    0, //slot_mod
    0, //slot_pow

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
OffsetMember exception_offsets[]={{NULL}};

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
    exception_offsets, //slot_offsests

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
        0, //slot_offsests

        0, //slot_cmp
    };
        
    object* o=finalize_type(&newtype);
    setup_type_offsets((TypeObject*)o);
    return o;
}

void setup_exception_type(){
    ExceptionType=(*(TypeObject*)finalize_type(&ExceptionType));
    fplbases.push_back(&ExceptionType);

    TypeError=(*(TypeObject*)new_type_exception(new string("TypeError"), new_tuple(), new_dict()));
    fplbases.push_back(&TypeError);
    
    ValueError=(*(TypeObject*)new_type_exception(new string("ValueError"), new_tuple(), new_dict()));
    fplbases.push_back(&ValueError);
    
    AttributeError=(*(TypeObject*)new_type_exception(new string("AttributeError"), new_tuple(), new_dict()));
    fplbases.push_back(&AttributeError);
    
    object* indexerr_bases=new_tuple();
    indexerr_bases->type->slot_mappings->slot_append(indexerr_bases, (object*)&ValueError);
    IndexError=(*(TypeObject*)new_type_exception(new string("IndexError"), indexerr_bases, new_dict()));
    fplbases.push_back(&IndexError);
    
    KeyError=(*(TypeObject*)new_type_exception(new string("KeyError"), new_tuple(), new_dict()));
    fplbases.push_back(&KeyError);
    
    NameError=(*(TypeObject*)new_type_exception(new string("NameError"), new_tuple(), new_dict()));
    fplbases.push_back(&NameError);
    
    MemoryError=(*(TypeObject*)new_type_exception(new string("MemoryError"), new_tuple(), new_dict()));
    fplbases.push_back(&MemoryError);
    setup_memory_error=true;
    
    object* recursionerr_bases=new_tuple();
    recursionerr_bases->type->slot_mappings->slot_append(recursionerr_bases, (object*)&MemoryError);
    RecursionError=(*(TypeObject*)new_type_exception(new string("RecursionError"), recursionerr_bases, new_dict()));
    fplbases.push_back(&RecursionError);

    object* stopiter_bases=new_tuple();
    stopiter_bases->type->slot_mappings->slot_append(stopiter_bases, (object*)&ValueError);
    StopIteration=(*(TypeObject*)new_type_exception(new string("StopIteration"), stopiter_bases, new_dict()));
    fplbases.push_back(&StopIteration);

    object* filenotfound_bases=new_tuple();
    filenotfound_bases->type->slot_mappings->slot_append(filenotfound_bases, (object*)&NameError);
    FileNotFoundError=(*(TypeObject*)new_type_exception(new string("FileNotFoundError"), filenotfound_bases, new_dict()));
    fplbases.push_back(&FileNotFoundError);

    object* invalidopr_bases=new_tuple();
    invalidopr_bases->type->slot_mappings->slot_append(invalidopr_bases, (object*)&TypeError);
    InvalidOperationError=(*(TypeObject*)new_type_exception(new string("InvalidOperationError"), invalidopr_bases, new_dict()));
    fplbases.push_back(&InvalidOperationError);

    object* importerr_bases=new_tuple();
    importerr_bases->type->slot_mappings->slot_append(importerr_bases, (object*)&FileNotFoundError);
    ImportError=(*(TypeObject*)new_type_exception(new string("ImportError"), importerr_bases, new_dict()));
    fplbases.push_back(&ImportError);

    KeyboardInterrupt=(*(TypeObject*)new_type_exception(new string("KeyboardInterrupt"), new_tuple(), new_dict()));
    fplbases.push_back(&KeyboardInterrupt);

    AssertionError=(*(TypeObject*)new_type_exception(new string("AssertionError"), new_tuple(), new_dict()));
    fplbases.push_back(&AssertionError);
}


typedef struct FileObject{
    OBJHEAD_EXTRA
    FILE* file;
    char* mode;
    bool open;
    object* name;
}FileObject;

static NumberMethods file_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    0, //slot_bool
};
static Mappings file_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};


object* file_new(object* type, object* args, object* kwargs);
void file_del(object* self);
object* file_repr(object* self);
object* file_new_fromfile(object* name, char* mode);
object* file_read_meth(object* args, object* kwargs);
object* file_close_meth(object* args, object* kwargs);
object* file_write_meth(object* args, object* kwargs);
object* file_seek_meth(object* args, object* kwargs);

Method file_methods[]={{"read", (cwrapperfunc)file_read_meth}, {"close", (cwrapperfunc)file_close_meth}\
                    , {"write", (cwrapperfunc)file_write_meth}, {"seek", (cwrapperfunc)file_seek_meth}, {NULL,NULL}};
GetSets file_getsets[]={{NULL,NULL}};
OffsetMember file_offsets[]={{NULL}};

TypeObject FileType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("file"), //name
    sizeof(FileObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)file_new, //slot_new
    (delfunc)file_del, //slot_del

    0, //slot_next
    0, //slot_iter

    (reprfunc)file_repr, //slot_repr
    (reprfunc)file_repr, //slot_str
    0, //slot_call

    &file_num_methods, //slot_number
    &file_mappings, //slot_mapping

    file_methods, //slot_methods
    file_getsets, //slot_getsets
    file_offsets, //slot_offsests

    0, //slot_cmp
};

void setup_file_type(){
    FileType=(*(TypeObject*)finalize_type(&FileType));
    fplbases.push_back(&FileType);
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
OffsetMember cwrapper_offsets[]={{NULL}};

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
    cwrapper_offsets, //slot_offsests

    0, //slot_cmp

    0, //slot_offsetget
    0, //slot_offsetset
};

void setup_cwrapper_type(){
    CWrapperType=(*(TypeObject*)finalize_type(&CWrapperType));
    CWrapperType.slot_new=NULL;
    fplbases.push_back(&CWrapperType);
}

object* slotwrapper_new_fromfunc(getter get, setter set, string name, TypeObject* basetype);
object* slotwrapper_new(object* type, object* args, object* kwargs);    
object* slotwrapper_repr(object* self);
object* slotwrapper_str(object* self);
object* slotwrapper_offsetget(object* obj, object* self);
object* slotwrapper_offsetset(object* obj, object* self, object* val);

typedef struct SlotWrapperObject{
    OBJHEAD_EXTRA
    getter get;
    setter set;
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
    0, //slot_call

    0, //slot_number
    0, //slot_mapping

    0, //slot_methods
    0, //slot_getsets
    0, //slot_offsets

    0, //slot_cmp

    (offsetgetfunc)slotwrapper_offsetget, //slot_offsetget
    (offsetsetfunc)slotwrapper_offsetset, //slot_offsetget
};

void setup_slotwrapper_type(){
    SlotWrapperType=(*(TypeObject*)finalize_type(&SlotWrapperType));
    fplbases.push_back(&SlotWrapperType);
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
object* float_mod(object* self, object* other);
object* float_pow(object* self, object* other);

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
object* new_float_fromstr(string v);
double round_double(double value, size_t prec);

typedef struct FloatObject{
    OBJHEAD_EXTRA
    double val;
}FloatObject;

static NumberMethods float_num_methods{
    (binopfunc)float_add, //slot_add
    (binopfunc)float_sub, //slot_sub
    (binopfunc)float_mul, //slot_mul
    (binopfunc)float_div, //slot_div
    (binopfunc)float_mod, //slot_div
    (binopfunc)float_pow, //slot_pow


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
OffsetMember float_offsets[]={{NULL}};

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
    float_offsets, //slot_offsests

    (compfunc)float_cmp, //slot_cmp
};

void setup_float_type(){
    FloatType=(*(TypeObject*)finalize_type(&FloatType));
    fplbases.push_back(&FloatType);
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
    0, //slot_mod
    0, //slot_pow

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
    0, //slot_offsests

    (compfunc)list_iter_cmp, //slot_cmp
};

void setup_listiter_type(){
    ListIterType=(*(TypeObject*)finalize_type(&ListIterType));
    fplbases.push_back(&ListIterType);
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
    0, //slot_mod
    0, //slot_pow

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
    sizeof(TupleIterObject), //var_base_size
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
    0, //slot_offsets

    (compfunc)tuple_iter_cmp, //slot_cmp
};

void setup_tupleiter_type(){
    TupleIterType=(*(TypeObject*)finalize_type(&TupleIterType));
    fplbases.push_back(&TupleIterType);
}


void dict_iter_del(object* self);
object* dict_iter_repr(object* self);
object* dict_iter_next(object* self);
object* dict_iter_cmp(object* self, object* other, uint8_t type);
object* dict_iter_bool(object* self);

typedef struct DictIterObject{
    OBJHEAD_VAR
    map<object*, object*>* val;
    vector<object*>* keys;
    uint32_t idx;
}DictIterObject;

Method dict_iter_methods[]={{NULL,NULL}};
GetSets dict_iter_getsets[]={{NULL,NULL}};

static NumberMethods dict_iter_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    (unaryfunc)dict_iter_bool, //slot_bool
};

static Mappings dict_iter_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject DictIterType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("dict_iter"), //name
    sizeof(DictIterObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    object_genericgetattr, //slot_getattr
    object_genericsetattr, //slot_setattr

    0, //slot_init
    0, //slot_new
    (delfunc)dict_iter_del, //slot_del

    (iternextfunc)dict_iter_next, //slot_next
    (unaryfunc)generic_iter_iter, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &dict_iter_num_methods, //slot_number
    &dict_iter_mappings, //slot_mapping

    dict_iter_methods, //slot_methods
    dict_iter_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)dict_iter_cmp, //slot_cmp
};

void setup_dictiter_type(){
    DictIterType=(*(TypeObject*)finalize_type(&DictIterType));
    fplbases.push_back(&DictIterType);
}

void str_iter_del(object* self);
object* str_iter_repr(object* self);
object* str_iter_next(object* self);
object* str_iter_cmp(object* self, object* other, uint8_t type);
object* str_iter_bool(object* self);

typedef struct StrIterObject{
    OBJHEAD_VAR
    string* val;
    uint32_t idx;
}StrIterObject;

Method str_iter_methods[]={{NULL,NULL}};
GetSets str_iter_getsets[]={{NULL,NULL}};

static NumberMethods str_iter_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    (unaryfunc)str_iter_bool, //slot_bool
};

static Mappings str_iter_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject StrIterType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("str_iter"), //name
    sizeof(StrIterObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    object_genericgetattr, //slot_getattr
    object_genericsetattr, //slot_setattr

    0, //slot_init
    0, //slot_new
    (delfunc)str_iter_del, //slot_del

    (iternextfunc)str_iter_next, //slot_next
    (unaryfunc)generic_iter_iter, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &str_iter_num_methods, //slot_number
    &str_iter_mappings, //slot_mapping

    str_iter_methods, //slot_methods
    str_iter_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)str_iter_cmp, //slot_cmp
};

void setup_striter_type(){
    StrIterType=(*(TypeObject*)finalize_type(&StrIterType));
    fplbases.push_back(&StrIterType);
}


void module_del(object* self);
object* module_repr(object* self);
object* module_cmp(object* self, object* other, uint8_t type);
object* module_new_fromdict(object* dict, object* name);
object* module_dict(object* self);

typedef struct ModuleObject{
    OBJHEAD_EXTRA
    object* dict;
    object* name;
}ModuleObject;

static NumberMethods module_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    0, //slot_bool
};

Method module_methods[]={{NULL,NULL}};
GetSets module_getsets[]={{NULL,NULL}};
OffsetMember module_offsets[]={{"__dict__", offsetof(ModuleObject, dict), true}, {NULL}};

static Mappings module_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

TypeObject ModuleType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("module"), //name
    sizeof(ModuleObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    offsetof(ModuleObject, dict), //dict_offset
    NULL, //dict
    object_genericgetattr, //slot_getattr
    object_genericsetattr, //slot_setattr
    0, //slot_init
    0, //slot_new
    0, //slot_del

    0, //slot_next
    0, //slot_iter

    (reprfunc)module_repr, //slot_repr
    (reprfunc)module_repr, //slot_str
    0, //slot_call

    &module_num_methods, //slot_number
    &module_mappings, //slot_mapping

    module_methods, //slot_methods
    module_getsets, //slot_getsets
    module_offsets, //slot_offsests

    (compfunc)module_cmp, //slot_cmp
};

void setup_module_type(){
    ModuleType=(*(TypeObject*)finalize_type(&ModuleType));
    ModuleType.slot_new=NULL;
    fplbases.push_back(&ModuleType);
}


object* slice_new(object* type, object* args, object* kwargs);
void slice_del(object* self);
object* slice_repr(object* self);
object* slice_cmp(object* self, object* other, uint8_t type);
object* slice_new_fromnums(object* start, object* end);

typedef struct SliceObject{
    OBJHEAD_EXTRA
    object* start;
    object* end;
}SliceObject;

static NumberMethods slice_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    0, //slot_bool
};

Method slice_methods[]={{NULL,NULL}};
GetSets slice_getsets[]={{NULL,NULL}};
OffsetMember slice_offsets[]={{NULL}};

static Mappings slice_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

TypeObject SliceType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("slice"), //name
    sizeof(SliceObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    object_genericgetattr, //slot_getattr
    object_genericsetattr, //slot_setattr
    0, //slot_init
    slice_new, //slot_new
    0, //slot_del

    0, //slot_next
    0, //slot_iter

    (reprfunc)slice_repr, //slot_repr
    (reprfunc)slice_repr, //slot_str
    0, //slot_call

    &slice_num_methods, //slot_number
    &slice_mappings, //slot_mapping

    slice_methods, //slot_methods
    slice_getsets, //slot_getsets
    slice_offsets, //slot_offsests

    (compfunc)slice_cmp, //slot_cmp
};

void setup_slice_type(){
    SliceType=(*(TypeObject*)finalize_type(&SliceType));
    fplbases.push_back(&SliceType);
}


void enum_del(object* self);
object* enum_next(object* self);
object* enum_new(object* type, object* args, object* kwargs);
object* enum_cmp(object* self, object* other, uint8_t type);

typedef struct EnumObject{
    OBJHEAD_VAR
    object* iterator;
    uint32_t idx;
}EnumObject;

Method enum_methods[]={{NULL,NULL}};
GetSets enum_getsets[]={{NULL,NULL}};

static NumberMethods enum_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    0, //slot_bool
};

static Mappings enum_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject EnumType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("enumerate"), //name
    sizeof(EnumObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    enum_new, //slot_new
    (delfunc)enum_del, //slot_del

    (iternextfunc)enum_next, //slot_next
    (unaryfunc)generic_iter_iter, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &enum_num_methods, //slot_number
    &enum_mappings, //slot_mapping

    enum_methods, //slot_methods
    enum_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)enum_cmp, //slot_cmp
};

void setup_enum_type(){
    EnumType=(*(TypeObject*)finalize_type(&EnumType));
    fplbases.push_back(&EnumType);
}


void range_del(object* self);
object* range_next(object* self);
object* range_new(object* type, object* args, object* kwargs);
object* range_cmp(object* self, object* other, uint8_t type);

typedef struct RangeObject{
    OBJHEAD_VAR
    uint32_t start;
    uint32_t end;
    uint32_t idx;
}RangeObject;

Method range_methods[]={{NULL,NULL}};
GetSets range_getsets[]={{NULL,NULL}};

static NumberMethods range_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    0, //slot_bool
};

static Mappings range_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject RangeType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("range"), //name
    sizeof(RangeObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    range_new, //slot_new
    0, //slot_del

    (iternextfunc)range_next, //slot_next
    (unaryfunc)generic_iter_iter, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &range_num_methods, //slot_number
    &range_mappings, //slot_mapping

    range_methods, //slot_methods
    range_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)range_cmp, //slot_cmp
};

void setup_range_type(){
    RangeType=(*(TypeObject*)finalize_type(&RangeType));
    fplbases.push_back(&RangeType);
}

void zip_del(object* self);
object* zip_next(object* self);
object* zip_new(object* type, object* args, object* kwargs);
object* zip_cmp(object* self, object* other, uint8_t type);


typedef struct ZipObject{
    OBJHEAD_VAR
    object** iterators;
    size_t n_iterators;
    uint32_t idx;
}ZipObject;

Method zip_methods[]={{NULL,NULL}};
GetSets zip_getsets[]={{NULL,NULL}};

static NumberMethods zip_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow

    0, //slot_neg

    0, //slot_bool
};

static Mappings zip_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject ZipType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("zip"), //name
    sizeof(ZipObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    zip_new, //slot_new
    0, //slot_del

    (iternextfunc)zip_next, //slot_next
    (unaryfunc)generic_iter_iter, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &zip_num_methods, //slot_number
    &zip_mappings, //slot_mapping

    zip_methods, //slot_methods
    zip_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)zip_cmp, //slot_cmp
};

void setup_zip_type(){
    ZipType=(*(TypeObject*)finalize_type(&ZipType));
    fplbases.push_back(&ZipType);
}

object* offsetwrapper_new_fromoffset(string name, size_t offset, bool readonly);
object* offsetwrapper_repr(object* self);
object* offsetwrapper_offsetget(object* obj, object* self);
object* offsetwrapper_offsetset(object* obj, object* self, object* val);

typedef struct OffsetWrapperObject{
    OBJHEAD_EXTRA
    size_t offset;
    string* name;
}OffsetWrapperObject;

TypeObject OffsetWrapperType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("OffsetWrapperType"), //name
    sizeof(OffsetWrapperObject), //size
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

    (reprfunc)offsetwrapper_repr, //slot_repr
    (reprfunc)offsetwrapper_repr, //slot_str
    0, //slot_call

    0, //slot_number
    0, //slot_mapping

    0, //slot_methods
    0, //slot_getsets
    0, //slot_offsets

    0, //slot_cmp

    offsetwrapper_offsetget, //slot_offsetget
    offsetwrapper_offsetset, //slot_offsetset
};


TypeObject OffsetWrapperReadonlyType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("OffsetWrapperReadonlyType"), //name
    sizeof(OffsetWrapperObject), //size
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

    (reprfunc)offsetwrapper_repr, //slot_repr
    (reprfunc)offsetwrapper_repr, //slot_str
    0, //slot_call

    0, //slot_number
    0, //slot_mapping

    0, //slot_methods
    0, //slot_getsets
    0, //slot_offsets

    0, //slot_cmp

    offsetwrapper_offsetget, //slot_offsetget
    0, //slot_offsetset
};

void setup_offsetwrapper_type(){
    OffsetWrapperType=(*(TypeObject*)finalize_type(&OffsetWrapperType));
    OffsetWrapperType.slot_new=NULL;
    fplbases.push_back(&OffsetWrapperType);
}

void setup_offsetwrapperreadonly_type(){
    OffsetWrapperReadonlyType=(*(TypeObject*)finalize_type(&OffsetWrapperReadonlyType));
    OffsetWrapperReadonlyType.slot_new=NULL;
    fplbases.push_back(&OffsetWrapperReadonlyType);
}


TypeObject SlotWrapperReadonlyType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("SlotWrapperReadonlyType"), //name
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
    0, //slot_call

    0, //slot_number
    0, //slot_mapping

    0, //slot_methods
    0, //slot_getsets
    0, //slot_offsets

    0, //slot_cmp

    (offsetgetfunc)slotwrapper_offsetget, //slot_offsetget
    0, //slot_offsetget
};

void setup_slotwrapperreadoly_type(){
    SlotWrapperReadonlyType=(*(TypeObject*)finalize_type(&SlotWrapperReadonlyType));
    fplbases.push_back(&SlotWrapperReadonlyType);
}



object* new_type(string* name, object* bases, object* dict);

object* type_new(object* type, object* args, object* kwargs){
    //Argument size checking
    if (CAST_INT(list_len(args))->val->to_long()!=3){
        vm_add_err(&ValueError, vm, "Expected 3 arguments, got %d",CAST_INT(list_len(args))->val->to_long());
        return NULL;
    }
    //
    if (!object_istype(list_index_int(args, 0)->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected first argument to be string, got type '%s'",list_index_int(args, 0)->type->name->c_str());
        return NULL;
    }
    if (!object_istype(list_index_int(args, 1)->type, &ListType) || \
    !object_istype(list_index_int(args, 1)->type, &TupleType)){
        vm_add_err(&ValueError, vm, "Expected first argument to be list or tuple, got type '%s'",list_index_int(args, 0)->type->name->c_str());
        return NULL;
    }
    if (!object_istype(list_index_int(args, 2)->type, &DictType)){
        vm_add_err(&ValueError, vm, "Expected first argument to be dict, got type '%s'",list_index_int(args, 0)->type->name->c_str());
        return NULL;
    }
    //
    string* name=CAST_STRING(list_index_int(args, 0))->val;
    object* bases=INCREF(list_index_int(args, 1));
    object* dict=INCREF(list_index_int(args, 2));
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
            return (object*)(list_index_int(args, 0)->type);
        }
        if (CAST_INT(list_len(args))->val->to_long()==3){
            object* args_=new_dict();
            args_->type->slot_mappings->slot_set(args_, str_new_fromstr("func"), list_index_int(args, 0));
            args_->type->slot_mappings->slot_set(args_, str_new_fromstr("name"), list_index_int(args, 1));
            args_->type->slot_mappings->slot_set(args_, str_new_fromstr("bases"), list_index_int(args, 2));
            return builtin___build_class__(NULL, args_);
        }
        vm_add_err(&ValueError, vm, "'type' takes 1 or 3 arguments");
        return NULL;
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
        TypeObject* base_tp=CAST_TYPE(list_index_int(self->type->bases, i-1));

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
    m->slot_mod=base_tp->slot_number->slot_mod;
    m->slot_pow=base_tp->slot_number->slot_pow;

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
    object* tp=(object*)fpl_malloc(sizeof(TypeObject));
    memcpy(tp, newtype, sizeof(TypeObject));
        
    TypeObject* tp_tp=CAST_TYPE(tp);


    //tp is the what we'll copy to...

    //Clean out bases
    if (tp_tp->bases==NULL){
        tp_tp->bases=new_list();
    }

    uint32_t total_bases = CAST_INT(list_len(tp_tp->bases))->val->to_long_long();
    if (total_bases==0 || CAST_TYPE(list_index_int(tp_tp->bases, total_bases-1))!=&ObjectType){
        list_append(tp_tp->bases, (object*)&ObjectType);
        total_bases = CAST_INT(list_len(tp_tp->bases))->val->to_long_long();
    }
    //This is a slower method than could theoritically be done.
    //I could just use implied list indexing (uses my internal knowledge of ListObject), but this
    //also breaks fewer rules...
    
    

    NumberMethods* m=(NumberMethods*)fpl_malloc(sizeof(NumberMethods));
    memset(m, 0, sizeof(NumberMethods));

    Mappings* ma=(Mappings*)fpl_malloc(sizeof(Mappings));
    memset(ma, 0, sizeof(Mappings));
    for (uint32_t i=total_bases; i>0; i--){
        TypeObject* base_tp=CAST_TYPE(list_index_int(tp_tp->bases, i-1));
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
    if (tp_tp->dict==NULL){
        tp_tp->dict=new_dict();
    }

    //This is a slower method than could theoritically be done.
    //I could just use implied list indexing (uses my internal knowledge of ListObject), but this
    //also breaks fewer rules.
    
    for (uint32_t i=total_bases; i>0; i--){
        TypeObject* base_tp=CAST_TYPE(list_index_int(tp_tp->bases, i-1));
        object* dict=base_tp->dict;

        for (auto k: (*CAST_DICT(dict)->val)){
            dict_set(tp_tp->dict, k.first, k.second);
        }  
    }


    //Adding type slot wrappers
    if (tp_tp->slot_number!=NULL){
        if (tp_tp->slot_number->slot_add){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_add, "__add__");
        }
        if (tp_tp->slot_number->slot_sub){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_sub, "__sub__");
        }
        if (tp_tp->slot_number->slot_mul){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_mul, "__mul__");
        }
        if (tp_tp->slot_number->slot_div){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_div, "__div__");
        }
        if (tp_tp->slot_number->slot_mod){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_mod, "__mod__");
        }
        if (tp_tp->slot_number->slot_pow){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_pow, "__pow__");
        }

        
        if (tp_tp->slot_number->slot_neg){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_neg, "__neg__");
        }
        if (tp_tp->slot_number->slot_bool){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_bool, "__bool__");
        }
        if (tp_tp->slot_number->slot_int){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_int, "__int__");
        }
        if (tp_tp->slot_number->slot_float){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_float, "__float__");
        }
    }

    if (tp_tp->slot_call){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_call, "__call__");
    }
    if (tp_tp->slot_del){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_del, "__del__");
    }
    if (tp_tp->slot_init){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_init, "__init__");
    }
    if (tp_tp->slot_iter){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_iter, "__iter__");
    }
    if (tp_tp->slot_new){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_new, "__new__");
    }
    if (tp_tp->slot_next){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_next, "__next__");
    }
    if (tp_tp->slot_repr){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_repr, "__repr__");
    }
    if (tp_tp->slot_str){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_str, "__str__");
    }

    

    if (tp_tp->slot_cmp){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_eq, "__eq__");
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_ne, "__ne__");
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_gt, "__gt__");
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_lt, "__lt__");
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_gte, "__gte__");
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_lte, "__lte__");
    }
}

object* setup_type_methods(TypeObject* tp){
    TypeObject* tp_tp=CAST_TYPE(tp);

    //Inherit methods
    uint32_t idx=0;
    while (tp_tp->slot_methods!=NULL && tp_tp->slot_methods[idx].name!=NULL){
        dict_set(tp_tp->dict, str_new_fromstr(tp_tp->slot_methods[idx].name), cwrapper_new_fromfunc((cwrapperfunc)tp_tp->slot_methods[idx].function, tp_tp->slot_methods[idx].name));
        idx++;
    }

    return (object*)tp;
}

object* setup_type_getsets(TypeObject* tp){
    TypeObject* tp_tp=CAST_TYPE(tp);

    //Inherit methods
    uint32_t idx=0;
    while (tp_tp->slot_getsets!=NULL && tp_tp->slot_getsets[idx].name!=NULL){
        dict_set(tp_tp->dict, str_new_fromstr(tp_tp->slot_getsets[idx].name), slotwrapper_new_fromfunc((getter)tp_tp->slot_getsets[idx].get, (setter)tp_tp->slot_getsets[idx].set, tp_tp->slot_getsets[idx].name, tp_tp));
        idx++;
    }

    return (object*)tp;
}

object* setup_type_offsets(TypeObject* tp){
    TypeObject* tp_tp=CAST_TYPE(tp);

    //Inherit methods
    uint32_t idx=0;
    while (tp_tp->slot_offsets!=NULL && tp_tp->slot_offsets[idx].name!=NULL){
        dict_set(tp_tp->dict, str_new_fromstr(tp_tp->slot_offsets[idx].name), offsetwrapper_new_fromoffset(tp_tp->slot_offsets[idx].name, tp_tp->slot_offsets[idx].offset, tp_tp->slot_offsets[idx].readonly));
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

object* type_bases_get(object* type){
    return CAST_SLOTWRAPPER(type)->basetype->bases;
}

object* new_type(string* name, object* bases, object* dict){
    reprfunc repr_func=NULL;
    newfunc new_func=(newfunc)newtp_new;
    initfunc init_func=NULL;
    delfunc del_func=NULL;
    iternextfunc next_func=NULL;
    unaryfunc iter_func=NULL;
    reprfunc str_func=NULL;
    callfunc call_func=NULL;
    getattrfunc getattr_func=NULL;
    setattrfunc setattr_func=NULL;

    NumberMethods number=(*(NumberMethods*)fpl_malloc(sizeof(NumberMethods)));
    memset(&number, 0, sizeof(NumberMethods));

    Mappings mappings=(*(Mappings*)fpl_malloc(sizeof(Mappings)));
    memset(&mappings, 0, sizeof(Mappings));

    if (NEWTP_PRIMARY_COPY){
        object* n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__repr__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            repr_func=(reprfunc)newtp_repr;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__init__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            init_func=(initfunc)newtp_init;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__del__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            del_func=(delfunc)newtp_del;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__next__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            next_func=(iternextfunc)newtp_next;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__iter__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            iter_func=(unaryfunc)newtp_iter;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__str__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            str_func=(reprfunc)newtp_str;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__call__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            call_func=(callfunc)newtp_call;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__getattr__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
            getattr_func=object_genericgetattr;
        }
        else{
            getattr_func=(getattrfunc)newtp_getattr;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__setattr__"));
        object* dela=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__delattr__"));
        if (n==NULL && dela!=NULL){
            n=dela;
        }
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
            setattr_func=object_genericsetattr;
        }
        else{
            setattr_func=(setattrfunc)newtp_setattr;
        }
    }
    if (NEWTP_NUMBER_COPY){
        object* n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__add__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_add=(binopfunc)newtp_add;
        }
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__sub__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_sub=(binopfunc)newtp_sub;
        }     
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__mul__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_mul=(binopfunc)newtp_mul;
        }     
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__div__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_div=(binopfunc)newtp_div;
        }   
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__mod__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_mod=(binopfunc)newtp_mod;
        }     
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__pow__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_mod=(binopfunc)newtp_pow;
        }     
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__neg__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_neg=(unaryfunc)newtp_neg;
        }  
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__bool__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_bool=(unaryfunc)newtp_bool;
        }    
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__int__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_int=(unaryfunc)newtp_int;
        }    
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__float__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_float=(unaryfunc)newtp_float;
        }       
    }
    if (NEWTP_MAPPINGS_COPY){
        object* n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__getitem__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            mappings.slot_get=(getfunc)newtp_get;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__setitem__"));
        object* deli=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__delitem__"));
        if (n==NULL && deli!=NULL){
            n=deli;
        }
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            mappings.slot_set=(setfunc)newtp_set;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__len__"));
        if (n==NULL){
            DECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            mappings.slot_len=(lenfunc)newtp_len;
        }
    }

    uint32_t maxsize=0;
    uint32_t maxvarsize=0;
    uint32_t nfplbases=0;
    for (uint32_t i=0; i<CAST_LIST(bases)->size; i++){
        object* base_=list_index_int(bases, i);
        TypeObject* base;
        if (object_istype(base_->type, &TypeType)){
            base=CAST_TYPE(base_);
        }
        else{
            base=base_->type;
        }
        if (std::find(fplbases.begin(), fplbases.end(), base) != fplbases.end()\
        || (base->slot_next!=NULL && (base->size>0 && base->var_base_size>0) ) ){
            nfplbases++;
            if (nfplbases==2){
                vm_add_err(&TypeError, vm, "Cannot inherit from more than one builtin type");
                return NULL;
            }
        }

        if (base->size>maxsize){
            maxsize=base->size;
        }
        if (base->var_base_size>maxvarsize){
            maxvarsize=base->var_base_size;
        }
    }

    size_t size=maxvarsize;
    if (maxsize>maxvarsize){
        size=maxsize;
    }
    if (size==0){
        size=sizeof(object);
    }

    map<object*, object*> orig_dict(*CAST_DICT(dict)->val);

    TypeObject newtype={
        0, //refcnt
        0, //ob_prev
        0, //ob_next
        0, //gen
        &TypeType, //type
        name, //name
        maxsize+sizeof(object*)+sizeof(object), //size
        maxvarsize+sizeof(object*)+sizeof(object), //var_base_size
        true, //gc_trackable
        bases, //bases
        size, //dict_offset
        dict, //dict
        getattr_func, //slot_getattr
        setattr_func, //slot_setattr

        init_func, //slot_init
        new_func, //slot_new
        del_func, //slot_del

        next_func, //slot_next
        iter_func, //slot_iter

        repr_func, //slot_repr
        str_func, //slot_str
        call_func, //slot_call

        &number, //slot_number
        &newtp_mappings, //slot_mapping

        newtp_methods, //slot_methods
        newtp_getsets, //slot_getsets
        newtp_offsets, //slot_offsests
        
        (compfunc)newtp_cmp, //slot_cmp
        
        0, //slot_offsetget
        0, //slot_offsetset
    };
    
    object* tp=finalize_type(&newtype);
    inherit_type_dict((TypeObject*)tp);
    setup_type_getsets((TypeObject*)tp);
    setup_type_methods((TypeObject*)tp);
    setup_type_offsets((TypeObject*)tp);

    //Override slot wrappers added by inherit_type_dict
    for (auto k: orig_dict){
        for (auto l: *CAST_DICT(CAST_TYPE(tp)->dict)->val){
            if (istrue(object_cmp(l.first, k.first, CMP_EQ))){
                dict_set(CAST_TYPE(tp)->dict, l.first, k.second);
            }
        }
    }
    
    object* n=object_getattr(tp, str_new_fromstr("__dict__"));
    CAST_OFFSETWRAPPER(n)->offset=size;

    return tp;
}

object* code_co_names(object* code){
    return CAST_CODE(code)->co_names;
}

object* code_co_consts(object* code){
    return CAST_CODE(code)->co_consts;
}

object* code_co_file(object* code){
    return CAST_CODE(code)->co_file;
}

object* code_co_code(object* code){
    return CAST_CODE(code)->co_code;
}

object* code_co_lines(object* code){
    return CAST_CODE(code)->co_lines;    
}


#include "types_dict_wrappers.cpp"
#include "typeobject_newtp.cpp"