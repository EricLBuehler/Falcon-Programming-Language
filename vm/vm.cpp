struct datastack* new_datastack(){
    struct datastack* data=(struct datastack*)malloc(sizeof(struct datastack));
    data->head=NULL;
    data->size=0;
    return data;
}

struct callstack* new_callstack(){
    struct callstack* call=(struct callstack*)malloc(sizeof(struct callstack));
    call->head=NULL;
    call->size=0;
    return call;
}

struct blockstack* new_blockstack(){
    struct blockstack* block=(struct blockstack*)malloc(sizeof(struct blockstack));
    block->head=NULL;
    block->size=0;
    return block;
}

void add_blockframe(uint32_t* ip, struct vm* vm, struct blockstack* stack, uint32_t arg, enum blocktype tp){
    struct blockframe* frame=(struct blockframe*)malloc(sizeof(struct blockframe));
    frame->next=stack->head;
    frame->type=tp;
    frame->arg=arg;
    frame->obj=NULL;
    frame->callstack_size=vm->callstack->size;
    frame->start_ip=(*ip);

    stack->size++;
    stack->head=frame;
}

inline void add_dataframe(struct vm* vm, struct datastack* stack, struct object* obj){
    struct dataframe* frame=(struct dataframe*)malloc(sizeof(struct dataframe));
    frame->next=stack->head;
    frame->obj=obj;

    stack->size++;
    stack->head=frame;
}

inline struct object* pop_dataframe(struct datastack* stack){
    struct dataframe* frame=stack->head;

    stack->head=frame->next;
    stack->size--;
    
    object* o=frame->obj;
    free(frame);
    return o;
}

void pop_blockframe(struct blockstack* stack){
    struct blockframe* frame=stack->head;

    stack->head=frame->next;
    stack->size--;

    free(frame);
}

struct blockframe* in_blockstack(struct blockstack* stack, enum blocktype type){
    struct blockframe* frame=stack->head;

    while (frame){
        if (frame->type==type){
            return frame;
        }
        frame=frame->next;
    }
    return NULL;
}

struct object* peek_dataframe(struct datastack* stack){    
    return stack->head->obj;
}

inline void add_callframe(struct callstack* stack, object* line, string* name, object* code){
    struct callframe* frame=(struct callframe*)malloc(sizeof(struct callframe));
    frame->name=name;
    frame->next=stack->head;
    frame->line=line;
    frame->code=code;
    frame->locals=NULL;
    frame->filedata=vm->filedata;

    stack->size++;
    stack->head=frame;
    
    if (stack->size-2==MAX_RECURSION){
        vm_add_err(&RecursionError, vm, "Maximum stack depth exceeded.");
    }
}

inline void pop_callframe(struct callstack* stack){
    struct callframe* frame=stack->head;

    stack->head=frame->next;
    stack->size--;
    
    free(frame);
}

void vm_add_err(TypeObject* exception, struct vm* vm, const char *_format, ...) {
    if (vm->exception!=NULL){
        return;
    }
    
    va_list args;
    const int length=256;
    char format[length];
    sprintf(format, "%s", _format);
    
    object* eargs=new_tuple();
    object* kwargs=new_dict();
    vm->exception=exception->type->slot_call((object*)exception, eargs, kwargs); //Create new exception object
    DECREF(eargs);
    DECREF(kwargs);
    
    char* msg = (char*)malloc(sizeof(char)*length);

    va_start(args, _format);
    vsnprintf(msg, length, format, args);
    va_end(args);
    
    if (CAST_EXCEPTION(vm->exception)->err!=NULL){
        DECREF(CAST_EXCEPTION(vm->exception)->err);
    }
    CAST_EXCEPTION(vm->exception)->err=str_new_fromstr(msg);
    free(msg);
}

object* vm_setup_err(TypeObject* exception, struct vm* vm, const char *_format, ...) {  
    va_list args;
    const int length=256;
    char format[length];
    sprintf(format, "%s", _format);
    
    object* eargs=new_tuple();
    object* kwargs=new_dict();
    object* exc=exception->type->slot_call((object*)exception, eargs, kwargs); //Create new exception object
    DECREF(eargs);
    DECREF(kwargs);

