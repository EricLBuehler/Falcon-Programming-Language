# Methods Documentation

## ```str```

### ```str.find```
```str.find(self, needle)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```  | ```str``` needle     |
### Example
```"abc".find("b") == 1 == True```
#### Returns
Index - ```int```

### ```str.replace```
```str.replace(self, needle, val)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```     | ```str``` to be replaced   |
| ```val```     | ```str``` to replace     |
### Example
```"abc".replace("a","b") == "bbc" == True```
#### Returns
New string - ```str```

### ```str.join```
```str.join(self, iter)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```iter```     | iterable to be joined   |
### Example
```" ".join([1,2,3]) == "1 2 3 " == True```
#### Returns
New string - ```str```

### ```str.split```
```str.split(self, needle)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```     | needle to split on   |
### Example
```"abc".split("b") == ["a","c"] == True```
#### Returns
New list - ```list```

### ```str.upper```
```str.upper(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```"abc".upper() == "ABC" == True```
#### Returns
New string - ```str```

### ```str.lower```
```str.lower(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```"ABC".lower() == "abc" == True```
#### Returns
New string - ```str```

### ```str.isspace```
```str.isspace(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```"abc".isspace() == False == True```
#### Returns
New boolean - ```bool```

### ```str.isalpha```
```str.isalpha(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```"abc".isalpha() == True == True```
#### Returns
New boolean - ```bool```

### ```str.isnumeric```
```str.isnumeric(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```"123".isnumeric() == True == True```
#### Returns
New boolean - ```bool```

### ```str.isupper```
```str.isupper(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```"abc".isupper() == False == True```
#### Returns
New boolean - ```bool```

### ```str.islower```
```str.islower(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```"abc".islower() == True == True```
#### Returns
New boolean - ```bool```
