# Falcon Programming Language
### Eric Buehler 2022 ###

<img src="logo.jpg" width="160" height="160" align="right" />

Falcon is an object-oriented interpreted programming language, with a focus on readability and simplicity. It is dynamically typed, and has automatic garbage collection based on reference counting and a tri-generational garbage collector. In comparison to other languages, it a hybrid of Python and C, with the benefits of Python and the syntax of C.

Falcon is implemented in C++. However, although the compiler and interpreter do not use object oriented features and could mostly be compiled in C, the lexer and parser make heavy use of such features.

### Requirements ###
- GCC compiler installed
- Only tested on Windows

### Builtins ###
- ```print```
- ```__build_class__```
- ```id```
- ```input```

<details>
<summary>Exceptions</summary>

- ```Exception```
- ```TypeError```
- ```ValueError```
- ```AttributeError```
- ```KeyError```
- ```NameError```
- ```IndexError```
    
</details>    


<details>
<summary>Types</summary>

- ```str```
- ```int```
- ```list```
- ```dict```
- ```tuple```
- ```object```
    
</details>    

### Sample Program ###

#### Program ####
```js
m="Placeholder value"

class Peregrine{
    var=m
    func __new__(self){
        print("__new__ called")
        return self
    }
    func __init__(self){
        print(self)
    }
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

l=list(1,2,3,4,5)

x=object()
y=object()


true = x == y
maybe = x is y
same = x is x
print(500==500)


print("Done")

print(l[3])
print(dictionary[[123]])

if 500 is 200{
    print("500 is not the same object as 200")
}
elif 500 is 500{
    print("500 is not the same object as 200, but is the same as 500")
}
else{
    print("Else condition reached")
}

i=0

func x(){
    i=i+1

    print(i)
    if (i==3){
        raise ValueError("I is 50!")
    }
    x()
}
x()
```

#### Output ####
When run with ```./fpl.exe program.fpl```

```
__new__ called
<Peregrine object @ 0xf17b78>
__new__ called
<Peregrine object @ 0xf1c110>
5
Value
Function f says: A
True
Done
4
2
In file 'program.fpl', line 59, in <module>
    raise ValueError("500 is not the same object as 200, but is the same as 500")
ValueError: 500 is not the same object as 200, but is the same as 500
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
