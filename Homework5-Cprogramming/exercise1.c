#include <stdio.h>
#include <stdint.h>
uint32_t z = 0xAABBCCDD;

int main(int argc, char const *argv[])
{
    /* code */
    uint16_t newbit = (z & 0xFF00)>> 8 ;

    printf("value of NewBit: %X [%x]\n",newbit,newbit);
    return 0;
}
