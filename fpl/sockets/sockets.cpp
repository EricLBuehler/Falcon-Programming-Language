#ifdef _WIN32
  /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501  /* Windows XP. */
  #endif
  #include <winsock2.h>
  #include <Ws2tcpip.h>
#else
  /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <arpa/inet.h>
  #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
  #include <unistd.h> /* Needed for close() */
#endif

typedef int SOCKET_T;


#include "sockettype.cpp"

int socket_init(){
    #ifdef _WIN32
        WSADATA wsa_data;
        return WSAStartup(0x0101, &wsa_data);
    #else
        return 0;
    #endif
}

int socket_cleanup(){
    #ifdef _WIN32
        return WSACleanup();
    #else
        return 0;
    #endif
}

int _socket_close(SOCKET_T sock){
    int status = 0;

    #ifdef _WIN32
        status = shutdown(sock, SD_BOTH);
        if (status == 0) { status = closesocket(sock); }
    #else
        status = shutdown(sock, SHUT_RDWR);
        if (status == 0) { status = close(sock); }
    #endif

    return status; 
}

object* socket_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }
    
    object* family=object_int(list_index_int(args, 0));
    
    if (family==NULL || !object_istype(family->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 0)->type->name->c_str());
        return NULL;
    }
    int af_family=CAST_INT(family)->val->to_int();
    if (af_family>AF_MAX){
        vm_add_err(&TypeError, vm, "Invalid address family value");
        return NULL;
    }
    
    object* sock=object_int(list_index_int(args, 1));
    
    if (sock==NULL || !object_istype(sock->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    int sock_kind=CAST_INT(sock)->val->to_int();

    object* o=new_object(CAST_TYPE(type));
    CAST_SOCKET(o)->af_family=af_family;
    CAST_SOCKET(o)->sock_kind=sock_kind;
    CAST_SOCKET(o)->fd=socket(af_family, sock_kind, 0);
    if (CAST_SOCKET(o)->fd==INVALID_SOCKET){
        #ifdef _WIN32
        int err=WSAGetLastError();
        #else
        int err=errno;
        errno=0;
        #endif
        vm_add_err(&OSError, vm, "Could not create socket: %d" , err);
        return NULL;
    }
    CAST_SOCKET(o)->closed=false;
    CAST_SOCKET(o)->server=NULL;
    
    return o;
}

object* socket_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (CAST_SOCKET(self)->af_family==CAST_SOCKET(other)->af_family &&\
        CAST_SOCKET(self)->sock_kind==CAST_SOCKET(other)->sock_kind &&\
        CAST_SOCKET(self)->fd==CAST_SOCKET(other)->fd){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (CAST_SOCKET(self)->af_family!=CAST_SOCKET(other)->af_family ||\
        CAST_SOCKET(self)->sock_kind!=CAST_SOCKET(other)->sock_kind ||\
        CAST_SOCKET(self)->fd!=CAST_SOCKET(other)->fd){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}

object* socket_repr(object* self){
    char fd[4];
    sprintf(fd, "%d",CAST_SOCKET(self)->fd);
    char fam[4];
    sprintf(fam, "%d",CAST_SOCKET(self)->af_family);
    char kind[4];
    sprintf(kind, "%d",CAST_SOCKET(self)->sock_kind);

    string s="<socket fd=";
    s+=string(fd);
    s+=" family=";
    s+=string(fam);
    s+=" type=";
    s+=string(kind);
    s+=">";

    return str_new_fromstr(s);
}

void socket_del(object* self){
    if (!CAST_SOCKET(self)->closed){
        _socket_close(CAST_SOCKET(self)->fd);
        if (CAST_SOCKET(self)->server!=NULL){
            fpl_free(CAST_SOCKET(self)->server);
        }
    }
}

object* socket_connect(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }

    object* self=list_index_int(args, 0);
    object* iter_=list_index_int(args, 1);
    
    if (CAST_SOCKET(self)->closed){
        vm_add_err(&InvalidOperationError, vm, "Socket closed");
        return NULL; 
    }

    if (iter_->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not iterable", iter_->type->name->c_str());
        return NULL; 
    }
    object* iter=iter_->type->slot_iter(iter_);

    

    object* one=new_int_fromint(0);
    object* two=new_int_fromint(1);
    
    object* o=iter->type->slot_next(iter);
    object* host=NULL;
    object* port=NULL;
    while (vm->exception==NULL){   
        if (host==NULL){
            host=o;
        }
        else if (port==NULL){
            port=o;
        }
        else{
            vm_add_err(&ValueError, vm, "Expected iterator of length 2, got non-2 length");
            return NULL;            
        }
        
        o=iter->type->slot_next(iter);
    }
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }

    CAST_SOCKET(self)->server=(struct sockaddr_in*)fpl_malloc(sizeof(struct sockaddr_in));

    const char* host_c=object_cstr(host).c_str();

    CAST_SOCKET(self)->server->sin_addr.s_addr = inet_addr(host_c);
    
    #ifndef _WIN32
    if (CAST_SOCKET(self)->server->sin_addr.s_addr==(in_addr_t)(-1)){
        vm_add_err(&ValueError, vm, "Invalid IP address, '%s'", host_c);
        return NULL;    
    }
    #else
    if (CAST_SOCKET(self)->server->sin_addr.s_addr==(u_long)(-1)){
        vm_add_err(&ValueError, vm, "Invalid IP address, '%s'", host_c);
        return NULL;    
    }
    #endif

	CAST_SOCKET(self)->server->sin_family = CAST_SOCKET(self)->af_family;

    object* port_=object_int(port);
    
    if (port_==NULL || !object_istype(port_->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", port->type->name->c_str());
        return NULL;
    }

	CAST_SOCKET(self)->server->sin_port = htons(CAST_INT(port_)->val->to_int());

    struct addrinfo hints,*res;
    int err;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = CAST_SOCKET(self)->af_family; 
    hints.ai_socktype = CAST_SOCKET(self)->sock_kind;
    int i=getaddrinfo(host_c,object_cstr(port).c_str(),&hints,&res);
    if (i!=0){
        vm_add_err(&OSError, vm, "[Errno %d] getaddrinfo failed for IP '%s', port '%s'" , i, host_c, object_cstr(port_).c_str());
        return NULL;
    }

    connect(CAST_SOCKET(self)->fd, (SOCKADDR *)CAST_SOCKET(self)->server, sizeof(SOCKADDR));

    return new_none();
}

object* socket_close(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 1 arguments, got %d", len);
        return NULL; 
    }
    object* self=list_index_int(args, 0);

    if (!CAST_SOCKET(self)->closed){
        _socket_close(CAST_SOCKET(self)->fd);
        if (CAST_SOCKET(self)->server!=NULL){
            fpl_free(CAST_SOCKET(self)->server);
        }
    }
    return new_none();
}

