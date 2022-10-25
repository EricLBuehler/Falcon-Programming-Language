enum token_type{
    T_ERROR_EOF,
    T_INT,
    T_FLOAT,
    T_IDENTIFIER,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_LPAREN,
    T_RPAREN,
    T_KWD,
    T_EOF,
    T_EQ,
    T_STR,
    T_NEWLINE,
    T_POW,
    T_LCURLY,
    T_RCURLY,
    T_COMMA,
    T_AND,
    T_OR,
    T_NOT,
    T_LSQUARE,
    T_RSQUARE,
    T_COLON,
    T_IN,
    T_IADD,
    T_ISUB,
    T_IMUL,
    T_IDIV,
    T_DOT,
    T_PERCENT,
    T_EE,
    T_NE,
    T_GT,
    T_GTE,
    T_LT,
    T_LTE,
    T_NONE,
    T_FALSE,
    T_TRUE,
    T_DOTIDENT,
    T_IS,
    T_IPOW,
    T_IMOD,
    T_ERR,
    T_TILDE,
    T_AMPERSAND,
    T_VBAR,
    T_LSHIFT,
    T_RSHIFT,
    T_IAMP,
    T_IVBAR,
    T_ILSH,
    T_IRSH,
    T_UNKNOWN,
    T_ISNOT,
    T_FLDIV,
    T_IFLDIV,
    T_QMARK,
    T_AT,
    T_CARET,
};

string token_type_to_str(enum token_type type){
    switch (type){
    case T_INT:return "int";
    case T_FLOAT:return "float";
    case T_IDENTIFIER:return "identifier";
    case T_PLUS:return "+";
    case T_MINUS:return "-";
    case T_MUL:return "*";
    case T_DIV:return "/";
    case T_LPAREN:return "(";
    case T_RPAREN:return ")";
    case T_KWD:return "keyword";
    case T_EOF:return "EOF";
    case T_EQ:return "=";
    case T_STR:return "str";
    case T_NEWLINE:return "\\n";
    case T_POW:return "**";
    case T_LCURLY:return "{";
    case T_RCURLY:return "}";
    case T_COMMA:return ",";
    case T_AND:return "and";
    case T_OR:return "or";
    case T_NOT:return "not";
    case T_LSQUARE:return "[";
    case T_RSQUARE:return "]";
    case T_COLON:return ":";
    case T_IN:return "in";
    case T_IADD:return "+=";
    case T_ISUB:return "-=";
    case T_IMUL:return "*=";
    case T_IDIV:return "/=";
    case T_DOT:return ".";
    case T_PERCENT:return "%";
    case T_EE:return "==";
    case T_NE:return "!=";
    case T_GT:return ">";
    case T_GTE:return ">=";
    case T_LT:return "<";
    case T_LTE:return "<=";
    case T_TRUE:return "True";
    case T_FALSE:return "False";
    case T_NONE:return "None";
    case T_DOTIDENT:return "dotidentifier";
    case T_IS:return "is";
    case T_IPOW:return "**=";
    case T_IMOD:return "%=";
    case T_ERR:return "ERR";
    case T_TILDE:return "~";
    case T_AMPERSAND:return "&";
    case T_VBAR:return "|";
    case T_LSHIFT:return "<<";
    case T_RSHIFT:return ">>";
    case T_UNKNOWN:return "UNKNOWN";
    case T_ISNOT:return "is not";
    case T_FLDIV:return "//";
    case T_IFLDIV:return "//=";
    case T_QMARK:return "?";
    case T_AT:return "@";
    case T_CARET:return "^";
    }
    return "";
}