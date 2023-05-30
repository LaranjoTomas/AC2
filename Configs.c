void send2displays (unsigned char value) {
    static int flag = 0;
    static const char codes[] = {};

    if(flag == 0) {
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0X80FF) | ( codes[value & 0x0F] << 8);
    } else {
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0X80FF) | ( codes[value >> 4] << 8);
    }
    flat = !flag;
}

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms):
}

char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}

void putStr(char *str) {
    int i = 0;
    for(i=0; str[i] != '\0';i++) {
        putc(str[i]);
    }
}

void _int_(vector) isr

void ADC(void) {
    int i, soma = 0;
    int *p = (int *) (&ADC1BUF0);
    for(i=0; i<N;i++) {
        soma += p[i*4];
    }
    soma = soma / N;

    average = (soma * Intervalo + 511) / 1023;
}


// Intruções para UART2

U2BRG = ((20000000 + 8 * bgp) / ( 16 * bgp)) -1;

U2MODEbits.BRGH = 1;

U2MODEbits.PDSEL = 2;   // 8 bits
U2MODEbits.STSEL = 1;   // 

U2STAbits.UTXEN = 1;
U2STAbits.URXEN = 1;

U2MODEbits.ON = 1;

IPC8bits.U2IP = 1;
IEC1bits.U2RXIE = 1;
IFS1bits.U2RXIF = 0;



// instruções para timers 

T2CONbits.TCKPS = 2; // 20*10^6 / 65536 * 4000 
PR2 = 33332; // Fout = 20MHz / 1 = / 4000 = 5000
TMR2 = 0; // Reset timer T2 count register
IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
IFS0bits.T2IF = 0; // FLAG

//com interrupções
IEC0bits.T2IE = 1; // Enable timer T2 interrupts
IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag
T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the timer configuration sequence)


// insctruções para oc2
OC2CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
OC2CONbits.OCTSEL = 0;// Use timer T2 as the time base for PWM generation
OC2RS = ((PR2 + 1) * duty_cycle) / 100; // ((PR2+1)*DUTY_CYCLE) / 100 
OC2CONbits.ON = 1; // Enable OC1 module



// instruções para ADC

AD1PCFGbits.PCFG4 = 0; // RB4 configured as analog input
AD1CON1bits.SSRC  = 7; // Conversion trigger selection bits: in this
AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
AD1CON2bits.SMPI = 1; // Interrupt is generated after N samples
AD1CHSbits.CH0SA = N-1; // replace 4 by the desired input
AD1CON1bits.ON = 1; // Enable A/D converter

AD1CON1bits.ASAM = 1; // Start conversion 
IFS1bits.AD1IF == 0  // FLAG

// com interrupções 
IFS1bits.AD1IF = 0; // clear A/D interrupt flag 
IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
IEC1bits.AD1IE = 1; // enable A/D interrupts 