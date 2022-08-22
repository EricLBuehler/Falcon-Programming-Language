object* file_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }
    object* name=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    if (!object_istype(name->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected string type name, got type '%s'", name->type->name->c_str());
        return NULL;
    }
    object* mode_=args->type->slot_mappings->slot_get(args, new_int_fromint(1));
    if (!object_istype(mode_->type, &StrType)){
        vm_add_err(&ValueError, vm, "Expected string type name, got type '%s'", mode_->type->name->c_str());
        return NULL;
    }

    string s=*CAST_STRING(name)->val;
    string mode=*CAST_STRING(mode_)->val;
    FILE* f=fopen(s.c_str(), mode.c_str());
    if (f==NULL){
        vm_add_err(&FileNotFoundError, vm, "File '%s' not found", s.c_str());
        return NULL;
    }

    object* o=new_object(&FileType);
    CAST_FILE(o)->name=name;
    CAST_FILE(o)->file=f;
    CAST_FILE(o)->open=true;
    CAST_FILE(o)->mode=(char*)malloc( (sizeof(char)*strlen(mode.c_str())) +1);
    memset(CAST_FILE(o)->mode, 0, sizeof(char)*strlen(mode.c_str()));
    memcpy(CAST_FILE(o)->mode, mode.c_str(), sizeof(char)*strlen(mode.c_str()));
    return o;
}

void file_del(object* self){
    DECREF(CAST_FILE(self)->name);
    if (CAST_FILE(self)->open){
        fclose(CAST_FILE(self)->file);
    }
    free(CAST_FILE(self)->mode);
}

object* file_repr(object* self){
    string s="";
    s+="<file '";
    s+=object_cstr(CAST_FILE(self)->name);
    s+="'>";
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
    CAST_FILE(o)->mode=(char*)malloc( (sizeof(char)*strlen(mode)) +1);
    memset(CAST_FILE(o)->mode, 0, sizeof(char)*strlen(mode));
    memcpy(CAST_FILE(o)->mode, mode, sizeof(char)*strlen(mode));
    return o;
}

object* file_read_meth(object* args, object* kwargs){    
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    if (object_istype(self->type, &TypeType)){
        vm_add_err(&TypeError, vm, "Expected file type, got type '%s'", self->type->name->c_str());
        return NULL;
    }
    if (!CAST_FILE(self)->open){
        vm_add_err(&ValueError, vm, "Attempting to read from a closed file");
        return NULL;
    }

    fseek(CAST_FILE(self)->file, 0, SEEK_END);
    long fsize = ftell(CAST_FILE(self)->file);
    fseek(CAST_FILE(self)->file, 0, SEEK_SET);  /* same as rewind(f); */

    char *s = (char*)malloc(fsize + 1);
    int i=fread(s, fsize, 1, CAST_FILE(self)->file);
    if (i==0){
        vm_add_err(&InvalidOperationError, vm, "Unable to read from file");
        return NULL;
    }
    s[fsize] = 0;
    string str(s);
    return str_new_fromstr(str);
}

object* file_seek_meth(object* args, object* kwargs){    
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    if (object_istype(self->type, &TypeType)){
        vm_add_err(&TypeError, vm, "Expected file type, got type '%s'", self->type->name->c_str());
        return NULL;
    }
    if (!CAST_FILE(self)->open){
        vm_add_err(&ValueError, vm, "Attempting to seek in a closed file");
        return NULL;
    }
    object* o=args->type->slot_mappings->slot_get(args, new_int_fromint(1));
    object* val=o->type->slot_number->slot_int(o);
    if (val==NULL){
        return val;
    }
    fseek(CAST_FILE(self)->file, CAST_INT(val)->val->to_int(), SEEK_SET );

    return new_none();
}

object* file_write_meth(object* args, object* kwargs){   
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=2){
        vm_add_err(&ValueError, vm, "Expected 2 arguments, got %d", len);
        return NULL;
    }
     
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    if (object_istype(self->type, &TypeType)){
        vm_add_err(&TypeError, vm, "Expected file type, got type '%s'", self->type->name->c_str());
        return NULL;
    }
    if (!CAST_FILE(self)->open){
        vm_add_err(&ValueError, vm, "Attempting to write to a closed file");
        return NULL;
    }

    string s=object_cstr(args->type->slot_mappings->slot_get(args, new_int_fromint(1)));
    int i=fprintf(CAST_FILE(self)->file, "%s", s.c_str() );
    if (i==0){
        vm_add_err(&InvalidOperationError, vm, "Unable to write to file");
        return NULL;
    }
    return new_none();
}

object* file_close_meth(object* args, object* kwargs){    
    object* self=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    if (object_istype(self->type, &TypeType)){
        vm_add_err(&TypeError, vm, "Expected file type, got type '%s'", self->type->name->c_str());
        return NULL;
    }

    fclose(CAST_FILE(self)->file);
    CAST_FILE(self)->open=false;
    return new_none();
}