#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

object* time_sleep(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* val=list_index_int(args, 1);
    long time;
    if (object_istype(val->type, &IntType)){
        time=CAST_INT(val)->val->to_long();
    }
    else{
        object* otherint=object_int(val);
        if (otherint==NULL || !object_istype(otherint->type, &IntType)){
            return NULL;
        }
        time=CAST_INT(otherint)->val->to_long();
        DECREF(otherint);
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(time*1000));

    return new_none();
}


object* time_sleep_ms(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    object* val=list_index_int(args, 1);
    long time;
    if (object_istype(val->type, &IntType)){
        time=CAST_INT(val)->val->to_long();
    }
    else{
        object* otherint=object_int(val);
        if (otherint==NULL || !object_istype(otherint->type, &IntType)){
            return NULL;
        }
        time=CAST_INT(otherint)->val->to_long();
        DECREF(otherint);
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(time));

    return new_none();
}

object* time_time(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }
    
    using namespace std::chrono;
    uint64_t time = time_point_cast<nanoseconds>(system_clock::now()).time_since_epoch().count();

    return new_int_frombigint(new BigInt(time));
}

object* time_strftime(object* self, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    
    string format=*CAST_STRING(list_index_int(args,0))->val;
    
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

    object* sleep=cwrapper_new_fromfunc_null((cwrapperfunc)time_sleep, "sleep");
    dict_set(dict, str_new_fromstr("sleep"), sleep);

    object* sleep_ms=cwrapper_new_fromfunc_null((cwrapperfunc)time_sleep_ms, "sleep_ms");
    dict_set(dict, str_new_fromstr("sleep_ms"), sleep_ms);

    object* time=cwrapper_new_fromfunc_null((cwrapperfunc)time_time, "time");
    dict_set(dict, str_new_fromstr("time"), time);

    object* strftime=cwrapper_new_fromfunc_null((cwrapperfunc)time_strftime, "strftime");
    dict_set(dict, str_new_fromstr("strftime"), strftime);

    return module_new_fromdict(dict, str_new_fromstr("time"));
}