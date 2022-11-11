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
object* int_not(object* self);
object* int_and(object* self, object* other);
object* int_or(object* self, object* other);
object* int_lshift(object* self, object* other);
object* int_rshift(object* self, object* other);
object* int_fldiv(object* self, object* other);
object* int_abs(object* self);
object* int_xor(object* self, object* other);

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
    (binopfunc)int_and, //slot_and
    (binopfunc)int_or, //slot_or
    (binopfunc)int_lshift, //slot_lshift
    (binopfunc)int_rshift, //slot_rshift
    (binopfunc)int_fldiv, //slot_fldiv
    (binopfunc)int_xor, //slot_xor

    (unaryfunc)int_neg, //slot_neg
    (unaryfunc)int_not, //slot_not
    (unaryfunc)int_abs, //slot_abs

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
    sizeof(IntObject), //size
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

object* str_add(object* self, object* other);
object* str_get(object* self, object* idx);
object* str_mul(object* self, object* other);

object* str_new_fromstr(string val);
object* string_join_meth(object* selftp, object* args, object* kwargs);
object* string_replace_meth(object* selftp, object* args, object* kwargs);
object* string_find_meth(object* selftp, object* args, object* kwargs);
object* string_split_meth(object* selftp, object* args, object* kwargs);
object* string_upper_meth(object* selftp, object* args, object* kwargs);
object* string_lower_meth(object* selftp, object* args, object* kwargs);
object* string_isspace_meth(object* selftp, object* args, object* kwargs);
object* string_isalpha_meth(object* selftp, object* args, object* kwargs);
object* string_isnumeric_meth(object* selftp, object* args, object* kwargs);
object* string_isupper_meth(object* selftp, object* args, object* kwargs);
object* string_islower_meth(object* selftp, object* args, object* kwargs);
object* string_count_meth(object* selftp, object* args, object* kwargs);
object* string_strip_meth(object* selftp, object* args, object* kwargs);
object* string_rstrip_meth(object* selftp, object* args, object* kwargs);
object* string_lstrip_meth(object* selftp, object* args, object* kwargs);
object* string_contains_meth(object* selftp, object* args, object* kwargs);
object* str_in(object* self, object* other);

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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    (unaryfunc)str_bool, //slot_bool
    (unaryfunc)str_int, //slot_int
    (unaryfunc)str_float, //slot_float
};

static Mappings str_mappings{
    str_get, //slot_get
    0, //slot_set
    str_len, //slot_len
};

Method str_methods[]={{"find", (cwrapperfunc)string_find_meth}, {"replace", (cwrapperfunc)string_replace_meth}\
                    , {"join", (cwrapperfunc)string_join_meth}, {"split", (cwrapperfunc)string_split_meth}\
                    , {"upper", (cwrapperfunc)string_upper_meth}, {"lower", (cwrapperfunc)string_lower_meth}\
                    , {"isnumeric", (cwrapperfunc)string_isnumeric_meth}, {"isalpha", (cwrapperfunc)string_isalpha_meth}\
                    , {"isspace", (cwrapperfunc)string_isspace_meth}, {"isupper", (cwrapperfunc)string_isupper_meth}\
                    , {"islower", (cwrapperfunc)string_islower_meth}, {"count", (cwrapperfunc)string_count_meth}\
                    , {"strip", (cwrapperfunc)string_strip_meth}, {"rstrip", (cwrapperfunc)string_rstrip_meth}\
                    , {"lstrip", (cwrapperfunc)string_lstrip_meth}, {"contains", (cwrapperfunc)string_contains_meth}, {NULL,NULL}};
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
object* list_append_meth(object* selftp, object* args, object* kwargs);
object* list_iter(object* self);
object* list_pop(object* self);
object* list_pop_meth(object* selftp, object* args, object* kwargs);
object* list_replace_meth(object* selftp, object* args, object* kwargs);
object* list_find_meth(object* selftp, object* args, object* kwargs);
object* list_remove_meth(object* selftp, object* args, object* kwargs);
object* list_insert_meth(object* selftp, object* args, object* kwargs);
object* list_extend_meth(object* selftp, object* args, object* kwargs);

object* list_add(object* self, object* other);
object* list_mul(object* self, object* other);

typedef struct ListObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
}ListObject;

Method list_methods[]={{"find", (cwrapperfunc)list_find_meth}, {"replace", (cwrapperfunc)list_replace_meth}, {"append", (cwrapperfunc)list_append_meth},\
                    {"pop", (cwrapperfunc)list_pop_meth}, {"remove", (cwrapperfunc)list_remove_meth}, {"insert", (cwrapperfunc)list_insert_meth},\
                    {"extend", (cwrapperfunc)list_extend_meth}, {NULL,NULL}};
GetSets list_getsets[]={{NULL,NULL}};
OffsetMember list_offsets[]={{NULL}};

static NumberMethods list_num_methods{
    (binopfunc)list_add, //slot_add
    0, //slot_sub
    (binopfunc)list_mul, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
object* dict_keys_meth(object* selftp, object* args, object* kwargs);
object* dict_values_meth(object* selftp, object* args, object* kwargs);
object* dict_flip_meth(object* selftp, object* args, object* kwargs);

typedef struct DictObject{
    OBJHEAD_VAR
    unordered_map<object*, object*>* val;
    vector<object*>* keys;
}DictObject;

static NumberMethods dict_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    (unaryfunc)dict_bool, //slot_bool
};

static Mappings dict_mappings{
    dict_get, //slot_get
    dict_set, //slot_set
    dict_len, //slot_len
};

Method dict_methods[]={{"flip", (cwrapperfunc)dict_flip_meth}, {"keys", (cwrapperfunc)dict_keys_meth}, {"values", (cwrapperfunc)dict_values_meth},{NULL,NULL}};
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
object* code_co_stack_size(object* code);
object* code_co_blockstack_size(object* code);

