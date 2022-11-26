#include <sys/stat.h>

object* file_new(object* type, object* args, object* kwargs){
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if ((len!=2 && len!=3)){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }
    object* name=list_index_int(args, 0);
    if (!object_istype(name->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected string type name, got type '%s'", name->type->name->c_str());
        return NULL;
    }
    object* mode_=list_index_int(args, 1);
    if (!object_istype(mode_->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected string type name, got type '%s'", mode_->type->name->c_str());
        return NULL;
    }
    object* encoding_=NULL;
    if (len!=3){
        encoding_=list_index_int(args, 2);
        if (!object_istype(encoding_->type, &StrType)){
            vm_add_err(&ValueError, vm, "Expected string type name, got type '%s'", encoding_->type->name->c_str());
            return NULL;
        }
    }

    string s=*CAST_STRING(name)->val;
    string mode=*CAST_STRING(mode_)->val;
    FILE* f=fopen(s.c_str(), mode.c_str());
    if (f==NULL){
        vm_add_err(&FileNotFoundError, vm, "File '%s' not found", s.c_str());
        return NULL;
    }

    object* o=new_object(CAST_TYPE(type));
    CAST_FILE(o)->name=name;
    CAST_FILE(o)->file=f;
    if (encoding_!=NULL){
        string s=object_cstr(encoding_);
        CAST_FILE(o)->encoding=(char*)fpl_malloc( (sizeof(char)*strlen(s.c_str())) +1);
        memset(CAST_FILE(o)->encoding, 0, sizeof(char)*strlen(s.c_str()));
        memcpy(CAST_FILE(o)->encoding, s.c_str(), sizeof(char)*strlen(s.c_str()));
    }
    else{
        string s="UTF-8";
        CAST_FILE(o)->encoding=(char*)fpl_malloc( (sizeof(char)*strlen(s.c_str())) +1);
        memset(CAST_FILE(o)->encoding, 0, sizeof(char)*strlen(s.c_str()));
        memcpy(CAST_FILE(o)->encoding, s.c_str(), sizeof(char)*strlen(s.c_str()));
    }
    CAST_FILE(o)->open=true;
    CAST_FILE(o)->mode=(char*)fpl_malloc( (sizeof(char)*strlen(mode.c_str())) +1);
    memset(CAST_FILE(o)->mode, 0, sizeof(char)*strlen(mode.c_str()));
    memcpy(CAST_FILE(o)->mode, mode.c_str(), sizeof(char)*strlen(mode.c_str()));
    return o;
}

void file_del(object* self){
    FPLDECREF(CAST_FILE(self)->name);
    if (CAST_FILE(self)->open){
        fclose(CAST_FILE(self)->file);
    }
    fpl_free(CAST_FILE(self)->mode);
    fpl_free(CAST_FILE(self)->encoding);
}

object* file_repr(object* self){
    string s="";
    s+="<file name='";
    s+=object_cstr(CAST_FILE(self)->name);
    s+="' ";
    s+=" mode='";
    string m=CAST_FILE(self)->mode;
    s+=m;
    s+="' ";    
    s+=" encoding='";
    string e=CAST_FILE(self)->encoding;
    s+=e;
    s+="'";    
    s+=">";
    return str_new_fromstr(s);
}

object* file_new_fromfile(object* name, char* mode){
    if (!object_istype(name->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected string type name, got type '%s'", name->type->name->c_str());
        return NULL;
    }
    string s=*CAST_STRING(name)->val;
    FILE* f=fopen(s.c_str(), mode);

    object* o=new_object(&FileType);
    CAST_FILE(o)->name=name;
    CAST_FILE(o)->file=f;
    CAST_FILE(o)->open=true;
    CAST_FILE(o)->mode=(char*)fpl_malloc( (sizeof(char)*strlen(mode)) +1);
    memset(CAST_FILE(o)->mode, 0, sizeof(char)*strlen(mode));
    memcpy(CAST_FILE(o)->mode, mode, sizeof(char)*strlen(mode));
    return o;
}

bool is_binary(char* mode){
    int idx=0;
    char c=mode[idx++];
    while (c){
        if (c=='b'){
            return true;
        }
        c=mode[idx++];
    }
    return false;
}

object* file_read_meth(object* selftp, object* args, object* kwargs){    
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }
    object* self=list_index_int(args, 0);
    if (!CAST_FILE(self)->open){
        vm_add_err(&ValueError, vm, "Attempting to read from a closed file");
        return NULL;
    }

    long start = ftell(CAST_FILE(self)->file);
    fseek(CAST_FILE(self)->file, 0, SEEK_END);
    long fsize = ftell(CAST_FILE(self)->file);
    fseek(CAST_FILE(self)->file, 0, start);

    char *s = (char*)fpl_malloc(fsize + 1);
    memset(s, 0, fsize);
    size_t i=fread(s, fsize, 1, CAST_FILE(self)->file);
    if (i==0 && fsize>0 && ferror(CAST_FILE(self)->file)){
        vm_add_err(&InvalidOperationError, vm, "Unable to read from file");
        return NULL;
    }
    s[fsize] = 0;

    if (!is_binary(CAST_FILE(self)->mode)){
        string str(s);
        return str_new_fromstr(str);
    }
    return bytes_new_frombytearr(s, fsize);
}

object* file_seek_meth(object* selftp, object* args, object* kwargs){    
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }
    object* self=list_index_int(args, 0);
    if (!CAST_FILE(self)->open){
        vm_add_err(&ValueError, vm, "Attempting to seek in a closed file");
        return NULL;
    }
    object* o=list_index_int(args, 1);
    object* val=o->type->slot_number->slot_int(o);
    if (val==NULL){
        return val;
    }
    fseek(CAST_FILE(self)->file, CAST_INT(val)->val->to_int(), SEEK_SET );

    return new_none();
}

object* file_write_meth(object* selftp, object* args, object* kwargs){   
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }
     
