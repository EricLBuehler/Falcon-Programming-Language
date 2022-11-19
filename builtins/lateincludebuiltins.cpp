object* builtin_eval(object* self, object* args){
    object* str=args->type->slot_mappings->slot_get(args, str_new_fromstr("string"));
    object* glbls=args->type->slot_mappings->slot_get(args, str_new_fromstr("globals"));
    object* locals=args->type->slot_mappings->slot_get(args, str_new_fromstr("locals"));

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