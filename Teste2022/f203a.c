#include <detpic32.h>
#define msVal 20000

void delay(unsigned int);

int main(void) {
    
    TRISB = 0x80FF; // 1000 0000 1111 1111
    TRISD = 0xFF9F; // 1111 1111 1001 1111
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    
    while(1) {
        LATB = (LATB & 0x80FF) | 0x2000; // F
        delay(250);
        LATB = (LATB & 0x80FF) | 0x0100; // A
        delay(250);
        LATB = (LATB & 0x80FF) | 0x0200; // B
        delay(250);
        LATB = (LATB & 0x80FF) | 0x4000; // G
        delay(250);
        LATB = (LATB & 0x80FF) | 0x1000; // E
        delay(250);
        LATB = (LATB & 0x80FF) | 0x0800; // D
        delay(250);
        LATB = (LATB & 0x80FF) | 0x0400; // C
        delay(250);
        LATB = (LATB & 0x80FF) | 0x4000; // G
        delay(250);
    }
    return 0;
}

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < msVal * ms);
}