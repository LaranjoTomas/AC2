#include <detpic32.h>

volatile int count=0;

char toBcd(unsigned char value) {
    return ((value / 10 << 4) + (value % 10));
}

void send2displays(unsigned char value) {
    static int flag;
    static const char codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    if (flag == 0) {
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x80FF) | (codes[value & 0x0F] << 8);
    } else {
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x80FF) | (codes[value >> 4] << 8);
    }
    flag = !flag;
}

int main(void) {
    T1CONbits.TCKPS = 2; // 20*10^6 / 65536 * 10 =30
    PR1 = 31249; // 20 * 10 ^6 / 64 = / 10 =31250
    TMR1 = 0; // Clear timer T2 count register
    T1CONbits.TON = 1; // Enable timer T1 (must be the last command of the
    // timer configuration sequence)
    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T1 interrupts
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag

    T2CONbits.TCKPS = 3; // 20*10^6 / 65536 * 50 =6
    PR2 = 49999; // 20 * 10 ^6 / 8 = / 50 =50000
    TMR2 = 0; // Clear timer T2 count register
    T2CONbits.TON = 1; // Enable timer T1 (must be the last command of the
    // timer configuration sequence)    
    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T1 interrupts
    IFS0bits.T2IF = 0; // Reset timer T1 interrupt flag

    TRISD = TRISD & 0xFF9F;
    TRISB = TRISB & 0x80FF;

    EnableInterrupts();

    while(1){
        printInt(count, 16 | 2 << 16);
        putChar('\r');
    }
    return 0;
}

void _int_(4) isr_T1(void) {
    if(count == 99) {
        count = 0;
    } else {
        count++;
    }

    IFS0bits.T1IF = 0;
}

void _int_(8) isr_T2(void) {
    send2displays(toBcd(count));
    IFS0bits.T2IF = 0;
}