typedef struct CodeObject{
    OBJHEAD_EXTRA
    object* co_names;
    object* co_consts;
    object* co_code;
    object* co_file;
    object* co_lines;
    uint32_t co_instructions;
    string* filedata;
    uint32_t* code;
    object* co_detailed_lines;
    object* co_stack_size;
    object* co_blockstack_size;
}CodeObject;

static NumberMethods code_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
{"co_stack_size", (getter)code_co_stack_size}, {"co_blockstack_size", (getter)code_co_blockstack_size}, {NULL,NULL}};
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
object* bool_and(object* self, object* other);
object* bool_or(object* self, object* other);
object* bool_not(object* self);
object* bool_lshift(object* self, object* other);
object* bool_rshift(object* self, object* other);
object* bool_fldiv(object* self, object* other);
object* bool_abs(object* self);
object* bool_xor(object* self, object* other);

object* new_bool_true();
object* new_bool_false();


typedef struct BoolObject{
    OBJHEAD_EXTRA
    uint8_t val;
}BoolObject;

bool istrue(object* boolean){
    if (boolean!=NULL && CAST_BOOL(boolean)->val==1){
        FPLDECREF(boolean);
        return true;
    }
    if (boolean!=NULL){
        FPLDECREF(boolean);
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
    (binopfunc)bool_and, //slot_and
    (binopfunc)bool_or, //slot_or
    (binopfunc)bool_lshift, //slot_lshift
    (binopfunc)bool_rshift, //slot_rshift
    (binopfunc)bool_fldiv, //slot_fldiv
    (binopfunc)bool_xor, //slot_xor

    (unaryfunc)bool_neg, //slot_neg
    (unaryfunc)bool_not, //slot_not
    (unaryfunc)bool_abs, //slot_abs

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

object* tuple_find_meth(object* selftp, object* args, object* kwargs);

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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
object* func_descrget(object* obj, object* self, object* owner);

typedef struct FuncObject{
    OBJHEAD_EXTRA
    object* code;
    object* dict;
    object* args; //Tuple
    object* kwargs; //Tuple
    uint32_t argc;
    object* name;
    object* closure;
    int functype;
    int flags;
    object* stargs;
    object* stkwargs;
    object* annotations;
    bool isgen;
    object* closure_annotations;
    object* globals;
    object* global_anno;
}FuncObject;

static NumberMethods func_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    (unaryfunc)func_bool, //slot_bool
};

static Mappings func_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method func_methods[]={{NULL,NULL}};
GetSets func_getsets[]={{NULL,NULL}};
OffsetMember func_offsets[]={{"__annotations__", offsetof(FuncObject, annotations), true}, {NULL}};

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

    func_descrget, //slot_descrget
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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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


typedef object* (*builtinfunc)(object*, object*);
object* new_builtin(builtinfunc function, object* name, object* args, object* kwargs, uint32_t argc, bool nargs);

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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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

    object* invalidopr_bases=new_tuple();
    invalidopr_bases->type->slot_mappings->slot_append(invalidopr_bases, (object*)&TypeError);
    InvalidOperationError=(*(TypeObject*)new_type_exception(new string("InvalidOperationError"), invalidopr_bases, new_dict()));
    fplbases.push_back(&InvalidOperationError);

    KeyboardInterrupt=(*(TypeObject*)new_type_exception(new string("KeyboardInterrupt"), new_tuple(), new_dict()));
    fplbases.push_back(&KeyboardInterrupt);

    AssertionError=(*(TypeObject*)new_type_exception(new string("AssertionError"), new_tuple(), new_dict()));
    fplbases.push_back(&AssertionError);

    object* zerodiv_bases=new_tuple();
    zerodiv_bases->type->slot_mappings->slot_append(zerodiv_bases, (object*)&ValueError);
    ZeroDivisionError=(*(TypeObject*)new_type_exception(new string("ZeroDivisionError"), zerodiv_bases, new_dict()));
    fplbases.push_back(&ZeroDivisionError);
    
    OSError=(*(TypeObject*)new_type_exception(new string("OSError"), new_tuple(), new_dict()));
    fplbases.push_back(&OSError);

    object* filenotfound_bases=new_tuple();
    filenotfound_bases->type->slot_mappings->slot_append(filenotfound_bases, (object*)&OSError);
    FileNotFoundError=(*(TypeObject*)new_type_exception(new string("FileNotFoundError"), filenotfound_bases, new_dict()));
    fplbases.push_back(&FileNotFoundError);

    object* importerr_bases=new_tuple();
    importerr_bases->type->slot_mappings->slot_append(importerr_bases, (object*)&FileNotFoundError);
    ImportError=(*(TypeObject*)new_type_exception(new string("ImportError"), importerr_bases, new_dict()));
    fplbases.push_back(&ImportError);
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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
object* file_read_meth(object* selftp, object* args, object* kwargs);
object* file_close_meth(object* selftp, object* args, object* kwargs);
object* file_write_meth(object* selftp, object* args, object* kwargs);
object* file_seek_meth(object* selftp, object* args, object* kwargs);
object* file_size_meth(object* selftp, object* args, object* kwargs);
object* file_flush_meth(object* selftp, object* args, object* kwargs);
object* file_enter(object* self);
object* file_exit(object* self);

Method file_methods[]={{"read", (cwrapperfunc)file_read_meth}, {"close", (cwrapperfunc)file_close_meth}\
                    , {"write", (cwrapperfunc)file_write_meth}, {"seek", (cwrapperfunc)file_seek_meth}\
                    , {"size", (cwrapperfunc)file_size_meth}, {"flush", (cwrapperfunc)file_flush_meth}, {NULL,NULL}};
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

    0, //slot_descrget
    0, //slot_descrset

    file_enter, //slot_enter
    file_exit, //slot_exit
};

void setup_file_type(){
    FileType=(*(TypeObject*)finalize_type(&FileType));
    fplbases.push_back(&FileType);
}

