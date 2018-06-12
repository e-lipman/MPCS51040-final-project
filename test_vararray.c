#include<stdlib.h>
#include<stdio.h>
#include"vararray.h"

int main(){
    vararray_handle h = vararray_create();
    printf("count: %d\n", vararray_size(h));

    for (int i=0; i<102; i++){
        int * my_int = (int *) malloc(sizeof(int));
        *my_int = i;
        vararray_push_back(h, my_int);
    }

    printf("count: %d\n", vararray_size(h));

    for (int i=0; i<vararray_size(h); i++){
        printf("%d ", *((int *) vararray_get(h,i)));
        free(vararray_get(h,i));
    }
    printf("\n");

    vararray_clear(h);
    printf("count: %d\n", vararray_size(h));

    vararray_destroy(h);
    
    return EXIT_SUCCESS;
}
