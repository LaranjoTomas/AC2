#include <detpic32.h>

void delay(unsigned int ms){
    resetCoreTimer();
    while(readCoreTimer() < 20000*ms);
}

int main(void) {

    OC3CONbits.OCM = 6; // PWM mode on OC3; fault pin disabled
    OC3CONbits.OCTSEL = 1;// Use timer T3 as the time base for PWM generation
    OC3RS = 20000; // Ton constant
    OC3CONbits.ON = 1; // Enable OC3 module

    OC5CONbits.OCM = 6; // PWM mode on OC5; fault pin disabled
    OC5CONbits.OCTSEL = 1;// Use timer T3 as the time base for PWM generation
    OC5RS = 20000; // Ton constant
    OC5CONbits.ON = 1; // Enable OC5 module

    TRISBbits.TRISB4 = 1; // RB4 digital output disconnected
    AD1PCFGbits.PCFG4= 0; // RB4 configured as analog input
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
    // mode an internal counter ends sampling and
    // starts conversion
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
    // interrupt is generated. At the same time,
    // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = N-1; // Interrupt is generated after N samples
    // (replace N by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; 
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
    // This must the last command of the A/D
    // configuration sequence 

    T3CONbits.TCKPS = 0; // 20*10^6 / 65536 * 1000 = 0,3
    PR3 = 19999; // Fout = 20MHz /1 = / 1000  =  20000
    TMR3 = 0; // Clear timer T3 count register
    T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the
    // timer configuration sequence)

    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts
    IFS0bits.T3IF = 0; // Reset T3IF flag

    unsigned int value;

    while(1) {
        AD1CON1bits.ASAM = 1; // Start conversion
        while(IFS1bits.AD1IF == 0); // Wait while conversion not done (AD1IF == 0)
        value = (100 * ADC1BUF0 + 511) / 1023;
        setPWM(value);
        IFS1bits.AD1IF = 0; // Reset AD1IF
        delay(1000); // Wait 1 second
    }
    return 0;

}

void setPWM(unsigned int dutyCycle)
{
    // duty_cycle must be in the range [0, 100]
    if (dutyCycle >= 0 && dutyCycle <= 100)
    {
        OC3RS = ((PR3 + 1) * dutyCycle) / 100;
        OC5RS = 200 * ((((PR3 +1 ) * (100 - dutyCycle)) / 100));
    }
} 

void _int_(12) isr_T3(void) {

    IFS0bits.T3IF = 0; // Reset T3IF flag
}