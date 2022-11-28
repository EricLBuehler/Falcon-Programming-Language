# Falcon ```thread``` module

-----------------------

## ```Thread```

Initialize a thread. The thread is not running.

```thread.Thread(callable)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```callable```    | callable to run    |

### Returns
Return ```Thread``` object

-----------------------

## ```Thread.start```

Start a thread.

```thread.Thread.start(thread, *args, **kwargs)```

*args and **kwargs arguments are passed through

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```thread```    | thread to start    |

### Returns
Return ```None```

-----------------------

## ```Thread.join```

Join a thread to the parent thread.

```thread.Thread.join(thread)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```thread```    | thread to join    |

### Returns
Return ```None```

-----------------------

## ```Thread.getid```

Get ID of a thread

```thread.Thread.getid(thread)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```thread```    | thread to join    |

### Returns
Return ```int``` that contains ID

-----------------------

## ```Mutex```

Create an unlocked mutex.

```thread.Mutex()```

### Returns
Return ```Mutex``` object

-----------------------

## ```Mutex.acquire```

Aquire a mutex

- Blocking operation

```thread.Mutex.acquire(mutex)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```mutex```    | mutex to acquire    |

### Returns
Return ```None```

-----------------------

## ```Mutex.release```

Release a mutex

```thread.Mutex.release(mutex)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```mutex```    | mutex to release    |

### Returns
Return ```None```

-----------------------

## ```Semaphore```

Create an unlocked semaphore.

```thread.Semaphore(v)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```v```    | starting value    |

### Returns
Return ```Mutex``` object

-----------------------

## ```Semaphore.acquire```

Aquire a semaphore, decrementing its value.
If the value is 0 before decrementing, the operation blocks.

```thread.Semaphore.acquire(semaphore, block=True)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```semaphore```    | mutex to acquire    |
| ```block```    | whether to block if the value is 0    |

### Returns
Return ```True``` if ```block``` is ```False```, or ```True``` if ```block``` is ```True``` and the operation did not block. Otherwise, return ```False```.

-----------------------

## ```Semaphore.release```

Release a semaphore, incrementing its value

```thread.Semaphore.release(semaphore)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```semaphore```    | semaphore to release    |

### Returns
Return ```None```