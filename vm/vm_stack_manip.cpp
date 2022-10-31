uint32_t MAX_RECURSION=500;

//void add_dataframe(struct vm* vm, struct datastack* stack, struct object* obj){
#define add_dataframe(vm, stack, obj)   stack->data[stack->size++]=obj;\
                                        if (stack->capacity==stack->size){stack->data=(object**)fpl_realloc(stack->data, sizeof(object*)*((stack->size + (stack->size >> 3) + 6) & ~3));stack->capacity=((stack->size + (stack->size >> 3) + 6) & ~3);};

//struct object* pop_dataframe(struct datastack* stack){
#define pop_dataframe(stack) stack->data[--stack->size]

//struct object* peek_dataframe(struct datastack* stack)
#define peek_dataframe(stack) stack->data[stack->size-1]

#define blockstack_head(stack) stack->data[stack->size-1]

//void add_blockframe(uint32_t* ip, struct vm* vm, struct blockstack* stack, uint32_t arg, enum blocktype tp)
#define add_blockframe(ip, vm, stack, arg_, tp)  stack->data[stack->size].type=tp;\
                                                stack->data[stack->size].arg=arg_;\
                                                stack->data[stack->size].obj=NULL;\
                                                stack->data[stack->size].callstack_size=vm->callstack->size;\
                                                stack->data[stack->size++].start_ip=*ip;\
                                                if (stack->capacity==stack->size){stack->data=(struct blockframe*)fpl_realloc(stack->data, sizeof(struct blockframe)*((stack->size + (stack->size >> 3) + 6) & ~3));stack->capacity=((stack->size + (stack->size >> 3) + 6) & ~3);};

//void pop_blockframe(struct blockstack* stack)
#define pop_blockframe(stack)   stack->data[--stack->size];

//void add_callframe(struct callstack* stack, object* line, string* name, object* code, object* callable, uint32_t* ip)
#define add_callframe(stack, line_, name_, code_, callable_, ip_)   stack->data[stack->size].name=name_;\
                                                                    stack->data[stack->size].line=line_;\
                                                                    stack->data[stack->size].code=code_;\
                                                                    stack->data[stack->size].callable=callable_;\
                                                                    stack->data[stack->size].annotations=new_dict();\
                                                                    stack->data[stack->size].ip=ip_;\
                                                                    stack->data[stack->size].locals=NULL;\
                                                                    stack->data[stack->size++].filedata=vm->filedata;\
                                                                    if (stack->size==MAX_RECURSION){vm_add_err(&RecursionError, vm, "Maximum stack depth exceeded.");};

//void pop_callframe(struct callstack* stack)
#define pop_callframe(stack)    stack->size--;

#define callstack_head(stack)   stack->data[stack->size-1]