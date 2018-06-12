#include "spatial_index.h"
#include<stdio.h>
#include<stdlib.h>

typedef struct element_holder_t {
    void * element;
    struct element_holder_t * next;
} element_holder_t;

typedef struct spatial_index_t{
    si_intersects_func intersects_func;
    si_bounding_box bounding_box;

    unsigned int count;
    element_holder_t * first;
    element_holder_t * last;
} spatial_index_t;

si_handle si_create(si_intersects_func contains, si_bounding_box box,
                    void * userdata){
    //create and initialize spatial index
    si_handle h = (si_handle) malloc (sizeof(spatial_index_t));
    h->intersects_func = contains;
    h->bounding_box = box;
    
    h->count = 0;
    h->first = NULL;
    h->last = NULL;

    return h;
}

bool si_walk(const_si_handle si, si_visit visitfunc, void * userdata){
    element_holder_t * current = si->first;
    while (current!=NULL){
        if (visitfunc(current->element,userdata)==false){
            return false;
        }
        current = current->next;
    }
    return true;
}

void si_insert(si_handle si, void * element){
    //create new element holder
    element_holder_t * new_holder = (element_holder_t *) malloc (sizeof(element_holder_t));
    new_holder->element = element;
    new_holder->next = NULL;
    
    //link into list
    if (si->count==0){
        si->first = new_holder;
        si->last = new_holder;
    }
    else {
        si->last->next = new_holder; //old last points to newest
        si->last = new_holder; //last in is now newest
    }
    si->count++;
}

bool si_remove(si_handle si, void * element){
    bool found = false;
    element_holder_t * prev = NULL;
    element_holder_t * current = si->first;
    while (current!=NULL && found == false){
        if (current->element == element){
            found = true;
        }
        else {
            prev = current;
            current = current->next;
        }
    }
    
    if (found==true){
        //if not current not first
        if (prev!=NULL){
            prev->next = current->next;
            //if current last
            if (prev->next == NULL){
                si->last=prev;
            }
        }
        //current is first
        else {
            si->first=current->next;
        }
        free(current);
        
        //update si struct
        si->count--;
    }
    return found;
}

unsigned int si_size(const_si_handle si){
    return si->count;
}

bool si_bbox(const_si_handle si, rectangle_t * rect){
    if (si->count==0){
        return false;
    }
    element_holder_t * current = si->first;
    rectangle_t r[si->count];
    for (int i=0; i<(si->count);i++){
        si->bounding_box(current->element, &r[i] ,NULL);
        current = current->next;
    }

    *rect = rectangle_combine_array(r, si->count);
    return true;
}

unsigned int si_intersect(const_si_handle si, const rectangle_t * rect,
                    si_visit visit_func,void * userdata){
    element_holder_t * current = si->first;
    while (current!=NULL){
        if (si->intersects_func(rect,current->element,NULL)){
            if (visit_func(current->element,userdata)==false){
                return false;
            }
        }
        current = current->next;
    }
    return true;
}

void si_destroy(si_handle si){
    element_holder_t * prev = NULL;
    element_holder_t * current = si->first;
    while (current!=NULL){
        prev = current;
        current = current->next;
        free(prev);
    }
    free(si);
}
