#include <detpic32.h>

#define msVal 20000

int main(void) {
    int media, V, *p;
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
    while(1) {
        AD1CON1bits.ASAM = 1; // Start conversion
        p = (int *)(&ADC1BUF0);
        for(; p <= (int *)(&ADC1BUFF); p+=4 ) {
            //código para calcular a
            //média das 4 amostras retiradas e, a partir dessa média, determinar a amplitude da tensão
            //na entrada do conversor A/D e imprimir o seu valor usando system calls.
            //A média das 4 amostras é dada por:
            media = (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3) / 4;
            V = (((33 - 0) * (media )) / 1023 ) + 0;

            printInt(media, 10 | 4 << 16);
            putChar(' ');
            printInt(V, 16 | 3 << 16);
            putChar(' ');

        }
    }
    return 0;
}