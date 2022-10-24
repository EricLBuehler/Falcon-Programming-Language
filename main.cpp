#include "fpl.h"

int main(int argc, char** argv) {
    signal(SIGINT, sigint);
    program="main.fpl";
    bool verbose=false;
    bool objdump=false;

    
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
    kwds.push_back("assert");
    kwds.push_back("continue");
    kwds.push_back("lambda");
    kwds.push_back("yield");
    kwds.push_back("with");

    glblargc_raw=argc;
    glblargv_raw=argv;
    
    if (argc==1){
        try{
            cout<<"Falcon Programming Language V"<<FPL_VERSION<<endl;
            cout<<"Eric Buehler 2022"<<endl;
            cout<<"Type copyright() for copyright and license information, exit() to exit"<<endl<<endl;
            
            //Prep constants and types
            new_gc();
            setup_types_consts();
            setup_modules();

            struct compiler* compiler = new_compiler();
            vm=new_vm(0, NULL, compiler->instructions, NULL); //data is still in scope...
            dict_set(::vm->globals, str_new_fromstr("__annotations__"), ::vm->callstack->head->annotations);
            dict_set(::vm->globals, str_new_fromstr("__name__"), str_new_fromstr("__main__"));
            
            while (true){
                struct vm* vm_=vm;
                vm=NULL;
                
                string data="";
                cout<<">>> ";
                getline(cin,data);
                if(!cin){
                    cin.clear();
                    while (!hit_sigint){}
                    hit_sigint=false;
                    vm=vm_;
                    continue;
                }
                
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
                    vm=vm_;
                    continue;
                }

                glblfildata=new string(data);
                vm_->filedata=&data;
                
                object* code=compile(compiler, ast, 0);
                if (code==NULL){
                    cout<<parseretglbl.header<<endl;
                    cout<<parseretglbl.snippet<<endl;
                    printf("%s\n",parseretglbl.error);
                    vm=vm_;
                    continue;
                }
                vm=vm_;

                vm->callstack->head->code=code;
                vm->ip=0;
                
                object* returned=run_vm(code, &vm->ip);

                if (returned==TERM_PROGRAM && vm->exception==NULL){
                    finalize_threads();
                        
                    socket_cleanup();
                    return 0;
                }
                
                compiler_del(compiler);
                FPLDECREF(code);
                compiler = new_compiler();
                //cout<<endl;
                if (returned!=NULL){
                    FPLDECREF(returned);
                }
            }

            finalize_threads();
                
            socket_cleanup();
            return 0;
        }
        catch (std::bad_alloc){
            memory_error();
        }
    }
    if (argc==2){
        if ((string)argv[1]==(string)"-h"){
            cout<<"FPL V1\n";
            cout<<"Eric Buehler 2022\n\n";
            cout<<"Standard run: fpl [PROGRAM NAME]\n";
            cout<<"Enter REPL: fpl \n";
            cout<<"Help: fpl -h\n";
            cout<<"Licenses: fpl -l\n";
            return 0;
        }
        if ((string)argv[1]==(string)"-l"){
            cout<<"FPL License\n";
            cout<<FPL_LICENSE;
            return 0;
        }
    }
    if (argc>=2){
        program=argv[1];
    }

    #ifdef DEBUG
    verbose=true;
    #endif
    
    try{
        execute(loadFile(program), objdump, verbose);
        return 0;
    }
    catch (std::bad_alloc){
        memory_error();
    }
}