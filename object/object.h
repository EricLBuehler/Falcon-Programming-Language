#include "BigInt.hpp"

typedef struct object*(*initfunc)(struct object*, struct object*, struct object* );
typedef struct object*(*iternextfunc)(struct object*);
typedef void (*delfunc)(struct object*);
typedef struct object*(*binopfunc)(struct object*,struct object*);
typedef struct object*(*unaryfunc)(struct object*);
typedef struct object*(*getfunc)(struct object*, struct object*);
typedef struct object*(*newfunc)(object* type, object* args, object* kwargs);
typedef struct object*(*lenfunc)(struct object*);
typedef struct object*(*reprfunc)(struct object*);
typedef object* (*setfunc)(object*, object*, object*);
typedef void (*appendfunc)(object*, object*);
typedef struct object*(*compfunc)(struct object*, struct object*, uint8_t type);
typedef object* (*callfunc)(object*, object*, object*);
typedef object* (*getattrfunc)(object*, object*);
typedef void (*setattrfunc)(object*, object*, object*);
typedef void (*posttpcallfunc)(object*);


typedef object* (*cwrapperfunc)(object*, object*);
typedef object* (*getsetfunc)(object*);


typedef struct{    
    //binops
    binopfunc slot_add;
    binopfunc slot_sub;
    binopfunc slot_mul;
    binopfunc slot_div;
    binopfunc slot_mod;
    binopfunc slot_pow;

    //unaryops
    unaryfunc slot_neg;

    //other
    unaryfunc slot_bool;
    unaryfunc slot_int;
    unaryfunc slot_float;
}NumberMethods;

typedef struct{
    const char* name;
    cwrapperfunc function;
}Method;

typedef struct{
    const char* name;
    size_t offset;
}OffsetMember;

typedef struct{
    const char* name;
    getsetfunc function;
}GetSets;

typedef struct{    
    getfunc slot_get;
    setfunc slot_set;
    lenfunc slot_len;
    appendfunc slot_append;
}Mappings;

typedef struct object_type{
    size_t refcnt;
    object* ob_prev;
    object* ob_next;
    uint32_t gen;
    object_type* type;

    string* name;
    size_t size;
    size_t var_base_size;
    bool gc_trackable;
    object* bases;
    size_t dict_offset; //If 0, no dict
    object* dict; //None if no dict
    getattrfunc slot_getattr;
    setattrfunc slot_setattr;

    initfunc slot_init;
    newfunc slot_new;
    delfunc slot_del;
    
    //Iterators
    iternextfunc slot_next;
    unaryfunc slot_iter;

    reprfunc slot_repr;
    reprfunc slot_str;
    callfunc slot_call;
    
    //number methods
    NumberMethods* slot_number;
    Mappings* slot_mappings;
    Method* slot_methods;
    GetSets* slot_getsets;
    OffsetMember* slot_offsets;

    compfunc slot_cmp;
    posttpcallfunc slot_post_tpcall;
}TypeObject;

#define OBJHEAD size_t refcnt; struct object* ob_prev; struct object* ob_next; uint32_t gen;
#define OBJHEAD_EXTRA OBJHEAD TypeObject* type;
#define OBJHEAD_VAR OBJHEAD_EXTRA uint32_t var_size; uint32_t gc_ref;


typedef struct object{
    OBJHEAD_EXTRA
}object;

typedef struct object_var{
    OBJHEAD_VAR
}object_var;

static object* immutable_objs=NULL;
uint32_t immutable_size=0;
static object* trueobj=NULL;
static object* falseobj=NULL;
static object* noneobj=NULL;

const size_t nbuiltins=44;
object* builtins[nbuiltins];

TypeObject TypeError;
TypeObject ValueError;
TypeObject AttributeError;
TypeObject IndexError;
TypeObject KeyError;
TypeObject NameError;
TypeObject MemoryError;
TypeObject RecursionError;
TypeObject StopIteration;
TypeObject FileNotFoundError;
TypeObject InvalidOperationError;
TypeObject ImportError;
TypeObject KeyboardInterrupt;
TypeObject AssertionError;

bool setup_memory_error=false;
bool hit_memory_error=false;

Parser parser;


