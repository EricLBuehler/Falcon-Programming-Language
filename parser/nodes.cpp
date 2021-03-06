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
};

enum precedence {
    LOWEST = 1,
    ASSIGN,
    LOGICAL_OR,   
    LOGICAL_AND,
    EQUALS,         
    LESSGREATER,  
    SUM,          
    PRODUCT, 
    UNARY,        
    CALL,         
    INDEX,         
};

enum precedence get_precedence(Token t){
    switch (t.type){
        case T_EQ:
        case T_IADD:
        case T_ISUB:
        case T_IMUL:
        case T_IDIV:
            return ASSIGN;
        case T_OR:
            return LOGICAL_OR;
        case T_AND:
            return LOGICAL_AND;
        case T_EE:
        case T_NE:
        case T_IS:
            return EQUALS;
        case T_GT:
        case T_LT:
        case T_GTE:
        case T_LTE:
            return LESSGREATER;
        case T_PLUS:
        case T_MINUS:
            return SUM;
        case T_MUL:
        case T_DIV:
        case T_PERCENT:
            return PRODUCT;
        case T_LPAREN:
            return CALL;
        case T_LSQUARE:
            return INDEX;
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
};

struct Call{
    Node* object;
    vector<Node*>* args;
    vector<Node*>* kwargs;
};

struct Class{
    Node* name;
    vector<Node*>* code;
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