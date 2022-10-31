#include <stdint.h>

#define DISPATCH() if((*ip)%GIL_MAX_SWITCH==0){GIL.unlock();GIL.lock();};arg=code_array[(*ip)+1];(*ip)+=2;goto *dispatch_table[code_array[(*ip)-2]];

#include "vm.cpp"