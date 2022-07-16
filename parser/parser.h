#include <stdarg.h>

#define skip_newline while (current_tok_is(T_NEWLINE)){this->advance();}

#include "nodes.cpp"
#include "parser.cpp"