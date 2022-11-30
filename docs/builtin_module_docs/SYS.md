# Falcon ```sys``` module

-----------------------

## ```getsizeof```

Get the number of bytes occupied by the object in memory. This excludes references to other objects or data structures by pointers, and includes the object metadata.

```sys.getsizeof(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | object to get size of    |

### Returns
Return ```int``` containing the size occupied by the object in memory

-----------------------

## ```getrefcnt```

Get the reference count of an object.

```sys.getrefcnt(object)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | object to get reference count of    |

### Returns
Return ```int``` containing the reference count

-----------------------

## ```path```

Interpreter path variable - **readonly**

-----------------------

## ```has_color```

Interpreter has_color variable - **read and write**

-----------------------

## ```argc```

Number of arguments passed to Falcon

-----------------------

## ```argv```

```tuple``` of ```str``` s containing the arguments

-----------------------

## ```version```

```str``` containing Falcon version