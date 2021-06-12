#include <stdio.h>
#include <stdlib.h>

/*
Binary files.



fwrite(address of the beginning of data in the memory , size of byes of one data unit, number of data units to write, to where - which file)
*/
int main()
{
    /*
    // example of how '0' '0' '65' '66' which is 16961 will be shown in binary.
    int x = 16961;
    unsigned int mask =1; //mask is used to make & operator with the int
    mask <<= 31; // 10000000  00000000  00000000  00000000
    int  i ;
    for(i =0 ; i < sizeof(int)*8 ; i++)
    {
        if(i%8 == 0) putchar('\t');
       (x&mask) ? printf("1") : printf("0");
        mask >>=1;
    }

 printf("\n%d\n",x);
 fwrite(&x,sizeof(int),1,stdout); //print every byte separately(8 bits instead of the whole 4 byte together(32 bits)
                                                                //so we get the memory image.
*/

FILE *fd1 , *fd2;
int buff[5] = {16961,-25,780,4680,12};
int buff2[100],buff3[100];
int i ,n_read;

if (!(fd1 = fopen("numbers1.txt", "w") ))// open a file with fopen "write"
   {
        fprintf(stderr, " cannot create file\n");
        exit(0);
   }
if(!(fd2 = fopen("numbers2.txt", "wb") ))// open a binary file with fopen "write binary"
{
    fprintf(stderr, " cannot create file\n");
    exit(0);
}
for( i = 0 ; i<5 ; i++)
    {
        fprintf(fd1, "%d\n",buff[i]); // print buff content into fd1(the text file)
    }
fwrite(buff,sizeof(int),5,fd2) ; // print with fwrite buff content into fd2)
fclose(fd1);
fclose(fd2);
if(!(fd1 = fopen("numbers1.txt", "r")) )// open a file with to read its content;
   {
        fprintf(stderr, " cannot create file\n");
        exit(0);
   }
if(!(fd2 = fopen("numbers2.txt", "rb")) )// open a file with to read its content;
{
    fprintf(stderr, " cannot create file\n");
    exit(0);
}
for(i = 0 ; i<5 ; i++)
    {
        fscanf(fd1, "%d\n",&buff2[i]); // getting the numbers from the text file into buff2
        printf("%d\n",buff2[i]);//print bufff2 to screen
    }
n_read  = fread(buff3,sizeof(int),5,fd2); // getting the numbers from the binary file to buff3
for(i = 0 ; i < n_read; i ++)
    printf("%d\n",buff3[i]);

fclose(fd1);
fclose(fd2);
    return 0;
}
