#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <ostream>
#include <cmath>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <float.h>
#include <stdlib.h>
#include <chrono>


#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <sys/types.h>

#include <iostream>

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
    if (argc==1){
        cout<<"Falcon Programming Language V1.00"<<endl;
        cout<<"Eric Buehler 2022"<<endl;
        cout<<"Type copyright() for copyright and license information, exit() to exit"<<endl<<endl;
        //Prep constants and types
        new_gc();
        setup_types_consts();
        setup_modules();

        struct compiler* compiler = new_compiler();
        vm=new_vm(0, NULL, compiler->instructions, NULL); //data is still in scope...
        vm->globals=new_dict();
        vm->callstack->head->locals=INCREF(vm->globals);
        
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
                continue;
            }
            if (data=="!exit"){
                break;
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
                return -1;
            }

            glblfildata=new string(data);
            vm_->filedata=&data;

            object* code=compile(compiler, ast);
            if (code==NULL){
                cout<<parseretglbl.header<<endl;
                cout<<parseretglbl.snippet<<endl;
                printf("%s\n",parseretglbl.error);
                return -1;
            }
            vm=vm_;

            vm->callstack->head->code=code;
            vm->ip=0;
            
            auto a=time_nanoseconds();
            object* returned=run_vm(code, &vm->ip);
            auto b=time_nanoseconds();

            if (returned==TERM_PROGRAM){
                return 0;
            }
            
            compiler_del(compiler);
            DECREF(code);
            compiler = new_compiler();
            //cout<<endl;
            if (returned!=NULL){
                DECREF(returned);
            }
        }
        return 0;
    }
    if (argc==2){
        if ((string)argv[1]==(string)"-h"){
            cout<<"FPL V1\n";
            cout<<"Eric Buehler 2022\n\n";
            cout<<"Standard run: fpl [PROGRAM NAME]\n";
            cout<<"Verbose run: fpl [PROGRAM NAME] -v\n";
            cout<<"Object dump run: fpl [PROGRAM NAME] -o\n";
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
        program=argv[1];
    }
    if (argc==3){
        program=argv[1];
        if ((string)argv[2]==(string)"-v"){
            verbose=true;
        }
        if ((string)argv[2]==(string)"-o"){
            objdump=true;
        }
    }

    if (argc==4){
        program=argv[1];
        if ((string)argv[2]==(string)"-v" || (string)argv[3]==(string)"-v"){
            verbose=true;
        }
        if ((string)argv[2]==(string)"-o" || (string)argv[3]==(string)"-o"){
            objdump=true;
        }
    } 
    
    execute(loadFile(program), objdump, verbose);
    return 0;
}