object* socket_send(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if ((len!=2 && len!=3) || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }
    
    object* self=list_index_int(args, 0);
    object* v=list_index_int(args, 1);
    if (CAST_SOCKET(self)->closed){
        vm_add_err(&InvalidOperationError, vm, "Socket closed");
        return NULL; 
    }
    object* flags=NULL;
    if (len==3){
        flags=object_int(list_index_int(args, 2));
        if (flags==NULL || !object_istype(flags->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 2)->type->name->c_str());
            return NULL;
        }
    }
    
    string s=object_cstr(v).c_str();
    char msg[s.size()+1];
    strcpy(msg, s.c_str());

    int i=send(CAST_SOCKET(self)->fd, msg , strlen(msg), (flags==NULL)? 0 : CAST_INT(flags)->val->to_long());
    
    if (i==SOCKET_ERROR){
        #ifdef _WIN32
        int err=WSAGetLastError();
        #else
        int err=errno;
        errno=0;
        #endif
        vm_add_err(&OSError, vm, "[Errno %d] send failed" , err);
        return NULL;
    }

    return new_int_fromint(i);
}

object* socket_recv(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if ((len!=2 && len!=3)|| CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 or 3 arguments, got %d", len);
        return NULL; 
    }

    object* self=list_index_int(args, 0);
    if (CAST_SOCKET(self)->closed){
        vm_add_err(&InvalidOperationError, vm, "Socket closed");
        return NULL; 
    }
    object* len_=object_int(list_index_int(args, 1));
    
    if (len_==NULL || !object_istype(len_->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    int buflen=CAST_INT(len_)->val->to_int();

    char* buf=(char*)fpl_malloc(buflen);
    memset(buf, 0, buflen);
    
    object* flags=NULL;
    if (len==3){
        flags=object_int(list_index_int(args, 2));
        if (flags==NULL || !object_istype(flags->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 2)->type->name->c_str());
            return NULL;
        }
    }
    
    int i=recv(CAST_SOCKET(self)->fd, buf, buflen-1, (flags==NULL)? 0 : CAST_INT(flags)->val->to_long());
    if (i==SOCKET_ERROR){
        #ifdef _WIN32
        int err=WSAGetLastError();
        #else
        int err=errno;
        errno=0;
        #endif
        vm_add_err(&OSError, vm, "[Errno %d] recv failed" , err);
        return NULL;
    } 
    
    return str_new_fromstr(string(buf));
}

object* socket_gethostbyname(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }

    object* self=list_index_int(args, 0);
    if (CAST_SOCKET(self)->closed){
        vm_add_err(&InvalidOperationError, vm, "Socket closed");
        return NULL; 
    }
    object* v=list_index_int(args, 1);
    const char* name=object_cstr(v).c_str();
    
	char ip[100];
	struct hostent *he;
	struct in_addr **addr_list;

    he=gethostbyname(name);
    if (he==NULL){
        #ifdef _WIN32
        int err=WSAGetLastError();
        #else
        int err=errno;
        errno=0;
        #endif
        vm_add_err(&OSError, vm, "[Errno %d] gethostbyname failed" , err);
        return NULL;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(int i = 0; addr_list[i] != NULL; i++){
		strncpy(ip , inet_ntoa(*addr_list[i]), 100);
	}
    
    return str_new_fromstr(ip);
}

object* socket_bind(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }

    object* self=list_index_int(args, 0);
    object* iter_=list_index_int(args, 1);
    
    if (CAST_SOCKET(self)->closed){
        vm_add_err(&InvalidOperationError, vm, "Socket closed");
        return NULL; 
    }

    if (iter_->type->slot_iter==NULL){
        vm_add_err(&TypeError, vm, "'%s' object is not iterable", iter_->type->name->c_str());
        return NULL; 
    }
    object* iter=iter_->type->slot_iter(iter_);

    

    object* one=new_int_fromint(0);
    object* two=new_int_fromint(1);
    
    object* o=iter->type->slot_next(iter);
    object* host=NULL;
    object* port=NULL;
    while (vm->exception==NULL){   
        if (host==NULL){
            host=o;
        }
        else if (port==NULL){
            port=o;
        }
        else{
            vm_add_err(&ValueError, vm, "Expected iterator of length 2, got non-2 length");
            return NULL;            
        }
        
        o=iter->type->slot_next(iter);
    }
    if (vm->exception!=NULL){
        FPLDECREF(vm->exception);
        vm->exception=NULL;
    }

    CAST_SOCKET(self)->server=(struct sockaddr_in*)fpl_malloc(sizeof(struct sockaddr_in));

    const char* host_c=object_cstr(host).c_str();

    CAST_SOCKET(self)->server->sin_addr.s_addr = inet_addr(host_c);
    
    #ifndef _WIN32
    if (CAST_SOCKET(self)->server->sin_addr.s_addr==(in_addr_t)(-1)){
        vm_add_err(&ValueError, vm, "Invalid IP address, '%s'", host_c);
        return NULL;    
    }
    #else
    if (CAST_SOCKET(self)->server->sin_addr.s_addr==(u_long)(-1)){
        vm_add_err(&ValueError, vm, "Invalid IP address, '%s'", host_c);
        return NULL;    
    }
    #endif

	CAST_SOCKET(self)->server->sin_family = CAST_SOCKET(self)->af_family;

    object* port_=object_int(port);
    
    if (port_==NULL || !object_istype(port_->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", port->type->name->c_str());
        return NULL;
    }

	CAST_SOCKET(self)->server->sin_port = htons(CAST_INT(port_)->val->to_int());
    
    int i=bind(CAST_SOCKET(self)->fd, (SOCKADDR *)CAST_SOCKET(self)->server, sizeof(SOCKADDR));
    if (i==SOCKET_ERROR){
        #ifdef _WIN32
        int err=WSAGetLastError();
        #else
        int err=errno;
        errno=0;
        #endif
        vm_add_err(&OSError, vm, "[Errno %d] bind failed for IP '%s', port '%s'" , err, host_c, object_cstr(port_).c_str());
        return NULL;
    }

    return new_none();
}

object* socket_listen(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=2 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL; 
    }

    object* self=list_index_int(args, 0);
    if (CAST_SOCKET(self)->closed){
        vm_add_err(&InvalidOperationError, vm, "Socket closed");
        return NULL; 
    }
    object* v=object_int(list_index_int(args, 1));
    
    if (v==NULL || !object_istype(v->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }
    int max=CAST_INT(v)->val->to_int();
    
    int i=listen(CAST_SOCKET(self)->fd, max);
    if (i==SOCKET_ERROR){
        #ifdef _WIN32
        int err=WSAGetLastError();
        #else
        int err=errno;
        errno=0;
        #endif
        vm_add_err(&OSError, vm, "[Errno %d] listen failed" , err);
        return NULL;
    }
    
    return new_none();
}

object* socket_accept(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL; 
    }

    object* self=list_index_int(args, 0);
    if (CAST_SOCKET(self)->closed){
        vm_add_err(&InvalidOperationError, vm, "Socket closed");
        return NULL; 
    }
    int c= sizeof(struct sockaddr_in);
    struct sockaddr_in client;

    SOCKET_T new_socket=accept(CAST_SOCKET(self)->fd, (struct sockaddr*)&client, &c);
    if (new_socket==INVALID_SOCKET){
        #ifdef _WIN32
        int err=WSAGetLastError();
        #else
        int err=errno;
        errno=0;
        #endif
        vm_add_err(&OSError, vm, "[Errno %d] accept failed" , err);
        return NULL;
    }

    //Get IP and port of connection of client using:
    char *client_ip = inet_ntoa(client.sin_addr);
    int client_port = ntohs(client.sin_port);

    object* o=new_object(&SocketType);
    CAST_SOCKET(o)->af_family=CAST_SOCKET(self)->af_family;
    CAST_SOCKET(o)->sock_kind=CAST_SOCKET(self)->sock_kind;
    CAST_SOCKET(o)->fd=new_socket;
    CAST_SOCKET(o)->closed=false;
    CAST_SOCKET(o)->server=NULL;

    object* tup=new_tuple();
    tuple_append(tup, o);
    object* tup2=new_tuple();
    tuple_append(tup2, str_new_fromstr(string(client_ip)));
    tuple_append(tup2, new_int_fromint(client_port));
    tuple_append(tup, tup2);
    FPLDECREF(o);
    FPLDECREF(tup2);
    return tup;
}

