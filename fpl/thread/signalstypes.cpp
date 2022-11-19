void mutex_del(object* self);
object* mutex_new(object* type, object* args, object* kwargs);
object* mutex_repr(object* self);
object* mutex_release_meth(object* selftp, object* args, object* kwargs);
object* mutex_acquire_meth(object* selftp, object* args, object* kwargs);

typedef struct MutexObject{
    OBJHEAD_EXTRA
    pthread_mutex_t* lock;
    bool locked; //false if unlocked
}MutexObject;

Method mutex_methods[]={{"acquire", (cwrapperfunc)mutex_acquire_meth}, {"release", (cwrapperfunc)mutex_release_meth}, {NULL,NULL}};
GetSets mutex_getsets[]={{NULL,NULL}};
OffsetMember mutex_offsets[]={{NULL}};

static NumberMethods mutex_num_methods{
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

static Mappings mutex_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject MutexType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("Mutex"), //name
    sizeof(MutexObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    mutex_new, //slot_new
    mutex_del, //slot_del

    0, //slot_next
    0, //slot_iter

    mutex_repr, //slot_repr
    mutex_repr, //slot_str
    0, //slot_call

    &mutex_num_methods, //slot_number
    &mutex_mappings, //slot_mapping

    mutex_methods, //slot_methods
    mutex_getsets, //slot_getsets
    mutex_offsets, //slot_offsets

    0, //slot_cmp
};

#define CAST_MUTEX(obj) ((MutexObject*)obj)



void semaphore_del(object* self);
object* semaphore_new(object* type, object* args, object* kwargs);
object* semaphore_repr(object* self);
object* semaphore_release_meth(object* selftp, object* args, object* kwargs);
object* semaphore_acquire_meth(object* selftp, object* args, object* kwargs);

typedef struct SemaphoreObject{
    OBJHEAD_EXTRA
    sem_t* sem;
}SemaphoreObject;

Method semaphore_methods[]={{"acquire", (cwrapperfunc)semaphore_acquire_meth}, {"release", (cwrapperfunc)semaphore_release_meth}, {NULL,NULL}};
GetSets semaphore_getsets[]={{NULL,NULL}};
OffsetMember semaphore_offsets[]={{NULL}};

static NumberMethods semaphore_num_methods{
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

static Mappings semaphore_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
    0, //slot_append
};

TypeObject SemaphoreType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("Semaphore"), //name
    sizeof(SemaphoreObject), //size
    0, //var_base_size
    true, //gc_trackable
    NULL, //bases
    0, //dict_offset
    NULL, //dict
    0, //slot_getattr
    0, //slot_setattr

    0, //slot_init
    semaphore_new, //slot_new
    semaphore_del, //slot_del

    0, //slot_next
    0, //slot_iter

    semaphore_repr, //slot_repr
    semaphore_repr, //slot_str
    0, //slot_call

    &semaphore_num_methods, //slot_number
    &semaphore_mappings, //slot_mapping

    semaphore_methods, //slot_methods
    semaphore_getsets, //slot_getsets
    semaphore_offsets, //slot_offsets

    0, //slot_cmp
};

#define CAST_SEMAPHORE(obj) ((SemaphoreObject*)obj)