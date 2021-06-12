#include <stdio.h>
#include <stdlib.h>

#define message(a,b) \
    printf("message from " #a " to " #b "\n" ); // # = "___" make a or b string

#define tokenprn(num) \
    printf("token" #num " = %d\n",token##num); // ## put the direct input of  a or b.

#define prn(num)\
    printf("%d\n", num); // print the num value.
/*
*macro that check for even numbers and send
* which and how many it found to a print macro.
*/
#define check(arr,size) \
    {   int i;\
        int res = 0;\
        for(i = 0 ; i < size ; i++)\
        {\
           if(arr[i]%2 == 0)\
            {\
                prn(arr[i])  \
                res++; \
            }\
        }\
        prn(res)\
    }


int main()
{
    int token50 = 190;
    int arr[] = {10,25,30,35,40};

    message(Itsik,Iris)
    tokenprn(50)

    check(arr,5)
    return 0;
}
