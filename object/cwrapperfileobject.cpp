object* cwrapperfile_new_fromfile(FILE* file, bool readable, bool writable){
    object* o=new_object(&CWrapperFileType);

    CAST_CWRAPPERFILE(o)->file=file;

    CAST_CWRAPPERFILE(o)->readable=readable;

    CAST_CWRAPPERFILE(o)->writable=writable;

    return o;
}

object* cwrapperfile_read_meth(object* selftp, object* args, object* kwargs){   
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
    if (!CAST_CWRAPPERFILE(self)->readable){
        vm_add_err(&InvalidOperationError, vm, "Unable to read from file");
        return NULL;
    }

    long start = ftell(CAST_CWRAPPERFILE(self)->file);
    fseek(CAST_CWRAPPERFILE(self)->file, 0, SEEK_END);
    long fsize = ftell(CAST_CWRAPPERFILE(self)->file);
    fseek(CAST_CWRAPPERFILE(self)->file, 0, start);

    char *s = (char*)fpl_malloc(fsize + 1);
    memset(s, 0, fsize);
    size_t i=fread(s, fsize, 1, CAST_CWRAPPERFILE(self)->file);
    if (i==0 && fsize>0 && ferror(CAST_CWRAPPERFILE(self)->file)){
        vm_add_err(&InvalidOperationError, vm, "Unable to read from file");
        return NULL;
    }
    s[fsize] = 0;

    string str(s);
    return str_new_fromstr(str);
}

object* cwrapperfile_write_meth(object* selftp, object* args, object* kwargs){   
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
    if (!CAST_CWRAPPERFILE(self)->writable){
        vm_add_err(&InvalidOperationError, vm, "Unable to write to file");
        return NULL;
    }
    
    string s=object_cstr(list_index_int(args, 1));

    int i=fprintf(CAST_CWRAPPERFILE(self)->file, "%s", s.c_str());
    if (i==0 && s.size()>0 && ferror(CAST_CWRAPPERFILE(self)->file)){
        vm_add_err(&InvalidOperationError, vm, "Unable to write to file");
        return NULL;
    }
    FPLINCREF(self);
    return self;
}

object* cwrapperfile_flush_meth(object* selftp, object* args, object* kwargs){  
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
    
    fflush(CAST_CWRAPPERFILE(self)->file);

    return new_none();
}