object* int_new(object* args, object* kwargs);
void int_del(object* self);
object* int_add(object* self, object* other);
object* int_sub(object* self, object* other);
object* int_mul(object* self, object* other);
object* int_div(object* self, object* other);
object* int_neg(object* self);
object* int_repr(object* self);
object* int_cmp(object* self, object* other, uint8_t type);

object* new_int_fromint(int v);
object* new_int_fromstr(string* v);
object* new_int_frombigint(BigInt* v);

#define CAST_INT(obj) ((IntObject*)obj)

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
};

TypeObject IntType={
    new string("int"), //name
    sizeof(IntObject)+sizeof(BigInt), //size
    false, //gc_trackable

    0, //slot_init
    (newfunc)int_new, //slot_new
    (delfunc)int_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append

    (reprfunc)int_repr, //slot_repr
    (reprfunc)int_repr, //slot_str
    0, //slot_call

    &int_num_methods, //slot_num

    (compfunc)int_cmp, //slot_cmp
};



object* str_new(object* args, object* kwargs);
void str_del(object* self);
object* str_len(object* self);
object* str_repr(object* self);
object* str_str(object* self);
object* str_cmp(object* self, object* other, uint8_t type);

object* str_new_fromstr(string* val);

#define CAST_STRING(obj) ((StrObject*)obj)

typedef struct StrObject{
    OBJHEAD_VAR
    string* val;
}StrObject;

TypeObject StrType={
    new string("str"), //name
    0, //size
    false, //gc_trackable

    0, //slot_init
    (newfunc)str_new, //slot_new
    (delfunc)str_del, //slot_del

    0, //slot_next
    0, //slot_get
    (lenfunc)str_len, //slot_len
    0, //slot_set
    0, //slot_append

    (reprfunc)str_repr, //slot_repr
    (reprfunc)str_str, //slot_str
    0, //slot_call

    0, //slot_num

    (compfunc)str_cmp, //slot_cmp
};



object* list_init(object* self, object* args, object* kwargs);
void list_del(object* self);
object* list_len(object* self);
object* list_get(object* self, object* idx);
void list_append(object* self, object* obj);
object* list_new(object* args, object* kwargs);
void list_set(object* self, object* idx, object* val);
object* list_repr(object* self);
object* list_next(object* self);
object* list_cmp(object* self, object* other, uint8_t type);

#define CAST_LIST(obj) ((ListObject*)obj)


typedef struct ListObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
    size_t idx;
}ListObject;

TypeObject ListType={
    new string("list"), //name
    0, //size
    true, //gc_trackable

    (initfunc)list_init, //slot_init
    (newfunc)list_new, //slot_new
    (delfunc)list_del, //slot_del

    (iternextfunc)list_next, //slot_next
    (getfunc)list_get, //slot_get
    (lenfunc)list_len, //slot_len
    (setfunc)list_set, //slot_set
    (appendfunc)list_append, //slot_append

    (reprfunc)list_repr, //slot_repr
    (reprfunc)list_repr, //slot_str
    0, //slot_call

    0, //slot_num

    (compfunc)list_cmp, //slot_cmp
};



object* dict_new(object* args, object* kwargs);
void dict_del(object* self);
object* dict_len(object* self);
object* dict_get(object* self, object* idx);
void dict_set(object* self, object* key, object* val);
object* dict_repr(object* self);
object* dict_cmp(object* self, object* other, uint8_t type);

#define CAST_DICT(obj) ((DictObject*)obj)


typedef struct DictObject{
    OBJHEAD_VAR
    unordered_map<object*, object*>* val;
}DictObject;

static TypeObject DictType={
    new string("dict"), //name
    0, //size
    true, //gc_trackable

    0, //slot_init
    (newfunc)dict_new, //slot_new
    (delfunc)dict_del, //slot_del

    0, //slot_next
    (getfunc)dict_get, //slot_get
    (lenfunc)dict_len, //slot_len
    (setfunc)dict_set, //slot_set
    0, //slot_appends

    (reprfunc)dict_repr, //slot_repr
    (reprfunc)dict_repr, //slot_str
    0, //slot_call

    0, //slot_num

    (compfunc)dict_cmp, //slot_cmp
};


object* code_init(object* self, object* args, object* kwargs);
void code_del(object* self);
object* code_new(object* args, object* kwargs);
object* code_repr(object* self);
object* code_cmp(object* self, object* other, uint8_t type);

#define CAST_CODE(obj) ((CodeObject*)obj)


