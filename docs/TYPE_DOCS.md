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

### ```str.encode```
Encodes ```self``` as encoding ```encoding```
```str.encode(self, encoding)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```encode```  | encoding     |
#### Returns
Bytes object containing encoded string - ```bytes```

### ```str.startswith```
Return ```True``` if the string starts with needle, otherwise, ```False```.
```str.startswith(self, needle)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```  | needle     |
#### Returns
Boolean - ```bool```

### ```str.endswith```
Return ```True``` if the string ends with needle, otherwise, ```False```.
```str.endswith(self, needle)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```  | needle     |
#### Returns
Boolean - ```bool```

### ```str.rfind```
Return the last occurence of needle, or -1;
```str.rfind(self, needle)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```  | needle     |
#### Returns
Index - ```int```


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
```None```

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
```list.remove(self, val)```
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

### ```list.extend```
Extends list by passed iterator
```list.extend(self, it)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```it```    | iterator     |
### Example
```
a=[1,2,3]
a.extend({4,5,6})
a == [1,2,3,4,5,6] == True
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

### ```file.size```
Returns the size of open file object.
```file.size(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```file("test.txt","r").size() == 3 == True```
#### Returns
Size - ```int```

### ```file.flush```
Flushes file
```file.flush(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
### Example
```file("test.txt","w").flush() == None == True```
#### Returns
```None```




## ```set```

### ```set.find```
Find and return index of first occurance of argument ```needle```, or -1.
```set.find(self, needle)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```  | needle     |
### Example
```{1,2,2,3}.find(2) == 1 == True```
#### Returns
Index - ```int```

### ```set.add```
Adds ```item``` to set if the item does not exist
```set.add(self, item)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```item```  | item to add     |
### Example
```{1,2,2,3}.add(4) == {1,2,2,3,4} == True```
#### Returns
None - ```None```

### ```set.remove```
Tries to remove ```item``` from the set. On failure, raise ```KeyError```
```set.remove(self, item)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```item```  | item to remove     |
### Example
```
a={1,2,3}
a.remove(3)
a == {1,2} == True
```
#### Returns
None - ```None```

### ```set.union```
Returns the union of set ```self``` and set ```other```.
```set.union(self, item)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```other```  | other set     |
### Example
```
a={1,2,3}
a=a.union({4,})
a == {1,2,3,4} == True
```
#### Returns
None - ```None```


## Bytes

### ```bytes.decode```
Decodes ```self``` into a string, interpreting bytes in ```self```as ```encoding```
```bytes.encode(self, encoding)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```encode```  | encoding     |
#### Returns
Decoded string - ```str```

### ```bytes.hex```
Converts byte object to a hexadecimal string.
```bytes.hex(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
#### Returns
Hexadecimal string - ```str```



## ```bytearray```

### ```bytearray.find```
Find and return index of first occurance of argument ```needle```, or -1.
```bytearray.find(self, needle)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```  | needle     |
### Example
```bytearray([1,2,3]).find(2) == 1 == True```
#### Returns
Index - ```int```

### ```bytearray.replace```
Replaces all occurances of argument ```needle``` with ```val```.
```bytearray.replace(self, needle, val)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```needle```  | needle to be replace     |
| ```val```  | value to replace     |
### Example
```bytearray([1,2,3]).replace(2,3) == bytearray([1,3,3]) == True```
#### Returns
Same object, altered - ```bytearray```

### ```bytearray.append```
Appends argument ```val``` to bytearray.
```bytearray.append(self, val)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```val```  | value to append     |
### Example
```bytearray([1,2,3]).append(4) == bytearray([1,2,3,4]) == True```
#### Returns
```None```

### ```bytearray.pop```
Pops element at idx from bytearray and returns item.
```bytearray.pop(self, idx=-1)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```idx```    | index to pop from     |
### Example
```bytearray([1,2,3]).pop() == 3 == True```
#### Returns
Popped value

### ```bytearray.insert```
Inserts value into bytearray at specified index
```bytearray.insert(self, idx, val)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```idx```    | index to insert at    |
| ```val```    | value to insert     |
### Example
```
a=bytearray([1,2,3])
a.insert(0, "A")
a == ["A",1,2,3] == True
```
#### Returns
```None```

### ```bytearray.remove```
Removes first occurrence of value from bytearray
```bytearray.remove(self, val)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```val```    | value to insert     |
### Example
```
a=bytearray([1,2,3])
a.remove(1)
a == bytearray([2,3]) == True
```
#### Returns
```None```

### ```bytearray.extend```
Extends bytearray by passed iterator
```bytearray.extend(self, it)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
| ```it```    | iterator     |
### Example
```
a=bytearray([1,2,3])
a.extend({4,5,6})
a == bytearray([1,2,3,4,5,6]) == True
```
#### Returns
```None```

### ```bytearray.hex```
Converts bytearray object to a hexadecimal string.
```bytearray.hex(self)```
#### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```self```    | self argument     |
#### Returns
Hexadecimal string - ```str```