inline bool DECREF(struct object* object);
inline object* INCREF(struct object* object);
object* in_immutables(object* obj);
object* new_object(TypeObject* type);
object_var* new_object_var(TypeObject* type, size_t size);
bool object_istype(TypeObject* self, TypeObject* other);
void gc_collect(struct vm* vm);
void type_set_cwrapper(TypeObject* tp, cwrapperfunc func, string name);

object* object_str(object* obj);
object* object_repr(object* obj);
void object_print(object* obj);
string object_cstr(object* obj);
object* object_getattr(object* obj, object* attr);
void object_setattr(object* obj, object* attr, object* val);
object* object_genericgetattr(object* obj, object* attr);
void object_genericsetattr(object* obj, object* attr, object* val);
bool object_find_bool_dict_keys(object* dict, object* needle);
object* object_call(object* obj, object* args, object* kwargs);
string object_crepr(object* obj);
bool object_issubclass(object* obj, TypeObject* t);
object* generic_iter_iter(object* self);
object* object_getattr_noerror(object* obj, object* attr);

object* builtin___build_class__(object* self, object* args);

#define list_index_int(self, i) CAST_LIST(self)->array[i]
#define tuple_index_int(self, i) CAST_TUPLE(self)->array[i]

const bool NEWTP_PRIMARY_COPY=true;
const bool NEWTP_NUMBER_COPY=true;

vector<TypeObject*> fplbases;

string* glblfildata=NULL;

object* run_vm(object* codeobj, uint32_t* ip);
struct vm* new_vm(uint32_t id, object* code, struct instructions* instructions, string* filedata);

void vm_add_err(TypeObject* exception, struct vm* vm, const char *_format, ...);
object* vm_setup_err(TypeObject* exception, struct vm* vm, const char *_format, ...);

inline void add_dataframe(struct vm* vm, struct datastack* stack, struct object* obj);
inline object* pop_dataframe(struct datastack* stack);
inline void add_callframe(struct callstack* stack, object* line, string* name, object* code);
inline void pop_callframe(struct callstack* stack);
void print_traceback();

object* new_list();
object* new_none();
object* new_dict();
object* new_tuple();
object* new_code_fromargs(object* args);
object* new_bool_true();
object* new_bool_false();
object* str_new_fromstr(string val);
object* new_int_fromint(int i);

object* finalize_type(TypeObject* newtype);
void inherit_type_dict(TypeObject* tp);
object* setup_type_methods(TypeObject* tp);
object* setup_type_getsets(TypeObject* tp);
object* setup_type_offsets(TypeObject* tp);

object* type_wrapper_add(object* args, object* kwargs);
object* type_wrapper_sub(object* args, object* kwargs);
object* type_wrapper_mul(object* args, object* kwargs);
object* type_wrapper_div(object* args, object* kwargs);
object* type_wrapper_pow(object* args, object* kwargs);
object* type_wrapper_mod(object* args, object* kwargs);

object* type_wrapper_bool(object* args, object* kwargs);
object* type_wrapper_neg(object* args, object* kwargs);
object* type_wrapper_int(object* args, object* kwargs);
object* type_wrapper_float(object* args, object* kwargs);

object* type_wrapper_del(object* args, object* kwargs);
object* type_wrapper_init(object* args, object* kwargs);
object* type_wrapper_new(object* args, object* kwargs);
object* type_wrapper_iter(object* args, object* kwargs);
object* type_wrapper_next(object* args, object* kwargs);
object* type_wrapper_str(object* args, object* kwargs);
object* type_wrapper_repr(object* args, object* kwargs);
object* type_wrapper_call(object* args, object* kwargs);

object* type_wrapper_eq(object* args, object* kwargs);
object* type_wrapper_ne(object* args, object* kwargs);
object* type_wrapper_gt(object* args, object* kwargs);
object* type_wrapper_lt(object* args, object* kwargs);
object* type_wrapper_gte(object* args, object* kwargs);
object* type_wrapper_lte(object* args, object* kwargs);

struct vm* vm=NULL;

enum blocktype{
    TRY_BLOCK,
    FOR_BLOCK,
};


struct blockframe* in_blockstack(struct blockstack* stack, enum blocktype type);

struct dataframe{
    struct object* obj;
    struct dataframe* next;
};

