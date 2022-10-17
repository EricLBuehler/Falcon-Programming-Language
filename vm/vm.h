#include <stdint.h>

uint32_t MAX_RECURSION=500;

#define DISPATCH() if (vm->exception!=NULL){goto exc;} if((*ip)%GIL_MAX_SWITCH==0){GIL.unlock();GIL.lock();};arg=code_array[(*ip)+1];(*ip)+=2;goto *dispatch_table[CAST_INT(code_array[(*ip)-2])->val->to_int()];

#include "vm.cpp"