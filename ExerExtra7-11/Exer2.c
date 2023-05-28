#include <detpic32.h>

volatile int count;

void delay (unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

unsigned int toBcd(unsigned int value) {
    return ((value / 10) << 4) + (value % 10);
}

void send2displays(unsigned char value) {
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;

    int digit_low = value & 0x0F;
    int digit_high = value >> 4;

    if (displayFlag == 0) {
        LATDbits.LATD5 = 1; 
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
    } else {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
    }
    displayFlag = !displayFlag;
}


int main(void) {

    // Configure Timer T1 (2 Hz with interrupts disabled)
    T1CONbits.TCKPS = 5; // / 20*10^6 / 65536 * 10 = 30
    PR1 = 62499; // Fout = 20MHz / (32) =  625000 / 10 = 62500
    TMR1 = 0; // Reset timer T1 count register
    T1CONbits.TON = 1; // Enable timer T1 (must be the last command of the timer configuration sequence)
    
    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T2 interrupts

    // Configure Timer T2 (2 Hz with interrupts disabled)
    T2CONbits.TCKPS = 3; // / 20*10^6 / 65536 * 50 = 6
    PR2 = 49999; // Fout = 20MHz / (8) =  5000000 / 50 = 50000
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the timer configuration sequence)

    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts

    TRISD = 0xFF9F;
    TRISB = 0x80FF;

    EnableInterrupts();

    while(1) {
        printInt(count, 16 | 2 << 16);
        putChar('\r');
    }
    return 0;
}

void _int_(4) isr_T1(void) {
    
    if (count == 99) {
        count = 0;
    } else {
        count++;
    }
    IFS0bits.T1IF = 0; // Reset T2 interrupt flag
}

void _int_(8) isr_T2(void) {
    send2displays(toBcd(count));
    IFS0bits.T2IF = 0; // Reset T2 interrupt flag
}