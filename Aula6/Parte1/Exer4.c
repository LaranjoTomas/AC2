#include <detpic32.h>

#define msVal 20000

int main(void) {
    TRISBbits.TRISB4 = 1; // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0; // RB4 configured as analog input (AN
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
    // mode an internal counter ends sampling and
    // starts conversion
    AD1CON2bits.SMPI = 3; // Interrupt is generated after N samples
    // (replace N by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
    // This must the last command of the A/D
    // configuration sequence
    int i;
    while(1) {
        AD1CON1bits.ASAM = 1; // Start conversion
        int *p = (int *)(&ADC1BUF0);
        for(; p <= (int *)(&ADC1BUFF); p+=4 ) {
            printInt( *p, 16 | 3 << 16); // Read conversion result (ADC1BUF0 value) and print it

            putChar(' ');
        }
    }
    return 0;
}