object* cwrapper_call(object* self, object* args, object* kwargs);
object* cwrapper_new_fromfunc(cwrapperfunc func, string name, object* tp);
object* cwrapper_repr(object* self);
void cwrapper_del(object* self);
object* cwrapper_new_fromfunc_null(cwrapperfunc func, string name);
object* cwrapper_descrget(object* obj, object* self, object* owner);

typedef struct CWrapperObject{
    OBJHEAD_EXTRA
    cwrapperfunc function;
    string* name;
    object* tp;
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
    cwrapper_del, //slot_del

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

    (descrgetfunc)cwrapper_descrget, //slot_descrget
    0, //slot_descrset
};

void setup_cwrapper_type(){
    CWrapperType=(*(TypeObject*)finalize_type(&CWrapperType));
    CWrapperType.slot_new=NULL;
    fplbases.push_back(&CWrapperType);
}

object* slotwrapper_new_fromfunc(getter get, setter set, string name);
object* slotwrapper_new(object* type, object* args, object* kwargs);    
object* slotwrapper_repr(object* self);
object* slotwrapper_str(object* self);
object* slotwrapper_descrget(object* obj, object* self, object* owner);
object* slotwrapper_descrset(object* obj, object* self, object* val);

typedef struct SlotWrapperObject{
    OBJHEAD_EXTRA
    getter get;
    setter set;
    string* name;
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

    (descrgetfunc)slotwrapper_descrget, //slot_descrget
    (descrsetfunc)slotwrapper_descrset, //slot_descrget
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
object* float_float(object* self);
object* float_int(object* self);
object* float_mod(object* self, object* other);
object* float_pow(object* self, object* other);
object* float_fldiv(object* self, object* other);
object* float_abs(object* self);

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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    (binopfunc)float_fldiv, //slot_fldiv
    0, //slot_xor


    (unaryfunc)float_neg, //slot_neg
    0, //slot_not
    (unaryfunc)float_abs, //slot_abs

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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
    ListIterType.slot_new=NULL;
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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
    TupleIterType.slot_new=NULL;
    fplbases.push_back(&TupleIterType);
}


void dict_iter_del(object* self);
object* dict_iter_repr(object* self);
object* dict_iter_next(object* self);
object* dict_iter_cmp(object* self, object* other, uint8_t type);
object* dict_iter_bool(object* self);

typedef struct DictIterObject{
    OBJHEAD_VAR
    unordered_map<object*, object*>* val;
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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
    DictIterType.slot_new=NULL;
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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
    StrIterType.slot_new=NULL;
    fplbases.push_back(&StrIterType);
}


void module_del(object* self);
object* module_repr(object* self);
object* module_cmp(object* self, object* other, uint8_t type);
object* module_new_fromdict(object* dict, object* name);
object* module_dict(object* self);
object* module_getattr(object* self, object* attr);

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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
    module_getattr, //slot_getattr
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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
    uint32_t step;
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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

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
object* offsetwrapper_descrget(object* obj, object* self, object* owner);
object* offsetwrapper_descrset(object* obj, object* self, object* val);

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

    offsetwrapper_descrget, //slot_descrget
    offsetwrapper_descrset, //slot_descrset
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

    offsetwrapper_descrget, //slot_descrget
    0, //slot_descrset
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

    (descrgetfunc)slotwrapper_descrget, //slot_descrget
    0, //slot_descrget
};

void setup_slotwrapperreadoly_type(){
    SlotWrapperReadonlyType=(*(TypeObject*)finalize_type(&SlotWrapperReadonlyType));
    fplbases.push_back(&SlotWrapperReadonlyType);
}

void super_del(object* self);
object* super_new(object* type, object* args, object* kwargs);
object* super_cmp(object* self, object* other, uint8_t type);
object* super_getattr(object* self, object* attr);


typedef struct SuperObject{
    OBJHEAD_VAR
    object* ob;
}SuperObjects;

Method super_methods[]={{NULL,NULL}};
GetSets super_getsets[]={{NULL,NULL}};

static NumberMethods super_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    0, //slot_bool
};

static Mappings super_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject SuperType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("super"), //name
    sizeof(SuperObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    super_getattr, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    super_new, //slot_new
    super_del, //slot_del

    0, //slot_next
    0, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &super_num_methods, //slot_number
    &super_mappings, //slot_mapping

    super_methods, //slot_methods
    super_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)super_cmp, //slot_cmp
};

void setup_super_type(){
    SuperType=(*(TypeObject*)finalize_type(&SuperType));
    fplbases.push_back(&SuperType);
}

void method_del(object* self);
object* method_new(object* type, object* args, object* kwargs);
object* method_cmp(object* self, object* other, uint8_t type);
object* method_call(object* self, object* args, object* kwargs);
object* method_repr(object* self);
object* method_new_impl(object* func, object* instance);

typedef struct MethodObject{
    OBJHEAD_VAR
    object* function;
    object* instance;
}MethodObject;

Method method_methods[]={{NULL,NULL}};
GetSets method_getsets[]={{NULL,NULL}};

static NumberMethods method_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    0, //slot_bool
};

static Mappings method_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject MethodType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("method"), //name
    sizeof(MethodObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    object_genericgetattr, //slot_getattr
    object_genericsetattr, //slot_setattr

    0, //slot_init
    method_new, //slot_new
    method_del, //slot_del

    0, //slot_next
    0, //slot_iter

    method_repr, //slot_repr
    method_repr, //slot_str
    method_call, //slot_call

    &method_num_methods, //slot_number
    &method_mappings, //slot_mapping

    method_methods, //slot_methods
    method_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)method_cmp, //slot_cmp
};

void setup_method_type(){
    MethodType=(*(TypeObject*)finalize_type(&MethodType));
    fplbases.push_back(&MethodType);
}



Method wrappermethod_methods[]={{NULL,NULL}};
GetSets wrappermethod_getsets[]={{NULL,NULL}};

static NumberMethods wrappermethod_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    0, //slot_bool
};

static Mappings wrappermethod_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

