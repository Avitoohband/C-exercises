#include <stdio.h>
#include <stdlib.h>

typedef struct node * ptr;
typedef struct node
{
    int data;
    ptr next;
}item;

void add2list(ptr *,int );
void printlist(ptr );
void freelist(ptr *);

int main()
{
    ptr h=NULL;
    int n;
    printf("enter number please\n");
    while (scanf("%d" ,&n)!=EOF)
    {
        add2list(&h,n);
    }
    printlist(h);
    printf("\n%p",h);
    freelist(&h);
    printf("\n%p",h);
    return 0;
}

void add2list(ptr *h,int n)
{
    ptr p1,p2,temp;
    temp =(ptr)malloc(sizeof(item));

    if(!temp)
    {
        printf("\nCannot allocate memory\n");
        exit(0);
    }
    temp->data = n; // first we're creating the node

    if(!(*h)) // if the list is empty
    {

        temp->next = NULL;
        *h = temp;
        return;
    }
    // if the list isn't empty
    p1 = *h;
    while ((p1)&&(p1->data < n)) // as long as it's not the end of the list
        //and the number is smaller
        //then the current number we're looking at in the node
    {
        p2 = p1;
        p1 = p1->next;
    }
    if(p1 == *h) // if the number is the smallest in the list, we want to put it first
    {
        temp->next = p1;
        *h = temp;
        return;
    }

    temp->next = p1; // p1 is node and p10dasd2 is the one  b4 him
    p2->next = temp; // we getting the new item to be between p1 and p2
    printf("normal");
}
void printlist(ptr h)
{
    {
        while (h)
        {
            printf("\n %d" ,h->data);
            h=h->next;
        }

    }
}

void freelist(ptr *h)
{
    ptr temp;
    while(*h)
    {
        temp=*h;
        *h=temp->next;
        free(temp);
    }
}
