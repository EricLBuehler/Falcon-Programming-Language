#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

object* time_sleep(object* self, object* args){
    object* val=dict_get(args, str_new_fromstr("n"));
    long time;
    if (object_istype(val->type, &IntType)){
        time=CAST_INT(val)->val->to_long();
    }
    else{
        object* otherint=object_int(val);
        if (otherint==NULL || !object_istype(otherint->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", val->type->name->c_str());
            return NULL; 
        }
        time=CAST_INT(otherint)->val->to_long();
        FPLDECREF(otherint);
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(time*1000));

    return new_none();
}


object* time_sleep_ms(object* self, object* args){
    object* val=dict_get(args, str_new_fromstr("n"));
    long time;
    if (object_istype(val->type, &IntType)){
        time=CAST_INT(val)->val->to_long();
    }
    else{
        object* otherint=object_int(val);
        if (otherint==NULL || !object_istype(otherint->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", val->type->name->c_str());
            return NULL; 
        }
        time=CAST_INT(otherint)->val->to_long();
        FPLDECREF(otherint);
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(time));

    return new_none();
}

object* time_timens(object* self, object* args){    
    using namespace std::chrono;
    uint64_t time = time_point_cast<nanoseconds>(system_clock::now()).time_since_epoch().count();

    return new_int_frombigint(new BigInt(time));
}

object* time_time(object* self, object* args){
    using namespace std::chrono;
    double time = (time_point_cast<nanoseconds>(system_clock::now()).time_since_epoch().count())/pow(10,9);

    return new_float_fromdouble(time);
}

object* time_strftime(object* self, object* args){
    object* val=dict_get(args, str_new_fromstr("n"));
    
    string format=object_cstr(val);
    
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    auto out_=std::put_time(&tm, format.c_str());
    
    std::stringstream buffer;
    buffer<<out_;
    string out(buffer.str());

    return str_new_fromstr(out);
}

object* new_time_module(){
    object* dict=new_dict();

    object* emptykw_args=new_tuple();

    object* args=new_tuple();
    args->type->slot_mappings->slot_append(args, str_new_fromstr("n"));
    object* ob=new_builtin(time_sleep, str_new_fromstr("sleep"), args, emptykw_args, 1, false);
    dict_set(dict, str_new_fromstr("sleep"), ob);
    FPLDECREF(ob);    
    
    
    ob=new_builtin(time_sleep_ms, str_new_fromstr("sleep_ms"), args, emptykw_args, 1, false);
    dict_set(dict, str_new_fromstr("sleep_ms"), ob);
    FPLDECREF(ob);    
    

    ob=new_builtin(time_time, str_new_fromstr("time"), emptykw_args, emptykw_args, 0, false);
    dict_set(dict, str_new_fromstr("time"), ob);
    FPLDECREF(ob);    
    

    object* strfargs=new_tuple();
    strfargs->type->slot_mappings->slot_append(args, str_new_fromstr("format"));
    ob=new_builtin(time_strftime, str_new_fromstr("strftime"), strfargs, emptykw_args, 1, false);
    dict_set(dict, str_new_fromstr("strftime"), ob);
    FPLDECREF(ob);    
    
    
    ob=new_builtin(time_timens, str_new_fromstr("time_ns"), emptykw_args, emptykw_args, 0, false);
    dict_set(dict, str_new_fromstr("time_ns"), ob);
    FPLDECREF(ob);    
    

    return module_new_fromdict(dict, str_new_fromstr("time"));
}