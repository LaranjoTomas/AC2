#include <detpic32.h>
#define msVal 20000

void delay (unsigned int);

int main (void) {

    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
    unsigned char segment;
    int i = 0;
    TRISB = TRISB & 0x80FF; // 1000 0000 1111 1111
    TRISD = TRISD & 0xFF9F; // 1111 1111 1001 1111
    while(1) {
        segment = 1;
        for(i=0; i< 7; i++) {
            LATB = (LATB & 0x80FF) | (segment << 8);
            delay(500);
            segment = segment << 1;
        }
        LATDbits.LATD5 = !LATDbits.LATD5;
        LATDbits.LATD6 = !LATDbits.LATD6;
    }
    return 0;
}

void delay (unsigned int ms) {
    unsigned int wait;

    resetCoreTimer();
    wait = msVal * ms;
    while (readCoreTimer() < wait);
}
