void thread_del(object* self);
object* thread_new(object* type, object* args, object* kwargs);
object* thread_cmp(object* self, object* other, uint8_t type);
object* thread_repr(object* self);
object* thread_start_meth(object* selftp, object* args, object* kwargs);
object* thread_join_meth(object* selftp, object* args, object* kwargs);

typedef struct ThreadObject{
    OBJHEAD_VAR
    object* callable;
    pthread_t* thread;
}ThreadObject;

Method thread_methods[]={{"start", (cwrapperfunc)thread_start_meth}, {"join", (cwrapperfunc)thread_join_meth}, {NULL,NULL}};
GetSets thread_getsets[]={{NULL,NULL}};
OffsetMember thread_offsets[]={{NULL}};

static NumberMethods thread_num_methods{
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

static Mappings thread_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject ThreadType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("Thread"), //name
    sizeof(ThreadObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    thread_new, //slot_new
    thread_del, //slot_del

    0, //slot_next
    0, //slot_iter

    thread_repr, //slot_repr
    thread_repr, //slot_str
    0, //slot_call

    &thread_num_methods, //slot_number
    &thread_mappings, //slot_mapping

    thread_methods, //slot_methods
    thread_getsets, //slot_getsets
    thread_offsets, //slot_offsets

    (compfunc)thread_cmp, //slot_cmp
};

#define CAST_THREAD(obj) ((ThreadObject*)obj)

typedef struct ThreadArgs{
    object* callable;
    object* args;
    object* kwargs;
}ThreadArgs;