object* socket_setsockopt(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if (len!=4 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 4 arguments, got %d", len);
        return NULL; 
    }

    object* self=list_index_int(args, 0);
    if (CAST_SOCKET(self)->closed){
        vm_add_err(&InvalidOperationError, vm, "Socket closed");
        return NULL; 
    }

    object* lvl=object_int(list_index_int(args, 1));
    
    if (lvl==NULL || !object_istype(lvl->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }

    object* nm=object_int(list_index_int(args, 2));
    
    if (nm==NULL || !object_istype(nm->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 2)->type->name->c_str());
        return NULL;
    }

    object* val=object_int(list_index_int(args, 3));
    
    if (val==NULL || !object_istype(val->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 3)->type->name->c_str());
        return NULL;
    }

    int i=CAST_INT(val)->val->to_int();
    int res=setsockopt(CAST_SOCKET(self)->fd, CAST_INT(lvl)->val->to_int(), CAST_INT(nm)->val->to_int(), (char*)&i, sizeof(int));
    if (res<0){
        #ifdef _WIN32
        int err=WSAGetLastError();
        #else
        int err=errno;
        errno=0;
        #endif
        vm_add_err(&OSError, vm, "[Errno %d] setsockopt failed" , err);
        return NULL;
    }

    return new_none();
}

