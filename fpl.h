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
#include <mutex>
#include <set>


#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>

#include <iostream>

using namespace std;

#define CALL_ERR (object*)0x1000
#define TERM_PROGRAM (object*)0x2000
#define SUCCESS (object*)0x3000

#define ERROR_RET(v) if (v==NULL){return CALL_ERR;};if (v==TERM_PROGRAM){return TERM_PROGRAM;};
#define LIST_TUP_LEN(l) CAST_TUPLE(l)->size

#define GIL_MAX_SWITCH 128

#define FPL_VERSION 1.02

std::mutex GIL;

string program;

string FPL_LICENSE=R""""(
MIT License

Copyright (c) 2022 Eric Buehler

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
)"""";

vector<string> kwds;

bool hit_sigint=false;




#include "utilities/utils.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "object/object.h"
#include "compiler/compiler.h"
#include "vm/vm.h"
#include "builtins/lateincludebuiltins.cpp"

#include <signal.h>


void sigint(int sig) {
    signal(sig, SIG_IGN);
    
    if (vm==NULL){
        cout<<"\nKeyboardInterrupt\n";
    }
    else{
        object* exc=vm_setup_err(&KeyboardInterrupt, vm, "");
        FPLDECREF(CAST_EXCEPTION(exc)->err);
        CAST_EXCEPTION(exc)->err=NULL;
        vm->exception=exc;
    }
    
    signal(SIGINT, sigint);
    hit_sigint=true;
};

int execute(string data, bool objdump, bool verbose){   
    //Prep constants and types
    new_gc();
    setup_types_consts();
    setup_modules();

    Lexer lexer(data,kwds);
    lexer.pos=Position(program);

    Position end=lexer.tokenize();

    int i=0;

    if (verbose){
        i=0;
        for (Token tok:lexer.tokens){
            cout << "Element " << i++ << " : ";
            cout<< tok <<"\n";
        }
    }

    if (verbose){
        cout<<"-------------------------------"<<endl;
    }


    parser=Parser(lexer.tokens, data);
    parse_ret ast=parser.parse();

    if (ast.errornum>0){
        cout<<ast.header<<endl;
        cout<<ast.snippet<<endl;
        cout<<ast.arrows<<endl;
        printf("%s\n",ast.error);
        return -1;
    }

    if (verbose){
        cout<<"Parsed.\n";
    }

    struct compiler* compiler = new_compiler();

    glblfildata=new string(data);
    
    object* code=compile(compiler, ast, 0);
    if (code==NULL){
        cout<<parseretglbl.header<<endl;
        cout<<parseretglbl.snippet<<endl;
        printf("%s\n",parseretglbl.error);
        return -1;
    }
    
    if (verbose){
        cout<<(*CAST_INT(list_len(CAST_CODE(code)->co_code))->val)/2<<" instructions."<<endl;
        cout<<object_cstr(list_len(CAST_CODE(code)->co_code))<<" bytes."<<endl<<endl;
    }

    vm=new_vm(0, code, compiler->instructions, new string(data)); //data is still in scope...
    dict_set(::vm->globals, str_new_fromstr("__annotations__"), ::vm->callstack->head->annotations);

    if (verbose){
        cout<<"Names: "<<object_cstr(CAST_CODE(code)->co_names)<<"\n";
        cout<<"Consts: "<<object_cstr(CAST_CODE(code)->co_consts)<<"\n";
        cout<<"Code: "<<object_cstr(CAST_CODE(code)->co_code)<<"\n";
        cout<<"--------\n";
    }
    
    object* returned=run_vm(code, &vm->ip);
    
    finalize_threads();
    socket_cleanup();

    if (verbose){
        cout<<"--------";
    }
    if (returned==CALL_ERR || returned==NULL || returned==TERM_PROGRAM){
        return -1;
    }
    if (verbose){
        cout<<"\nReturned: "<<object_cstr(returned);

        if (vm->exception==NULL){
            cout<<"\nIP: "<<vm->ip<<"\n\n";

            for (auto k: (*CAST_DICT(vm->globals)->val)){
                cout<<object_cstr(k.first)<<" = "<<(*CAST_STRING(object_repr(k.second))->val)<<endl;
            }
            
        }
        else{
            delete &lexer;
            compiler_del(compiler);
            vm_del(vm);
            FPLDECREF(code);
            FPLDECREF(returned);
            return -1;
        }

        if (objdump){
            cout<<endl<<endl;
            cout<<"Gen 0:\n";
            object* node=gc.gen0;
            while (node){
                cout<<(*CAST_STRING(node->type->slot_repr(node))->val)<<" ";
                cout<<node->type->name->c_str()<<" has "<<node->refcnt<<" ref\n";
                node=node->ob_next;
            }
            cout<<"\n";

            cout<<"Gen 1:\n";
            node=gc.gen1;
            while (node){
                cout<<(*CAST_STRING(node->type->slot_repr(node))->val)<<" ";
                cout<<node->type->name->c_str()<<" has "<<node->refcnt<<" ref\n";
                node=node->ob_next;
            }
            cout<<"\n";

            cout<<"Gen 2:\n";
            node=gc.gen2;
            while (node){
                cout<<(*CAST_STRING(node->type->slot_repr(node))->val)<<" ";
                cout<<node->type->name->c_str()<<" has "<<node->refcnt<<" ref\n";
                node=node->ob_next;
            }
            cout<<"\n";

            cout<<"Immutables:\n";
            
            node=immutable_objs;
            while (node){
                cout<<(*CAST_STRING(node->type->slot_repr(node))->val)<<" ";
                cout<<node->type->name->c_str()<<" has "<<node->refcnt<<" refs\n";
                node=node->ob_next;
            }
            cout<<"\n\n";

            uint32_t total=0;
            
            node=gc.gen0;
            while (node){
                total++;
                node=node->ob_next;
            }
            
            node=gc.gen1;
            while (node){
                total++;
                node=node->ob_next;
            }
            node=gc.gen2;
            while (node){
                total++;
                node=node->ob_next;
            }
            
            node=immutable_objs;
            while (node){
                total++;
                node=node->ob_next;
            }
            

            cout<<"gen0 "<<gc.gen0_n<<"\ngen1 "<<gc.gen1_n<<"\ngen2 "<<gc.gen2_n;
            cout<<"\nImmutable "<<immutable_size;
            cout<<"\nTotal "<<total;
        }
    }
    return 0;
}
