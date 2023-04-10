#include <detpic32.h>
#define msVal 20000

void delay (unsigned int);

void send2displays(unsigned char value) {
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;

    int digit_low = value & 0x0F;
    int digit_high = value >> 4;

    if (displayFlag == 0) {
        LATDbits.LATD5 = 1; 
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
        displayFlag = 1;
    } else {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
        displayFlag = 0;
    }
}

unsigned char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}


int main (void) {

    TRISB = TRISB & 0X80FF; // 1000 0000 1111 1111
    TRISD = TRISD & 0XFF9F; // 1111 1111 1001 1111
    int counter;
    int i;
    while(1) {
        i = 0;
        do {
            send2displays(toBcd(counter));
            //wait 500ms
            delay(10);
        } while(++i < 50);
        
        if(PORTBbits.RB0 == 1) {
            counter++;
            if(counter == 60) {
                counter = 0;
            }
        } else {
            if(counter == 0) {
                counter = 60;
            }
            counter--;
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