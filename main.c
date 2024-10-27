#include <stdio.h>
#include <stdint.h>

int main(){
    uint8_t a = 0b00000100;
    uint8_t b = 0b11111010;

    uint16_t c = a + ~b;

    printf("%x\n", b);
    printf("%x\n", ~b);

    printf("%x\n", c);
    printf("%d\n", c);

    return 0;    
}