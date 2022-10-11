
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
        return TERM_PROGRAM;
    }
    
    struct vm* vm_=::vm;
    ::vm=new_vm(0, code, compiler->instructions, &data); //data is still in scope...
    
    ::vm->globals=glbls;
    ::vm->callstack->head->locals=locals;
    dict_set(::vm->globals, str_new_fromstr("__annotations__"), ::vm->callstack->head->annontations);
    ::vm->global_annotations=::vm->callstack->head->annontations;

    object* ret=run_vm(code, NULL, &::vm->ip);
    object* dict=::vm->callstack->head->locals;
    
    vm_del(::vm);
    ::vm=vm_;

    return new_none();
}