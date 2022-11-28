# Falcon ```sockets``` module

-----------------------

## ```socket```

Open a new socket.

```socket.socket(family, socket)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```family```    | address family    |
| ```socket```    | socket type    |

### Returns
Return ```Socket``` object

-----------------------

## ```socket.close```

Close an open socket.

```socket.socket.close(socket)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```socket```    | socket to close    |

### Returns
Return ```None```

-----------------------

## ```socket.connect```

Connect to a remote socket server using the address and port.

```socket.socket.connect(socket, info)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```socket```    | socket to connect with    |
| ```info```    | tuple of ```(IP, PORT)``` |

### Returns
Return ```None```

-----------------------

## ```socket.send```

Send the data encoded in a string or bytes object through an open socket.

```socket.socket.send(socket, data)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```socket```    | socket to send information through   |
| ```data```    | data to send |

### Returns
Return ```int``` number of bytes sent

-----------------------

## ```socket.recv```

Recieve data from an open socket.

```socket.socket.recv(socket, len, flags=0)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```socket```    | socket to recieve from    |
| ```len```    | number of bytes to recieve |
| ```flags```    | flags for ```C``` ```recv()``` function |

### Returns
Return ```bytes``` object that contains the recieved information.

-----------------------

## ```socket.gethostbyname```

Get the IP address of a given host name.

```socket.socket.gethostbyname(name)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```name```    | hostname |

### Returns
Return ```bytes``` object that contains the recieved information.

-----------------------

## ```socket.bind```

Bind a socket to an address and port.
This allows the socket to act as a server.

```socket.socket.bind(socket, info)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```socket```    | socket to connect with    |
| ```info```    | tuple of ```(IP, PORT)``` |

### Returns
Return ```None```

-----------------------

## ```socket.listen```

Enable a server to accept ```n``` connections.

```socket.socket.listen(socket, n)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```socket```    | socket to connect with    |
| ```n```    | ```int``` backlog |

### Returns
Return ```None```

-----------------------

## ```socket.accept```

Enable a server to accept ```n``` connections.

```socket.socket.accept(socket)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```socket```    | socket to accept connections    |

### Returns
Return ```tuple``` of ```(CLIENT, (IP, PORT) )```

-----------------------

## ```socket.setsockopt```

Set socket options at level and name to value.

```socket.socket.setsockopt(socket, level, name, value)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```socket```    | socket to set option    |
| ```level```    | socket level    |
| ```name```    | name to set option    |
| ```value```    | value to set    |

### Returns
Return ```None```

-----------------------

## ```socket.getsockopt```

Get socket options at level and name, and return the results in a buffer.

```socket.socket.getsockopt(socket, level, name, len=4)```

- Note: len is by default the native size of an integer on the machine (```sizeof(int)``` in ```C```)

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```socket```    | socket to get information from    |
| ```level```    | socket level    |
| ```name```    | name to set option    |
| ```len```    | length of buffer    |

### Returns
Return ```list``` containing the buffer to be interpreted by the caller.

-----------------------

## ```socket.getsockname```

Get socket information.

```socket.socket.getsockname(socket)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```socket```    | socket to get information about    |

### Returns
Return ```tuple``` containing the information ```(IP, PORT)```

-----------------------

## ```socket.gethostname```

Get hostname.

```socket.socket.gethostname()```

### Returns
Return ```str``` containing host name

-----------------------

## ```socket.sethostname```

Set hostname.

- **Only available on Windows**

```socket.socket.sethostname(name)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```name```    | ```str``` name    |

### Returns
Return ```None```
Return ```None```

-----------------------

## ```socket.sendall```

Send all of the data encoded in a string or bytes object through an open socket.

```socket.socket.sendall(socket, data)```

### Arguments
| Argument      | Information       |
| ------------- | ----------------- |
| ```socket```    | socket to send information through   |
| ```data```    | data to send |

### Returns
Return ```int``` total number of bytes sent

-----------------------

## Address Families

### ```AF_UNIX```
Local to host

### ```AF_INET```
INET TCP, UDP, etc.

### ```AF_INET6```
INET version 6

-----------------------

## Socket Types

### ```SOCK_STREAM```
Stream socket

### ```SOCK_DGRAM```
Datagram socket

### ```SOCK_RAW```
Raw protocol interface

### ```SOCK_RDM```
Reliably-delivered interface

### ```SOCK_SEQPACKET```
Sequential socket stream

### ```INADDR_ANY```
Connect from all active ports 

```0.0.0.0```

### ```INADDR_BROADCAST```
Any host (has the same effect on ```socket.bind()``` as ```INADDR_ANY```)

```255.255.255.255```

### ```INADDR_LOOPBACK```
Loopback interface

```127.0.0.1```

-----------------------

## Flags

### ```MSG_CONFIRM```
- **Not available on Windows**
Send confirmation message to the sender.

### ```MSG_DONTWAIT```
- **Not available on Windows**
Nonblocking operation

### ```MSG_EOR```
- **Not available on Windows**
Terminates a record

### ```MSG_MORE```
- **Not available on Windows**
Indicates the caller has more data to send

### ```MSG_NOSIGNAL```
- **Not available on Windows**
Only raise ```EPIPE``` ```errno``` error, do not generate ```SIGPIPE``` signal

### ```MSG_NOSIGNAL```
- **Not available on Windows**
Only raise ```EPIPE``` ```errno``` error, do not generate ```SIGPIPE``` signal

### ```MSG_DONTROUTE```
Do not use a gateway, only use interface addresses

### ```MSG_OOB```
Send and process out of band data

## Socket options

### Level
#### ```SOL_SOCKET```
Socket level options

### Options
#### ```SO_DEBUG```
Debug info recording

#### ```SO_BROADCAST```
Allow broadcast messages to be sent

#### ```SO_REUSEADDR```
Allow local address reuse

#### ```SO_KEEPALIVE```
Keep connections alive

#### ```SO_LINGER```
Linger on close if data is present

#### ```SO_OOBINLINE```
Leave OOB data inline

#### ```SO_SNDBUF```
Send buffer size

#### ```SO_RCVBUF```
Recieve buffer size

#### ```SO_DONTROUTE```
Do not use a gateway, only use interface addresses

#### ```SO_RCVLOWAT```
Recieve low-water mark

#### ```SO_SNDLOWAT```
Send low-water mark

#### ```SO_RCVTIMEO```
Recieve timeout

#### ```SO_SNDTIMEO```
Send timeout

#### ```SO_ENABLED```
Expands to ```1```, may be used to enable an option and for readability instead of ```1```.

#### ```SO_DISABLED```
Expands to ```0```, may be used to enable an option and for readability instead of ```0```.

#### ```SO_CONDITIONAL_ACCEPT```
- **Only available on Windows**
Enable true conditional accept

## Other

### ```SOMAXCONN```
Maximum queue length specifiable by ```socket.listen()```