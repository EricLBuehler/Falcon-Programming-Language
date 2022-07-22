object* type_new(object* type, object* args, object* kwargs);
void type_del(object* self);
object* type_repr(object* self);
object* type_cmp(object* self, object* other, uint8_t type);
object* type_call(object* self, object* args, object* kwargs);
object* type_get(object* self, object* attr);
void type_set(object* obj, object* attr, object* val);

#define CAST_TYPE(obj) ((TypeObject*)obj)
#define CAST_TYPE_(obj) ((TypeObjectHeap*)obj)

typedef struct TypeObjectHeap{
    OBJHEAD_EXTRA
    TypeObject otype;
}TypeObjectHeap;


TypeObject TypeType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("type"), //name
    sizeof(TypeObjectHeap), //size
    false, //gc_trackable
    NULL, //bases
    0, //dict_offset
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

    (compfunc)type_cmp, //slot_cmp
};

TypeObject* finalize_type(TypeObject* type){
    //Inherit here..
    return type;
}

object* new_type(TypeObject type, string* name, object* bases, object* dict){
    TypeObject* type_=(TypeObject*)malloc(sizeof(TypeObject));
    memcpy(type_, &type, sizeof(TypeObject));
    TypeObject newtype={
        0, //refcnt
        0, //ob_prev
        0, //ob_next
        0, //gen
        type_, //type
        name, //name
        type.size, //size
        type.gc_trackable, //gc_trackable
        bases, //bases
        type.dict_offset, //dict_offset
        dict, //dict
        type.slot_getattr, //slot_getattr
        type.slot_setattr, //etattr

        type.slot_init, //slot_init
        type.slot_new, //slot_new
        type.slot_del, //slot_del

        type.slot_next, //slot_next
        type.slot_get, //slot_get
        type.slot_len, //slot_len
        type.slot_set, //slot_set
        type.slot_append, //slot_append

        type.slot_repr, //slot_repr
        type.slot_str, //slot_str
        type.slot_call, //slot_call

        type.slot_number, //slot_number

        type.slot_cmp, //slot_cmp
    };
    finalize_type(&newtype);

    object* t=new_object(&TypeType);
    CAST_TYPE_(t)->otype=newtype;
    return t;
}

object* type_new(object* type, object* args, object* kwargs);
void type_del(object* self);
object* type_repr(object* self);
object* type_cmp(object* self, object* other, uint8_t type);
object* type_call(object* self, object* args, object* kwargs);
void setup_type_type(){
    finalize_type(&TypeType);
}