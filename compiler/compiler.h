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
};

#define INTLIT(node) ((IntLiteral*)(node))
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

#define NAMEIDX(obj) (*CAST_INT(obj->type->slot_len(obj))->val).to_long_long()-1

struct object* compile(struct compiler* compiler, parse_ret ast);

#include "compiler.cpp"