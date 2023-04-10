#include <detpic32.h>

#define msVal 20000

void delay(unsigned int);

//Implemente, em linguagem C, um contador crescente módulo 10, atualizado a uma
//frequência de 4Hz. O resultado deverá ser observando nos 4 LED ligados aos portos RE6 a
//RE3.

int main(void)
{
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE3 = 0;

    int i = 0;
    while(1)
    {
        delay(250);
        LATE = (LATE & 0x87) | (i << 3); //1000 0111
        i = (i + 1) % 10;
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