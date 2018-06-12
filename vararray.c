#define CHUNK 50

#include "vararray.h"

struct vararray_t {
    void ** array;
    unsigned int count;
    unsigned int open;
};

static void room_for_more(vararray_handle h){
    if (h->open==0){
        h->array = realloc(h->array,(h->count+CHUNK)*sizeof(void *));
        h->open+=CHUNK;
    }
}

vararray_handle vararray_create(){
    vararray_handle h = (vararray_handle) malloc(sizeof(struct vararray_t));;
    h->count=0;
    h->open=0;
    h->array = NULL;
    return h;
}

void vararray_clear(vararray_handle h){
    h->open += h->count;
    h->count=0;
    return;
}

unsigned int vararray_size(vararray_handle h){
    return h->count;
}

void vararray_push_back(vararray_handle h, void * element){
    room_for_more(h);
    assert(h->open>0);
    h->array[h->count] = element;
    h->count++;
    h->open--;
    assert(h->open>=0);
    return;
}

void * vararray_get(vararray_handle h, unsigned int idx){
    assert(idx < h->count);
    return h->array[idx];
}

void vararray_destroy(vararray_handle h){
    free(h->array);
    free(h);
    return;
}





