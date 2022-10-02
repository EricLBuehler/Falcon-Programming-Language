# Methods Documentation

## ```str```

### ```str.find```
Find and return index of first occurance of ```needle```, or -1 if not found.
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
Replace all occurances in a string.
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
Join an iterator with a string.
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
Split a string on string ```needle```.
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
Return a new string with all characters uppercase.
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
Return a new string with all characters lowercase.
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
Return ```True``` if string is only whitespace, otherwise, ```False```.
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
Return```True``` if string is only alphabetic characters, otherwise, ```False```.
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
Returns ```True``` if string is only numbers, otherwise, ```False```.
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
Return ```True``` if string is only uppercase characters, otherwise, ```False```.
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
Return ```True``` if string is only lowercase characters, otherwise, ```False```.
```str.islower(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```"abc".islower() == True == True```
#### Returns
New boolean - ```bool```



## ```list```

### ```list.find```
Find and return index of first occurance of argument ```needle```, or -1.
```list.find(self, needle)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```  | needle     |
### Example
```[1,2,3].find(2) == 1 == True```
#### Returns
Index - ```int```

### ```list.replace```
Replaces all occurances of argument ```needle``` with ```val```.
```list.replace(self, needle, val)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```  | needle to be replace     |
| ```val```  | value to replace     |
### Example
```[1,2,3].replace(2,3) == [1,3,3] == True```
#### Returns
Same object, altered - ```list```

### ```list.append```
Appends argument ```val``` to list.
```list.append(self, val)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```val```  | value to append     |
### Example
```[1,2,3].append(4) == [1,2,3,4] == True```
#### Returns
New list - ```list```

### ```list.pop```
Pops last element from list and returns item.
```list.pop(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```[1,2,3].pop() == 3 == True```
#### Returns
Popped value



## ```dict```

### ```dict.keys```
Returns a list of keys.
```dict.keys(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```{1:2}.keys() == [1] == True```
#### Returns
List of keys - ```list```

### ```dict.values```
Returns a list of values.
```dict.values(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```{1:2}.values() == [2] == True```
#### Returns
List of values - ```list```

