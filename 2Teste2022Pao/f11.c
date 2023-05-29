#include <detpic32.h>

volatile int duty_cycle = 25;

void delay(unsigned int ms ) {

    resetCoreTimer();
    while(readCoreTimer() < 2000 * ms); 
}

int main(void){

    TRISB = TRISB | 0x0009; // 1001

    T2CONbits.TCKPS = 2; // 20*10^6 / 65536 * 4000 
    PR2 = 33332; // Fout = 20MHz / 1 = / 4000 = 5000
    TMR2 = 0; // Reset timer T2 count register
    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the timer configuration sequence)

    OC2CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC2CONbits.OCTSEL = 0;// Use timer T3 as the time base for PWM generation
    OC2RS = ((PR2 + 1) * duty_cycle) / 100; // ((PR2+1)*DUTY_CYCLE) / 100 
    OC2CONbits.ON = 1; // Enable OC1 module
    
    EnableInterrupts();

    while(1) {

        if((PORTB & 0x000F) == 0x0001) {
            duty_cycle = 25;
        } else if ((PORTB & 0x000F) == 0x0008) {
            duty_cycle = 70;
        }
        printInt10(duty_cycle);
        delay(250);
    }

    return 0;
}

void _int_(8) isr_T2(void) {

    IFS0bits.T2IF = 0;
}