object* wrappermethod_call(object* self, object* args, object* kwargs);
object* wrappermethod_new_impl(object* func, object* instance);
object* wrappermethod_repr(object* self);

TypeObject WrapperMethodType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("wrapper_method"), //name
    sizeof(MethodObject), //size
    0, //var_base_size
    true, //gc_trackable
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

    wrappermethod_repr, //slot_repr
    wrappermethod_repr, //slot_str
    wrappermethod_call, //slot_call

    &method_num_methods, //slot_number
    &method_mappings, //slot_mapping

    method_methods, //slot_methods
    method_getsets, //slot_getsets
    0, //slot_offsets

    0, //slot_cmp
};

void setup_wrappermethod_type(){
    WrapperMethodType.bases=new_tuple();
    tuple_append(WrapperMethodType.bases, (object*)(&MethodType));
    WrapperMethodType=(*(TypeObject*)finalize_type(&WrapperMethodType));
    fplbases.push_back(&WrapperMethodType);
}


object* staticmethod_descrget(object* instance, object* self, object* owner);
object* staticmethod_repr(object* self);
object* staticmethod_new(object* cls, object* args, object* kwargs);
void staticmethod_del(object* self);
object* staticmethod_cmp(object* self, object* other, uint8_t type);

Method staticmethod_methods[]={{NULL,NULL}};
GetSets staticmethod_getsets[]={{NULL,NULL}};

static NumberMethods staticmethod_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    0, //slot_bool
};

static Mappings staticmethod_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

typedef struct StaticMethodObject{
    OBJHEAD_VAR
    object* function;
}StaticMethodObject;

TypeObject StaticMethodType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("staticmethod"), //name
    sizeof(StaticMethodObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    staticmethod_new, //slot_new
    staticmethod_del, //slot_del

    0, //slot_next
    0, //slot_iter

    staticmethod_repr, //slot_repr
    staticmethod_repr, //slot_str
    0, //slot_call

    &staticmethod_num_methods, //slot_number
    &staticmethod_mappings, //slot_mapping

    staticmethod_methods, //slot_methods
    staticmethod_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)staticmethod_cmp, //slot_cmp

    staticmethod_descrget, //slot_descrget
};

void setup_staticmethod_type(){
    StaticMethodType=(*(TypeObject*)finalize_type(&StaticMethodType));
    fplbases.push_back(&StaticMethodType);
}

object* classmethod_repr(object* self);
object* classmethod_new(object* cls, object* args, object* kwargs);
void classmethod_del(object* self);
object* classmethod_cmp(object* self, object* other, uint8_t type);
object* classmethod_descrget(object* instance, object* self, object* owner);

Method classmethod_methods[]={{NULL,NULL}};
GetSets classmethod_getsets[]={{NULL,NULL}};

static NumberMethods classmethod_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    0, //slot_bool
};

static Mappings classmethod_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

typedef struct ClassMethodObject{
    OBJHEAD_VAR
    object* function;
}ClassethodObject;

TypeObject ClassMethodType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("classmethod"), //name
    sizeof(ClassMethodObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    classmethod_new, //slot_new
    classmethod_del, //slot_del

    0, //slot_next
    0, //slot_iter

    classmethod_repr, //slot_repr
    classmethod_repr, //slot_str
    0, //slot_call

    &classmethod_num_methods, //slot_number
    &classmethod_mappings, //slot_mapping

    classmethod_methods, //slot_methods
    classmethod_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)classmethod_cmp, //slot_cmp

    classmethod_descrget, //slot_descrget
};

void setup_classmethod_type(){
    ClassMethodType=(*(TypeObject*)finalize_type(&ClassMethodType));
    fplbases.push_back(&ClassMethodType);
}



void map_del(object* self);
object* map_next(object* self);
object* map_new(object* type, object* args, object* kwargs);
object* map_cmp(object* self, object* other, uint8_t type);


typedef struct MapObject{
    OBJHEAD_VAR
    object** iterators;
    size_t n_iterators;
    uint32_t idx;
    object* func;
}MapObject;

Method map_methods[]={{NULL,NULL}};
GetSets map_getsets[]={{NULL,NULL}};

static NumberMethods map_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    0, //slot_bool
};

static Mappings map_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject MapType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("map"), //name
    sizeof(MapObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    map_new, //slot_new
    0, //slot_del

    (iternextfunc)map_next, //slot_next
    (unaryfunc)generic_iter_iter, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &map_num_methods, //slot_number
    &map_mappings, //slot_mapping

    map_methods, //slot_methods
    map_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)map_cmp, //slot_cmp
};

void setup_map_type(){
    MapType=(*(TypeObject*)finalize_type(&MapType));
    fplbases.push_back(&MapType);
}

void property_del(object* self);
object* property_new(object* type, object* args, object* kwargs);
object* property_cmp(object* self, object* other, uint8_t type);
object* property_call(object* self, object* args, object* kwargs);
object* property_repr(object* self);
object* property_descrget(object* instance, object* self, object* owner);
object* property_descrset(object* instance, object* self, object* value);
object* property_getter(object* self, object* args, object* kwargs);
object* property_setter(object* self, object* args, object* kwargs);
object* property_deleter(object* self, object* args, object* kwargs);

typedef struct PropertyObject{
    OBJHEAD_VAR
    object* get;
    object* set;
    object* del;
}PropertyObject;

Method property_methods[]={{"getter", property_getter}, {"setter", property_setter},\
                        {"deleter", property_deleter}, {NULL,NULL}};
GetSets property_getsets[]={{NULL,NULL}};

static NumberMethods property_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    0, //slot_bool
};

static Mappings property_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject PropertyType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("property"), //name
    sizeof(PropertyObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    property_new, //slot_new
    property_del, //slot_del

    0, //slot_next
    0, //slot_iter

    property_repr, //slot_repr
    property_repr, //slot_str
    0, //slot_call

    &property_num_methods, //slot_number
    &property_mappings, //slot_mapping

    property_methods, //slot_methods
    property_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)property_cmp, //slot_cmp

    property_descrget, //slot_descrget
    property_descrset, //slot_descrset
};

