#include <stdio.h>
#include <stdlib.h>
#include <string.h> // to use memcpy


void swap(void *, void *, int);

int main()
{
    int x = 5 , y = 8;
    double x1= 2.5,y1=3.66;
    float x2 = 16.87, y2 = 51.88;
    char x3 = 'a' ,y3 = 'z';

    printf("int check\n");
    printf("%d %d\n",x,y);
    swap(&x,&y, sizeof(x));
    printf("%d %d\n\n",x,y);

    printf("double check\n");
    printf("%g %g\n",x1,y1);
    swap(&x1,&y1, sizeof(x1));
    printf("%g %g\n\n",x1,y1);

    printf("float check\n");
    printf("%f %f\n",x2,y2);
    swap(&x2,&y2, sizeof(x2));
    printf("%f %f\n\n",x2,y2);

    printf("char check\n");
    printf("%c %c\n",x3,y3);
    swap(&x3,&y3, sizeof(x3));
    printf("%c %c\n",x3,y3);

    return 0;
}

void swap(void *p1, void *p2, int size)
{
    char *temp = (char *)malloc(size);// the size doesn't matter because char will move only 1 byte every time so it can fit any size of bytes.
    // memcpy copying (to here,from here, how many bytes to copy)
    memcpy(temp,p1,size); //temp = p1
    memcpy(p1,p2,size); // p1 = p2
    memcpy(p2,temp,size); // p2= temp
    free(temp); // free the the pointer.
    /*
    char p;
    char *q1 = (char *)p1; // point to the given values with char casting
    char *q2 = (char *)p2;
    int i;

    for (i = 0 ; i < size ; i++)
    {
        p=q1[i];
        q1[i]=q2[i];
        q2[i] = p;

    }
    // here we don't use free because we want to keep the point(so the values remain the same) after we leave the function.
    */
}

