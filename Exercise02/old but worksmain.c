/*
*This program will get 30 different names and storage them into an array
*then it will print 10 random names from that array
*
*Author Avi Tuchband
*Version 02-12-19
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/********* Defines and Global Variables******************/
#define NUM_OF_NAMES 30
#define MAX_NAME_LENGTH 20
char NAMES[NUM_OF_NAMES][MAX_NAME_LENGTH];
/********* Functions Prototypes *************************/
void get_name();
void fill_array();
void print_names();
int check_no_duplicates();
/************ Main Program *******************************/
int main()
{
    srand(time(NULL));
    int i;
    fill_array();
    printf ("The choosen names are :");
    print_names();
    putchar('\n');
    if (check_no_duplicates())
    {
        printf("The random names are :");
        for(i=0 ; i<10 ; i++)
        {
            get_name();
        }
    }
    else
    {
        printf("You have duplicates, the program will exit now");
    }

    return 0;
}
/************ Functions  *******************************/
/*prints a random name from the array*/
void get_name()
{
    printf("\n%s",NAMES[rand()%31]);
}
/*gets names from the user\file and put it into the array */
void fill_array()
{
    int i=0;
    printf("Hello there, Please enter 30 different names\n");

    while(i<NUM_OF_NAMES)
    {
     scanf("%s",NAMES[i]);
     if(NAMES[i][0] == '\n')
     continue;
     i++;
    }
}
/*prints all the names that was given buy the user\file*/
void print_names()
{
    int  i;
    for (i=0 ; i<NUM_OF_NAMES ; i++)
    {
        putchar('\n');
        printf("%d) %s",i+1,NAMES[i]);
    }
}
/*checks if there are duplicates names in the array, if there is , exiting the program*/
int check_no_duplicates()
{
    int i,j,k,counter;
    for(i =0 ; i<NUM_OF_NAMES-1 ; i++)
    {
        for(j=i+1 ; j<NUM_OF_NAMES ; j++)
        {
            if(strlen(NAMES[i]) == strlen(NAMES[j]))
            {
                counter = 0;
                k = 0;
                while(tolower(NAMES[i][k]) == tolower(NAMES[j][k]) && NAMES[i][k] != '\0')
                {
                    counter++;
                    k++;
                }
                if(counter == strlen(NAMES[i]))
                return 0;
            }
        }
    }
    return 1;
}