    char *msg = (char*)malloc(sizeof(char)*length);

    va_start(args, _format);
    vsnprintf(msg, length, format, args);
    va_end(args);
    
    if (CAST_EXCEPTION(vm->exception)->err!=NULL){
        DECREF(CAST_EXCEPTION(vm->exception)->err);
    }
    CAST_EXCEPTION(exc)->err=str_new_fromstr(msg);
    return exc;
}

struct vm* new_vm(uint32_t id, object* code, struct instructions* instructions, string* filedata){
    struct vm* vm=(struct vm*)malloc(sizeof(struct vm));
    vm->id=id;
    vm->ret_val=0;
    vm->ip=0;
    vm->objstack=new_datastack();
    vm->callstack=new_callstack();
    vm->blockstack=new_blockstack();
    
    vm->exception=NULL;

    vm->filedata=filedata;
    
    if (::vm==NULL){
        ::vm=vm;
    }

    add_callframe(vm->callstack, new_int_fromint(0), new string("<module>"), code);
    
    return vm;
}

void vm_del(struct vm* vm){
    struct callframe* i=vm->callstack->head;
    while (i){
        struct callframe* i_=i=i->next;;
        free(i);
        i=i_;
    }
    struct dataframe* j=vm->objstack->head;
    while (j){
        struct dataframe* j_=j=j->next;;
        free(i);
        j=j_;
    }

    DECREF(vm->globals);
    delete vm->filedata;
    DECREF(vm->exception);
}

void vm_add_var_locals(struct vm* vm, object* name, object* value){
    for (auto k: (*CAST_DICT(vm->callstack->head->locals)->val)){
        if (istrue(object_cmp(name, k.first, CMP_EQ))){
            ((object_var*)CAST_DICT(vm->callstack->head->locals)->val->at(k.first))->gc_ref--;
        }
    }
    
    if (value->type->size==0){
        ((object_var*)value)->gc_ref++;
    }

    dict_set(vm->callstack->head->locals, name, value); //If globals is same obj as locals then this will still update both
}

struct object* vm_get_var_locals(struct vm* vm, object* name){
    struct callframe* frame=vm->callstack->head;
    
    while(frame){
        for (auto k: (*CAST_DICT(frame->locals)->val)){
            if (istrue(object_cmp(name, k.first, CMP_EQ))){
                return  CAST_DICT(frame->locals)->val->at(k.first);
            }
        }

        frame=frame->next;
    }
    
    for (size_t i=0; i<nbuiltins; i++){
        if (object_istype(builtins[i]->type, &BuiltinType)){
            if (istrue(object_cmp(CAST_BUILTIN(builtins[i])->name, name, CMP_EQ))){
                return builtins[i];
            }
        }
        else if (object_istype(builtins[i]->type, &TypeType)){
            if (CAST_TYPE(builtins[i])->name->compare((*CAST_STRING(name)->val))==0){
                return builtins[i];
            }
        }
    }
    
    vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
    return NULL;
}

void vm_add_var_globals(struct vm* vm, object* name, object* value){
    for (auto k: (*CAST_DICT(vm->globals)->val)){
        if (istrue(object_cmp(name, k.first, CMP_EQ))){
            ((object_var*)CAST_DICT(vm->globals)->val->at(k.first))->gc_ref--;
        }
    }
    if (value->type->size==0){
        ((object_var*)value)->gc_ref++;
    }
    dict_set(vm->globals, name, value); //If globals is same obj as locals then this will still update both
}

struct object* vm_get_var_globals(struct vm* vm, object* name){
    for (auto k: (*CAST_DICT(vm->globals)->val)){
        if (istrue(object_cmp(name, k.first, CMP_EQ))){
            return  CAST_DICT(vm->globals)->val->at(k.first);
        }
    }
    for (size_t i=0; i<nbuiltins; i++){
        if (object_istype(builtins[i]->type, &BuiltinType)){
            if (istrue(object_cmp(CAST_BUILTIN(builtins[i])->name, name, CMP_EQ))){
                return builtins[i];
            }
        }
        else if (object_istype(builtins[i]->type, &TypeType)){
            if (CAST_TYPE(builtins[i])->name->compare((*CAST_STRING(name)->val))==0){
                return builtins[i];
            }
        }
    }
    vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
    return NULL;
}

