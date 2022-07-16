typedef struct _gc{
    object* gen0;
    object* gen1;
    object* gen2;
    
    uint32_t gen0_n;
    uint32_t gen1_n;
    uint32_t gen2_n;

    uint32_t gen0_thresh;
    uint32_t gen1_thresh;
    uint32_t gen2_thresh;
}_gc;

static _gc gc;

#include "gc.cpp"