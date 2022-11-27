inline void* fpl_malloc(size_t size){
    void* ptr=malloc(size);
    if (ptr==NULL){
        memory_error();
    }
    return ptr;
}

inline void* fpl_calloc(size_t nitems, size_t size){
    void* ptr=calloc(nitems, size);
    if (ptr==NULL){
        memory_error();
    }
    return ptr;
}

inline void* fpl_realloc(void* oldptr, size_t size){
    void* ptr=realloc(oldptr, size);
    if (ptr==NULL){
        memory_error();
    }
    return ptr;
}

inline void fpl_free(void* ptr){
    free(ptr);
}