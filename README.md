# Falcon Programming Language
### Eric Buehler 2022 ###

<img src="https://github.com/EricLBuehler/Falcon-Programming-Language/blob/a18101360097063069c242e865a4595219c0f937/logo.jpg" width="208" height="150" align="right" />

Falcon is an object-oriented interpreted programming language, with a focus on readability and simplicity. It is dynamically typed, and has automatic garbage collection based on reference counting and a 3 generation generational garbage collector. In comparison to other languages, it a hybrid of Python and C, with the benefits of Python and the syntax of C.

Falcon is implemented in C++. However, although the compiler and interpreter do not use object oriented features and could mostly be compiled in C, the lexer and parser make heavy use of such features.

### Requirements ###
- GCC compiler installed
- Only tested on Windows

### Sample Program ###
```js
m="Placeholder value"

class Peregrine{
    var=m
    func f(self){
        print(self.var)
    }
}

x=Peregrine()

y=Peregrine()

a=Peregrine.var
x.var=5
y.var="Value"
b=x.var

x.f()
y.f()

func f(x,c="A"){
    print("Function f says: ","")
    print(c)
}

f(1)

dictionary={1:[1,2,3], 2:{1:"A"}, "Hello":"World", [123]:2}
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
Below are useful references.
- <a href='https://github.com/dannyvankooten/pepper-lang'>Pepper Lang</a>
- <a href='https://github.com/davidcallanan/py-myopl-code'>py-myopl-code</a>
- <a href='https://github.com/faheel/BigInt'>BigInt</a> (I used this library)
- <a href='https://github.com/python/cpython'>CPython</a>

### Licenses ###
- [MIT License](LICENSE)
- [BigInt - MIT License](https://github.com/EricLBuehler/Falcon-Programming-Language/blob/main/object/BigInt.hpp)
