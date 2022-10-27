void socket_del(object* self);
object* socket_new(object* type, object* args, object* kwargs);
object* socket_cmp(object* self, object* other, uint8_t type);
object* socket_repr(object* self);
object* socket_connect(object* self, object* args, object* kwargs);
object* socket_close(object* self, object* args, object* kwargs);
object* socket_send(object* self, object* args, object* kwargs);
object* socket_recv(object* self, object* args, object* kwargs);
object* socket_gethostbyname(object* self, object* args, object* kwargs);
object* socket_bind(object* self, object* args, object* kwargs);
object* socket_listen(object* self, object* args, object* kwargs);
object* socket_accept(object* self, object* args, object* kwargs);
object* socket_setsockopt(object* self, object* args, object* kwargs);
object* socket_getsockopt(object* self, object* args, object* kwargs);
object* socket_getsockname(object* self, object* args, object* kwargs);
object* socket_sendall(object* self, object* args, object* kwargs);

object* socket_enter(object* self);
object* socket_exit(object* self);

typedef struct SocketObject{
    OBJHEAD_VAR
    SOCKET_T fd;
    int af_family;
    int sock_kind;
    bool closed;
    struct sockaddr_in* server;
}SocketObject;

Method socket_methods[]={{"connect",socket_connect}, {"close",socket_close}, {"send",socket_send}\
                        , {"recv",socket_recv}, {"gethostbyname",socket_gethostbyname}\
                        , {"bind",socket_bind}, {"listen",socket_listen}, {"accept",socket_accept}\
                        , {"setsockopt",socket_setsockopt}, {"getsockopt",socket_getsockopt}\
                        , {"getsockname",socket_getsockname}, {"sendall",socket_sendall}, {NULL,NULL}};

GetSets socket_getsets[]={{NULL,NULL}};
OffsetMember socket_offsets[]={{NULL}};

static NumberMethods socket_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div
    0, //slot_mod
    0, //slot_pow
    0, //slot_and
    0, //slot_or
    0, //slot_lshift
    0, //slot_rshift
    0, //slot_fldiv
    0, //slot_xor

    0, //slot_neg
    0, //slot_not
    0, //slot_abs

    0, //slot_bool
};

static Mappings socket_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject SocketType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("socket"), //name
    sizeof(SocketObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    socket_new, //slot_new
    socket_del, //slot_del

    0, //slot_next
    0, //slot_iter

    socket_repr, //slot_repr
    socket_repr, //slot_str
    0, //slot_call

    &socket_num_methods, //slot_number
    &socket_mappings, //slot_mapping

    socket_methods, //slot_methods
    socket_getsets, //slot_getsets
    socket_offsets, //slot_offsets

    (compfunc)socket_cmp, //slot_cmp

    0, //slot_descrget
    0, //slot_descrset

    socket_enter, //slot_enter
    socket_exit, //slot_exit
};

#define CAST_SOCKET(obj) ((SocketObject*)obj)