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
    bool nofree;
    bool inclass;
    object* lines_detailed;
    int stack_size;
    int blockstack_size;
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
#define TERNARY(node) ((Ternary*)node)
#define DECORATOR(node) ((Decorator*)node)
#define ANONIDENT(node) ((AnnotatedIdentifier*)node)
#define ANONDOT(node) ((AnnotatedDot*)node)
#define YIELD(node) ((Yield*)node)
#define WITH(node) ((With*)node)
#define LISTCOMP(node) ((ListComp*)node)
#define DICTCOMP(node) ((DictComp*)node)
#define REF(node) ((Ref*)node)

#define NAMEIDX(obj) CAST_LIST(obj)->size-1


struct object* compile(struct compiler* compiler, parse_ret ast, int fallback_line);
uint32_t num_instructions(vector<Node*>* nodes, scope s);
uint32_t num_instructions(Node* node, scope s);
uint32_t num_instructions_keep(Node* node, scope s);

#define COMPILER_ERROR 0xfff
#define COMPILER_NOLINEANNO 0xffff

#include "compiler.cpp"