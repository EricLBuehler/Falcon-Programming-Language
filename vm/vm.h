#include <stdint.h>

uint32_t MAX_RECURSION=500;

object* idx0;
object* idx1;
object* idx2;
#include "binary_ops/simple.cpp"
#include "binary_ops/complex.cpp"
#include "unary_ops/unary.cpp"
#include "vm.cpp"