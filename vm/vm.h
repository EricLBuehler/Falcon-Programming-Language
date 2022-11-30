#include <stdint.h>

#define DISPATCH()  if (GET_EVALBREAKER() || hit_sigint){goto exc;};\
                    if(vm->exec++%GIL_MAX_SWITCH==0 && vm->exec>0){\
                        vm->exec=0;\
                        gil_lock(gil_unlock());\
                    }\
                    arg=code_array[(*ip)+1];\
                    (*ip)+=2;\
                    goto *dispatch_table[code_array[(*ip)-2]];

#include "vm.cpp"