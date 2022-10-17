void new_gc(){
    gc.gen0=NULL;
    gc.gen1=NULL;
    gc.gen2=NULL;

    gc.gen0_n=0;
    gc.gen1_n=0;
    gc.gen2_n=0;

    gc.gen0_thresh=700;
    gc.gen1_thresh=100;
    gc.gen2_thresh=10;
}

bool no_outside_refs(object* obj){ //Must only be called with obj->type->gc_trackable true
    if ( ((object_var*)obj)->gc_ref - ((object*)obj)->refcnt<0){
        return true;
    }
    if (((object*)obj)->refcnt==0){
        return true;
    }
    
    return false; 
}

void gc_collect(int gen){
    object* obj;
    object* next;
    if (gen==1){
        obj=gc.gen1;
    }
    else if (gen==2){
        obj=gc.gen2;
    }
    else{
        obj=gc.gen0;
    }
    while (obj){
        if (obj->type->gc_trackable && no_outside_refs(obj) ){ //No outside references, so we can free
            if (((object_var*)obj)->gc_ref - ((object*)obj)->refcnt<0){
                if (obj->type->slot_del!=NULL){
                    obj->type->slot_del(obj);
                    if (obj->refcnt>0){
                        goto goaround;
                    }
                }
            }
            object* next=obj->ob_next;
            if (obj->gen!=2){                
                if (obj->gen==1){
                    if (obj->ob_prev!=NULL){
                        obj->ob_prev->ob_next=obj->ob_next;
                    }
                    else{
                        gc.gen1=obj->ob_next;
                    }                    
                    if (obj->ob_next!=NULL){
                        obj->ob_next->ob_prev=obj->ob_prev;
                    }
                    gc.gen1_n--;
                }
                else{
                    if (obj->ob_prev!=NULL){
                        obj->ob_prev->ob_next=obj->ob_next;
                    }
                    else{
                        gc.gen0=obj->ob_next;
                    }   
                    if (obj->ob_next!=NULL){
                        obj->ob_next->ob_prev=obj->ob_prev;
                    }
                    gc.gen0_n--;
                }

                free(obj);
            } 
            obj=next;
            continue;
        }
        else{
            if (gen!=2){
                if (obj->ob_next!=NULL){
                    obj->ob_next->ob_prev=NULL;
                }
                
                if (gen==1){
                    gc.gen1=obj->ob_next;
                }
                else{
                    gc.gen0=obj->ob_next;
                }
                obj->gen++;

                if (gen==0){
                    if (gc.gen1!=NULL && gc.gen1->ob_next!=NULL){
                        gc.gen1->ob_next->ob_prev=obj->ob_next;
                    }
                    next=obj->ob_next;
                    obj->ob_next=gc.gen1;
                    gc.gen1=obj;
                    obj->ob_prev=NULL;
                    gc.gen1_n++;
                    
                }
                else if (gen==1){
                    if (gc.gen2!=NULL && gc.gen2->ob_next!=NULL){
                        gc.gen2->ob_next->ob_prev=obj->ob_next;
                    }
                    next=obj->ob_next;
                    obj->ob_next=gc.gen2;
                    gc.gen2=obj;
                    obj->ob_prev=NULL;
                    gc.gen2_n++;
                }
            }
            obj=next;
            continue;
        }
        goaround:
        obj=obj->ob_next;
    }

    if (gen==1){
        gc.gen1_n=0;
    }
    else if (gen==2){
        gc.gen2_n=0;
    }
    else{
        gc.gen0_n=0;
    }
}