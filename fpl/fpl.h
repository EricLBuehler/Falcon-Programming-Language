object** modules=NULL;

#include "time.cpp"

typedef object* (*newmodulefunc)(void);
const size_t nmodules=1;
newmodulefunc newmodules[] = {(newmodulefunc)new_time_module, NULL};

void setup_modules(){
    int i=0;
    modules=(object**)malloc(sizeof(object*)*nmodules);
    newmodulefunc mod=newmodules[i++];
    while (mod){
        modules[i-1]=mod();
        mod=newmodules[i++];
    }
}