#include <detpic32.h>
int main(void)
{
    // Configure all (digital I/O, analog input, A/D module)
    TRISBbits.TRISB4 = 1; // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0; // RB4 configured as analog input (AN
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
    
    AD1CON2bits.SMPI = 0;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;
    
    IPC6bits.AD1IP = 2;
    IFS1bits.AD1IF = 0; 
    IEC1bits.AD1IE = 1;

    EnableInterrupts(); // Global Interrupt Enable
    // Start A/D conversion
    AD1CON1bits.ASAM = 1; // Start conversion
    
    while(1); // all activity is done by the IS
    return 0;
}

// Interrupt Handler
void _int_(27) isr_adc(void) // Replace VECTOR by the A/D vector
// number - see "PIC32 family data
// sheet" (pages 74-76)
{
    // Read conversion result (ADC1BUF0) and print it
    printInt(ADC1BUF0, 16 | 3 << 10);
    putChar(' ');
    // Start A/D conversion
    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
    IFS1bits.AD1IF = 0; // Reset AD1IF flag
}