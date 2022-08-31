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
};

string token_type_to_str(enum token_type type){
    switch (type){
    case T_INT:return "INT";
    case T_FLOAT:return "FLOAT";
    case T_IDENTIFIER:return "IDENT";
    case T_PLUS:return "PLUS";
    case T_MINUS:return "MINUS";
    case T_MUL:return "MUL";
    case T_DIV:return "DIV";
    case T_LPAREN:return "LPAREN";
    case T_RPAREN:return "RPAREN";
    case T_KWD:return "KWD";
    case T_EOF:return "EOF";
    case T_EQ:return "EQ";
    case T_STR:return "STR";
    case T_NEWLINE:return "NEWLINE";
    case T_POW:return "POW";
    case T_LCURLY:return "LCURLY";
    case T_RCURLY:return "RCURLY";
    case T_COMMA:return "COMMA";
    case T_AND:return "AND";
    case T_OR:return "OR";
    case T_NOT:return "NOT";
    case T_LSQUARE:return "LSQUARE";
    case T_RSQUARE:return "RSQUARE";
    case T_COLON:return "COLON";
    case T_IN:return "IN";
    case T_IADD:return "IADD";
    case T_ISUB:return "ISUB";
    case T_IMUL:return "IMUL";
    case T_IDIV:return "IDIV";
    case T_DOT:return "DOT";
    case T_PERCENT:return "PERCENT";
    case T_EE:return "EE";
    case T_NE:return "NE";
    case T_GT:return "GT";
    case T_GTE:return "GTE";
    case T_LT:return "LT";
    case T_LTE:return "LTE";
    case T_TRUE:return "TRUE";
    case T_FALSE:return "FALSE";
    case T_NONE:return "NONE";
    case T_DOTIDENT:return "DOTIDENT";
    case T_IS:return "IS";
    case T_IPOW:return "IPOW";
    case T_IMOD:return "IMOD";
    case T_ERR:return "ERR";
    }
    return "";
}