#include <stdio.h>
#include <stdlib.h>
//#include <>

/*
fopen("what file to open", "for what purpose") // to open the file
FILE * //pointer for the file
example to use: FILE * fd = fopen(...)
we need to check fd like this
if(!fd) {puts()error; exit(0);}

when we finish we need to close with fclose(fd)
to release file and let the op to know the file isn't in use anymore.



output functions (write to flies)

fprinf
fputc
fputs
fwrite

intput functions(read from files)

fscaf
fgetc
fgets
fread
*/
int main()
{
    FILE *fd; // pointer to work with the file
    int ch; //to get chars from the files
    fpos_t p1; // fpos_t to mark\save positions in the file
    long d1; // long integer
    char buf[100];

    if(!(fd = fopen("text1.txt","r+"))) // open text 1 to read and write(only if file exist)
        //r+ point the FILE * to the beginning of the text file.
    {
        fprintf(stderr,"cannot open file\n");// same as print f just to print as error
        //when we print something to the standard error output, we can go to the errors files later and see all errors we made.
        //but only if we have changed the default of the location and it's not the screen.
        exit(0);
    }

//------------------------------------------------------------------------
//read the file, char by char, and print each char and its ascii code
while(!feof(fd)) // while we aren't at the end of the file
{
    ch = fgetc(fd);// get the char to ch
    printf("%c:%d, ",ch,ch);// char:ascii1
}
//------------------------------------------------------------------------
rewind(fd); // return fd to the beginning of the file
//------------------------------------------------------------------------
//fsetpos() and fgetpos() examples
puts("\n--------------------------------------------------------------------------");
while((ch = fgetc(fd)) != '\n') // get char and check if it isn't enter
    ; // ch will get \n and the the pointer then will point to the beginning of the new line

fgetpos(fd,&p1); // save the current position (second line)

do
{
    ch = fgetc(fd); // get the char
    putchar(ch); // print it
}
    while(ch!='\n') // stop when it is enter
    ;

fsetpos(fd,&p1); // return to the second line
do // again
{
    ch=fgetc(fd);
    putchar(ch);
}
while(ch != '\n') // stop when it is enter
    ;
// after all of this we will be at the beginning of the third line
//------------------------------------------------------------------------
// ftell() and fseek() examples
d1 = (ftell(fd)); // the distance in bytes from the beginning of the file to the current position
rewind(fd); // return to the beginning of the file
puts("\n--------------------------------------------------------------------------");
// print the first 2 chars of the first line
ch = fgetc(fd);
putchar(ch);
ch = fgetc(fd);
putchar(ch);
puts("\n--------------------------------------------------------------------------");
fseek(fd,d1,SEEK_SET) ;// fd will move to new position, d1 the distance to move(in bytes) , SEEK_SET where to jump from
//SEET_SET 0 jump from the beginning of the file
//SEEK_CUR 1 jump from current position
//SEEK_END jump from the end of the file
//fseek() got us to the beginning of the third line
//print 2 chars of the 3rd line
ch = fgetc(fd);
putchar(ch);
ch = fgetc(fd);
putchar(ch);
puts("\n--------------------------------------------------------------------------");
//------------------------------------------------------------------------
//fgets() and fputs() examples
//fgets (readStringTo[],chars to read limit(read -1 cuz of saving place for '\0',pointer of the file)) get the string to a string value,
//will stop if EOF\enter\chars to read limit
rewind(fd); // return to the beginning of the file
fgets(buf,90,fd);// read from fd, read to buf, read 90(89) char for the most
puts(buf); // there will be 2 \n cuz puts automatically use \n and fgets get the \n as well then it stops
// now we at the beginning of the second line
puts("\n--------------------------------------------------------------------------");
fseek(fd,0,SEEK_END); // jump to the end of the file
fputs(buf,fd) ;// add the first line to the end of the file, creating a 4th line
rewind(fd); // return to the beginning of the file
puts("\n--------------------------------------------------------------------------");
while(!feof(fd)) // check if it isn't the end of the file
    putchar(ch=fgetc(fd)); // print the whole text
//------------------------------------------------------------------------
 // at the end of the code, when we work with files, we always close the file.

fclose(fd);
    return 0;
}
