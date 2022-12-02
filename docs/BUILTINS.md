# Falcon Builtin Functions

### Builtins
- ```print```
- ```__build_class__```
- ```id```
- ```input```
- ```repr```
- ```iter```
- ```next```
- ```round```
- ```enumerate```
- ```range```
- ```zip```
- ```globals```
- ```locals```
- ```alllocals```
- ```len```
- ```copyright```
- ```exit```
- ```isinstance```
- ```eval```
- ```super```
- ```getattr```
- ```setattr```
- ```iscallable```
- ```reverse```
- ```isiter```
- ```min```
- ```max```
- ```map```
- ```abs```
- ```method```
- ```classmethod```
- ```staticmethod```
- ```property```
- ```getannotation```
- ```sum```
- ```hasattr```
- ```dir```
- ```bin```
- ```hex```
- ```ord```
- ```chr```
- ```unicode_name```
- ```compile```

### Exceptions
- ```Exception```
- ```TypeError```
- ```ValueError```
- ```AttributeError```
- ```KeyError```
- ```NameError```
- ```IndexError```
- ```MemoryError```
- ```RecursionError```
- ```StopIteration```
- ```FileNotFoundError```
- ```InvalidOperationError```
- ```ImportError```
- ```KeyboardInterrupt```
- ```AssertionError```
- ```ZeroDivisionError```
- ```OverflowError```
- ```NotImplementedError```


______________________

## ```print```
Print objects with specified seperator and end.

```print(..., sep=' ', end='\n')```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```...```    | objects to print     |
| ```sep```  | object to print that seperates objects |
| ```end``` | object to print last     |

### Notes
Calls ```__str__``` on all objects passed before printing.

### Returns
```None```


______________________

## ```repr```
Get ```__repr__``` of an object

```repr(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | object to get ```__repr__``` of     |

### Notes
Calls ```__repr__``` on object passed.

### Returns
Return value from ```__repr__``` method call.


______________________

## ```__build_class__```
Build a class (```type```) from provided parameters.

```__build_class__(doc, bases, name, func)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```doc```    | Docstring    |
| ```bases```    | Bases to inherit from (left -> right increasing  in inheritance level)    |
| ```name```    | ```str``` object for name    |
| ```func```    | ```function``` object for class body    |

### Notes
```func``` parameter is called, and resulting local variables are static variables for the generated class.

### Returns
Return generated type


______________________

## ```type```

Type object.

There are 2 possible argument lists that may be passed in different scenarios:

### Argument list 1

```type(object)```

Get the type of an object

| Argument      | Information       |
| ------------- | ----------------- |
| ```object```  | Object to get type of |

### Argument list 2

```type(name, bases, dict)```

Generate a new type from arguments. Dissimilar to ```__build_class__``` because it takes a ```dict``` argument directly. ```__doc__``` will be set to ```None```.

| Argument      | Information       |
| ------------- | ----------------- |
| ```name```  | ```str``` object that is the name of new type |
| ```bases```  | ```tuple``` or ```list``` object to be the bases of the new type |
| ```dict```  | ```dict``` object to ```__dict``` of the new type |

### Returns
Returns a type object.


______________________

## ```int```
Create a new ```int``` object.

