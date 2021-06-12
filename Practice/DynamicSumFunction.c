#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int sum(int, ...);
int sum2(int, ...);
int main()
{
    printf("the sum is : %d\n",sum(5,4,2,15,214,4));
    printf("the sum2 is : %d\n",sum2(4,2,15,214,4,-99));
    return 0;
}

int sum(int nov,...) // nov = number of values
{
    int result = 0;
    va_list p;
    va_start(p,nov);
    while(nov--)
    {
        result +=va_arg(p,int);
    }
    va_end(p);
    return result;
}

int sum2(int nov,...) // nov = number of values
{
    int result = 0;
    va_list p;
    va_start(p,nov);
    while(*p != -99)
    {
        result +=va_arg(p,int);
    }
    va_end(p);
    return result+nov;
}

