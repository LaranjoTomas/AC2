#include <detpic32.h>
#define msVal 20000

void delay(unsigned int);

int main(void) {
    volatile int aux;

    TRISBbits.TRISB4 = 1; // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0; // RB4 configured as analog input (AN
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
    // mode an internal counter ends sampling and
    // starts conversion
    AD1CON2bits.SMPI = 1; // Interrupt is generated after N samples
    // (replace N by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter

    while(1) {
        AD1CON1bits.ASAM = 1; // Start conversion
        LATDbits.LATD11 = 1;

        while (IFS1bits.AD1IF == 0); // Wait while conversion not done (AD1IF == 0)

        LATDbits.LATD11 = 0;

        aux = (ADC1BUF0, 16 | 3 << 16); // Read conversion result (ADC1BUF0 value) and store it in aux
        printInt(ADC1BUF0, 16 | 3 << 16); // Print aux
        putChar('\n');
        IFS1bits.AD1IF = 0; // Reset AD1IF flag
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