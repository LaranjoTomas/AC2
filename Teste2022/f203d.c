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

        
        p = (ADC1BUF0*8)/1024;

        LATB = (LATB & 0x80FF) | 0x2000; // F1
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x0100; // A1
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x0200; // B1
        delay(delays[p]);
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | 0x1000; // E2
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x0800; // D2
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x0400; // C2
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x0200; // B2
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x0100; // A2
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x2000; // F2
        delay(delays[p]);
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | 0x0400; // C1
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x0800; // D1
        delay(delays[p]);
        LATB = (LATB & 0x80FF) | 0x1000; // E1
        delay(delays[p]);
        IFS1bits.AD1IF = 0; // Reset AD1IF flag
    }
    return 0;
}

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < msVal * ms);
}

