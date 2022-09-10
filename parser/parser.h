#include <stdarg.h>

#define skip_newline while (current_tok_is(T_NEWLINE)){this->advance();}
#define reverse_non_newline this->backadvance(); while (!current_tok_is(T_NEWLINE) && !current_tok_is(T_EOF)){this->backadvance();}

#include "nodes.cpp"
#include "parser.cpp"