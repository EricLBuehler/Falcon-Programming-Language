#include <pthread.h>

vector<pthread_t*> fpl_threads;

#include "threadtype.cpp"
#include "threadobject.cpp"

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

    fpl_threads.clear();

    object* dict=new_dict();

    dict_set(dict, str_new_fromstr("Thread"), (object*)&ThreadType);

    return module_new_fromdict(dict, str_new_fromstr("thread"));
}