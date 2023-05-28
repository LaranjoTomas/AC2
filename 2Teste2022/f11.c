#include <detpic32.h>

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms );
}

int main(void) {
    int count;
    TRISB = TRISB & 0x000F; // 0000 0000 0000 1111
    TRISE = TRISE & 0xFF00; // 1111 1111 0000 0000

    count = (!PORTB & 0x000F); // (!(1010) = 0101) & 1111 = 0101
    LATE = (LATE & 0xFF00) | count; 

    while(1) {
        if ( count == 0b11111111) {
            count = (!PORTB & 0x000F);
        } 
        count = (count << 1) + 1;
        LATE = (LATE & 0xFF00) | count;
        delay(1000); // 1 hz
    }
    return 0;
}