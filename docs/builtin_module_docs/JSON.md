# Falcon ```json``` module

-----------------------

## ```encode```

Converts an object into its JSON counterpart. 

```json.encode``` makes the following conversions:
- ```None``` -> ```null```
- ```True``` -> ```true```
- ```False``` -> ```false```

Expects ```str``` for keys, and the following types for values:
- ```str```
- ```int```
- ```float```
- ```dict```
- ```tuple```
- ```list```
- ```bool```
- ```NoneType```

```json.encode(object)```


### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | ```dict``` object to encode    |

### Returns
Return ```str``` that is encoded JSON string.

-----------------------

## ```decode```

Decodes JSON string into a ```dict``` that contains the information.

```json.decode``` makes the following conversions:
- ```null``` -> ```None```
- ```true``` -> ```True```
- ```false``` -> ```False```

```json.decode(object)```


### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```object```    | JSON string (```str```) to decode    |

### Returns
Return ```dict``` that is the decoded information.
