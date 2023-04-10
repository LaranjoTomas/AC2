#include <detpic32.h>
#define msVal 20000

void delay (unsigned int);

int main (void) {

        TRISB = 0x0004; // RB2 como entrada
        TRISE = 0xFFC0; // RE5 a RE0 como saida
        LATE = 0X0001;


        while (1) {
                if (PORTBbits.RB2 == 0) {
                if (LATE == 0x0020) {
                        LATE = 0X0001;
                        delay(333);
                } else {
                        LATE = LATE << 1;
                        delay(142);
                }
                } else {
                if (LATE == 0x0020) {
                        LATE=0X0001;
                        delay(142);
                } else {
                        LATE = LATE << 1;
                }
                }
        }
        return 0;
}

void delay (unsigned int ms) {
        resetCoreTimer();
        while (readCoreTimer() < msVal * ms);
}