object* socket_getsockopt(object* selftp, object* args, object* kwargs){
    long len= CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long();
    if ((len!=4 && len!=3) || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_long() != 0){
        vm_add_err(&ValueError, vm, "Expected 3 or 4 arguments, got %d", len);
        return NULL; 
    }

    object* self=list_index_int(args, 0);
    if (CAST_SOCKET(self)->closed){
        vm_add_err(&InvalidOperationError, vm, "Socket closed");
        return NULL; 
    }

    object* lvl=object_int(list_index_int(args, 1));
    
    if (lvl==NULL || !object_istype(lvl->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 1)->type->name->c_str());
        return NULL;
    }

    object* nm=object_int(list_index_int(args, 2));
    
    if (nm==NULL || !object_istype(nm->type, &IntType)){
        vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 2)->type->name->c_str());
        return NULL;
    }

    object* lenv=NULL;
    
    if (len==4){
        lenv=object_int(list_index_int(args, 3));
        
        if (lenv==NULL || !object_istype(lenv->type, &IntType)){
            vm_add_err(&TypeError, vm, "'%s' object cannot be coerced to int", list_index_int(args, 3)->type->name->c_str());
            return NULL;
        }
    }

    socklen_t i=sizeof(int);
    if (lenv!=NULL){
        i=(socklen_t)CAST_INT(lenv)->val->to_int();
    }
    char* buf=(char*)fpl_malloc(i);

    int res=getsockopt(CAST_SOCKET(self)->fd, CAST_INT(lvl)->val->to_int(), CAST_INT(nm)->val->to_int(), buf, &i);
    if (res<0){
        #ifdef _WIN32
        int err=WSAGetLastError();
        #else
        int err=errno;
        errno=0;
        #endif
        vm_add_err(&OSError, vm, "[Errno %d] setsockopt failed" , err);
        return NULL;
    }

    if (lenv==NULL){
        return new_int_fromint(*((char*)buf));
    }


    object* list=new_list();
    for (int i=0; i<i; i++){
        list_append(list, new_int_fromint(*((char*)(buf+sizeof(char)*i))));
    }

    return list;
}

