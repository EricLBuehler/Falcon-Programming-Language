#include <pthread.h>
#include <semaphore.h>

vector<pthread_t*> fpl_threads;

#include "threadtype.cpp"
#include "threadobject.cpp"
#include "signalstypes.cpp"
#include "mutexobject.cpp"
#include "semaphoreobject.cpp"

void finalize_threads(){
    for (pthread_t* t: fpl_threads){
        pthread_join(*t, NULL);
    }
}

object* new_thread_module(){
    ThreadType=(*(TypeObject*)finalize_type(&ThreadType));
    fplbases.push_back(&ThreadType);
    inherit_type_dict(&ThreadType);
    setup_type_offsets(&ThreadType);
    setup_type_getsets(&ThreadType);
    setup_type_methods(&ThreadType);
    
    MutexType=(*(TypeObject*)finalize_type(&MutexType));
    fplbases.push_back(&MutexType);
    inherit_type_dict(&MutexType);
    setup_type_offsets(&MutexType);
    setup_type_getsets(&MutexType);
    setup_type_methods(&MutexType);
    
    SemaphoreType=(*(TypeObject*)finalize_type(&SemaphoreType));
    fplbases.push_back(&SemaphoreType);
    inherit_type_dict(&SemaphoreType);
    setup_type_offsets(&SemaphoreType);
    setup_type_getsets(&SemaphoreType);
    setup_type_methods(&SemaphoreType);

    fpl_threads.clear();

    object* dict=new_dict();

    dict_set_noinc_noret(dict, str_new_fromstr("Thread"), (object*)&ThreadType);
    dict_set_noinc_noret(dict, str_new_fromstr("Mutex"), (object*)&MutexType);
    dict_set_noinc_noret(dict, str_new_fromstr("Semaphore"), (object*)&SemaphoreType);

    FPLINCREF(((object*)&ThreadType));
    FPLINCREF(((object*)&MutexType));
    FPLINCREF(((object*)&SemaphoreType));

    return module_new_fromdict(dict, str_new_fromstr("thread"));
}