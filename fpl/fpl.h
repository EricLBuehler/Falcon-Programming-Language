object** modules=NULL;

#include "time.cpp"
#include "math.cpp"
#include "random.cpp"
#include "thread/thread.cpp"
#include "sys.cpp"
#include "os.cpp"
#include "sockets/sockets.cpp"
#include "turtle/turtle.cpp"

typedef object* (*newmodulefunc)(void);
const size_t nmodules=8;
newmodulefunc newmodules[] = {(newmodulefunc)new_random_module, (newmodulefunc)new_time_module,\
                            (newmodulefunc)new_math_module, (newmodulefunc)new_thread_module,\
                            (newmodulefunc)new_sys_module, (newmodulefunc)new_os_module,\
                            (newmodulefunc)new_socket_module, (newmodulefunc)new_turtle_module, NULL};

void setup_modules(){
    int i=0;
    modules=(object**)fpl_malloc(sizeof(object*)*nmodules);
    newmodulefunc mod=newmodules[i++];
    while (mod){
        modules[i-1]=mod();
        mod=newmodules[i++];
    }
}