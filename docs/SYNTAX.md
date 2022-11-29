# Falcon Keywords and Syntax


Note: **Words in all-caps may be substituted by the user**

## Keywords

- ```func```
- ```class```
- ```return```
- ```if```
- ```else```
- ```elif```
- ```raise```
- ```try```
- ```except```
- ```finally```
- ```for```
- ```break```
- ```while```
- ```import```
- ```from```
- ```del```
- ```assert```
- ```continue```
- ```lambda```
- ```yield```
- ```with```

______________________

### ```func```

The ```func``` keyword is used to define functions. The basic format is as follows:

```
func NAME(*args, **kwargs){
    CODE
}
```

The arguments (positional and keyword) may also include annotations in the form below:

```
NAME: TYPE = VALUE
```

### ```class```

The ```class``` keyword is used to define classes. The basic format is as follows:

```
class NAME {
    BODY
}
```

Inherited may be implemented as follows, with ```BASES``` representing a tuple of bases:

```
class NAME (BASES) {
    BODY
}
```

### ```return```
The ```return``` keyword allows the user to return a value from a function. It may only be used in functions.


### ```if```, ```elif```, ```else```
The ```if```, ```elif```, and ```else``` keywords implement control flow. 

Control flow blocks begin with an ```if``` keyword, followed by an expression, and then code that will execute if the condition is truthy. See below:

```
if 1+1==2 {
    print("1+2 is equal to 2")
}
```

Next, ```elif``` blocks may be chained to optimally test other cases, providing the cases above it are falsy. They follow the same format as ```if```.

```
if 1+2==2 {
    print("This is not true.")
}
elif 1+1==2 {
    print("1+2 is equal to 2")
}
```


Finally, an ```else``` block may be written to execute if none of the above conditions are truthy.

```
if 1+2==2 {
    print("This is not true.")
}
elif 1+3==2 {
    print("1+3 is equal to 2")
}
else {
    print("Nothing else is true.")
}
```


### ```raise```
The ```raise``` keyword may be used to raise exceptions at runtime. The syntax is as follows:

```
raise EXCEPTION
```

Where ```EXCEPTION``` is an object that is a subclass of ```Exception```


### ```try```, ```except```, ```finally```
The ```try```, ```except```, and ```finally``` keywords implement exception handling. 

These blocks begin with an ```try``` keyword, and then a code block.

```
try {
    CODE
}
```

Next, ```except``` blocks may be chained to handle any raised exceptions.

```
try {
    CODE
}
except EXCEPTION NAME{
    CODE
}
```

Where the exception is optionally stored in ```NAME```. See below for the other option, where the exception is not stored.

```
try {
    CODE
}
except EXCEPTION{
    CODE
}
```

Finally, an ```finally``` block may be written to unconditionally execute after the above blocks.

```
try {
    CODE
}
except EXCEPTION {
    CODE
}
finally {
    CODE
}
```

### ```for```
The ```for``` loop provides a method to iterate over an iterator. See below:

```
for NAME in ITERATOR {
    CODE
}
```

An else statement may be placed below a for loop to execute if the for loop runs without ```break```ing out.
```
for NAME in ITERATOR {
    CODE
}
else{
    CODE
}
```

### ```while```
The ```while``` loop provides a method to conditionally loop while the given expression is truthy.

```
CODE
while EXPRESSION {
    CODE
}
```

An else statement may be placed below a while loop to execute if the for loop runs without ```break```ing out.
```
CODE
while EXPRESSION {
    CODE
}
else{
    CODE
}
```

### ```break```
The ```break``` keyword breaks the control flow out of a loop, which may be a ```for``` loop or a ```while``` loop.

### ```continue```
The ```continue``` keyword contiinues the control flow out of a loop, which may be a ```for``` loop or a ```while``` loop. The control flow goes to the top of the loop header, bypassing any other code.

