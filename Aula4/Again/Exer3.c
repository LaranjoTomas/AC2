#include <detpic32.h>
#define msVal 20000

void delay(unsigned int);


int main (void) {
    
    TRISB = TRISB & 0x80FF; // 1000 0000 1111 1111
    TRISD = TRISD & 0xFF9F; // 1111 1111 1001 1111

    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;

    char c;
    
    while(1) {
        c = getChar();
        if(c >= 'a' || c <= 'g') {
            LATB = (LATB & 0x80FF) | (1 << (8 + (c - 'a')));
            putChar(c);
        } else if (c >= 'A' || c <= 'G') {
            LATB = (LATB & 0x80FF) | (1 << (8 + (c - 'A')));
            putChar(c);
        }
    }
    return 0;
}

void delay (unsigned int ms) {
    unsigned int wait;

    resetCoreTimer();
    wait = msVal * ms;
    while (readCoreTimer() < wait);
}