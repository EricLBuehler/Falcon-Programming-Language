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
#define IF(node) ((If*)(node))
#define ELSE(node) ((Else*)(node))
#define CONTROL(node) ((Control*)(node))
#define SUBSCR(node) ((Subscript*)(node))

#define NAMEIDX(obj) (*CAST_INT(obj->type->slot_len(obj))->val).to_long_long()-1

parse_ret parseretglbl;

struct object* compile(struct compiler* compiler, parse_ret ast);
uint32_t num_instructions(vector<Node*>* nodes);
uint32_t num_instructions(Node* node);

#include "compiler.cpp"