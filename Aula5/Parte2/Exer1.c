#include <detpic32.h>

#define msVal 20000

void delay(unsigned int);


unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

void send2displays(unsigned char value)
{
    static const char display7Scodes[] = {0x3F, 0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
    static char displayFlag = 0;
    // select display high
    // send digit_high (dh) to display: dh = value >> 4
    // select display low
    // send digit_low (dl) to display: dl = value & 0x0F
    if(displayFlag == 0) {
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | (display7Scodes[value & 0x0F] << 8);
        displayFlag = 1;
    } else {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | (display7Scodes[value >> 4] << 8);
        displayFlag = 0;
    }
}

int main(void)
{
    // declare variables
    // initialize ports
    TRISB = 0x80FF;
    TRISD = 0xFF9F;
    
    int counter = 0;
    while(1) {
        int i = 0;
        do {
            send2displays(toBcd(counter));
            // wait 20 ms (1/50Hz)
            delay(10);
        } while(++i < 50);
        // increment counter (mod 60)
        counter++;
        if (counter == 60) {
            counter = 0;
        }
    }
    return 0;
}


void delay (unsigned int ms)
{
    unsigned int wait;

    resetCoreTimer();
    wait = msVal * ms;
    while (readCoreTimer() < wait);
}