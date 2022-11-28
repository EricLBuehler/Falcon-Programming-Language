object* builtin_eval(object* self, object* args){
    object* str=dict_get_opti_deref(args, str_new_fromstr("string"));
    object* glbls=dict_get_opti_deref(args, str_new_fromstr("globals"));
    object* locals=dict_get_opti_deref(args, str_new_fromstr("locals"));

    if (!object_issubclass(peek_dataframe(vm->objstack), &ExceptionType)){
        vm_add_err(&TypeError, vm, "Exceptions must be subclass of Exception");
        return NULL;
    }

    
    if (!object_issubclass(glbls, &DictType)){
        vm_add_err(&TypeError, vm, "Expected dict object, got '%s' object.", glbls->type->name->c_str());
        return NULL;
    }
    
    if (!object_issubclass(locals, &DictType)){
        vm_add_err(&TypeError, vm, "Expected dict object, got '%s' object.", locals->type->name->c_str());
        return NULL;
    }

    string data=*CAST_STRING(str)->val;

    Lexer lexer(data,kwds);
    lexer.pos=Position(program);

    Position end=lexer.tokenize();

    Parser p=parser;
    parser=Parser(lexer.tokens, data);
    parse_ret ast=parser.parse();
    parser=p;

    if (ast.errornum>0){
        cout<<ast.header<<endl;
        cout<<ast.snippet<<endl;
        cout<<ast.arrows<<endl;
        printf("%s\n",ast.error);
        FPLDECREF(str);
        FPLDECREF(glbls);
        FPLDECREF(locals);
        return TERM_PROGRAM;
    }

    struct compiler* compiler = new_compiler();

    string* g=glblfildata;
    glblfildata=new string(data);
    object* code=compile(compiler, ast, 0);
    glblfildata=g;
    
    if (code==NULL){
        cout<<parseretglbl.header<<endl;
        cout<<parseretglbl.snippet<<endl;
        cout<<parseretglbl.arrows<<endl;
        printf("%s\n",parseretglbl.error);
        FPLDECREF(str);
        FPLDECREF(glbls);
        FPLDECREF(locals);
        return TERM_PROGRAM;
    }
    
    struct vm* vm_=::vm;
    struct vm* vm=new_vm(interpreter.vm_map->size(), code, compiler->instructions, &data); //data is still in scope...
    interpreter_add_vm(interpreter.vm_map->size(), vm);
    
    ::vm->globals=glbls;
    ::callstack_head(vm->callstack).locals=locals;
    dict_set_noret(::vm->globals, str_new_fromstr("__annotations__"), ::callstack_head(vm->callstack).annotations);
    ::vm->global_annotations=::callstack_head(vm->callstack).annotations;

    object* ret=run_vm(code, &::vm->ip);
    object* dict=::callstack_head(vm->callstack).locals;
    
    vm_del(::vm);
    ::vm=vm_;

    FPLDECREF(str);
    FPLDECREF(glbls); //vm_del DECREFs, so this deletes
    FPLDECREF(locals); //Down to 1
    FPLDECREF(locals); //delete

    list_index_int(CAST_BUILTIN(self)->kwargs, 0)=new_dict();
    list_index_int(CAST_BUILTIN(self)->kwargs, 1)=new_dict();

    return new_none();
}