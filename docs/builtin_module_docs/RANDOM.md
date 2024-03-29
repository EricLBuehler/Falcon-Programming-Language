# Falcon ```random``` module

-----------------------

## ```randint```

Generate random integer between 2 values (inclusive)

```random.randint(lo, hi)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```lo```    | low boundary    |
| ```hi```    | high boundary    |

### Returns
Return ```int``` random value.

-----------------------

## ```random```

Generate random integer between 2 values (inclusive)

```random.random(lo=0, hi=1)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```lo```    | low boundary    |
| ```hi```    | high boundary    |

### Returns
Return ```float``` random value.

-----------------------

## ```choice```

Choose a random element in a sequence.

```random.choice(seq)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```seq```    | sequence to choose item from    |

### Returns
Return choice.

-----------------------

## ```shuffle```

Shuffle an iterator

```random.choice(iter)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```iter```    | iterator to shuffle    |

### Returns
Return ```list``` shuffled iterator.

-----------------------

## ```RAND_MAX```

Random maximum value returned by ```C``` ```rand()``` function.