#include <stdio.h>
#include <stdlib.h>

/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function does the standard realloc progress
 */
void * reallocate(void * oldPointer, size_t newSize)
{
    void *realloc_res;

    realloc_res  = realloc(oldPointer,newSize);
    if(!realloc_res)
    {
        fprintf(stderr,"realloc failed\n");
        exit(1);
    }
    return realloc_res;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function does the standard malloc progress
 */
void * mallocate(size_t newSize)
{
    void * newPointer;
    newPointer = malloc(newSize) ;
    if(!newPointer)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    return newPointer;
}
/*--------------------------------------------------------------------------------------------------------------------*/



