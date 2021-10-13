#include <stdio.h>
#include <stdint.h>
uint8_t x;
int main(int argc, char const *argv[])
{
    x=0b11100101;
    x=x|(1<<4);
    printf("a: %x\n",x);
    x=0b10101010;
    x=(x>>4)&0x0F;
    printf("b: %x\n",x); 
    x=0b00000000;
    x=x||0x30;
    printf("c: %x\n",x);    
    x=0b11111111;
    x=x&&0x00;
    printf("d: %x\n",x);    
    return 0; 
}
 