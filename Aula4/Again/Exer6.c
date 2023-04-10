#include <detpic32.h>
#define msVal 20000

void delay (unsigned int);

int main(void) {

    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    
    TRISB = TRISB & 0x000F; // 0000 0000 0000 1111
    TRISD = TRISD & 0xFF9F; // 1111 1111 1001 1111
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    while(1) {
        // read dip-switch
        // convert to 7 segments code
        // send to display

        int dip = PORTB & 0x000F;
        int code = display7Scodes[dip];

        LATB = (LATB & 0x80FF) | (code << 8);
        delay(500);

    }
    return 0;
}

void delay (unsigned int ms) {
    unsigned int wait;

    resetCoreTimer();
    wait = msVal * ms;
    while (readCoreTimer() < wait);
}