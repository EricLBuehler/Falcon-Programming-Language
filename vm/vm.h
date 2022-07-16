#include <stdint.h>

void vm_add_err(struct vm* vm, const char *_format, ...);
void add_dataframe(struct vm* vm, struct datastack* stack, struct object* obj);
struct object* pop_dataframe(struct datastack* stack);
void append_to_list(struct gc* gc, struct object* object);


#include "binary_ops/simple.cpp"
#include "unary_ops/unary.cpp"
#include "vm.cpp"