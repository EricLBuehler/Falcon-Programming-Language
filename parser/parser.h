#include <stdarg.h>

#define skip_newline while (current_tok_is(T_NEWLINE)){this->advance();}
#define reverse_non_newline this->backadvance(); while (!current_tok_is(T_NEWLINE) && !current_tok_is(T_EOF)){this->backadvance();}

#define FUNCTION_NORMAL 1
#define FUNCTION_STATIC 2
#define FUNCTION_CLASS 3
#define FUNCTION_LAMBDA 4

#include "nodes.cpp"
#include "parser.cpp"