### ```import```, ```from```
The ```import``` keyword imports a module by name. See below for a simple use case:

```import NAME```

The ```from``` keyword may also be used to import objects from a module:

```from NAME import ATTR, ATTR2```

Imported modules may also be stored under an alternative, specified name:

```import MODULE as NAME```

### ```del```
The ```del``` keyword facilitates deleting variables, items, and slices.

Deleting variable:
```del NAME```

Deleting item:
```del SEQ[IDX]```

Deleting slice:
```del SEQ[START:STOP:STEP]```

### ```assert```
The ```assert``` keyword allows pass-fail conditions to be implemented. If the provided expression is truthy, then the statement passes with no side-effects. Otherwise, an ```AssertionError``` is raised.

```assert EXPR```

### ```lambda```
The ```lambda``` keyword allows for anonymous function creation. The format is essentially the same as the ```func``` keyword, with the only difference being the lack of a specified name and the reccomendation to keep the function body short, and on one line. A ```lambda``` function **is** considered a function, and both ```return``` and ```yield``` may be used.

```lambda x, y {return x+y}```

### ```yield```
The ```yield``` keyword is used to create a generator function. It may only be used inside of a function, and will temporarily transfer control flow back to the caller, along with a return value (expression).

```yield EXPR```

### ```with```
The ```with``` keyword is used to simplify the opening and closing of file descriptors and other, similar objects. It uses the ```__enter__``` and ```__exit__``` methods to implement this functionality, and the syntax is as follows:

```
with EXPRESSION NAME {
    CODE
}
```

Where ```EXPRESSION``` represents an expression that creates an object that defines ```__enter__``` and ```__exit__```, and ```NAME``` represents the name for that expression to be stored as.

## Syntax

### ```:```
Global variable access

### ```::```
Nonlocal variable access

### Slicing
Slicing allows a segment, or pattern, from a sequence to be extracted. A slice object is generated.

```SEQ[START:STOP:STEP]```

A ```slice``` object will be created, and used to subscript ```SEQ```. The behavior is not defined by ```slice```, as it is merely a container for the defined values.
Although no values (```START```, etc.) are required, values that are not provided will be filled in with ```None``` in the ```slice``` object attributes.

### Indexing
Indexing allows an item to be retrieved froma sequence.

```SEQ[IDX]```

### Type annotations
Type annotations are suggested hints may be present in function arguments, or in assignment statements.

```NAME: TYPE```

### Function return value type annotation
Function return value type annotations define a suggested hint about the return value of a function, and are written differently for normal ```func``` functions and ```lambda``` functions. 

```func``` functions:

```
<function definition>: TYPE {
    CODE
}
```

```lambda``` functions:
```<function definition> { CODE } : TYPE```

### Format strings
Format strings using code snippets
```f"Text {CODE}"```

### Byte strings
Creates a bytes object from a string
```b"Text```

### Unicode escape
Encode a unicode character using a 4 digit hexadecimal number, where ```XXXX``` represents the mandatory 4 digit hexadecimal string.
```\uXXXX```

### Unicode name literal
Encode a unicode character using its name, where ```...``` represents a valid name that represents a unicode code point.
```\N...```

### Attribute access
```OBJ.ATTR```

### *args and **kwargs
```*args``` and ```**kwargs``` are a way of accepting variable positional and keyword arguments. ```*args```, or ```*NAME```, will store any extraneous positional arguments in a ```tuple```, in a variable named ```NAME```. Similarly, ```**kwargs```, or ```**NAME```, will store any extraneous keyword arguments in a ```dict```, in a variable named ```NAME```.

### Argument unpacking
When calling an object, a syntax similar to ```*args``` and ```**kwargs``` may be used for argument unpacking. In the call, a ```*NAME``` will unpack iterator ```NAME``` into the positional argument list. Similarly, a ```**NAME``` will unpack ```dict``` ```NAME``` into the keyword argument list.