void setup_property_type(){
    PropertyType=(*(TypeObject*)finalize_type(&PropertyType));
    fplbases.push_back(&PropertyType);
}


void gen_del(object* self);
object* gen_repr(object* self);
object* gen_cmp(object* self, object* other, uint8_t type);
object* gen_next(object* self);
object* new_generator_impl(object* func, object* locals);

typedef struct GeneratorObject{
    OBJHEAD_EXTRA
    object* func;
    object* locals;
    uint32_t ip;
    bool done;
    struct datastack* objstack;
    struct callstack* callstack;
    struct blockstack* blockstack;
}GeneratorObject;

static NumberMethods gen_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    0, //slot_bool
};

static Mappings gen_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

Method gen_methods[]={{NULL,NULL}};
GetSets gen_getsets[]={{NULL,NULL}};
OffsetMember gen_offsets[]={{NULL}};

TypeObject GeneratorType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("generator"), //name
    sizeof(GeneratorObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    (getattrfunc)object_genericgetattr, //slot_getattr
    (setattrfunc)object_genericsetattr, //slot_setattr

    0, //slot_init
    0, //slot_new
    (delfunc)gen_del, //slot_del

    gen_next, //slot_next   
    generic_iter_iter, //slot_iter

    (reprfunc)gen_repr, //slot_repr
    (reprfunc)gen_repr, //slot_str
    0, //slot_call

    &gen_num_methods, //slot_number
    &gen_mappings, //slot_mapping

    gen_methods, //slot_methods
    gen_getsets, //slot_getsets
    gen_offsets, //slot_offsests

    (compfunc)gen_cmp, //slot_cmp
};

void setup_gen_type(){
    GeneratorType=(*(TypeObject*)finalize_type(&GeneratorType));
    fplbases.push_back(&GeneratorType);
}


object* set_init(object* self, object* args, object* kwargs);
void set_del(object* self);
object* set_len(object* self);
void set_append(object* self, object* obj);
object* set_new(object* type, object* args, object* kwargs);
object* set_repr(object* self);
object* set_next(object* self);
object* set_cmp(object* self, object* other, uint8_t type);
object* set_bool(object* self);
object* set_iter(object* self);

object* set_find_meth(object* selftp, object* args, object* kwargs);
object* set_add_meth(object* selftp, object* args, object* kwargs);
object* set_remove_meth(object* selftp, object* args, object* kwargs);
object* set_union_meth(object* selftp, object* args, object* kwargs);

typedef struct SetObject{
    OBJHEAD_VAR
    vector<object*>* vec;
}SetObject;

static NumberMethods set_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    (unaryfunc)set_bool, //slot_bool
};


static Mappings set_mappings{
    0, //slot_get
    0, //slot_set
    set_len, //slot_len
    set_append, //slot_append
};

Method set_methods[]={{"find", (cwrapperfunc)set_find_meth}, {"add", (cwrapperfunc)set_add_meth}, {"remove", (cwrapperfunc)set_remove_meth}, {"union", (cwrapperfunc)set_union_meth}, {NULL,NULL}};
GetSets set_getsets[]={{NULL,NULL}};
OffsetMember set_offsets[]={{NULL}};

TypeObject SetType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("set"), //name
    sizeof(SetObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)set_new, //slot_new
    (delfunc)set_del, //slot_del

    0, //slot_next
    (unaryfunc)set_iter, //slot_iter

    (reprfunc)set_repr, //slot_repr
    (reprfunc)set_repr, //slot_str
    0, //slot_call

    &set_num_methods, //slot_number
    &set_mappings, //slot_mapping

    set_methods, //slot_methods
    set_getsets, //slot_getsets
    set_offsets, //slot_offsests

    (compfunc)set_cmp, //slot_cmp
};

void setup_set_type(){
    SetType=(*(TypeObject*)finalize_type(&SetType));
    fplbases.push_back(&SetType);
}


void set_iter_del(object* self);
object* set_iter_repr(object* self);
object* set_iter_next(object* self);
object* set_iter_cmp(object* self, object* other, uint8_t type);
object* set_iter_bool(object* self);

typedef struct SetIterObject{
    OBJHEAD_VAR
    vector<object*>* vec;
    uint32_t idx;
}SetIterObject;

Method set_iter_methods[]={{NULL,NULL}};
GetSets set_iter_getsets[]={{NULL,NULL}};

static NumberMethods set_iter_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    (unaryfunc)set_iter_bool, //slot_bool
};

static Mappings set_iter_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject SetIterType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("set_iter"), //name
    0, //size
    sizeof(SetIterObject), //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    object_genericgetattr, //slot_getattr
    object_genericsetattr, //slot_setattr

    0, //slot_init
    0, //slot_new
    (delfunc)set_iter_del, //slot_del

    (iternextfunc)set_iter_next, //slot_next
    (unaryfunc)generic_iter_iter, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &set_iter_num_methods, //slot_number
    &set_iter_mappings, //slot_mapping

    set_iter_methods, //slot_methods
    set_iter_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)set_iter_cmp, //slot_cmp
};

void setup_setiter_type(){
    SetIterType=(*(TypeObject*)finalize_type(&SetIterType));
    SetIterType.slot_new=NULL;
    fplbases.push_back(&SetIterType);
}

object* bytes_new(object* type, object* args, object* kwargs);
void bytes_del(object* self);
object* bytes_len(object* self);
object* bytes_repr(object* self);
object* bytes_str(object* self);
object* bytes_cmp(object* self, object* other, uint8_t type);
object* bytes_bool(object* self);
object* bytes_iter(object* self);

object* bytes_add(object* self, object* other);
object* bytes_get(object* self, object* idx);
object* bytes_mul(object* self, object* other);

