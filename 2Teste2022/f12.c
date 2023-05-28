#include <detpic32.h>

volatile unsigned int N = 4,
volatile unsigned char average2 = 0;


void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms );
}

void send2displays(unsigned char value) {
    static int displayFlag=0;
    static const char display7cods[] = {0x3F, 0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

    if (displayFlag = 0) {
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x80FF) | (display7codes[value & 0x0F] << 8);
    } else {
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x80FF) | (display7codes[value >> 4] << 8);
    }
    displayFlag = !displayFlag;
}

int main(void) {

    TRISE = TRISE & 0xFF00;
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;

    T2CONbits.TCKPS = 2; // 20*10^6 / 65536 / 100 =  3
    PR2 = 49999; // Fout = 20MHz / 4 = / 100 = 50000
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the
    // timer configuration sequence)

    IPC2bits.T2IP = 2; // prioridade da interrupção (1 a 6)
    IFS0bits.T2IF = 0; // limpar pedido de interrupção do temporizador 2
    IEC0bits.T2IE = 1; 

    AD1PCFGbits.PCFG4 = 0; // RBx configured as analog input
    AD1CON1bits.SSRC  = 7; // Conversion trigger selection bits: in this
    // mode an internal counter ends sampling and
    // starts conversion
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
    // interrupt is generated. At the same time,
    // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = N-1; // Interrupt is generated after N samples
    // (replace N by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace 4 by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
    // This must the last command of the A/D
    // configuration sequence

    IFS1bits.AD1IF = 0; // clear A/D interrupt flag 
    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
    IEC1bits.AD1IE = 1; // enable A/D interrupts 

    EnableInterrupts();

    while(1) {
        AD1CON1bits.ASAM = 1; // Start conversion 
        while( IFS1bits.AD1IF == 0 ); // Wait while conversion not done
    }
    return 0;
}

void _int_(27) isr_adc(void){

    int soma = 0;
    int i;
    int *p = (int *) (&ADC1BUF0);
    for (i = 0; i < 4; i++){
        soma += p[i*4];
    }
    int average = soma / 4;

    average = (average * 8 + 511 ) / 1023;

    average2 = (average * 16 + 511 ) / 1023;

    if ((average >= 1) & (average <= 8)) {
        if (average == 1) {
            LATE = (LATE & 0xFF00) | 0x0001;
        } else if (average == 2) {
            LATE = (LATE & 0xFF00) | 0x0002;
        } else if (average == 3) {
            LATE = (LATE & 0xFF00) | 0x0004;
        } else if (average == 4) {
            LATE = (LATE & 0xFF00) | 0x0008;
        } else if (average == 5) {
            LATE = (LATE & 0xFF00) | 0x0010;
        } else if (average == 6) {
            LATE = (LATE & 0xFF00) | 0x0020;
        } else if (average == 7) {
            LATE = (LATE & 0xFF00) | 0x0040;
        } else if (average == 8) {
            LATE = (LATE & 0xFF00) | 0x0080;
        } else  {
            LATE = (LATE & 0xFF00) | 0x0000;
        }
    }
    AD1CON1bits.ASAM = 1; // Start conversion 
    IFS1bits.AD1IF == 0
}

void _int_(8) isr_T2(void) {
    send2displays(average2);
    IFS0bits.T2IF = 0;
}