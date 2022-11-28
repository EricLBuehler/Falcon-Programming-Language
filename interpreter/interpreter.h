typedef struct _interpreter{
    unordered_map<int, struct vm*>* vm_map;
    bool eval_breaker;
    struct object* path;
}_interpreter;


#define SET_EVALBREAKER() interpreter.eval_breaker=true
#define GET_EVALBREAKER() interpreter.eval_breaker
#define CLEAR_EVALBREAKER() interpreter.eval_breaker=false


struct vm* vm=NULL;

_interpreter interpreter;

#include "interpreter.cpp"