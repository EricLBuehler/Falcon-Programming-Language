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
```str.split(self, needle="")```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```     | needle to split on, defaults to ```""```   |
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

### ```str.count```
Get count of substring in a string
```str.count(self, substr)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```substr```     | substring   |
### Example
```"abca".count("a") == 2 == True```
#### Returns
New int - ```int```

### ```str.strip```
Strips whitespace off a string
```str.strip(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```"12  ".strip() == "12" == True```
#### Returns
New string - ```str```

### ```str.lstrip```
Strips leading whitespace off a string
```str.kstrip(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```"12  ".lstrip() == "12  " == True```
#### Returns
New string - ```str```

### ```str.rstrip```
Strips trailing whitespace off a string
```str.strip(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```"12  ".rstrip() == "12" == True```
#### Returns
New string - ```str```

### ```str.contains```
Return ```True``` is needle is found in ```self```, otherwise, ```False```
```str.contains(self, needle)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```  | ```str``` needle     |
### Example
```"abc".contains("c") == True```
#### Returns
Boolean - ```bool```


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
Pops element at idx from list and returns item.
```list.pop(self, idx=-1)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```idx```    | index to pop from     |
### Example
```[1,2,3].pop() == 3 == True```
#### Returns
Popped value

### ```list.insert```
Inserts value into list at specified index
```list.insert(self, idx, val)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```idx```    | index to insert at    |
| ```val```    | value to insert     |
### Example
```
a=[1,2,3]
a.insert(0, "A")
a == ["A",1,2,3] == True
```
#### Returns
```None```

### ```list.remove```
Removes first occurrence of value from list
```list.insert(self, val)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```val```    | value to insert     |
### Example
```
a=[1,2,3]
a.remove(1)
a == [2,3] == True
```
#### Returns
```None```



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

### ```dict.flip```
Create a new dictionary with the key-value pairs in the original dictionary flipped.
```dict.flip(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```{1:2}.flip() == {2:1} == True```
#### Returns
New dict - ```dict```



## ```tuple```

### ```tuple.find```
Find and return index of first occurance of argument ```needle```, or -1.
```tuple.find(self, needle)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```  | needle     |
### Example
```(1,2,3).find(2) == 1 == True```
#### Returns
Index - ```int```



## ```file```

### ```file.read```
Returns contents of an open, readable file object.
```file.read(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```file("test.txt"),"r".read() == "ABC" == True```
#### Returns
Contents of file - ```str```

### ```file.close```
Closes a file object.
```file.close(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```file("test.txt","r").close() == None == True```
#### Returns
```None```

### ```file.seek```
Seeks to absolute position in an open file object.
```file.seek(self, idx)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```idx```    | absolute index to seek to     |
### Example
```file("test.txt","r").seek(10) == None == True```
#### Returns
```None```

### ```file.write```
Writes ```val.__str__()``` to open, writeable file object.
```file.seek(self, val)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```val```    | object to write     |
### Example
```file("test.txt", "w").write(123).read() == "123" == True```
#### Returns
File object - ```file```
