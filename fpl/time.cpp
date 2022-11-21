#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

object* time_sleep(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("n"));
    double time;
    if (object_istype(val->type, &IntType)){
        time=CAST_INT(val)->val->to_long();
        FPLDECREF(val);
    }
    else{
        object* otherflt=object_float(val);
        FPLDECREF(val);
        if (otherflt==NULL || !object_istype(otherflt->type, &FloatType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
            return NULL; 
        }
        time=CAST_FLOAT(otherflt)->val;
        if (time<0){
            vm_add_err(&ValueError, vm, "Cannot wait for time less than 0 s");
            return NULL; 
        }
        FPLDECREF(otherflt);
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds((long)(time*1000)));

    return new_none();
}


object* time_sleep_ms(object* self, object* args){
    object* val=dict_get_opti_deref(args, str_new_fromstr("n"));
    double time;
    if (object_istype(val->type, &IntType)){
        time=CAST_INT(val)->val->to_long();
        FPLDECREF(val);
    }
    else{
        object* otherflt=object_float(val);
        FPLDECREF(val);
        if (otherflt==NULL || !object_istype(otherflt->type, &FloatType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to float", val->type->name->c_str());
            return NULL; 
        }
        time=CAST_FLOAT(otherflt)->val;
        if (time<0){
            vm_add_err(&ValueError, vm, "Cannot wait for time less than 0 ms");
            return NULL; 
        }
        FPLDECREF(otherflt);
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds((long)time));

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
    object* val=dict_get_opti_deref(args, str_new_fromstr("n"));
    
    string format=object_cstr(val);
    FPLDECREF(val);
    
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
    tuple_append_noinc(args, str_new_fromstr("n"));
    object* ob=new_builtin(time_sleep, str_new_fromstr("sleep"), args, emptykw_args, 1, false);
    dict_set_noret(dict, str_new_fromstr("sleep"), ob);
    FPLDECREF(ob);    
    
    
    ob=new_builtin(time_sleep_ms, str_new_fromstr("sleep_ms"), args, emptykw_args, 1, false);
    dict_set_noret(dict, str_new_fromstr("sleep_ms"), ob);
    FPLDECREF(ob);    
    

    ob=new_builtin(time_time, str_new_fromstr("time"), emptykw_args, emptykw_args, 0, false);
    dict_set_noret(dict, str_new_fromstr("time"), ob);
    FPLDECREF(ob);    
    

    object* strfargs=new_tuple();
    tuple_append_noinc(args, str_new_fromstr("format"));
    ob=new_builtin(time_strftime, str_new_fromstr("strftime"), strfargs, emptykw_args, 1, false);
    dict_set_noret(dict, str_new_fromstr("strftime"), ob);
    FPLDECREF(ob);    
    
    
    ob=new_builtin(time_timens, str_new_fromstr("time_ns"), emptykw_args, emptykw_args, 0, false);
    dict_set_noret(dict, str_new_fromstr("time_ns"), ob);
    FPLDECREF(ob);    
    

    return module_new_fromdict(dict, str_new_fromstr("time"));
}