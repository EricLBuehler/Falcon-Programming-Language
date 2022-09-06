void* fpl_malloc(size_t size){
    void* ptr=malloc(size);
    if (ptr==NULL){
        memory_error();
    }
    return ptr;
}