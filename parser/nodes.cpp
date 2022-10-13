enum nodetype{
    N_INT,
    N_FLOAT,
    N_BINOP,
    N_IDENT,
    N_STR,
    N_ASSIGN,
    N_UNARY,
    N_FUNC,
    N_CALL,
    N_TRUE,
    N_FALSE,
    N_NONE,
    N_CLASS,
    N_DOT,
    N_DOTASSIGN,
    N_DOTCALL,
    N_RETURN,
    N_LIST,  //
    N_TUPLE, // Same internally (List)
    N_DICT,
    N_CONTROL,
    N_IF,
    N_ELSE,
    N_SUBSCR,
    N_RAISE,
    N_STORE_SUBSCR,
    N_TRY_EXCEPT_FINALLY,
    N_TRY,
    N_EXCEPT,
    N_FINALLY,
    N_FOR,
    N_BREAK, //No data struct
    N_CONTINUE, //No data struct
    N_WHILE,
    N_MULTIIDENT,
    N_IMPORT,
    N_FROM,
    N_SLICE,
    N_STORE_SLICE,
    N_DEL,
    N_GLBL_IDENT,
    N_FSTRING,
    N_ASSERT,
    N_NONLOCAL, //No data struct
    N_TERNARY,
    N_DECORATOR,
    N_ANONIDENT,
    N_ANONASSIGN, //No data struct
    N_ANONNONLOCAL, //No data struct
    N_ANONGLBL_IDENT, //No data struct
    N_ANONDOT,
    N_YIELD,
    N_SET, //No data struct (List)
};

enum precedence {
    LOWEST = 1,
    ASSIGN,
    LOGICAL_OR,   
    LOGICAL_AND,
    LOGICAL_NOT, 
    EQUALS,         
    LESSGREATER,
    BITWISE_OR_PREC,
    BITWISE_AND_PREC,
    BITWISE_SHIFT_PREC,
    SUM,          
    PRODUCT, 
    EXP, 
    BITWISE_NOT_PREC,
    CALL,        
    INDEX,
    UNARY,
    TERNARY,        
};

enum precedence get_precedence(Token t){
    switch (t.type){
        case T_EQ:
        case T_IADD:
        case T_ISUB:
        case T_IMUL:
        case T_IDIV:
        case T_IMOD:
        case T_IPOW:
        case T_IAMP:
        case T_IVBAR:
        case T_ILSH:
        case T_IRSH:
        case T_IFLDIV:
            return ASSIGN;
        case T_OR:
            return LOGICAL_OR;
        case T_AND:
            return LOGICAL_AND;
        case T_NOT:
            return LOGICAL_NOT;
        case T_EE:
        case T_NE:
        case T_IS:
        case T_IN:
            return EQUALS;
        case T_GT:
        case T_LT:
        case T_GTE:
        case T_LTE:
            return LESSGREATER;
        case T_PLUS:
        case T_MINUS:
            return SUM;
        case T_TILDE:
            return BITWISE_NOT_PREC;
        case T_MUL:
        case T_DIV:
        case T_PERCENT:
        case T_FLDIV:
            return PRODUCT;
        case T_POW:
            return EXP;
        case T_LPAREN:
            return CALL;
        case T_LSQUARE:
            return INDEX;
        case T_AMPERSAND:
            return BITWISE_AND_PREC;
        case T_VBAR:
            return BITWISE_OR_PREC;
        case T_LSHIFT:
        case T_RSHIFT:
            return BITWISE_SHIFT_PREC;
        case T_QMARK:
            return TERNARY;
        default:
            return LOWEST;
    }
}



struct Node{
    void* node;
    enum nodetype type;
    Position* start;
    Position* end;
};

struct IntLiteral{
    string* literal;
};

struct FloatLiteral{
    string* literal;
};

struct StringLiteral{
    string* literal;
};

struct Identifier{
    string* name;
};

struct BinOp{
    Node* left;
    enum token_type opr;
    Node* right;
};

struct UnaryOp{
    enum token_type opr;
    Node* right;
};

struct Assign{
    Node* name;
    Node* right;
};

struct Func{
    Node* name;
    vector<Node*>* code;
    vector<Node*>* args;
    vector<Node*>* kwargs;
    int type;
    bool starargs;
    bool starkwargs;
    Node* stargs;
    Node* stkwargs;
    Node* rettp;
};

struct Call{
    Node* object;
    vector<Node*>* args;
    vector<Node*>* kwargs;
    vector<int>* stargs;
    vector<int>* stkwargs;
};

struct Class{
    Node* name;
    vector<Node*>* code;
    vector<Node*>* bases;
};

struct Dot{
    vector<Node*>* names;
};

struct DotAssign{
    Node* dot;
    Node* right;
};

struct DotCall{
    Node* dot;
    vector<Node*>* args;
    vector<Node*>* kwargs;
    vector<int>* stargs;
    vector<int>* stkwargs;
};

struct Return{
    Node* node;
};

struct List{
    vector<Node*>* list;
};

struct Dict{
    vector<Node*>* keys;
    vector<Node*>* vals;
};

struct If{
    Node* expr;
    vector<Node*>* code;
};

struct Else{
    Node* base;
    vector<Node*>* code;
};

struct Control{
    vector<Node*>* bases;
};

struct Subscript{
    Node* left;
    Node* expr;
};

struct Raise{
    Node* expr;
};

struct StoreSubscript{
    Node* left;
    Node* expr;
};

struct TryExceptFinally{
    vector<Node*>* bases;
};

struct Try{
    vector<Node*>* code;
};

struct Except{
    Node* type;
    Node* name;
    vector<Node*>* code;
};

struct Finally{
    vector<Node*>* code;
};

struct For{
    vector<Node*>* code;
    Node* ident;
    Node* expr;
    Node* elsen;
};

struct While{
    vector<Node*>* code;
    Node* expr;
};

struct MultiIdentifier{
    vector<string*>* name;
};

struct Import{
    vector<Node*>* libnames;
    vector<Node*>* names;
};

struct From{
    Node* name;
    vector<Node*>* names;
};

struct Slice{
    Node* left;
    Node* right;
};

struct StoreSlice{
    Node* left;
    Node* expr;
};

struct Del{
    Node* expr;
};

struct GlblIdent{
    Node* name;
};

struct Assert{
    Node* expr;
};

struct Ternary{
    Node* left;
    Node* expr1;
    Node* expr2;
};

struct Decorator{
    Node* name;
    Node* function;
    Node* decorator;
};

struct AnnotatedIdentifier{
    string* name;
    Node* tp;
};

struct AnnotatedDot{
    vector<Node*>* names;
    Node* tp;
};

struct Yield{
    Node* expr;
};


void destroy_node(struct Node* node){
    if (node->type==N_INT){
        delete ((IntLiteral*)(node->node))->literal;
    }
    else if (node->type==N_FLOAT){
        delete ((FloatLiteral*)(node->node))->literal;
    }
    else if (node->type==N_BINOP){
        destroy_node(((BinOp*)(node->node))->left);
        destroy_node(((BinOp*)(node->node))->right);
        free(&((BinOp*)(node->node))->opr);
    }

    delete node->start;
    delete node->end;

    free(node->node);
    free(node);
}