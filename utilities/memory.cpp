using word_t = intptr_t;

struct block{
    block* next;
    size_t size;
    word_t ptr[1];
};

struct memory_struct{
    void* head=NULL;
    size_t size=2048;
    size_t allocsize=0;
    size_t elemsize=0;
};


struct memory_struct memory;

void* FPLMalloc(size_t size){
    if (size>512){
        return malloc(size);
    }
    if (memory.head==NULL || memory.size==memory.allocsize){
        memory.head=realloc(memory.head, memory.size);
        memory.allocsize=memory.size;
        memory.size=memory.size*2;
    }
    block* b=(block*)malloc(sizeof(block)+size);
    memory.elemsize+=sizeof(block)+size;
    if (memory.elemsize>=memory.allocsize){
        memory.head=realloc(memory.head, memory.size);
        memory.allocsize=memory.size;
        memory.size=memory.size*2;
    }
    b->next=NULL;
    b->size=size;
    memcpy(memory.head+memory.elemsize, b, sizeof(block)+size);
    return b;
}