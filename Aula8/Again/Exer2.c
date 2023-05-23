#include <detpic32.h>

int main(void) {
    // Configure Timer T3 (2 Hz with interrupts disabled)
    T3CONbits.TCKPS = 7; // 20*10^6 / 65536 *2 = 152
    PR3 = 39061; // Fout = (20MHz / 256) = 78125 / 2 = 39062.5 Hz
    TMR3 = 0; // Clear timer T3 count register
    T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the timer configuration sequence)

    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts

    EnableInterrupts(); // Global Interrupt Enable

    while(1);
    return 0;
}

void _int_(12) isr_T3(void) { // Replace VECTOR by the timer T3 vector number
    static int i = 0;
    i++;
    if (i == 2) {
        putChar('a');
        i = 0;
    }
    
    // Reset T3IF
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag
}