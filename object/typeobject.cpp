object* type_new(object* type, object* args, object* kwargs);
void type_del(object* self);
object* type_repr(object* self);
object* type_cmp(object* self, object* other, uint8_t type);
object* type_call(object* self, object* args, object* kwargs);
object* type_get(object* self, object* attr);
void type_set(object* obj, object* attr, object* val);


TypeObject TypeType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("type"), //name
    sizeof(TypeObject), //size
    false, //gc_trackable
    NULL, //bases
    offsetof(TypeObject, dict), //dict_offset
    NULL, //dict
    (getattrfunc)type_get, //slot_getattr
    (setattrfunc)type_set, //slot_getattr

    0, //slot_init
    (newfunc)type_new, //slot_new
    (delfunc)type_del, //slot_del

    0, //slot_next
    0, //slot_get
    0, //slot_len
    0, //slot_set
    0, //slot_append

    (reprfunc)type_repr, //slot_repr
    (reprfunc)type_repr, //slot_str
    (callfunc)type_call, //slot_call

    0, //slot_number

    0, //slot_cmp
};


object* newtp_init(object* self, object* args, object* kwargs);
object* newtp_new(object* self, object* args, object* kwargs);
void newtp_del(object* self);
object* newtp_next(object* self);
object* newtp_get(object* self, object* idx);
object* newtp_len(object* self);
void newtp_set(object* self, object* idx, object* val);
void newtp_append(object* self, object* val);
object* newtp_repr(object* self);
object* newtp_str(object* self);
object* newtp_call(object* self, object* args, object* kwargs);
object* newtp_cmp(object* self, object* other, uint8_t type);

object* newtp_add(object* self, object* other);
object* newtp_sub(object* self, object* other);
object* newtp_mul(object* self, object* other);
object* newtp_div(object* self, object* other);

object* newtp_neg(object* self);


NumberMethods newtp_number={    
    //binops
    newtp_add,
    newtp_sub,
    newtp_mul,
    newtp_div,

    //unaryops
    newtp_neg,
};

typedef struct NewTypeObject{
    OBJHEAD_EXTRA;
    object* dict;
}NewTypeObject;

#define CAST_NEWTYPE(obj) ((NewTypeObject*)(obj))


object* new_type(string* name, object* bases, object* dict){
    TypeObject newtype={
        0, //refcnt
        0, //ob_prev
        0, //ob_next
        0, //gen
        &TypeType, //type
        name, //name
        sizeof(NewTypeObject), //size
        true, //gc_trackable
        bases, //bases
        0, //dict_offset
        dict, //dict
        object_genericgetattr, //slot_getattr
        object_genericsetattr, //slot_setattr

        newtp_init, //slot_init
        newtp_new, //slot_new
        newtp_del, //slot_del

        newtp_next, //slot_next
        newtp_get, //slot_get
        newtp_len, //slot_len
        newtp_set, //slot_set
        newtp_append, //slot_append

        newtp_repr, //slot_repr
        newtp_str, //slot_str
        newtp_call, //slot_call

        &newtp_number, //slot_number

        newtp_cmp, //slot_cmp
    };
        
    return finalize_type(&newtype);
}

object* type_new(object* type, object* args, object* kwargs);
void type_del(object* self);
object* type_repr(object* self);
object* type_cmp(object* self, object* other, uint8_t type);
object* type_call(object* self, object* args, object* kwargs);
void setup_type_type(){
    TypeType=(*(TypeObject*)finalize_type(&TypeType));
}
