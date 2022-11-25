#include "codepoint_names.h"

#ifdef _WIN32
#include "win_iconv.c"
#else
#include <iconv.h>
#endif