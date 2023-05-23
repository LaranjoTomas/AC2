#include <detpic32.h>
#define msVal 20000

void delay(unsigned int);

int main(void) {
    TRISE = TRISE & 0xFFFD;
    LATE = LATE & 0X0000;

    PORTD = PORTD & 0X0100;

    while(1) {
        if(PORTDbits.RD8 == 0) {
            LATEbits.LATE1 = !LATEbits.LATE1;
            delay(3000);
        }
        LATEbits.LATE1 = !LATEbits.LATE1;
    }
    return 0;
}

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < msVal * ms);
}