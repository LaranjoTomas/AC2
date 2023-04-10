#include <detpic32.h>
#define msVal 20000

void delay (unsigned int);

int main (void) {
        int i = 0;
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
        // This must the last command of the A/D
        // configuration sequence
        while(1) {
                AD1CON1bits.ASAM = 1; // Start conversion
                while (IFS1bits.AD1IF == 0); // Wait while conversion not done (AD1IF == 0)
                int media = 0;
                for(i=0; i<16; i++){
                        media = (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3) / 4;
                         
                }
                delay(200);
                printInt(media, 16 | 3 << 16);
                putChar('\r');
        }
        return 0;
}

void delay (unsigned int ms) {
        resetCoreTimer();
        while (readCoreTimer() < msVal * ms);
}