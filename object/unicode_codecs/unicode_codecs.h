#include "codepoint_names.h"

#ifdef _WIN32
#include "win_iconv.c"
#else
#include <iconv.h>
#endif

//Get name of unicode chr from codepoint
string get_name_from_cp(int cp){
    if (cp>=sizeof(_cp_names)){
        return "";
    }
    return _cp_names[cp];
}
//Get codepoint of unicode chr from name
int get_cp_from_name(string name){
    if (_cp_names_rev.find(name)==_cp_names_rev.end()){
        return -1;
    }
    return _cp_names_rev[name];
}