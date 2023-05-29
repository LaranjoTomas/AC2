#include <detpic32.h>

volatile unsigned char intervalo;

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}

void send2displays(unsigned char value) {

    static const char display7codes[] = {0x3F, 0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
    static int flag = 0;

    if (flag == 0){
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x80FF) | (display7codes[value & 0x0F] << 8);
    } else { 
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x80FF) | (display7codes[value >> 4] << 8);
    }
    flag = !flag;
}

int main(void) {

    TRISD = TRISD & 0xFF9F;
    TRISB = TRISB & 0x80FF;
    
    T2CONbits.TCKPS = 2; // 20*10^6 / 65536 * 120 =  2,54
    PR2 = 41665; // (20MHZ/(4 * 120Hz)) - 1 = 41666
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the
    // timer configuration sequence)

    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag

    AD1PCFGbits.PCFG4 = 0; // RB4 configured as analog input
    AD1CON1bits.SSRC  = 7; // Conversion trigger selection bits: in this
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 1; // Interrupt is generated after N samples
    AD1CHSbits.CH0SA = 4; // replace 4 by the desired input
    AD1CON1bits.ON = 1; // Enable A/D converter

    EnableInterrupts(); // Macro defined in "detpic32.h"

    while(1) {

        AD1CON1bits.ASAM = 1; // Start conversion 
        while( IFS1bits.AD1IF == 0 ); // Wait while conversion not done

        int soma = 0;
        int i, average;
        int *p = (int *) (&ADC1BUF0);
        for (i = 0; i < 2; i++) {
            soma += p[i*4];
        }
        average = soma / 2;

        intervalo =((average * 50 +511) / 1023) + 15;

        delay(100);

        IFS1bits.AD1IF == 0;
    }
    return 0;
}

void _int_(8) isr_T2(void) {
    send2displays(toBcd(intervalo));
    IFS0bits.T2IF = 0;
}
