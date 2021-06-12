/*
author Avi Toohband
version 18-11-19

this program will get string of a decimal value with ints between(0-9)
and print the binary value of the the decimal number

*/
#include <stdio.h>
#define MAX_LINE 120

// functions prototypes
void translate_dec_hex(char str[]);
int sizeofint (int num);

int main()
{
    char str[MAX_LINE];
    printf("Hello there, you may type a bunch of numbers\n");
    scanf("%s",str);
    translate_dec_hex(str);
    return 0;
}

// functions

// print 0 or 1 depends on the binary value of the str;
void translate_dec_hex(char str[])
{
    int converted = atoi(str); // convert the string to int
    unsigned msk =1; // to make & operator with our converted string
    msk <<= sizeofint(converted)-1; // put 1 in msk from the left

    putchar('\n');

    while(msk)
    {
        printf("%d",(msk&converted)? 1 : 0);
        msk >>= 1; // check next bit to the right
    }
    putchar('\n');
    }
    //checking how many bits are required
    int sizeofint (int num)
    {
        int counter =0;
        unsigned msk  = 1;

        while(msk)
        {
         counter++;
         msk <<= 1;
        }
        return counter;
    }

