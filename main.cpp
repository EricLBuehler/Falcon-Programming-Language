#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <ostream>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <float.h>
#include <stdlib.h>
#include <map>
using namespace std;

#define DEBUG

string program;

#include "utilities/utils.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "object/object.h"
#include "compiler/compiler.h"
#include "vm/vm.h"

int execute(string data, bool objdump, bool verbose){
    //Prep constants and types
    new_gc();
    setup_types_consts();

    vector<string> kwds;
    kwds.push_back("func");
    kwds.push_back("class");

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


    Parser parser(lexer.tokens, data);
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


    object* code=compile(compiler, ast);
    if (verbose){
        cout<<(*CAST_INT(CAST_CODE(code)->co_code->type->slot_len(CAST_CODE(code)->co_code))->val)/2<<" instructions."<<endl;
        cout<<object_cstr(CAST_CODE(code)->co_code->type->slot_len(CAST_CODE(code)->co_code))<<" bytes."<<endl<<endl;
    }

    vm=new_vm(0, code, compiler->instructions, &data); //data is still in scope...
    vm->globals=new_dict();
    vm->callstack->head->locals=INCREF(vm->globals);

    if (verbose){
        cout<<"Names: "<<object_cstr(CAST_CODE(code)->co_names)<<"\n";
        cout<<"Consts: "<<object_cstr(CAST_CODE(code)->co_consts)<<"\n";
        cout<<"Code: "<<object_cstr(CAST_CODE(code)->co_code)<<"\n";
        cout<<"--------\n";
    }
    object* returned=run_vm(code, &vm->ip);
    if (verbose){
        cout<<"--------";
    }
    
    if (returned==CALL_ERR || returned==NULL){
        return -1;
    }
    if (verbose){
        cout<<"\nReturned: "<<object_cstr(returned);

        if (!vm->haserr){
            cout<<"\nIP: "<<vm->ip<<"\n\n";
            
            for (auto k: (*CAST_DICT(vm->globals)->val)){
                cout<<(*CAST_STRING(object_str(k.first))->val)<<" = "<<(*CAST_STRING(object_str(k.second))->val)<<endl;
            }
        }
        else{
            delete &lexer;
            compiler_del(compiler);
            vm_del(vm);
            DECREF(code);
            DECREF(returned);
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

int main(int argc, char** argv) {
    program="main.fpl";
    bool verbose=false;
    bool objdump=false;

    if (argc==2){
        if ((string)argv[1]==(string)"-h"){
            cout<<"FPL V1\n";
            cout<<"Eric Buehler 2022\n\n";
            cout<<"Standard run: fpl [PROGRAM NAME]\n";
            cout<<"Verbose run: fpl [PROGRAM NAME] -v\n";
            cout<<"Object dump run: fpl [PROGRAM NAME] -o\n";
            cout<<"Enter REPL: fpl \n";
            cout<<"Help: fpl -h\n";
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