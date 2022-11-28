# Falcon ```time``` module

-----------------------

## ```sleep```

Sleep for ```n``` seconds

```time.sleep(n)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```n```    | ```int``` or ```float``` time period in seconds    |

### Returns
Return ```None```

-----------------------

## ```sleep_ms```

Sleep for ```n``` milliseconds

```time.sleep_ms(n)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```n```    | ```int``` or ```float``` time period in milliseconds    |

### Returns
Return ```None```

-----------------------

## ```time```

Get time in seconds past Unix epoch (Unix time)

```time.time()```

### Returns
Return ```float``` or ```int``` containing time

-----------------------

## ```time_ns```

Get time in nanoseconds past Unix epoch (Unix time)

```time.time_ns()```

### Returns
Return ```float``` or ```int``` containing time

-----------------------

## ```strftime```

Get a formatted value of the time.
See https://en.cppreference.com/w/cpp/io/manip/put_time for format codes

```time.strftime(format)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```format```    | ```str``` format string    |

### Returns
Return ```str``` formatted time