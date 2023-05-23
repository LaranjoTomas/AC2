#include <detpic32.h>

int main(void)
{
    // Configure ports, Timer T2, interrupts and external interrupt INT1
    T2CONbits.TCKPS = 7; // 20*10^6 / 65536 * 3 = 101
    PR2 = 26040; // Fout = (20MHz / 256) = 78125 / 3 = 26041 Hz
    TMR2 = 0; // Clear timer T3 count register
    T2CONbits.TON = 1; // Enable timer T3 (must be the last command of the timer configuration sequence)

    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts

    IPC1bits.INT1IP = 3; // Interrupt priority (must be in range [1..6])
    IEC0bits.INT1IE = 1; // Enable timer int1 interrupts

    EnableInterrupts();
    while(1);
    return 0;
}

void _int_(8) isr_T2(void){
    putChar('a');
    IFS0bits.T2IF = 0;
}

void _int_(7) isr_INT1(void){
    putChar('b');
    IFS0bits.INT1IF = 0;
}