typedef struct CodeObject{
    OBJHEAD_EXTRA
    object* co_names;
    object* co_consts;
    object* co_code;
    object* co_file;
    object* co_lines;
}CodeObject;

TypeObject CodeType={
    new string("code"), //name
    sizeof(CodeObject), //size
    true, //gc_trackable

    (initfunc)code_init, //slot_init
    (newfunc)code_new, //slot_new
    (delfunc)code_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append

    (reprfunc)code_repr, //slot_repr
    (reprfunc)code_repr, //slot_str
    0, //slot_call

    0, //slot_num

    (compfunc)code_cmp, //slot_cmp
};


object* bool_new( object* args, object* kwargs);
void bool_del(object* self);
object* bool_add(object* self, object* other);
object* bool_sub(object* self, object* other);
object* bool_mul(object* self, object* other);
object* bool_div(object* self, object* other);
object* bool_neg(object* self);
object* bool_repr(object* self);
object* bool_cmp(object* self, object* other, uint8_t type);

object* new_bool_true();
object* new_bool_false();

#define CAST_BOOL(obj) ((BoolObject*)obj)

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
};

TypeObject BoolType={
    new string("bool"), //name
    sizeof(BoolObject), //size
    false, //gc_trackable

    0, //slot_init
    (newfunc)bool_new, //slot_new
    (delfunc)bool_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append

    (reprfunc)bool_repr, //slot_repr
    (reprfunc)bool_repr, //slot_str
    0, //slot_call

    &bool_num_methods, //slot_num

    (compfunc)bool_cmp, //slot_cmp
};


object* tuple_init(object* self, object* args, object* kwargs);
void tuple_del(object* self);
object* tuple_len(object* self);
object* tuple_get(object* self, object* idx);
void tuple_append(object* self, object* obj);
object* tuple_new(object* args, object* kwargs);
object* tuple_repr(object* self);
object* tuple_next(object* self);
object* tuple_cmp(object* self, object* other, uint8_t type);

#define CAST_TUPLE(obj) ((TupleObject*)obj)


typedef struct TupleObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
    size_t idx;
}TupleObject;

TypeObject TupleType={
    new string("tuple"), //name
    0, //size
    true, //gc_trackable

    (initfunc)tuple_init, //slot_init
    (newfunc)tuple_new, //slot_new
    (delfunc)list_del, //slot_del

    (iternextfunc)tuple_next, //slot_next
    (getfunc)tuple_get, //slot_get
    (lenfunc)tuple_len, //slot_len
    0, //slot_set
    (appendfunc)tuple_append, //slot_append

    (reprfunc)tuple_repr, //slot_repr
    (reprfunc)tuple_repr, //slot_str
    0, //slot_call

    0, //slot_num

    (compfunc)tuple_cmp, //slot_cmp
};


void func_del(object* self);
object* func_new(object* args, object* kwargs);
object* func_repr(object* self);
object* func_cmp(object* self, object* other, uint8_t type);
object* func_call(object* self, object* args, object* callfunc);

#define CAST_FUNC(obj) ((FuncObject*)obj)


typedef struct FuncObject{
    OBJHEAD_EXTRA
    object* code;
    object* dict;
    object* args; //Tuple
    object* kwargs; //Tuple
    uint32_t argc;
    object* name;
}FuncObject;

TypeObject FuncType={
    new string("function"), //name
    sizeof(FuncObject), //size
    true, //gc_trackable

    0, //slot_init
    (newfunc)func_new, //slot_new
    (delfunc)func_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append

    (reprfunc)func_repr, //slot_repr
    (reprfunc)func_repr, //slot_str
    (callfunc)func_call, //slot_call

    0, //slot_num

    (compfunc)func_cmp, //slot_cmp
};


object* none_new( object* args, object* kwargs);
void none_del(object* self);
object* none_repr(object* self);
object* none_cmp(object* self, object* other, uint8_t type);

object* new_none();

#define CAST_NONE(obj) ((NoneObject*)obj)

typedef struct NoneObject{
    OBJHEAD_EXTRA
}NoneObject;

TypeObject NoneType={
    new string("NoneType"), //name
    sizeof(NoneObject), //size
    false, //gc_trackable

    0, //slot_init
    (newfunc)none_new, //slot_new
    (delfunc)none_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append

    (reprfunc)none_repr, //slot_repr
    (reprfunc)none_repr, //slot_str
    0, //slot_call

    0, //slot_num

    (compfunc)none_cmp, //slot_cmp
};