object* bytes_new_frombytearr(char* val, int lne);

typedef struct BytesObject{
    OBJHEAD_EXTRA
    char* val;
    uint32_t len;
}BytesObject;

static NumberMethods bytes_num_methods{
    (binopfunc)bytes_add, //slot_add
    0, //slot_sub
    (binopfunc)bytes_mul, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    0, //slot_bool
    0, //slot_int
    0, //slot_float
};

static Mappings bytes_mappings{
    bytes_get, //slot_get
    0, //slot_set
    bytes_len, //slot_len
};

Method bytes_methods[]={{NULL,NULL}};
GetSets bytes_getsets[]={{NULL,NULL}};
OffsetMember bytes_offsets[]={{NULL}};

TypeObject BytesType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("bytes"), //name
    sizeof(BytesObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    (newfunc)bytes_new, //slot_new
    (delfunc)bytes_del, //slot_del

    0, //slot_next
    (unaryfunc)bytes_iter, //slot_iter

    (reprfunc)bytes_repr, //slot_repr
    (reprfunc)bytes_str, //slot_str
    0, //slot_call

    &bytes_num_methods, //slot_number
    &bytes_mappings, //slot_mapping

    bytes_methods, //slot_methodsS
    bytes_getsets, //slot_getsets
    bytes_offsets, //slot_offsests

    (compfunc)bytes_cmp, //slot_cmp
};

void setup_bytes_type(){
    BytesType=(*(TypeObject*)finalize_type(&BytesType));
    fplbases.push_back(&BytesType);
}


void bytes_iter_del(object* self);
object* bytes_iter_repr(object* self);
object* bytes_iter_next(object* self);
object* bytes_iter_cmp(object* self, object* other, uint8_t type);
object* bytes_iter_bool(object* self);

typedef struct BytesIterObject{
    OBJHEAD_VAR
    char* val;
    uint32_t len;
    uint32_t idx;
}BytesIterObject;

Method bytes_iter_methods[]={{NULL,NULL}};
GetSets bytes_iter_getsets[]={{NULL,NULL}};

static NumberMethods bytes_iter_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    0, //slot_bool
};

static Mappings bytes_iter_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject BytesIterType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("bytes_iter"), //name
    0, //size
    sizeof(BytesIterObject), //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    object_genericgetattr, //slot_getattr
    object_genericsetattr, //slot_setattr

    0, //slot_init
    0, //slot_new
    (delfunc)bytes_iter_del, //slot_del

    (iternextfunc)bytes_iter_next, //slot_next
    (unaryfunc)generic_iter_iter, //slot_iter

    0, //slot_repr
    0, //slot_str
    0, //slot_call

    &bytes_iter_num_methods, //slot_number
    &bytes_iter_mappings, //slot_mapping

    bytes_iter_methods, //slot_methods
    bytes_iter_getsets, //slot_getsets
    0, //slot_offsets

    (compfunc)bytes_iter_cmp, //slot_cmp
};

void setup_bytesiter_type(){
    BytesIterType=(*(TypeObject*)finalize_type(&BytesIterType));
    BytesIterType.slot_new=NULL;
    fplbases.push_back(&BytesIterType);
}






////////////////////////////////////////////////////////////////////////////////////////

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
    object* o=list_index_int(args, 1);
    FPLINCREF(o);
    object* bases=o;
    o=list_index_int(args, 2);
    FPLINCREF(o);
    object* dict=o;
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
    ERROR_RET_NOCALLERR(o);
    if (o != NULL && o->type->slot_posttpcall!=NULL){
        o->type->slot_posttpcall(o);
    }
    if (o != NULL && o->type->slot_init!=NULL){
        object* v=o->type->slot_init(o, args, kwargs);
        ERROR_RET_NOCALLERR(v);
    }
    return o;
}

object* type_get(object* self, object* attr){
    //ESSENTIALLY the same as object_genericgetattr, but this checks metatype

    // metatype should alwaye be TypeType.? right??
    
    //Check us and then our bases

    object* res=NULL;
    //First check metatype
    
    //Check type dict
    TypeObject* tpfound=NULL;
    if (self->type->dict!=0){
        object* dict = self->type->dict;
        if (object_find_bool_dict_keys(dict, attr)){
            tpfound=self->type;
            res=dict_get(dict, attr);
        }
    }

    if (res==NULL){
        uint32_t total_bases = CAST_INT(list_len(self->type->bases))->val->to_long_long();
        for (uint32_t i=total_bases; i>0; i--){
            TypeObject* base_tp=CAST_TYPE(list_index_int(self->type->bases, i-1));

            //Check type dict
            if (base_tp->dict!=0){
                object* dict = base_tp->dict;
                if (object_find_bool_dict_keys(dict, attr)){
                    res=dict_get(dict, attr);
                    tpfound=base_tp;
                    break;
                }
            }
        }
    }

    if (res!=NULL){
        if (res->type->slot_descrget!=NULL){
            object* r=res->type->slot_descrget(self, res, (object*)tpfound);
            return r;
        }
    }
    

    //Check self dict
    if (res==NULL && CAST_TYPE(self)->dict!=0){
        object* dict = CAST_TYPE(self)->dict;
        if (object_find_bool_dict_keys(dict, attr)){
            res=dict_get(dict, attr);
        }
    }

