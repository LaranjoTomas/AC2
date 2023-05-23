#include <detpic32.h>

int main(void) {
    // Configure Timer T1 (2 Hz with interrupts disabled)
    T1CONbits.TCKPS = 6; // 20*10^6 / 65536 *5 = 61
    PR1 = 62499; // Fout = (20MHz / 64) = 312500 / 5 = 62500 Hz
    TMR1 = 0; // Clear timer T3 count register
    T1CONbits.TON = 1; // Enable timer T3 (must be the last command of the timer configuration sequence)

    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T1 interrupts

    // Configure Timer T3 (2 Hz with interrupts disabled)
    T3CONbits.TCKPS = 4; // 20*10^6 / 65536 *25 = 12
    PR3 = 49999; // Fout = (20MHz / 16) = 1.250.000 / 25 =  50000Hz
    TMR3 = 0; // Clear timer T3 count register
    T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the timer configuration sequence)

    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts
    
    // Reset T1IF and T3IF flag
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt fla


    
    LATD = LATD && 0xFFFA; // RD0 & RD2 = 0
    

    TRISE = TRISE && 0xFFF5; // RE1 & RE3 = 0
    LATE = LATE && 0x0000; 
    EnableInterrupts(); // Global Interrupt Enable

    while(1);
    return 0;
}

void _int_(4) isr_T1(void) {
    putChar('1');
    // toggle RD0
    LATD = LATD && 0x0001;
    // toggle RE1
    LATEbits.LATE1 = !LATEbits.LATE1;
    // Reset T1IF
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag
}

void _int_(12) isr_T3(void) { // Replace VECTOR by the timer T3 vector number
    putChar('3');
    // toggle RD2
    LATD = LATD && 0x0004;
    // toggle RE3
    LATEbits.LATE3 = !LATEbits.LATE3;
    // Reset T3IF
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag
}