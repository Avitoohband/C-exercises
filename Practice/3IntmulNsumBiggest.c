#include <stdio.h>
#include <stdlib.h>

int main()
{
    int x=0,y=0,z=0,mul=0,res=0;
    scanf("%d%d%d",&x,&y,&z) ;

    mul = x*y;
    mul = (x*z > mul) ? x*z : mul;
    mul = (y*z > mul) ? y*z : mul;

    if(mul == x*y) res = mul+ z;
    if(mul == x*z) res = mul+ y;
    if(mul == y*z) res = mul+ x;

    printf("the biggest sum is %d",res);
    return 0;
}
