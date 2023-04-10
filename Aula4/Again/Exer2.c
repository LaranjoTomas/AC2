#include <detpic32.h>

#define msVal 20000

void delay(unsigned int);

int main(void) {

    TRISE = TRISE & 0xFF87; // FFC3 = 1111 1111 1000 0111
    
    int counter = 0;
    while(1) {
        
        delay(250);

        LATE = (LATE & 0xFF87) | (counter << 2);
        printInt(counter, 10 | 1 << 16);
        if (counter == 9) {
            counter = 0;
        }
        counter++;
    };
    return 0;
}

void delay (unsigned int ms) {
    unsigned int wait;

    resetCoreTimer();
    wait = msVal * ms;
    while (readCoreTimer() < wait);
}