object* socket_enter(object* self){
    return self;
}

object* socket_exit(object* self){
    if (!CAST_SOCKET(self)->closed){
        _socket_close(CAST_SOCKET(self)->fd);
        if (CAST_SOCKET(self)->server!=NULL){
            fpl_free(CAST_SOCKET(self)->server);
        }
    }
    return new_none();
}

object* new_socket_module(){
    SocketType=(*(TypeObject*)finalize_type(&SocketType));
    fplbases.push_back(&SocketType);
    inherit_type_dict(&SocketType);
    setup_type_offsets(&SocketType);
    setup_type_getsets(&SocketType);
    setup_type_methods(&SocketType);

    object* dict=new_dict();
    
    dict_set(dict, str_new_fromstr("socket"), (object*)&SocketType);
    
    //Address family types
    #ifdef AF_UNIX
    dict_set(dict, str_new_fromstr("AF_UNIX"), new_int_fromint(AF_UNIX));
    #endif
    dict_set(dict, str_new_fromstr("AF_INET"), new_int_fromint(AF_INET));
    dict_set(dict, str_new_fromstr("AF_INET6"), new_int_fromint(AF_INET6));
    
    //Socket Type types
    dict_set(dict, str_new_fromstr("SOCK_STREAM"), new_int_fromint(SOCK_STREAM));
    dict_set(dict, str_new_fromstr("SOCK_DGRAM"), new_int_fromint(SOCK_DGRAM));
    dict_set(dict, str_new_fromstr("SOCK_RAW"), new_int_fromint(SOCK_RAW));
    dict_set(dict, str_new_fromstr("SOCK_RDM"), new_int_fromint(SOCK_RDM));
    dict_set(dict, str_new_fromstr("SOCK_SEQPACKET"), new_int_fromint(SOCK_SEQPACKET));
    dict_set(dict, str_new_fromstr("INADDR_ANY"), new_int_fromint(INADDR_ANY));
    dict_set(dict, str_new_fromstr("INADDR_BROADCAST"), new_int_fromint(INADDR_BROADCAST));

