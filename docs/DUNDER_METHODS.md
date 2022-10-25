# Falcon Programming Lanugage Dunder Methods

## Constructors
- ```__init__(self, ...)```
- ```__new__(cls, ...)```
- ```__del__(self)``` 

## Iterators
- ```__next__(self)```
- ```__iter__(self)```
- ```__getitem__(self, idx)```
- ```__setitem__(self, idx, val)```
- ```__delitem__(self, idx)```
- ```__len__(self)```

## Conversions
- ```__str__(self)```
- ```__repr__(self)```
- ```__bool__(self)```
- ```__int__(self)```
- ```__float__(self)```

## Arithmetic
- ```__add__(self, other)```
- ```__sub__(self, other)```
- ```__mul__(self, other)```
- ```__div__(self, other)```
- ```__mod__(self, other)```
- ```__pow__(self, other)```
- ```__floordiv__(self, other)```
- ```__neg__(self)```
- ```__abs__(self)```

## Call
- ```__call__(self, ...)```

## Bitwise
- ```__not__(self)```
- ```__and__(self, other)```
- ```__or__(self, other)```
- ```__xor__(self, other```
- ```__lshift__(self, number)```
- ```__rshift__(self, number)```

## Descriptor
- ```__get__(self, instance)```
- ```__set__(self, instance, val)```
- ```__delete__(self, instance)```

## Attribute Lookup
- ```__getattr__(self, attr)```
- ```__setattr__(self, attr, val)```
- ```__delattr__(self, attr)```
