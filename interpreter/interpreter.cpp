void interpreter_remove_vm(int id){
    interpreter.vm_map->erase(interpreter.vm_map->find(id));
}

void interpreter_add_vm(int id, struct vm* vm){
    (*interpreter.vm_map)[id]=vm; 
}

void interpreter_init(){
    interpreter.vm_map=new unordered_map<int, struct vm*>;
    interpreter.vm_map->clear();
    CLEAR_EVALBREAKER();
}