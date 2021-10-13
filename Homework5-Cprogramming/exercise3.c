#include <stdio.h>
#include <stdint.h>
#define REM(a,b) (a%b)
uint16_t result;
uint16_t x =2;
int main(int argc, char const *argv[])
{
    printf("\n Problema 3");
    result = REM(5,2);
    printf("\n Inciso a: %x", result);
    result = REM(5+2,x);
    printf("\n Inciso b: %x", result);
    result = REM(5,x-2);
    printf("\n Inciso c: %x", result);
    return 0;
}