    //Flags
    #ifndef _WIN32
    dict_set(dict, str_new_fromstr("MSG_CONFIRM"), new_int_fromint(MSG_CONFIRM));
    dict_set(dict, str_new_fromstr("MSG_DONTROUTE"), new_int_fromint(MSG_DONTROUTE));
    dict_set(dict, str_new_fromstr("MSG_DONTWAIT"), new_int_fromint(MSG_DONTWAIT));
    dict_set(dict, str_new_fromstr("MSG_EOR"), new_int_fromint(MSG_EOR));
    dict_set(dict, str_new_fromstr("MSG_MORE"), new_int_fromint(MSG_MORE));
    dict_set(dict, str_new_fromstr("MSG_NOSIGNAL"), new_int_fromint(MSG_NOSIGNAL));
    dict_set(dict, str_new_fromstr("MSG_DONTROUTE"), new_int_fromint(MSG_DONTROUTE));
    #endif
    dict_set(dict, str_new_fromstr("MSG_DONTROUTE"), new_int_fromint(MSG_DONTROUTE));
    dict_set(dict, str_new_fromstr("MSG_OOB"), new_int_fromint(MSG_OOB));

    //Socket options (level)
    dict_set(dict, str_new_fromstr("SOL_SOCKET"), new_int_fromint(SOL_SOCKET));

    //Socket options (options)
    dict_set(dict, str_new_fromstr("SO_DEBUG"), new_int_fromint(SO_DEBUG));
    dict_set(dict, str_new_fromstr("SO_BROADCAST"), new_int_fromint(SO_BROADCAST));
    dict_set(dict, str_new_fromstr("SO_REUSEADDR"), new_int_fromint(SO_REUSEADDR));
    dict_set(dict, str_new_fromstr("SO_KEEPALIVE"), new_int_fromint(SO_KEEPALIVE));
    dict_set(dict, str_new_fromstr("SO_LINGER"), new_int_fromint(SO_LINGER));
    dict_set(dict, str_new_fromstr("SO_OOBINLINE"), new_int_fromint(SO_OOBINLINE));
    dict_set(dict, str_new_fromstr("SO_SNDBUF"), new_int_fromint(SO_SNDBUF));
    dict_set(dict, str_new_fromstr("SO_RCVBUF"), new_int_fromint(SO_RCVBUF));
    dict_set(dict, str_new_fromstr("SO_DONTROUTE"), new_int_fromint(SO_DONTROUTE));
    dict_set(dict, str_new_fromstr("SO_RCVLOWAT"), new_int_fromint(SO_RCVLOWAT));
    dict_set(dict, str_new_fromstr("SO_RCVTIMEO"), new_int_fromint(SO_RCVTIMEO));
    dict_set(dict, str_new_fromstr("SO_SNDLOWAT"), new_int_fromint(SO_SNDLOWAT));
    dict_set(dict, str_new_fromstr("SO_SNDTIMEO"), new_int_fromint(SO_SNDTIMEO));
    dict_set(dict, str_new_fromstr("SO_ENABLED"), new_int_fromint(1));
    dict_set(dict, str_new_fromstr("SO_DISABLED"), new_int_fromint(0));
    dict_set(dict, str_new_fromstr("SO_CONDITIONAL_ACCEPT"), new_int_fromint(SO_CONDITIONAL_ACCEPT));
    

    socket_init();

    return module_new_fromdict(dict, str_new_fromstr("socket"));
}