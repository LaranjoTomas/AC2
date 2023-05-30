#include <detpic32.h>

volatile int duty_cycle = 50;

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main(void) {

    T3CONbits.TCKPS = 0;                // 20*10^6 / 65536 * 1000 = 0,3
    PR3 = 19999;                          // Fout = (20MHz / (1 * 1000)) - 1 =   20000
    TMR3 = 0;                           // Reset timer T3 count register
    T3CONbits.TON = 1;                  // Enable timer T3 (must be the last command of the timer configuration sequence)
    
    // Configurar OC3 PWM
    OC3CONbits.OCM = 6;                 
    OC3CONbits.OCTSEL = 1;              // Use timer T3 as the time base for PWM generation
    OC3RS = (20000000 * duty_cycle) / 100;                 
    OC3CONbits.ON = 1;                 

    // Configurar ADC
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4= 0;
    AD1CON1bits.SSRC = 7; 
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 3;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    EnableInterrupts();

    while(1) {
        AD1CON1bits.ASAM = 1;
        while(IFS1bits.AD1IF == 0);
        int i, soma = 0;
        int *p = (int *) (&ADC1BUF0);
        for(i = 0; i < 4; i++) {
            soma += p[i * 4];
        }
        soma = soma / 4;

        duty_cycle = (soma * 100 + 511) / 1023;

        if(dutycycle >= 0 && duty_cycle <= 100) {
            OC3RS = (20000000 * duty_cycle) / 100;  
        }

        IFS1bits.AD1IF = 0;
        delay(100);
    }
    return 0;

}