    object* self=list_index_int(args, 0);
    if (!CAST_FILE(self)->open){
        vm_add_err(&ValueError, vm, "Attempting to write to a closed file");
        return NULL;
    }

    int i;

    if (is_binary(CAST_FILE(self)->mode)){
        object* bytes=object_bytes(list_index_int(args, 1));

        if (bytes==NULL || !object_issubclass(bytes, &BytesType)){
            vm_add_err(&TypeError, vm, "Expecting string or bytes-like object, got '%s' object",list_index_int(args, 1)->type->name->c_str());
            return NULL;
        }
        i=fprintf(CAST_FILE(self)->file, "%s", CAST_BYTES(bytes)->val);
        if (i==0 && CAST_BYTES(bytes)->len>0 && ferror(CAST_FILE(self)->file)){
            vm_add_err(&InvalidOperationError, vm, "Unable to write to file");
            return NULL;
        }
        FPLINCREF(self);
        return self;
    }
    
    string s=object_cstr(list_index_int(args, 1));
    
    iconv_t cd = iconv_open(CAST_FILE(self)->encoding, "UTF-8");
    if((int) cd == -1) {
        if (errno == EINVAL) {
            vm_add_err(&ValueError, vm, "Invalid conversion");
            return NULL; 
        }
    }

    size_t s_size=s.size();  
    size_t new_size=(s.size()+1)*sizeof(uint32_t);

    const char* orig_str=s.c_str();
    char* converted=(char*)fpl_calloc(new_size, sizeof(char));
    char* start=converted;
    
    int ret = iconv(cd, &orig_str, &s_size, &converted, &new_size);
    
    if((iconv_t)ret == (iconv_t)(-1)) {
        vm_add_err(&ValueError, vm, "Invalid multibyte sequence encountered");
        return NULL; 
    }
    iconv_close(cd);

    i=fprintf(CAST_FILE(self)->file, "%s", start);
    if (i==0 && s.size()>0 && ferror(CAST_FILE(self)->file)){
        vm_add_err(&InvalidOperationError, vm, "Unable to write to file");
        return NULL;
    }
    FPLINCREF(self);
    return self;
}

object* file_close_meth(object* selftp, object* args, object* kwargs){   
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }

    object* self=list_index_int(args, 0);
    fclose(CAST_FILE(self)->file);
    CAST_FILE(self)->open=false;
    return new_none();
}

object* file_size_meth(object* selftp, object* args, object* kwargs){    
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }

    object* self=list_index_int(args, 0);
    if (!CAST_FILE(self)->open){
        vm_add_err(&ValueError, vm, "Attempting to get size of a closed file");
        return NULL;
    }
    
    long start = ftell(CAST_FILE(self)->file);
    fseek(CAST_FILE(self)->file, 0, SEEK_END);
    long fsize = ftell(CAST_FILE(self)->file);
    fseek(CAST_FILE(self)->file, 0, start);

    return new_int_fromint(fsize);
}

object* file_flush_meth(object* selftp, object* args, object* kwargs){    
    int len=CAST_LIST(args)->size;
    if (CAST_DICT(kwargs)->val->size()!=0){
        vm_add_err(&ValueError, vm, "Expected no keyword arguments, got %d", CAST_DICT(kwargs)->val->size());
        return NULL;
    }
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }

    object* self=list_index_int(args, 0);
    if (!CAST_FILE(self)->open){
        vm_add_err(&ValueError, vm, "Attempting to flush closed file");
        return NULL;
    }
    
    fflush(CAST_FILE(self)->file);

    return new_none();
}


object* file_enter(object* self){
    FPLINCREF(self);
    return self;
}

object* file_exit(object* self){
    fclose(CAST_FILE(self)->file);
    CAST_FILE(self)->open=false;
    return new_none();
}