struct callframe{
    object* line;
    string* name;
    struct callframe* next;
    struct object* locals;
    object* code;
    string* filedata;
};

struct blockframe{
    enum blocktype type;
    uint32_t arg;
    object* obj;
    uint32_t other;
    struct blockframe* next;
    uint32_t callstack_size;
    uint32_t start_ip;
};

struct callstack{
    struct callframe* head;
    uint32_t size;
};

struct blockstack{
    struct blockframe* head;
    uint32_t size;
};

struct datastack{
    struct dataframe* head;
    uint32_t size;
};

struct vm{
    struct datastack* objstack;
    struct callstack* callstack;
    struct blockstack* blockstack;
    uint32_t id;

    int ret_val;
    uint32_t ip;

    object* exception;
    string* filedata;

    object* accumulator;
    object* globals;
};

#define CAST_VAR(obj) ((object_var*)obj)
#define CAST_INT(obj) ((IntObject*)obj)
#define CAST_STRING(obj) ((StrObject*)obj)
#define CAST_LIST(obj) ((ListObject*)obj)
#define CAST_DICT(obj) ((DictObject*)obj)
#define CAST_CODE(obj) ((CodeObject*)obj)
#define CAST_BOOL(obj) ((BoolObject*)obj)
#define CAST_TUPLE(obj) ((TupleObject*)obj)
#define CAST_FUNC(obj) ((FuncObject*)obj)
#define CAST_NONE(obj) ((NoneObject*)obj)
#define CAST_BUILTIN(obj) ((BuiltinObject*)obj)
#define CAST_TYPE(obj) ((TypeObject*)obj)
#define CAST_EXCEPTION(obj) ((ExceptionObject*)obj)
#define CAST_FILE(obj) ((FileObject*)obj)
#define CAST_CWRAPPER(obj) ((CWrapperObject*)obj)
#define CAST_SLOTWRAPPER(obj) ((SlotWrapperObject*)obj)
#define CAST_FLOAT(obj) ((FloatObject*)obj)
#define CAST_LISTITER(obj) ((ListIterObject*)obj)
#define CAST_TUPLEITER(obj) ((TupleIterObject*)obj)
#define CAST_DICTITER(obj) ((DictIterObject*)obj)
#define CAST_STRITER(obj) ((StrIterObject*)obj)
#define CAST_FILE(obj) ((FileObject*)obj)
#define CAST_MODULE(obj) ((ModuleObject*)obj)
#define CAST_SLICE(obj) ((SliceObject*)obj)
#define CAST_ENUM(obj) ((EnumObject*)obj)
#define CAST_RANGE(obj) ((RangeObject*)obj)
#define CAST_ZIP(obj) ((ZipObject*)obj)


#define object_istype(this, other) (this==other)


#define CMP_EQ 0
#define CMP_GT 1
#define CMP_GTE 2
#define CMP_LT 3
#define CMP_LTE 4
#define CMP_NE 4

#define SETSLOT(tp, base, slot) if (base_tp->slot!=NULL){tp->slot=base->slot;}
object* finalize_type(TypeObject* newtype);

#ifdef DEBUG
ostream& operator<<(ostream& os, object* o){
    cout<<object_crepr(o);
    return os;
}

ostream& operator<<(ostream& os, TypeObject* o){
    cout<<object_crepr((object*)o);
    return os;
}
#endif

#include "typeobject.cpp"
#include "types.cpp"
#include "gc.h"
#include "object.cpp"
#include "../fpl/fpl.h"

#include "../builtins/builtins.h"
#include "integerobject.cpp"
#include "stringobject.cpp"
#include "listobject.cpp"
#include "dictobject.cpp"
#include "codeobject.cpp"
#include "boolobject.cpp"
#include "tupleobject.cpp"
#include "funcobject.cpp"
#include "noneobject.cpp"
#include "builtinobject.cpp"
#include "objectobject.cpp"
#include "exceptionobject.cpp"
#include "fileobject.cpp"
#include "cwrapperobject.cpp"
#include "slotwrapperobject.cpp"
#include "floatobject.cpp"
#include "moduleobject.cpp"
#include "sliceobject.cpp"
#include "enumobject.cpp"
#include "rangeobject.cpp"
#include "zipobject.cpp"

