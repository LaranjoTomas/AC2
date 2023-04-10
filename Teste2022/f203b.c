#include <detpic32.h>
#define msVal 20000

void delay(unsigned int);

int main(void) {
    int p;
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
    
    TRISB = 0x80FF; // 1000 0000 1111 1111
    TRISD = 0xFF9F; // 1111 1111 1001 1111
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    
    int delays[] = {250, 500, 750, 1000, 1250, 1500, 1750, 2000};


    while(1) {

        AD1CON1bits.ASAM = 1; // Start conversion

        while (IFS1bits.AD1IF == 0); // Wait while conversion not done (AD1IF == 0)

        printInt(ADC1BUF0, 10 | 4 << 16); // Read conversion result (ADC1BUF0 value) and print it
        putChar('\n');

        // delay pode variar de 0,25s a 2s, com 8 niveis (0,25 , 0,5 , 0,75 , 1 , 1,25 , 1,5 , 1,75 , 2s)
        // dependendo do valor de ADC1BUF0

        p = (ADC1BUF0*8)/1024;


        LATB = (LATB & 0x80FF) | 0x2000; // F
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x0100; // A
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x0200; // B
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x4000; // G
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x1000; // E
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x0800; // D
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x0400; // C
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x4000; // G
        delay(delays[p]);
        
        IFS1bits.AD1IF = 0; // Reset AD1IF flag
    }
    return 0;
}

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < msVal * ms);
}

