# Falcon Programming Language - Revision 3
### Eric Buehler 2022 ###

Falcon is an object-oriented interpreted programming language, with a focus on readability and simplicity. It is dynamically typed, and has automatic garbage collection based on reference counting and a 3 generation generational garbage collector. In comparison to other languages, it a hybrid of Python and C, with the benefits of Python and the syntax of C.

Falcon is implemented in C++. However, although the compiler and interpreter could mostly be compiled in C, and the object oriented featured are used heavily in the lexer and parser.

### Requirements ###
- GCC compiler installed
- Only tested on Windows

### Sample Program ###
```
class Peregrine{
    a="Falcon"
}

func f(){
    print("Hello!")
}

x=Peregrine()

y=Peregrine()
```

### Usage ###
Build entire Falcon programming language
```build.bat```

Run a script
```./fpl program```

Help
```./fpl -h```

### License ###
[MIT License](LICENSE)
