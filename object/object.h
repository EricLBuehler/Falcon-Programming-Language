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
typedef void (*setfunc)(object*, object*, object*);
typedef void (*appendfunc)(object*, object*);
typedef struct object*(*compfunc)(struct object*, struct object*, uint8_t type);
typedef object* (*callfunc)(object*, object*, object*);
typedef object* (*getattrfunc)(object*, object*);
typedef void (*setattrfunc)(object*, object*, object*);


typedef object* (*cwrapperfunc)(object*, object*);


typedef struct{    
    //binops
    binopfunc slot_add;
    binopfunc slot_sub;
    binopfunc slot_mul;
    binopfunc slot_div;

    //unaryops
    unaryfunc slot_neg;

    //other
    unaryfunc slot_bool;
}NumberMethods;

typedef struct{
    const char* name;
    cwrapperfunc function;
}Method;

typedef struct object_type{
    size_t refcnt;
    object* ob_prev;
    object* ob_next;
    uint32_t gen;
    object_type* type;

    string* name;
    size_t size;
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
    getfunc slot_get;
    lenfunc slot_len;
    setfunc slot_set;
    appendfunc slot_append;

    reprfunc slot_repr;
    reprfunc slot_str;
    callfunc slot_call;
    
    //number methods
    NumberMethods* slot_number;
    Method* slot_methods;

    compfunc slot_cmp;
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
const size_t nbuiltins=20;
object* builtins[nbuiltins];

TypeObject TypeError;
TypeObject ValueError;
TypeObject AttributeError;
TypeObject IndexError;
TypeObject KeyError;
TypeObject NameError;
TypeObject MemoryError;
TypeObject RecursionError;

Parser parser;


bool DECREF(struct object* object);
struct object* INCREF(struct object* object);
object* in_immutables(object* obj);
object* new_object(TypeObject* type);
object_var* new_object_var(TypeObject* type, size_t size);
bool object_istype(TypeObject* self, TypeObject* other);
void gc_collect(struct vm* vm);

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

object* run_vm(object* codeobj, uint32_t* ip);
void vm_add_err(TypeObject* exception, struct vm* vm, const char *_format, ...);
object* vm_setup_err(TypeObject* exception, struct vm* vm, const char *_format, ...);
void add_dataframe(struct vm* vm, struct datastack* stack, struct object* obj);
struct object* pop_dataframe(struct datastack* stack);
void append_to_list(struct gc* gc, struct object* object);
void add_callframe(struct callstack* stack, object* line, string* name, object* code);
struct callframe* pop_callframe(struct callstack* stack);

object* new_list();
object* new_none();
object* new_dict();
object* new_tuple();
object* new_code_fromargs(object* args);
object* new_bool_true();
object* new_bool_false();
object* str_new_fromstr(string* val);
object* new_int_fromint(int i);

struct vm* vm=NULL;

enum blocktype{
    TRY_BLOCK,
};

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
#define CAST_BOBJ(obj) ((ClassObject*)obj)
#define CAST_TYPE(obj) ((TypeObject*)obj)
#define CAST_EXCEPTION(obj) ((ExceptionObject*)obj)
#define CAST_FILE(obj) ((FileObject*)obj)
#define CAST_CWRAPPER(obj) ((CWrapperObject*)obj)

#define object_istype(this, other) (this==other)

#define CMP_EQ 0

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
#include "stringstreamobject.cpp"
#include "cwrapperobject.cpp"

void setup_types_consts(){
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
    setup_stringstream_type();  
    setup_cwrapper_type();  

    setup_builtins();
    
    inherit_type_methods(&ListType);
}