    if (res==NULL){
        uint32_t total_bases = CAST_INT(list_len(CAST_TYPE(self)->bases))->val->to_long_long();
        for (uint32_t i=total_bases; i>0; i--){
            TypeObject* base_tp=CAST_TYPE(list_index_int(CAST_TYPE(self)->bases, i-1));

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
        vm_add_err(&AttributeError, vm, "%s has no attribute '%s'",CAST_TYPE(self)->name->c_str(), object_cstr(attr).c_str());
    }
    else{
        if (res->type->slot_descrget!=NULL){
            object* r=res->type->slot_descrget(self, res, (object*)tpfound);
            return r;
        }
    }
    
    return res;
}

void type_set(object* obj, object* attr, object* val){
    //Check dict
    
    if (obj->type->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + obj->type->dict_offset));
        dict_set(dict, attr, val);
        return;
    }
    vm_add_err(&AttributeError, vm, "%s object is read only",obj->type->name->c_str());
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
    m->slot_not=base_tp->slot_number->slot_not;
    m->slot_and=base_tp->slot_number->slot_and;
    m->slot_or=base_tp->slot_number->slot_or;
    m->slot_lshift=base_tp->slot_number->slot_lshift;
    m->slot_rshift=base_tp->slot_number->slot_rshift;
    m->slot_fldiv=base_tp->slot_number->slot_fldiv;
    m->slot_abs=base_tp->slot_number->slot_abs;
    m->slot_xor=base_tp->slot_number->slot_xor;

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
    
    SETSLOT(tp_tp, base_tp, slot_descrget);
    SETSLOT(tp_tp, base_tp, slot_descrset);

    SETSLOT(tp_tp, base_tp, slot_enter);
    SETSLOT(tp_tp, base_tp, slot_exit);

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

void inherit_type_dict_nofill(TypeObject* tp){
    TypeObject* tp_tp=CAST_TYPE(tp);

    uint32_t total_bases = CAST_INT(list_len(tp_tp->bases))->val->to_long_long();

    
    //Setup type dict
    if (tp_tp->dict==NULL){
        tp_tp->dict=new_dict();
    }

    //This is a slower method than could theoritically be done.
    //I could just use implied list indexing (uses my internal knowledge of ListObject), but this
    //also breaks fewer rules.
    
    for (uint32_t i=total_bases; i>1; i--){
        TypeObject* base_tp=CAST_TYPE(list_index_int(tp_tp->bases, i-1));
        object* dict=base_tp->dict;

        for (auto k: (*CAST_DICT(dict)->val)){
            dict_set(tp_tp->dict, k.first, k.second);
        }  
    }

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
        if (tp_tp->slot_number->slot_and){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_and, "__and__");
        }
        if (tp_tp->slot_number->slot_or){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_or, "__or__");
        }
        if (tp_tp->slot_number->slot_lshift){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_lshift, "__lshift__");
        }
        if (tp_tp->slot_number->slot_rshift){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_rshift, "__rshift__");
        }
        if (tp_tp->slot_number->slot_fldiv){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_fldiv, "__floordiv__");
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
        if (tp_tp->slot_number->slot_abs){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_abs, "__abs__");
        }
        if (tp_tp->slot_number->slot_not){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_not, "__not__");
        }
        if (tp_tp->slot_number->slot_xor){
            type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_xor, "__xor__");
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
    if (tp_tp->slot_descrget){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_descrget, "__get__");
    }
    if (tp_tp->slot_descrset){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_descrset, "__set__");
    }
    if (tp_tp->slot_getattr){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_getattr, "__getattr__");
    }
    if (tp_tp->slot_setattr){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_setattr, "__setattr__");
    }
    if (tp_tp->slot_enter){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_enter, "__enter__");
    }
    if (tp_tp->slot_exit){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_exit, "__exit__");
    }

    

    if (tp_tp->slot_cmp){
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_eq, "__eq__");
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_ne, "__ne__");
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_gt, "__gt__");
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_lt, "__lt__");
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_gte, "__gte__");
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_lte, "__lte__");
        type_set_cwrapper(tp, (cwrapperfunc)type_wrapper_in, "__in__");
    }
}

object* setup_type_methods(TypeObject* tp){
    TypeObject* tp_tp=CAST_TYPE(tp);

    //Inherit methods
    uint32_t idx=0;
    while (tp_tp->slot_methods!=NULL && tp_tp->slot_methods[idx].name!=NULL){
        dict_set(tp_tp->dict, str_new_fromstr(tp_tp->slot_methods[idx].name), cwrapper_new_fromfunc((cwrapperfunc)tp_tp->slot_methods[idx].function, tp_tp->slot_methods[idx].name, (object*)tp_tp));
        idx++;
    }

    return (object*)tp;
}

object* setup_type_getsets(TypeObject* tp){
    TypeObject* tp_tp=CAST_TYPE(tp);

    uint32_t total_bases = CAST_INT(list_len(tp_tp->bases))->val->to_long_long();
    
    for (uint32_t i=total_bases; i>0; i--){
        TypeObject* base_tp=CAST_TYPE(list_index_int(tp_tp->bases, i-1));
        //Inherit methods
        uint32_t idx=0;
        while (base_tp->slot_getsets!=NULL && base_tp->slot_getsets[idx].name!=NULL){
            dict_set(base_tp->dict, str_new_fromstr(base_tp->slot_getsets[idx].name), slotwrapper_new_fromfunc((getter)base_tp->slot_getsets[idx].get, (setter)base_tp->slot_getsets[idx].set, base_tp->slot_getsets[idx].name));
            idx++;
        }
    }

    //Inherit methods
    uint32_t idx=0;
    while (tp_tp->slot_getsets!=NULL && tp_tp->slot_getsets[idx].name!=NULL){
        dict_set(tp_tp->dict, str_new_fromstr(tp_tp->slot_getsets[idx].name), slotwrapper_new_fromfunc((getter)tp_tp->slot_getsets[idx].get, (setter)tp_tp->slot_getsets[idx].set, tp_tp->slot_getsets[idx].name));
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
    object* f=cwrapper_new_fromfunc(func, name, (object*)tp);
    dict_set(tp->dict, str_new_fromstr(*CAST_CWRAPPER(f)->name), f);
}

object* type_bool(object* self){
    return new_bool_true();
}

object* type_dict(object* type){
    if (!object_istype(type->type, &TypeType)){
        return type->type->dict;
    }
    return CAST_TYPE(type)->dict;
}

object* type_bases_get(object* type){
    if (!object_istype(type->type, &TypeType)){
        return type->type->bases;
    }
    return CAST_TYPE(type)->bases;
}

object* new_type(string* name, object* bases, object* dict){
    reprfunc repr_func=NULL;
    newfunc new_func=NULL;
    initfunc init_func=NULL;
    delfunc del_func=_newtp_del;
    iternextfunc next_func=NULL;
    unaryfunc iter_func=NULL;
    reprfunc str_func=NULL;
    callfunc call_func=NULL;
    getattrfunc getattr_func=NULL;
    setattrfunc setattr_func=NULL;
    descrgetfunc get_func=NULL;
    descrsetfunc set_func=NULL;
    unaryfunc enter_func=NULL;
    unaryfunc exit_func=NULL;

