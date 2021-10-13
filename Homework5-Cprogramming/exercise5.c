#include <stdio.h>
#include <stdint.h>
uint32_t x,n,a,b,c;
int main(int argc, char const *argv[])
{
    n=2;
    x=0x0000;
    a=x|(1<<(n+1))|(1<<(n+4));
    printf("a: %x\n",a);
    x=0xFFFF;
    b=x&~(1<<(n-1))&~(1<<(n+1));
    printf("b: %x\n",b);
    x=0xFFFF;
    x=x^(1<<(n+3))^(1<<(n+4));
    printf("c: %x\n",x);

    /* code */
    return 0;
}
