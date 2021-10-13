#include <stdio.h>
#include <stdint.h>
uint16_t x =0xFFFF;
int main(int argc, char const *argv[])
{
    if(x & 1<<10){
        printf("true\n");
    }else{
        printf("false\n");
    }
    return 0;
}
