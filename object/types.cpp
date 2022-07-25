object* int_new(object* type, object* args, object* kwargs);
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

    (reprfunc)int_repr, //slot_repr
    (reprfunc)int_repr, //slot_str
    0, //slot_call

    &int_num_methods, //slot_number

    (compfunc)int_cmp, //slot_cmp
};

void setup_int_type(){
    finalize_type(&IntType);
}



object* str_new(object* type, object* args, object* kwargs);
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

    (reprfunc)str_repr, //slot_repr
    (reprfunc)str_str, //slot_str
    0, //slot_call

    0, //slot_number

    (compfunc)str_cmp, //slot_cmp
};

void setup_str_type(){
    finalize_type(&StrType);
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

#define CAST_LIST(obj) ((ListObject*)obj)


typedef struct ListObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
    size_t idx;
}ListObject;


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

    0, //slot_number

    (compfunc)list_cmp, //slot_cmp
};

void setup_list_type(){
    finalize_type(&ListType);
}



object* dict_new(object* type, object* args, object* kwargs);
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

    (reprfunc)dict_repr, //slot_repr
    (reprfunc)dict_repr, //slot_str
    0, //slot_call

    0, //slot_number

    (compfunc)dict_cmp, //slot_cmp
};

void setup_dict_type(){
    finalize_type(&DictType);
}



object* code_init(object* self, object* args, object* kwargs);
void code_del(object* self);
object* code_new(object* type, object* args, object* kwargs);
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

    (reprfunc)code_repr, //slot_repr
    (reprfunc)code_repr, //slot_str
    0, //slot_call

    0, //slot_number

    (compfunc)code_cmp, //slot_cmp
};

void setup_code_type(){
    finalize_type(&CodeType);
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

    (reprfunc)bool_repr, //slot_repr
    (reprfunc)bool_repr, //slot_str
    0, //slot_call

    &bool_num_methods, //slot_number

    (compfunc)bool_cmp, //slot_cmp
};

void setup_bool_type(){
    finalize_type(&BoolType);
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

#define CAST_TUPLE(obj) ((TupleObject*)obj)


typedef struct TupleObject{
    OBJHEAD_VAR
    object** array; //Pointer to array
    size_t capacity;
    size_t size;
    size_t idx;
}TupleObject;

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

    0, //slot_number

    (compfunc)tuple_cmp, //slot_cmp
};

void setup_tuple_type(){
    finalize_type(&TupleType);
}


void func_del(object* self);
object* func_new(object* type, object* args, object* kwargs);
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

    (reprfunc)func_repr, //slot_repr
    (reprfunc)func_repr, //slot_str
    (callfunc)func_call, //slot_call

    0, //slot_number

    (compfunc)func_cmp, //slot_cmp
};

void setup_func_type(){
    finalize_type(&FuncType);
}


object* none_new( object* args, object* kwargs);
void none_del(object* self);
object* none_repr(object* self);
object* none_cmp(object* self, object* other, uint8_t type);

#define CAST_NONE(obj) ((NoneObject*)obj)

typedef struct NoneObject{
    OBJHEAD_EXTRA
}NoneObject;

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

    (reprfunc)none_repr, //slot_repr
    (reprfunc)none_repr, //slot_str
    0, //slot_call

    0, //slot_number

    (compfunc)none_cmp, //slot_cmp
};

void setup_none_type(){
    finalize_type(&NoneType);
}


void builtin_del(object* self);
object* builtin_repr(object* self);
object* builtin_cmp(object* self, object* other, uint8_t type);
object* builtin_call(object* self, object* args, object* kwargs);

#define CAST_BUILTIN(obj) ((BuiltinObject*)obj)

typedef struct BuiltinObject{
    OBJHEAD_EXTRA
    object* (*function)(object*, object*);
    object* name;
    object* args; //Tuple
    object* kwargs; //Tuple
    uint32_t argc;
}BuiltinObject;

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

    (reprfunc)builtin_repr, //slot_repr
    (reprfunc)builtin_repr, //slot_str
    (callfunc)builtin_call, //slot_call

    0, //slot_number

    (compfunc)builtin_cmp, //slot_cmp
};

void setup_builtin_type(){
    finalize_type(&BuiltinType);
}


object* object_new(object* type, object* args, object* kwargs);
object* object_repr_(object* self);
object* object_init(object* self, object* args, object* kwargs);
object* object_cmp_(object* self, object* other, uint8_t type);

#define CAST_BOBJ(obj) ((ClassObject*)obj)


typedef struct ObjectObject{
    OBJHEAD_EXTRA
}ObjectObject;

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

    (reprfunc)object_repr_, //slot_repr
    (reprfunc)object_repr_, //slot_str
    0, //slot_call

    0, //slot_number

    (compfunc)object_cmp_, //slot_cmp
};

void setup_object_type(){
    finalize_type(&ObjectType);
}



object* type_new(object* type, object* args, object* kwargs){
    return new_none();
}

void type_del(object* self){}

object* type_repr(object* self){
    string s="<class '"+(*CAST_TYPE(self)->name)+"'>";
    return str_new_fromstr(new string(s));
}

object* type_call(object* self, object* args, object* kwargs){
    object* o=CAST_TYPE(self)->slot_new(self, args, kwargs);
    o->type->slot_init(o, args, kwargs);
    return o;
}

object* type_get(object* self, object* attr){
    //ESSENTIALLY the same as object_genericgetattr, but this checks metatype

    // metatype should alwaye be TypeType.? right??
    
    //Check us and then our bases

    //Check instance dict
    if ( CAST_TYPE(self)->dict_offset!=0){
        object* dict= (*(object**)((char*)self + self->type->dict_offset));
        if (object_find_bool_dict_keys(dict, attr)){
            return dict->type->slot_get(dict, attr);
        }
    }
    //Check type dict
    if (CAST_TYPE(self)->dict!=0){
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


    vm_add_err(vm, "AttributeError: %s has no attribute '%s'",object_cstr(self).c_str(), object_cstr(attr).c_str());
    return NULL;
}

void type_set(object* obj, object* attr, object* val){
    //Check dict
    if (CAST_TYPE(obj)->dict_offset!=0){
        object* dict= (*(object**)((char*)obj + CAST_TYPE(obj)->dict_offset));
        dict->type->slot_set(dict, attr, val);
        return;
    }
    vm_add_err(vm, "AttributeError: %s is read only",object_cstr(obj).c_str());
    return;
}

void _inherit_slots(TypeObject* tp_tp, TypeObject* base_tp){
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

    //Inheritance of number methods could be updated to be special
    SETSLOT(tp_tp, base_tp, slot_number);
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
    
    for (uint32_t i=total_bases; i>0; i--){
        TypeObject* base_tp=CAST_TYPE(tp_tp->bases->type->slot_get(tp_tp->bases, new_int_fromint(i-1)));
        //Dirty inheritance here... go over each
        _inherit_slots(tp_tp, base_tp);
        //
    }
    //Hack to ensure retention of original slots
    _inherit_slots(tp_tp, newtype);

    tp_tp->refcnt=1;

    return tp;
}