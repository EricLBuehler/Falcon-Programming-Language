# Falcon Programming Language
### Eric Buehler 2022 ###

<img src="https://github.com/EricLBuehler/Falcon-Programming-Language/blob/a18101360097063069c242e865a4595219c0f937/logo.jpg" width="208" height="150" align="right" />

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

x=Peregrine()

y=Peregrine()

a=Peregrine.a
b=x.a

func f(x,c="A"){
    print("Function f says: ","")
    print(c)
}

f(1,2)
```

### Usage ###
Build entire Falcon programming language
```build.bat```

Run a script
```./fpl program```

Enter REPL
```./fpl```

Help
```./fpl -h```

### Links ###
- https://github.com/dannyvankooten/pepper-lang
- https://github.com/faheel/BigInt

### License ###
[MIT License](LICENSE)