void print_traceback(){
    struct callframe* callframe=vm->callstack->head;
    while (callframe){    
        if (callframe->name==NULL){
            callframe=callframe->next;
        }
        cout<<"In file '"+program/*object_cstr(CAST_CODE(callframe->code)->co_file)*/+"', line "+to_string(CAST_INT(callframe->line)->val->to_int()+1)+", in "+(*callframe->name)<<endl;
        
        int line=0;
        int target=CAST_INT(callframe->line)->val->to_int();
        int startidx=0;
        int endidx=0;
        int idx=0;
        bool entered=false;
        while (true){
            if (line==target && !entered){
                startidx=idx;
                entered=true;
            }
            if (entered && ((*CAST_CODE(callframe->code)->filedata)[idx]=='\n' || (*CAST_CODE(callframe->code)->filedata)[idx]=='\0')){
                endidx=idx;
                break;
            }
            else if ((*CAST_CODE(callframe->code)->filedata)[idx]=='\n'){
                line++;
            }
            idx++;
        }

        string snippet="";
        for (int i=startidx; i<endidx; i++){
            snippet+=(*CAST_CODE(callframe->code)->filedata)[i];
        }

        cout<<"    "<<remove_spaces(snippet)<<endl;
        
        callframe=callframe->next;
    }
}

object* import_name(string data, object* name){
    vector<string> kwds;
    kwds.push_back("func");
    kwds.push_back("class");
    kwds.push_back("return");
    kwds.push_back("if");
    kwds.push_back("else");
    kwds.push_back("elif");
    kwds.push_back("raise");
    kwds.push_back("try");
    kwds.push_back("except");
    kwds.push_back("finally");
    kwds.push_back("for");
    kwds.push_back("break");
    kwds.push_back("while");
    kwds.push_back("import");
    kwds.push_back("from");
    kwds.push_back("del");

    Lexer lexer(data,kwds);
    lexer.pos=Position(program);

    Position end=lexer.tokenize();

    parser=Parser(lexer.tokens, data);
    parse_ret ast=parser.parse();

    if (ast.errornum>0){
        cout<<ast.header<<endl;
        cout<<ast.snippet<<endl;
        cout<<ast.arrows<<endl;
        printf("%s\n",ast.error);
        exit(0);
    }

    struct compiler* compiler = new_compiler();
    glblfildata=new string(data);
    object* code=compile(compiler, ast);
    if (code==NULL){
        cout<<parseretglbl.header<<endl;
        cout<<parseretglbl.snippet<<endl;
        printf("%s\n",parseretglbl.error);
        exit(0);
    }
    CAST_CODE(code)->co_file=object_repr(name);
    struct vm* vm_=::vm;
    ::vm=new_vm(0, code, compiler->instructions, &data); //data is still in scope...
    ::vm->globals=new_dict();
    ::vm->callstack->head->locals=INCREF(::vm->globals);
    object* ret=run_vm(code, &::vm->ip);
    object* dict=::vm->callstack->head->locals;
    ::vm=vm_;

    object* o=module_new_fromdict(dict, name);

    return o;
}

void vm_del_var_locals(struct vm* vm, object* name){
    for (auto k: (*CAST_DICT(vm->callstack->head->locals)->val)){
        if (istrue(object_cmp(name, k.first, CMP_EQ))){
            ((object_var*)CAST_DICT(vm->callstack->head->locals)->val->at(k.first))->gc_ref--;

            dict_del_item(vm->callstack->head->locals, name);
            return;
        }
    }
    vm_add_err(&NameError, vm, "Cannot find name %s.", object_cstr(object_repr(name)).c_str());
    return;
}


object* _vm_step(object* instruction, object* arg, struct vm* vm, uint32_t* ip){
    //Run one instruction
    switch (CAST_INT(instruction)->val->to_int()){
        case LOAD_CONST:{
            add_dataframe(vm, vm->objstack, list_get(CAST_CODE(vm->callstack->head->code)->co_consts, arg));
            break;
        }

        case STORE_GLOBAL:{
            vm_add_var_globals(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), peek_dataframe(vm->objstack));
            break;
        }

