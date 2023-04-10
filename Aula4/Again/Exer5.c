#include <detpic32.h>
#define msVal 20000

void delay (unsigned int);

int main (void) {

    unsigned char segment;
    int i = 0;
    TRISB = TRISB & 0x80FF; // 1000 0000 1111 1111
    TRISD = TRISD & 0xFF9F; // 1111 1111 1001 1111
    LATD = LATD & 0xFFDF;   // 1111 1111 1101 1111

    while(1) {
        segment = 1;
        for(i=0; i< 7; i++) {
            LATB = (LATB & 0x80FF) | (segment << 8);
            
            LATD = (LATD & 0x80FF) | 0x0020;

            delay(500);
            segment = segment << 1;
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