```int(object, base=10)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | object to convert to ```int```    |
| ```base```    | ```int``` object to interpret the base of the passed arugment ```object``` as  |

### Notes
Calls ```__int__``` on object passed.

### Returns
Return value from ```__int__``` method call.


______________________

## ```str```
Create a new ```str``` object.

```str(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | object to convert to ```str```    |

### Notes
Calls ```__str__``` on object passed.

### Returns
Return value from ```__str__``` method call.


______________________

## ```dict```
Create a new ```divt``` object.

```dict(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator```    | iterable that returns 2 values per ```__next__``` call    |

### Notes
During the iteration over ```iterator```, the first return value is the key, the second is the value.

### Returns
New ```dict``` object.


______________________

## ```list```
Create a new ```list``` object.

```list(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator```    | object to iterator over to construct list    |

### Notes
```None```

### Returns
New ```list``` object.


______________________

## ```tuple```
Create a new ```tuple``` object.

```tuple(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator```    | object to iterator over to construct tuple    |

### Notes
```None```

### Returns
New ```tuple``` object.


______________________

## ```object```
Create a new ```object``` object.

```object()```

### Arguments
```None```

### Notes
```None```

### Returns
New ```object``` object.


______________________

## ```id```
Get memory address of object.

```id(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | object to get address of    |

### Notes
```None```

### Returns
```int``` object containing the address


______________________

## ```input```
Get user input and print a prompt.

```input(prompt)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```prompt```    | prompt to print    |

### Notes
Calls ```__str__``` on prompt before printing.

### Returns
```str``` object containing the user input.


______________________

## ```file```
Open a file.

```file(name, mode, encoding='UTF-8')```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```name```    | ```str``` object containing name of file to open    |
| ```mode```    | ```str``` object containing mode to open the file in   |
| ```encoding```    | ```str``` object containing encoding that data will be converted to before being written to the file, if the mode is non-binary |

### Notes
```None```

### Returns
Open ```file``` object.


______________________

## ```float```
Create a new ```float``` object.

```float(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | object to convert to ```float```    |

### Notes
Calls ```__float__``` on object passed.

### Returns
Return value from ```__float__``` method call.


______________________

## ```iter```
Attempt to create an iterator

```iter(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator```    | object that defines ```__iter__```    |

### Notes
Calls ```__iter__``` on object passed.

### Returns
Return value from ```__iter__``` method call.


______________________

## ```next```
Get next value from an iterator

```next(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator```    | object that defines ```__next__```    |

### Notes
Calls ```__next__``` on object passed.

### Returns
Return value from ```__next__``` method call.


______________________

## ```slice```
Create slice object, used to slice iterators

```slice(start, stop, step)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```start```    | ```int``` start position or ```None```    |
| ```stop```    | ```int``` stop position or ```None```    |
| ```step```    | ```int``` step     |

### Notes
```None```

### Returns
New ```slice``` object


______________________

## ```bool```
Create a new ```bool``` object.

```bool(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | object to convert to ```bool```    |

### Notes
Calls ```__bool__``` on object passed.

### Returns
Return value from ```__bool__``` method call.


______________________

## ```round```
Round an object.

```round(object, digits=0)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | object to round    |
| ```digits```    | no. of digits after decimal to round to    |

### Notes
```object``` parameter must define ```__float__```

### Returns
Return rounded value as ```float``` object.


______________________

## ```enumerate```
Create an ```enum``` object.

```enumerate(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator```    | iterator    |

### Notes
```iterator``` parameter must define ```__iter__```

### Returns
Return new ```enum``` object.


______________________

## ```range```
Create an ```range``` object.

```range(start)```
```range(start, stop, step=1)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```start```    | ```int``` object for start value    |
| ```stop```    | ```int``` object for stop value    |
| ```step```    | ```int``` object for step value    |

### Notes
```None```

### Returns
Return new ```range``` object.


______________________

## ```zip```
Create an ```zip``` object.

```zip(...)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```...```    | objects that are iterables    |

### Notes
```None```

### Returns
Return new ```zip``` object.


______________________

## ```globals```
Get globals ```dict```.

```globals()```

### Arguments
```None```

### Notes
```None```

### Returns
Return a ```dict``` containing the globals.


______________________

## ```locals```
Get globals ```dict```.

```locals()```

### Arguments
```None```

### Notes
```None```

### Returns
Return a ```dict``` containing the scope's locals.


______________________

## ```alllocals```
Get globals ```dict```.

```alllocals()```

### Arguments
```None```

### Notes
```None```

### Returns
Return a ```list``` of ```dict``` containing all scope's locals and the globals.


______________________

## ```exit```
Terminate program.

```exit()```

### Arguments
```None```

### Notes
```None```

### Returns
Return ```None```.


______________________

## ```copyright```
Get copyright information.

```copyright()```

### Arguments
```None```

### Notes
```None```

### Returns
Return ```str``` that is the copyright information.


______________________

## ```len```
Get length of an iterator

```len(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator```    | iterator    |

### Notes
Calls ```__len__``` on object passed.

### Returns
Return value from ```__len__``` method call.


______________________

## ```isinstance```
Check if an object is an instance of another.

```isinstance(object, type)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | object to check instance    |
| ```type```    | ```type``` object to check if instance of   |

### Notes
```None```

### Returns
Return ```bool``` ```True``` if ```object``` is an instance of ```type```, otherwise ```False```


______________________

## ```eval```
Evaluate code snippet.

```eval(snippet, globals={}, locals={})```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```snippet``` | ```str``` object containing code snippet    |
| ```globals```    | ```dict``` that is globals for execution   |
| ```locals```    | ```dict``` that islocals for execution   |

### Notes
```None```

### Returns
Return ```None```


______________________

## ```super```
Create proxy object.

```super(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | object to create proxy of    |

### Notes
```None```

### Returns
Return ```super``` object.


______________________

## ```getattr```
Get attribute of an object.

```getattr(object, attr)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | object to create proxy of    |
| ```attr``` | ```str``` attribute    |

### Notes
```None```

### Returns
Return attribute object.


______________________

## ```setattr```
Set attribute of an object.

```setattr(object, attr, val)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | object to create proxy of    |
| ```attr``` | ```str``` attribute    |
| ```val``` | value to set    |

### Notes
```None```

### Returns
Return ```None```


______________________

## ```abs```
Get absolute value of an object.

```abs(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | object to get absolute value of    |

### Notes
Calls ```__abs__``` on object passed.

### Returns
Return value from ```__abs__``` method call.


______________________

## ```iscallable```
Check if an object is callable.

```iscallable(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | object to check if callable   |

### Notes
Checks ```__call__``` method.

### Returns
Return ```True``` if the object is callable, otherwise ```False```


______________________

## ```reverse```
Reverse a sequence.

```reverse(seq)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```seq``` | sequence to reverse   |

### Notes
seq must define ```__get__```.

### Returns
Return new ```list```


______________________

## ```isiter```
Check if an object is iterable.

```isiter(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | object to check if callable   |

### Notes
Checks ```__iter__``` method.

### Returns
Return ```True``` if the object is iterable, otherwise ```False```


______________________

## ```min```
Get the minimum value in an iterator.

```min(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator``` | iterator to get minimum value from  |

### Notes
```None```

### Returns
Return minimum value.


______________________

## ```map```
Apply mapping to iterators

```map(func, ...)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```func``` | mapping to apply  |
| ```...``` | iterators  |

### Notes
```None```

### Returns
Return ```map``` object.


______________________

## ```method```
Create new ```method```.

```method(func, instance)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```func``` | function to wrap  |
| ```instance``` | instance of object that contains the ```func```  |

### Notes
```None```

### Returns
Return ```method``` object.


______________________

## ```classmethod```
Create new ```classmethod```, supposed to be used as a decorator.

```classmethod(func)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```func``` | function to wrap  |

### Notes
```None```

### Returns
Return ```classmethod``` object.


______________________

## ```staticmethod```
Create new ```staticmethod```, supposed to be used as a decorator.

```staticmethod(func)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```func``` | function to wrap  |

### Notes
```None```

### Returns
Return ```staticmethod``` object.


______________________

## ```property```
Create new ```property``` to intercept get/set and deletion of an attribute.

```property(get=None, set=None, del=None)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```get``` | getter function  |
| ```set``` | setter function  |
| ```del``` | deleter function  |

### Notes
```None```

### Returns
Return ```property``` object.


______________________

## ```getannotation```
Get annotation from current scope.

```getannotation(name)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```name``` | name to get annotation of  |

### Notes
```None```

### Returns
Return annotation.


______________________

## ```set```
Create a set from an iterator.

```set(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator``` | iterator  |

### Notes
Iterator must define ```__iter__``` method.

### Returns
Return new ```set```.


______________________

## ```hasattr```
Check if an object has the specified attribute.

```hasattr(object, attr)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | object to check instance    |
| ```attr```    | ```str``` attribute   |

### Notes
```None```

### Returns
Return ```bool``` ```True``` if ```object``` has the attribute, otherwise ```False```.


______________________

## ```bytes```
Create a bytes object from an iterator.

```bytes(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator``` | iterator  |

### Notes
Iterator must define ```__iter__``` method.

### Returns
Return new ```bytes```.


______________________

## ```sum```
Calculate the sum of the values in an iterator.

```sum(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator``` | iterator  |

### Notes
Iterator must define ```__iter__``` method.

### Returns
Return the sum.


______________________

## ```dir```
Get the keys of the ```__dict__``` of an object, or the keys of locals().

```dir(object)``` - Default
```dir()``` - Get the keys of locals().

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | object to get ```__dict__```.   |

### Notes
```None```

### Returns
Return new ```list```.


______________________

## ```bin```
Get the binary representation of an int.

```bin(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | ```int``` object to get binary representation of  |

### Notes
```None```

### Returns
Return new ```str```.


______________________

## ```hex```
Get the hexadecinal representation of an int.

```hex(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | ```int``` object to get hexadecimal representation of  |

### Notes
```None```

### Returns
Return new ```str```.


______________________

## ```chr```
Convert unicode code point value to string.

```chr(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | ```int``` object that is the code point  |

### Notes
```None```

### Returns
Return new ```str```.


______________________

## ```ord```
Get the ordinal value (unicode code point) of a ```str``` of length 1.

```ord(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | ```str``` object that has a length of 1  |

### Notes
```None```

### Returns
Return new ```int```.


______________________

## ```unicode_name```
Get the unicode name of an ordinal value.

```unicode_name(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object``` | ```int``` object that is the code point  |

### Notes
```None```

### Returns
Return new ```str``` that is the name.


______________________

## ```bytearray```
Create a bytearray object from an iterator.
The bytearray object is a mutable version of the bytes object.

```bytearray(iterator)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iterator``` | iterator  |

### Notes
Iterator must define ```__iter__``` method.

### Returns
Return new ```bytearray```.


______________________

## ```function```
Type of a function. The ```function``` type has the following "special" attributes:
- ```__annotations__```
- ```__doc__```
- ```__args__```
- ```__defaults__```

```function(name, code, globals)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```name``` | name of function  |
| ```code``` | code object for  function  |
| ```globals``` | globals to use during execution of function  |

### Notes
```None```

### Returns
Does not return.


______________________

## ```compile```
Compile code snippet.

```compile(snippet, name)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```snippet``` | ```str``` object containing code snippet    |
| ```name``` | ```str``` object contaiing the name for the code snippet    |

### Notes
```None```

### Returns
Return ```code``` object