        case LOAD_GLOBAL:{
            add_dataframe(vm, vm->objstack, vm_get_var_globals(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg) ));
            break;
        }

        case STORE_NAME:{
            vm_add_var_locals(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), peek_dataframe(vm->objstack));
            break;
        }

        case LOAD_NAME:{
            add_dataframe(vm, vm->objstack, vm_get_var_locals(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg) ));
            break;
        }

        case BINOP_ADD:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_add(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand type for +: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            break;
        }

        case BINOP_SUB:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_sub(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand type for -: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            break;
        }

        case BINOP_MUL:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_mul(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand type for *: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            break;  
        }

        case BINOP_DIV:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_div(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand type for /: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            break;
        }

        case BINOP_IS:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=left==right ? new_bool_true() : new_bool_false();
            add_dataframe(vm, vm->objstack, ret);
            break;
        }

        case BINOP_EE:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_cmp(left, right, CMP_EQ);
            add_dataframe(vm, vm->objstack, ret);
            break;
        }

        case BINOP_NE:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_cmp(left, right, CMP_NE);
            add_dataframe(vm, vm->objstack, ret);
            break;
        }

        case UNARY_NEG:{
            struct object* right=pop_dataframe(vm->objstack);
            
            object* ret=object_neg(right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid unary operand -: '%s'.", right->type->name->c_str());
            }
            break;
        }

        case MAKE_FUNCTION:{
            object* code=pop_dataframe(vm->objstack); //<- Code
            object* args=pop_dataframe(vm->objstack); //<- Args
            object* kwargs=pop_dataframe(vm->objstack); //<- Kwargs
            object* name=pop_dataframe(vm->objstack); //<- Name
            
            object* func=func_new_code(code, args, kwargs, CAST_INT(arg)->val->to_int(), name);
            add_dataframe(vm, vm->objstack, func);
            break;
        }

        case RETURN_VAL: {
            return pop_dataframe(vm->objstack);
        }

        case CALL_METHOD: {
            object* function=pop_dataframe(vm->objstack);
            object* head=pop_dataframe(vm->objstack);
            
            uint32_t argc=CAST_INT(arg)->val->to_int()+1;
            uint32_t posargc=CAST_INT(pop_dataframe(vm->objstack))->val->to_int()+1;
            uint32_t kwargc=argc-posargc;

            if (function->type->slot_call==NULL){
                vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
                return NULL;
            }
                

            //Setup kwargs
            object* kwargs=new_dict();
            object* val;
            for (uint32_t i=0; i<kwargc; i++){
                val=pop_dataframe(vm->objstack);
                dict_set(kwargs, pop_dataframe(vm->objstack), val);
            }
            //

            //Setup args
            object* args=new_tuple();
            if (!object_istype(head->type, &TypeType)){
                tuple_append(args, head);
            }
            for (uint32_t i=0; i<posargc-1; i++){
                tuple_append(args, pop_dataframe(vm->objstack));
            }
            
            //

            //Call
            object* ret=function->type->slot_call(function, args, kwargs);
            if (ret==NULL){
                return CALL_ERR;
            }
            add_dataframe(vm, vm->objstack, ret);
            break;
        }

        case CALL_FUNCTION: {
            object* function=pop_dataframe(vm->objstack);

            uint32_t argc=CAST_INT(arg)->val->to_int();
            uint32_t posargc=CAST_INT(pop_dataframe(vm->objstack))->val->to_int();
            uint32_t kwargc=argc-posargc;     

            if (function->type->slot_call==NULL){
                vm_add_err(&TypeError, vm, "'%s' object is not callable.",function->type->name->c_str());
                return NULL;
            }

            //Setup kwargs
            object* kwargs=new_dict();
            object* val;
            for (uint32_t i=0; i<kwargc; i++){
                val=pop_dataframe(vm->objstack);
                dict_set(kwargs, pop_dataframe(vm->objstack), val);
            }
            //

            //Setup args
            object* args=new_tuple();
            for (uint32_t i=0; i<posargc; i++){
                tuple_append(args, pop_dataframe(vm->objstack));
            }
            
            //
            
            //Call
            object* ret=function->type->slot_call(function, args, kwargs);
            if (ret==NULL){ 
                return CALL_ERR;
            }
            
            add_dataframe(vm, vm->objstack, ret);
            
            break;            
        }

        case BUILD_LIST: {
            object* list=new_list();
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                list_append(list, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, list);
            break;
        }

        case BUILD_TUPLE: {
            object* tuple=new_tuple();
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                tuple_append(tuple, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, tuple);
            break;
        }

        case BUILD_DICT: {
            object* dict=new_dict();
            for (int i=0; i<CAST_INT(arg)->val->to_int(); i++){
                object* name=pop_dataframe(vm->objstack);
                dict_set(dict, name, pop_dataframe(vm->objstack));
            }
            add_dataframe(vm, vm->objstack, dict);
            break;
        }

        case LOAD_BUILD_CLASS: {
            add_dataframe(vm, vm->objstack, BUILTIN_BUILD_CLASS);
            break;
        }

        case LOAD_REGISTER_POP: {
            vm->accumulator=pop_dataframe(vm->objstack);
            break;
        }

        case READ_REGISTER_PUSH: {
            add_dataframe(vm, vm->objstack, vm->accumulator);
            break;
        }      

        case LOAD_ATTR: {
            object* obj=pop_dataframe(vm->objstack);
            object* attr=list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg);
            add_dataframe(vm, vm->objstack, object_getattr(obj, attr));
            break;
        }  

        case STORE_ATTR: {
            object* obj=pop_dataframe(vm->objstack);
            object* val=peek_dataframe(vm->objstack); //For multiple assignment
            object* attr=list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg);
            object_setattr(obj, attr, val);
            break;
        }

        case POP_JMP_TOS_FALSE: {
            object* o=pop_dataframe(vm->objstack);
            object* val=object_istruthy(o);
            if (!istrue(val)){
                DECREF(val);
                (*ip)=(*ip)+CAST_INT(arg)->val->to_long();
                break;
            }
            DECREF(val);
            break;
        }

        case JUMP_DELTA: {
            (*ip)=(*ip)+CAST_INT(arg)->val->to_long();
            break;
        }

        case BINOP_SUBSCR: {
            object* idx=pop_dataframe(vm->objstack);
            object* base=pop_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, object_get(base, idx));
            break;
        }

        case RAISE_EXC: {
            if (!object_issubclass(peek_dataframe(vm->objstack), &ExceptionType)){
                vm_add_err(&TypeError, vm, "Exceptions must be subclass of Exception");
                break;
            }
            if (vm->exception!=NULL){
                DECREF(vm->exception);
            }
            vm->exception=pop_dataframe(vm->objstack);
            break;
        }

        case STORE_SUBSCR: {
            object* val=pop_dataframe(vm->objstack);
            object* idx=pop_dataframe(vm->objstack);
            object* base=pop_dataframe(vm->objstack);
            object_set(base, idx, val);
            add_dataframe(vm, vm->objstack, base);
            break;
        }

        case DUP_TOS: {
            add_dataframe(vm, vm->objstack, peek_dataframe(vm->objstack));
            break;
        }

        case POP_TOS: {
            pop_dataframe(vm->objstack);
            break;
        }

        case SETUP_TRY: {
            add_blockframe(ip, vm, vm->blockstack, CAST_INT(arg)->val->to_int(), TRY_BLOCK);
            break;
        }

        case FINISH_TRY: {
            pop_blockframe(vm->blockstack);
            break;
        }

        case BINOP_EXC_CMP: {
            object* self=pop_dataframe(vm->objstack);
            object* other=pop_dataframe(vm->objstack);
            if (object_istype(CAST_TYPE(self), other->type) || object_issubclass(other, CAST_TYPE(self))){
                add_dataframe(vm, vm->objstack, new_bool_true());
                break;
            }
            add_dataframe(vm, vm->objstack, new_bool_false());
            break;
        }

        case BINOP_GT: {
            object* right=pop_dataframe(vm->objstack);
            object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_gt(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand type for >: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            break;
        }

        case BINOP_GTE: {
            object* right=pop_dataframe(vm->objstack);
            object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_gte(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand type for >: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            break;
        }

        case BINOP_LT: {
            object* right=pop_dataframe(vm->objstack);
            object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_lt(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand type for >: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            break;
        }

        case BINOP_LTE: {
            object* right=pop_dataframe(vm->objstack);
            object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_lte(left, right);
            if (ret!=NULL){
                add_dataframe(vm, vm->objstack, ret);
            }
            else{
                vm_add_err(&TypeError, vm, "Invalid operand type for >: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
            }
            break;
        }

        case EXTRACT_ITER: {
            object* it=pop_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, it->type->slot_iter(it));
            break;
        }

        case FOR_TOS_ITER: {
            if (vm->blockstack->size==0 || vm->blockstack->head->type!=FOR_BLOCK || (vm->blockstack->head->type==FOR_BLOCK && vm->blockstack->head->arg!=CAST_INT(arg)->val->to_int()) ){
                add_blockframe(ip, vm, vm->blockstack, CAST_INT(arg)->val->to_int(), FOR_BLOCK);
                vm->blockstack->head->start_ip-=2;
            }
            object* it=peek_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, it->type->slot_next(it));
            if (vm->exception!=NULL){
                DECREF(vm->exception);
                vm->exception=NULL;
                (*ip)=CAST_INT(arg)->val->to_long();
                pop_blockframe(vm->blockstack);
            }
            break;
        }

        case JUMP_TO: {
            (*ip)=CAST_INT(arg)->val->to_long();
            break;
        }

        case BREAK_LOOP: {
            if (vm->blockstack->head->type!=FOR_BLOCK){
                break;
            }
            (*ip)=vm->blockstack->head->arg;
            pop_blockframe(vm->blockstack);
            break;
        }

        case CONTINUE_LOOP: {
            if (vm->blockstack->head->type!=FOR_BLOCK){
                break;
            }
            (*ip)=vm->blockstack->head->start_ip;
            break;
        }

        case UNPACK_SEQ: {
            object* o=peek_dataframe(vm->objstack);
            uint32_t len=CAST_INT(o->type->slot_mappings->slot_len(o))->val->to_int();
            if (len>CAST_INT(arg)->val->to_int()){
                vm_add_err(&ValueError, vm, "Too many values to unpack, expected %d", len, CAST_INT(arg)->val->to_int());
                return NULL;
            }
            if (len<CAST_INT(arg)->val->to_int()){
                vm_add_err(&ValueError, vm, "Not enough values to unpack, expected %d", len, CAST_INT(arg)->val->to_int());
                return NULL;
            }
            for (uint32_t i=len; i>0; i--){
                add_dataframe(vm, vm->objstack, o->type->slot_mappings->slot_get(o, new_int_fromint(i-1)));
            }
            break;
        }

        case BINOP_IADD:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_add(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand type for +: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                return NULL;
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            break;
        }

        case BINOP_ISUB:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_sub(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand type for -: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                return NULL;
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            break;
        }

        case BINOP_IMUL:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_mul(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand type for *: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                return NULL;
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            break;  
        }

        case BINOP_IDIV:{
            struct object* right=pop_dataframe(vm->objstack);
            struct object* left=pop_dataframe(vm->objstack);
            
            object* ret=object_div(left, right);
            if (ret==NULL){
                vm_add_err(&TypeError, vm, "Invalid operand type for /: '%s', and '%s'.", left->type->name->c_str(), right->type->name->c_str());
                return NULL;
            }
            vm_add_var_locals(vm, CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg), ret);
            break;
        }

        case IMPORT_NAME: {
            object* name=CAST_CODE(vm->callstack->head->code)->co_names->type->slot_mappings->slot_get(CAST_CODE(vm->callstack->head->code)->co_names, arg);
            string nm=*CAST_STRING(name)->val;

            string data="";
                    
            string name_=nm+".fpl";

            struct stat st;
            if( stat(nm.c_str(),&st) == 0 || stat(name_.c_str(),&st) == 0 ){
                if( st.st_mode & S_IFDIR ){//Directory
                    object* dict=new_dict();
                    
                    DIR *dr;
                    struct dirent *en;
                    dr = opendir(nm.c_str());
                    if (dr) {
                        while ((en = readdir(dr)) != NULL) {
                            if (string(en->d_name)==string(".") || string(en->d_name)==string("..")){
                                continue;
                            }

                            
                            string extension="";
                            string filename="";
                            bool extwr=true;
                            for (int i=string(en->d_name).size(); i>0; i--){
                                if (string(en->d_name).at(i-1)=='.' && extwr){
                                    extwr=false;
                                    continue;
                                }
                                if (!extwr){
                                    filename+=string(en->d_name).at(i-1);
                                }
                                if (extwr){
                                    extension+=string(en->d_name).at(i-1);
                                }
                            }
                            reverse(extension.begin(), extension.end());
                            reverse(filename.begin(), filename.end());
                            if (extension!="fpl"){
                                continue;
                            }

                            //try en->d_name
                            //Later try nm as folder
                            FILE* f=fopen((nm+"/"+string(en->d_name)).c_str(), "rb");
                            if (f==NULL){
                                vm_add_err(&ImportError, vm, "'%s' not found", (nm+"/"+string(en->d_name)).c_str());
                                return NULL;
                            }


                            fseek(f, 0, SEEK_END);
                            long fsize = ftell(f);
                            fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

                            char *s = (char*)malloc(fsize + 1);
                            int i=fread(s, fsize, 1, f);
                            if (i==0 && fsize>0){
                                vm_add_err(&InvalidOperationError, vm, "Unable to read from file");
                                return NULL;
                            }
                            s[fsize] = 0;
                            string str(s);
                            
                            object* o=import_name(str, str_new_fromstr(filename));
                            dict_set(dict, str_new_fromstr(filename), o);
                        }
                        closedir(dr);
                    }
                    
                    object* o=module_new_fromdict(dict, str_new_fromstr(string(nm)));
                    add_dataframe(vm, vm->objstack, o);

                    break;
                    
                }
                else{ //File
                    //try nm.fpl
                    //Later try nm as folder
                    FILE* f=fopen(name_.c_str(), "rb");

                    fseek(f, 0, SEEK_END);
                    long fsize = ftell(f);
                    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

                    char *s = (char*)malloc(fsize + 1);
                    int i=fread(s, fsize, 1, f);
                    if (i==0 && fsize>0){
                        vm_add_err(&InvalidOperationError, vm, "Unable to read from file");
                        return NULL;
                    }
                    s[fsize] = 0;
                    string str(s);
                    data=str;
                }
            }
            else{
                
                bool done=false;
                for (int i=0; i<nmodules; i++){
                    if (istrue(object_cmp(name, CAST_MODULE(modules[i])->name, CMP_EQ))){
                        add_dataframe(vm, vm->objstack, modules[i]);
                        done=true;
                        break;
                    }
                }
                if (done){
                    break;
                }

                vm_add_err(&ImportError, vm, "'%s' not found", nm.c_str());
                return NULL;
            }
            
            

            object* o=import_name(data, name);
            add_dataframe(vm, vm->objstack, o);

            break;
        }

        case IMPORT_FROM_MOD: {
            object* names=pop_dataframe(vm->objstack);
            object* lib=pop_dataframe(vm->objstack);
            for (uint32_t i=0; i<CAST_INT(names->type->slot_mappings->slot_len(names))->val->to_int(); i++){
                object* o=object_getattr(lib, list_index_int(names, i));
                if (o==NULL){
                    DECREF(vm->exception);
                    vm->exception=NULL;
                    vm_add_err(&ImportError,vm, "Cannot import name '%s' from '%s'",CAST_STRING(list_index_int(names, i))->val->c_str(), CAST_STRING(CAST_MODULE(lib)->name)->val->c_str());
                    return NULL;
                }
                vm_add_var_locals(vm, list_index_int(names, i), o);
            }
            break;
        }

        case MAKE_SLICE: {
            object* end=pop_dataframe(vm->objstack);
            object* start=pop_dataframe(vm->objstack);
            object* base=peek_dataframe(vm->objstack);
            add_dataframe(vm, vm->objstack, slice_new_fromnums(start, end));
            break;
        }

        case DEL_SUBSCR: {
            object* idx=pop_dataframe(vm->objstack);
            object* base=pop_dataframe(vm->objstack);
            object_del_item(base, idx);
            break;
        }

        case DEL_NAME: {
            vm_del_var_locals(vm, list_get(CAST_CODE(vm->callstack->head->code)->co_names, arg));
            break;
        }
        
        default:
            return NULL;
            
    };
    return NULL;
}

