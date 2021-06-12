#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS 127

int suffix(char str[], char c);

int main()
{
    char c, str[MAX_CHARS];

    printf("please type string and char:\n");
    scanf("%c",&c);
    gets(str);
    printf("the input is :%c\t%s",c,str);
    printf("\n%d",suffix(str,c));

    return 0;
}
int suffix(char str[], char c)
{
    int i = 0,j, counter=0;
    while (str[i] != '\0')
    {
        if(c == str[i])
        {
            putchar('\n');

            for(j=i ; j < MAX_CHARS; j++ )
                {
                    if(str[j] == '\0' || str[j] == '\n')
                        break;
                    printf("%c",str[j]);
                }

            counter++;
        }
        i++;
    }
    return counter;
}
