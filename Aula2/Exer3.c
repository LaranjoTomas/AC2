#include <detpic32.h>
#define msVal 20000

void delay(unsigned int);

int main(void) {
    int cnt1=0, cnt5=0, cnt10=0, i=0;
    
    while(1) {
        cnt1++;
        cnt5 += 5;
        cnt10 += 10;

        putChar('\r');
        printInt(cnt1, 10 | 5 << 16);
        putChar(' ');
        printInt(cnt5, 10 | 5 << 16);
        putChar(' ');
        printInt(cnt10, 10 | 5 << 16);
        putChar(' ');
        putChar('\n');
    
        delay(1000);
    }

    
    return 0;
}

void delay(unsigned int ms) {
    unsigned int wait;

    resetCoreTimer();
    wait = msVal * ms;
    while (readCoreTimer() < wait);
}