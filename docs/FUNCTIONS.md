# Functions and Methods

## Non-methods
### Functions
Functions are defined with the ```func``` keyword. They are the basis for methods.
```func <name> (<arguments>){<code>}```
### Lambda Functions
Lambda functions are anonymous functions and have no name.
```lambda <arguments> {<code>}```

## Methods
Methods are acutally functions at the base level, but are diffrentiated by the use of the ```method```, ```staticmethod```, and ```classmethod``` types to extend functionality. There is no way to define a method, as there is to define a function.
### Methods
Methods are functions, and it is convention for the ```self``` parameter to represent the instance.
```func <name> (self, <arguments>){<code>}```

### Staticmethods
Staticmethods are methods that take no instance arguments.
```
@staticmethod
func <name> (<arguments>){<code>}
```

### Classmethods
Classmethods are methods that take an instance argument in the form of the type (class) of the instance. It is convention to call this parameter ```cls```.
```
@classmethod
func <name> (cls, <arguments>){<code>}
```
## Iterators
### Generators
Generators are any functions witha ```yield``` statement.