object* run_vm(object* codeobj, uint32_t* ip){
    object* code=CAST_CODE(codeobj)->co_code;
    object* lines=CAST_CODE(codeobj)->co_lines;
    
    uint32_t linetup_cntr=0;
    object* instruction;
    uint32_t instructions=CAST_CODE(codeobj)->co_instructions;
    object* linetup=list_index_int(lines, linetup_cntr);
    
    vm->callstack->head->line=list_index_int(linetup, 2);
    while ((*ip)<instructions){
        instruction=list_index_int(code, (*ip)++);
        if (((*ip)-1)/2>=(*CAST_INT(list_index_int(linetup, 1))->val)){
            linetup=list_index_int(lines, linetup_cntr++);
            vm->callstack->head->line=list_index_int(linetup, 2);
        }
        //cout<<instruction<<","<<list_index_int(code, *ip)<<endl;
        object* obj=_vm_step(instruction, list_index_int(code, (*ip)++), vm, ip);

        if (obj==CALL_ERR){
            struct blockframe* frame=in_blockstack(vm->blockstack, TRY_BLOCK);
            if (frame!=NULL && (frame->arg==3 || frame->arg%2==0)){// && frame->callstack_size==vm->callstack->size){
                if (vm->callstack->size-frame->callstack_size!=0){
                    return NULL;
                }
                add_dataframe(vm, vm->objstack, vm->exception);
                frame->obj=INCREF(vm->exception);
                if (vm->exception!=NULL){
                    DECREF(vm->exception);
                }
                vm->exception=NULL;
                frame->other=linetup_cntr;
                
                (*ip)=frame->arg+4; //skip jump
                frame->arg=1;
                continue;
            }

            if (vm->exception==NULL){
                return NULL;
            }
            
            print_traceback();
            
            cout<<vm->exception->type->name->c_str();
            if (CAST_EXCEPTION(vm->exception)->err!=NULL){
                cout<<": "<<object_cstr(CAST_EXCEPTION(vm->exception)->err);
            }
            cout<<endl;
    
            if (vm->exception!=NULL){
                DECREF(vm->exception);
            }
            vm->exception=NULL;
            return NULL;
        }  

        if (obj!=NULL){
            return obj;
        }
        else if (vm->exception!=NULL){
            struct blockframe* frame=in_blockstack(vm->blockstack, TRY_BLOCK);
            if (frame!=NULL && (frame->arg==3 || frame->arg%2==0)){
                if (vm->callstack->size-frame->callstack_size!=0){
                    return NULL;
                }
                add_dataframe(vm, vm->objstack, vm->exception);
                frame->obj=INCREF(vm->exception);
                if (vm->exception!=NULL){
                    DECREF(vm->exception);
                    vm->exception=NULL;
                }
                frame->other=linetup_cntr;
                
                (*ip)=frame->arg+4; //skip jump
                frame->arg=1;
                continue;
            }
            else if (frame!=NULL && frame->obj!=NULL && frame->arg!=3){
                print_traceback();
                
                cout<<frame->obj->type->name->c_str();
                if (CAST_EXCEPTION(frame->obj)->err!=NULL){
                    cout<<": "<<object_cstr(CAST_EXCEPTION(frame->obj)->err);
                }
                cout<<endl;
                if ((void*)frame->obj==(void*)vm->exception){ //Reraised
                    return NULL;
                }
                cout<<endl<<"While handling the above exception, another exception was raised."<<endl<<endl;
            }
            
            
            print_traceback();
            
            cout<<vm->exception->type->name->c_str();
            if (CAST_EXCEPTION(vm->exception)->err!=NULL){
                cout<<": "<<object_cstr(CAST_EXCEPTION(vm->exception)->err);
            }
            cout<<endl;
    
            if (vm->exception!=NULL){
                DECREF(vm->exception);
            }
            vm->exception=NULL;
            return NULL;
        }
    }
    return new_none();
}
