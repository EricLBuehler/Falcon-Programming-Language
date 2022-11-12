#include "nxjson/nxjson.c"

object* json_decode_object(const nx_json* json);
object* json_encode_dict(object* dict);

object* json_decode_array(const nx_json* json){
    object* list=new_list();
    const nx_json* item=nx_json_item(json, 0);
    while (item){
        switch (item->type){
            case NX_JSON_NULL:
                list_append(list, new_none());
                break;
            case NX_JSON_STRING:
                list_append(list, str_new_fromstr(string(item->text_value)));
                break;
            case NX_JSON_INTEGER:
                list_append(list, new_int_fromint(item->num.s_value));
                break;
            case NX_JSON_DOUBLE:
                list_append(list, new_float_fromdouble(item->num.dbl_value));
                break;
            case NX_JSON_BOOL:
                list_append(list, item->num.u_value? new_bool_true() : new_bool_false());
                break;
            case NX_JSON_OBJECT:
                list_append(list, json_decode_object(item));
                break;
            case NX_JSON_ARRAY:
                list_append(list, json_decode_array(item));
                break;
        }
        item=item->next;
    }
    return list;
}

object* json_decode_object(const nx_json* json){
    object* dict=new_dict();
    const nx_json* item=nx_json_item(json, 0);
    while (item){
        switch (item->type){
            case NX_JSON_NULL:
                dict_set_noret(dict, str_new_fromstr(string(item->key)), new_none());
                break;
            case NX_JSON_STRING:
                dict_set_noret(dict, str_new_fromstr(string(item->key)), str_new_fromstr(string(item->text_value)));
                break;
            case NX_JSON_INTEGER:
                dict_set_noret(dict, str_new_fromstr(string(item->key)), new_int_fromint(item->num.s_value));
                break;
            case NX_JSON_DOUBLE:
                dict_set_noret(dict, str_new_fromstr(string(item->key)), new_float_fromdouble(item->num.dbl_value));
                break;
            case NX_JSON_BOOL:
                dict_set_noret(dict, str_new_fromstr(string(item->key)), item->num.u_value? new_bool_true() : new_bool_false());
                break;
            case NX_JSON_OBJECT:
                dict_set_noret(dict, str_new_fromstr(string(item->key)), json_decode_object(item));
                break;
            case NX_JSON_ARRAY:
                dict_set_noret(dict, str_new_fromstr(string(item->key)), json_decode_array(item));
                break;
        }
        item=item->next;
    }
    return dict;
}

object* json_decode(object* selftp, object* args){
    object* str=dict_get(args, str_new_fromstr("string"));

    const nx_json* json=nx_json_parse_utf8((char*)object_cstr(str).c_str());
    FPLDECREF(str);
    if (json!=NULL){
        return json_decode_object(json);
    }
    return NULL;
}

object* json_encode_list(object* list){
    string s="";
    s+="[";
    for (size_t i=0; i<CAST_LIST(list)->size; i++){
        object* itm=CAST_LIST(list)->array[i];

        if (object_istype(itm->type, &StrType)){
            s+="\""+object_cstr(itm)+"\"";
        }
        else if (object_istype(itm->type, &IntType)){
            s+=object_cstr(itm);
        }
        else if (object_istype(itm->type, &FloatType)){
            if (isinf(CAST_FLOAT(itm)->val) || isnan(CAST_FLOAT(itm)->val)){
                s+="null";
            }
            else{
                s+=object_cstr(itm);
            }
        }
        else if (object_istype(itm->type, &BoolType)){
            s+=CAST_BOOL(itm)->val ? "true" : "false";
        }
        else if (object_istype(itm->type, &NoneType)){
            s+="null";
        }
        else if (object_istype(itm->type, &DictType)){
            object* o=json_encode_dict(itm);
            if (o==NULL){
                return NULL;
            }
            s+=object_cstr(o);
        }
        else if (object_istype(itm->type, &ListType) || object_istype(itm->type, &TupleType)){
            object* o=json_encode_list(itm);
            if (o==NULL){
                return NULL;
            }
            s+=object_cstr(o);
        }
        else{
            vm_add_err(&TypeError, vm, "Expected str, int, float, bool, None, dict, list, or tuple object, got '%s' object", itm->type->name->c_str());
            return NULL; 
        }

        if (i!=CAST_LIST(list)->size-1){
            s+=", ";
        }
    }
    s+="]";
    return str_new_fromstr(s);
}


object* json_encode_dict(object* dict){
    string s="{";
    int i=0;
    int size=CAST_DICT(dict)->val->size();
    for (auto k: (*CAST_DICT(dict)->val)){
        if (!object_istype(k.first->type, &StrType)){
            vm_add_err(&TypeError, vm, "Expected str, got '%s'", k.first->type->name->c_str());
            return NULL; 
        }
        s+="\""+object_cstr(k.first)+"\"";
        s+=": ";
        if (object_istype(k.second->type, &StrType)){
            s+="\""+object_cstr(k.second)+"\"";
        }
        else if (object_istype(k.second->type, &IntType)){
            s+=object_cstr(k.second);
        }
        else if (object_istype(k.second->type, &FloatType)){
            if (isinf(CAST_FLOAT(k.second)->val) || isnan(CAST_FLOAT(k.second)->val)){
                s+="null";
            }
            else{
                s+=object_cstr(k.second);
            }
        }
        else if (object_istype(k.second->type, &BoolType)){
            s+=CAST_BOOL(k.second)->val ? "true" : "false";
        }
        else if (object_istype(k.second->type, &NoneType)){
            s+="null";
        }
        else if (object_istype(k.second->type, &DictType)){
            object* o=json_encode_dict(k.second);
            if (o==NULL){
                return NULL;
            }
            s+=object_cstr(o);
        }
        else if (object_istype(k.second->type, &ListType) || object_istype(k.second->type, &TupleType)){
            object* o=json_encode_list(k.second);
            if (o==NULL){
                return NULL;
            }
            s+=object_cstr(o);
        }
        else{
            vm_add_err(&TypeError, vm, "Expected str, int, float, bool, None, dict, list, or tuple object, got '%s' object", k.second->type->name->c_str());
            return NULL; 
        }

        if (i++!=size-1){
            s+=", ";
        }
    }
    s+="}";
    return str_new_fromstr(s);
}

object* json_encode(object* selftp, object* args){
    object* head=dict_get(args, str_new_fromstr("object"));

    if (!object_istype(head->type, &DictType)){
        vm_add_err(&TypeError, vm, "Expected dict, got '%s'", head->type->name->c_str());
        FPLDECREF(head);
        return NULL; 
    }

    object* d=json_encode_dict(head);

    FPLDECREF(head);
    
    return d;
}

object* new_json_module(){
    object* dict=new_dict();
    
    object* emptykwargs=new_tuple();

    object* decodeargs=new_tuple();
    decodeargs->type->slot_mappings->slot_append(decodeargs, str_new_fromstr("string"));
    object* decode=new_builtin(json_decode, str_new_fromstr("decode"), decodeargs, emptykwargs, 1, false);
    dict_set_noret(dict, str_new_fromstr("decode"), decode);
    FPLDECREF(decode);

    object* encodeargs=new_tuple();
    encodeargs->type->slot_mappings->slot_append(encodeargs, str_new_fromstr("object"));
    object* encode=new_builtin(json_encode, str_new_fromstr("encode"), encodeargs, emptykwargs, 1, false);
    dict_set_noret(dict, str_new_fromstr("encode"), encode);
    FPLDECREF(encode);

    return module_new_fromdict(dict, str_new_fromstr("json"));
}