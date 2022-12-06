# Falcon Programming Language - Version 1.2.4
### Eric Buehler 2022 ###

<img src="logo.png"/>

Falcon is a multithreaded, multi-paradigm interpreted programming language, with a focus on readability and simplicity. It is both strongly and dynamically typed, and has automatic garbage collection based on reference counting and a tri-generational garbage collector. In comparison to other languages, it a hybrid of Python and C, with the benefits of Python and the syntax of C.

### Documentation ##
- [Dunder Attributes](https://github.com/EricLBuehler/Falcon-Programming-Language/blob/main/docs/DUNDER_ATTRS.md)
- [Types of functions](https://github.com/EricLBuehler/Falcon-Programming-Language/blob/main/docs/FUNCTIONS.md)
- [Equivalent C++ Types to Falcon Types](https://github.com/EricLBuehler/Falcon-Programming-Language/blob/main/docs/TYPE_CPPBASE.md)
- [Builtin type methods](https://github.com/EricLBuehler/Falcon-Programming-Language/blob/main/docs/TYPE_DOCS.md)
- [Conventions](https://github.com/EricLBuehler/Falcon-Programming-Language/blob/main/docs/CONVENTIONS.md)
- [Builtin Functions](https://github.com/EricLBuehler/Falcon-Programming-Language/blob/main/docs/BUILTINS.md)
- [Builtin Modules](https://github.com/EricLBuehler/Falcon-Programming-Language/blob/main/docs/builtin_module_docs/BUILTIN_MODULES.MD)
- [Syntax](https://github.com/EricLBuehler/Falcon-Programming-Language/blob/main/docs/SYNTAX.md)
- [Config Files](https://github.com/EricLBuehler/Falcon-Programming-Language/blob/main/docs/CONFIG.md)

### Sample Program ###

<details>
<summary>Program</summary>

```js
m="Placeholder value"

class Falcon{
    var=m
    func sound(self){
        print("Sound!")
    }
}


class Peregrine(Falcon){
    var=m
    func __new__(self){
        print("__new__ called")
        return super(self).__new__(self)
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

x.sound()

print(x.__bases__)

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


try{
    if 500 is 200{
        raise Exception("500 is not the same object as 200")
    }
    elif 500 is 500{
        raise TypeError("500 is not the same object as 200, but is the same as 500")
    }
    else{
        raise NameError("Else condition reached")
    }
}
except Exception e{
    print(e)
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

try{
    x()
}
except Exception e{
    print(e)
}

val=2
print(val.__mul__(5))

func fib(n){
    if (n<=1){
        return n
    }
    else{
        return fib(n-1)+fib(n-2)
    }
}

print(fib(10))

pi=238649.2131693410000000
x=pi*100
print(str(pi))
print(int("1234"))
print(float("10.222"))

for n in [10,20,30]{
    print(n)
}

i=0
while i<10{
    print(i)
    i+=1
}

print(True and False)
print(True or False)
print(not 1==1)

print(100^2)
print(100^2*3)
print(100^6)
```

</details>

When run with ```./fpl.exe program.fpl```

<details>
<summary>Output</summary>

```
__new__ called        
<Peregrine @ 0xba7a98>
__new__ called        
<Peregrine @ 0xba7b40>
5
Value
Sound!
[<class 'Falcon'>, <class 'object'>]
Function f says:  
A
True
Done
4
2
500 is not the same object as 200, but is the same as 500
1
2
3
I is 50!
10
55
238649.213169341
1234
10.222
10
20
30
False
True
False
102
98
98
```

</details>

Click [here](https://github.com/EricLBuehler/Falcon-Programming-Language/tree/main/example) for more example programs.

### Usage ###
Build Falcon in debug mode
```./build.cmd```

Build Falcon in release mode
```./release.cmd```

Run a script
```falcon program```

Manual compilation
```g++ main.cpp -o falcon -static-libstdc++ -static -lws2_32 -O3 -fno-gcse```

Enter REPL
```falcon```

Help
```falcon -h```

### Links ###
- <a href='https://github.com/dannyvankooten/pepper-lang'>Pepper Lang</a>
- <a href='https://github.com/davidcallanan/py-myopl-code'>py-myopl-code</a>
- <a href='https://github.com/faheel/BigInt'>BigInt</a> (Used for ```int``` type)
- <a href='https://github.com/python/cpython'>CPython</a>
- <a href='https://eli.thegreenplace.net/2012/07/12/computed-goto-for-efficient-dispatch-tables'>Computed GOTOs</a>
- <a href='https://github.com/thestr4ng3r/nxjson'>NXJSON</a>

### Licenses ###
- [MIT License](LICENSE)
- [BigInt - MIT License](https://github.com/EricLBuehler/Falcon-Programming-Language/blob/main/object/BigInt.hpp)
