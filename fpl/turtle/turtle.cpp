#include "CTurtle/CTurtle.hpp"

object* new_turtle_module(){
    object* dict=new_dict();

    return module_new_fromdict(dict, str_new_fromstr("turtle"));
}