void setup_types_consts(){
    fplbases.clear();

    setup_object_type(); 

    trueobj=_new_bool_true();
    falseobj=_new_bool_false();
    noneobj=_new_none();

    setup_type_type();
    setup_int_type();
    setup_str_type();
    setup_list_type();
    setup_dict_type();
    setup_code_type();
    setup_bool_type();
    setup_tuple_type();
    setup_func_type();
    setup_none_type();
    setup_builtin_type();
    setup_exception_type();
    setup_file_type();  
    setup_cwrapper_type();  
    setup_slotwrapper_type();
    setup_float_type();
    setup_listiter_type();
    setup_tupleiter_type();
    setup_dictiter_type();
    setup_striter_type();
    setup_module_type();
    setup_slice_type();
    setup_enum_type();
    setup_range_type();
    setup_zip_type();

    setup_builtins();
    
    inherit_type_dict(&ObjectType);
    setup_type_getsets(&ObjectType);
    setup_type_offsets(&ObjectType);

    inherit_type_dict(&TypeType);
    setup_type_getsets(&TypeType);
    setup_type_offsets(&TypeType);

    inherit_type_dict(&IntType);
    setup_type_offsets(&IntType);
    setup_type_getsets(&IntType);

    inherit_type_dict(&StrType);
    setup_type_offsets(&StrType);
    setup_type_getsets(&StrType);
    setup_type_methods(&StrType);

    inherit_type_dict(&ListType);
    setup_type_methods(&ListType);
    setup_type_offsets(&ListType);
    setup_type_getsets(&ListType);

    inherit_type_dict(&DictType);    
    setup_type_offsets(&DictType);
    setup_type_getsets(&DictType);
    setup_type_methods(&DictType);

    inherit_type_dict(&CodeType);
    setup_type_offsets(&CodeType);
    setup_type_getsets(&CodeType);

    inherit_type_dict(&BoolType);
    setup_type_offsets(&BoolType);
    setup_type_getsets(&BoolType);

    inherit_type_dict(&TupleType);
    setup_type_offsets(&TupleType);
    setup_type_getsets(&TupleType);

    inherit_type_dict(&FuncType);
    setup_type_offsets(&FuncType);
    setup_type_getsets(&FuncType);

    inherit_type_dict(&NoneType);
    setup_type_offsets(&NoneType);
    setup_type_getsets(&NoneType);

    inherit_type_dict(&BuiltinType);
    setup_type_offsets(&BuiltinType);
    setup_type_getsets(&BuiltinType);

    inherit_type_dict(&ExceptionType);
    setup_type_offsets(&ExceptionType);
    setup_type_getsets(&ExceptionType);

    inherit_type_dict(&FileType);
    setup_type_offsets(&FileType);
    setup_type_methods(&FileType);
    setup_type_getsets(&FileType);

    inherit_type_dict(&CWrapperType);
    setup_type_offsets(&CWrapperType);
    setup_type_getsets(&CWrapperType);

    inherit_type_dict(&FloatType); 
    setup_type_offsets(&FloatType);
    setup_type_getsets(&FloatType);

    inherit_type_dict(&ListIterType); 
    setup_type_offsets(&ListIterType);
    setup_type_getsets(&ListIterType);

    inherit_type_dict(&TupleIterType);    
    setup_type_offsets(&TupleIterType);
    setup_type_getsets(&TupleIterType);

    inherit_type_dict(&StrIterType);    
    setup_type_offsets(&StrIterType);
    setup_type_getsets(&StrIterType);
    
    inherit_type_dict(&ModuleType);
    setup_type_offsets(&ModuleType);
    setup_type_getsets(&ModuleType);
    
    inherit_type_dict(&SliceType);
    setup_type_offsets(&SliceType);
    setup_type_getsets(&SliceType);

    inherit_type_dict(&EnumType);
    setup_type_offsets(&EnumType);
    setup_type_getsets(&EnumType);

    inherit_type_dict(&RangeType);
    setup_type_offsets(&RangeType);
    setup_type_getsets(&RangeType);

    inherit_type_dict(&ZipType);
    setup_type_offsets(&ZipType);
    setup_type_getsets(&ZipType);
}