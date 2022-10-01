enum opcode{
    LOAD_CONST,
    STORE_NAME,
    LOAD_NAME,
    STORE_GLOBAL,
    LOAD_GLOBAL,
    BINOP_ADD,
    BINOP_SUB,
    UNARY_NEG,
    BINOP_MUL,
    BINOP_DIV,
    MAKE_FUNCTION,
    RETURN_VAL,
    CALL_FUNCTION,
    BUILD_TUPLE,
    BUILD_DICT,
    LOAD_BUILD_CLASS,
    LOAD_REGISTER_POP,
    READ_REGISTER_PUSH,
    LOAD_ATTR,
    STORE_ATTR,
    CALL_METHOD,
    BUILD_LIST,
    BINOP_IS,
    BINOP_EE,
    POP_JMP_TOS_FALSE,
    JUMP_DELTA,
    BINOP_SUBSCR,
    RAISE_EXC,
    STORE_SUBSCR,
    DUP_TOS,
    POP_TOS,
    SETUP_TRY,
    FINISH_TRY,
    BINOP_EXC_CMP,
    BINOP_GT,
    BINOP_GTE,
    BINOP_LT,
    BINOP_LTE,
    FOR_TOS_ITER,
    JUMP_TO,
    EXTRACT_ITER,
    BREAK_LOOP,
    CONTINUE_LOOP,
    UNPACK_SEQ,
    BINOP_IADD,
    BINOP_ISUB,
    BINOP_IMUL,
    BINOP_IDIV,
    IMPORT_NAME,
    IMPORT_FROM_MOD,
    MAKE_SLICE,
    BINOP_NE,
    DEL_SUBSCR,
    DEL_NAME,
    BINOP_MOD,
    BINOP_POW,
    BINOP_IPOW,
    BINOP_IMOD,
    BINOP_AND,
    BINOP_OR,
    UNARY_NOT,
    BUILD_STRING,
    POP_JMP_TOS_TRUE,
    RAISE_ASSERTIONERR,
    DEL_GLBL,
    DEL_ATTR,
    MAKE_CLOSURE,
    LOAD_NONLOCAL,
    STORE_NONLOCAL,
    DEL_NONLOCAL,
    BITWISE_NOT,
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_LSHIFT,
    BITWISE_RSHIFT,
    BINOP_IAND,
    BINOP_IOR,
    BINOP_ILSH,
    BINOP_IRSH,
    MAKE_CLASSMETH,
    MAKE_STATICMETH,
};

enum scope{
    SCOPE_GLOBAL,
    SCOPE_LOCAL,
};

struct instruction{
    enum opcode opcode;
    uint32_t arg;
    struct instruction* next;
    Position* start;
    Position* end;
};

struct instructions{
    struct instruction* first;
    uint32_t count;
};

struct compiler{
    struct object* consts;
    struct object* names;
    struct instructions* instructions;
    enum scope scope;
    object* lines;
    bool keep_return;
};

#define INTLIT(node) ((IntLiteral*)(node))
#define FLOATLIT(node) ((IntLiteral*)(node))
#define STRLIT(node) ((StringLiteral*)(node))
#define IDENTI(node) ((Identifier*)(node))
#define ASSIGN(node) ((Assign*)(node))
#define BINOP(node) ((BinOp*)(node))
#define UNARYOP(node) ((UnaryOp*)(node))
#define FUNCT(node) ((Func*)(node))
#define CALL(node) ((Call*)(node))
#define CLASS(node) ((Class*)(node))
#define DOT(node) ((Dot*)(node))
#define DOTASSIGN(node) ((DotAssign*)(node))
#define DOTCALL(node) ((DotCall*)(node))
#define RETURN(node) ((Return*)(node))
#define LIST(node) ((List*)(node))
#define DICT(node) ((Dict*)(node))
#define IF(node) ((If*)(node))
#define ELSE(node) ((Else*)(node))
#define CONTROL(node) ((Control*)(node))
#define SUBSCR(node) ((Subscript*)(node))
#define RAISE(node) ((Raise*)(node))
#define STSUBSCR(node) ((StoreSubscript*)(node))
#define TRYEXCEPTFINALLY(node) ((TryExceptFinally*)(node))
#define TRY(node) ((Try*)(node))
#define FINALLY(node) ((Finally*)(node))
#define EXCEPT(node) ((Except*)(node))
#define FOR(node) ((For*)node)
#define WHILE(node) ((While*)node)
#define MULTIIDENT(node) ((MultiIdentifier*)node)
#define IMPORT(node) ((Import*)node)
#define FROM(node) ((From*)node)
#define SLICE(node) ((Slice*)node)
#define STSLICE(node) ((StoreSlice*)node)
#define DEL(node) ((Del*)node)
#define GLBLIDENT(node) ((GlblIdent*)node)
#define ASSERT(node) ((Assert*)node)

#define NAMEIDX(obj) (*CAST_INT(obj->type->slot_mappings->slot_len(obj))->val).to_long_long()-1


struct object* compile(struct compiler* compiler, parse_ret ast, int fallback_line);
uint32_t num_instructions(vector<Node*>* nodes, scope s);
uint32_t num_instructions(Node* node, scope s);
uint32_t num_instructions(vector<Node*>* nodes, scope s);
uint32_t num_instructions(Node* node, scope s);

bool compiler_nofree=false;

#include "compiler.cpp"