    NumberMethods number=(*(NumberMethods*)fpl_malloc(sizeof(NumberMethods)));
    memset(&number, 0, sizeof(NumberMethods));

    Mappings mappings=(*(Mappings*)fpl_malloc(sizeof(Mappings)));
    memset(&mappings, 0, sizeof(Mappings));

    if (NEWTP_PRIMARY_COPY){
        object* n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__repr__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            repr_func=(reprfunc)newtp_repr;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__init__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            init_func=(initfunc)newtp_init;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__new__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            new_func=(newfunc)newtp_new;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__del__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            del_func=(delfunc)newtp_del;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__next__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            next_func=(iternextfunc)newtp_next;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__iter__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            iter_func=(unaryfunc)newtp_iter;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__str__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            str_func=(reprfunc)newtp_str;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__call__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            call_func=(callfunc)newtp_call;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__get__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            get_func=(descrgetfunc)newtp_descrget;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__set__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;

            n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__delete__"));
            if (n==NULL){
                FPLDECREF(vm->exception);
                vm->exception=NULL;
            }
            else{
                set_func=(descrsetfunc)newtp_descrset;
            }
        }
        else{
            set_func=(descrsetfunc)newtp_descrset;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__enter__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            enter_func=newtp_enter;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__exit__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            exit_func=newtp_exit;
        }
    }
    if (NEWTP_NUMBER_COPY){
        object* n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__add__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_add=(binopfunc)newtp_add;
        }
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__sub__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_sub=(binopfunc)newtp_sub;
        }     
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__mul__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_mul=(binopfunc)newtp_mul;
        }     
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__div__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_div=(binopfunc)newtp_div;
        }   
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__mod__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_mod=(binopfunc)newtp_mod;
        }     
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__pow__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_mod=(binopfunc)newtp_pow;
        }     
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__neg__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_neg=(unaryfunc)newtp_neg;
        }  
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__bool__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_bool=(unaryfunc)newtp_bool;
        }    
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__int__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_int=(unaryfunc)newtp_int;
        }    
        
        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__float__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_float=(unaryfunc)newtp_float;
        }     

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__not__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_not=(unaryfunc)newtp_not;
        }      

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__and__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_and=(binopfunc)newtp_and;
        }      

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__or__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_or=(binopfunc)newtp_or;
        }       

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__lshift__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_lshift=(binopfunc)newtp_lshift;
        }      

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__rshift__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_rshift=(binopfunc)newtp_rshift;
        }   

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__floordiv__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_fldiv=(binopfunc)newtp_fldiv;
        }       

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__abs__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_abs=(unaryfunc)newtp_abs;
        }        

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__xor__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            number.slot_xor=(binopfunc)newtp_xor;
        }   
    }
    if (NEWTP_MAPPINGS_COPY){
        object* n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__getitem__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
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
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            mappings.slot_set=(setfunc)newtp_set;
        }

        n=dict->type->slot_mappings->slot_get(dict, str_new_fromstr("__len__"));
        if (n==NULL){
            FPLDECREF(vm->exception);
            vm->exception=NULL;
        }
        else{
            mappings.slot_len=(lenfunc)newtp_len;
        }
    }

    if (repr_func==NULL && str_func!=NULL){
        repr_func=str_func;
    }
    else if (repr_func!=NULL && str_func==NULL){
        str_func=repr_func;
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
        size=offsetof(NewTypeObject, dict);
    }

    unordered_map<object*, object*> orig_dict(*CAST_DICT(dict)->val);

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
        newtp_getattr, //slot_getattr
        newtp_setattr, //slot_setattr

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
        
        get_func, //slot_descrget
        set_func, //slot_descrset
        
        enter_func, //slot_enter
        exit_func, //slot_exit

        newtp_post_tpcall, //slot_posttpcall
    };
    object* tp=finalize_type(&newtype);
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
    
    object* n=dict_get(CAST_TYPE(tp)->dict, str_new_fromstr("__dict__"));
    CAST_OFFSETWRAPPER(n)->offset=size;

    vector<string> to_del;
    to_del.clear();
    for (auto k: (*CAST_DICT(CAST_TYPE(tp)->dict)->val)){
        string s=object_cstr(k.first);

        string new_name="_"+(*name)+s;
        object* nw=str_new_fromstr(new_name);

        if (s.size()>=2 && s.rfind("__", 0)==0 && s.rfind("__", s.size()-2)!=s.size()-2 &&\
        CAST_DICT(CAST_TYPE(tp)->dict)->val->find(nw)==CAST_DICT(CAST_TYPE(tp)->dict)->val->end()){
            object* o=k.second;
            dict_set(CAST_TYPE(tp)->dict, str_new_fromstr("_"+(*name)+s), o);
            to_del.push_back(s);
        }

        FPLDECREF(nw);
    }

    for (string s: to_del){
        dict_set(CAST_TYPE(tp)->dict, str_new_fromstr(s), NULL);
    }
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
    return CAST_CODE(code)->co_detailed_lines;    
}

object* code_co_stack_size(object* code){
    return CAST_CODE(code)->co_stack_size;    
}

object* code_co_blockstack_size(object* code){
    return CAST_CODE(code)->co_stack_size;    
}


#include "types_dict_wrappers.cpp"
#include "typeobject_newtp.cpp"