#include <chrono>
#include <thread>

object* time_sleep(object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long();
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 argument, got %d", len);
        return NULL; 
    }
    object* val=args->type->slot_mappings->slot_get(args, new_int_fromint(1));
    long time;
    if (object_istype(val->type, &IntType)){
        time=CAST_INT(val)->val->to_long();
    }
    else{
        object* otherint=object_int(val);
        if (otherint==NULL){
            return NULL;
        }
        time=CAST_INT(otherint)->val->to_long();
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(time*1000));

    return new_none();
}


object* time_sleep_ms(object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long();
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 argument, got %d", len);
        return NULL; 
    }
    object* val=args->type->slot_mappings->slot_get(args, new_int_fromint(1));
    long time;
    if (object_istype(val->type, &IntType)){
        time=CAST_INT(val)->val->to_long();
    }
    else{
        object* otherint=object_int(val);
        if (otherint==NULL){
            return NULL;
        }
        time=CAST_INT(otherint)->val->to_long();
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(time));

    return new_none();
}

object* new_time_module(){
    object* dict=new_dict();

    object* sleep=cwrapper_new_fromfunc((cwrapperfunc)time_sleep, "sleep");
    dict_set(dict, str_new_fromstr("sleep"), sleep);

    object* sleep_ms=cwrapper_new_fromfunc((cwrapperfunc)time_sleep_ms, "sleep_ms");
    dict_set(dict, str_new_fromstr("sleep_ms"), sleep_ms);

    return module_new_fromdict(dict, str_new_fromstr("time"));
}