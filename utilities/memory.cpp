#define MALLOC_PAGE_SIZE 4096

inline void* fpl_malloc(size_t size){
    void* ptr=malloc(size);
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