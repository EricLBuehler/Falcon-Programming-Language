#include "files.cpp"
#include "strings.cpp"
#include "text.cpp"

void memory_error();
char int_base_check(string s, int base);
int decode_code_point(char** s);
void encode_code_point(char **s, char *end, int code);
string codept_to_str(uint32_t num);
#include "memory.cpp"

#ifndef NULL
#define NULL 0
#endif