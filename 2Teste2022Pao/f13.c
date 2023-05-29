#include <detpic32.h>

volatile int count = 15;

void putc(char byte2send)
{
    // wait while UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy byte2send to the UxTXREG register
    U2TXREG = byte2send;
}

void putString(char *str) {
    int i;
    for(i=0; str[i] != '\0';i++) {
        putc(str[i]);
    }
}

char getc(void)
{
    // Wait while URXDA == 0
    while(U2STAbits.URXDA == 0);
    // Return UxRXREG
    return U2RXREG;
} 

void send2Led(unsigned char value ) {
    LATE = (LATE & 0xFFE1) | (value << 1);
}

int main(void) {
    U2BRG = ((20000000 + 8 * 9600) / ( 16 * 9600)) -1;
    
    U2MODEbits.BRGH = 1;

    TRISE = TRISE & 0xFFE1;

    U2MODEbits.PDSEL = 2;
    U2MODEbits.STSEL = 1;

    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;

    U2MODEbits.ON = 1;

    IPC8bits.U2IP = 1;
    IEC1bits.U2RXIE = 1;
    IFS1bits.U2RXIF = 0;

    EnableInterrupts();


    while(1) {
        send2Led(count);
    }
    return 0;
}

void _int_(32) isr_U2(void) {
    char c;
    c = getc();

    if(c == 'U') {
        count++;
        if (count > 15) {
            count = 0 ;
        }
    } else if(c == 'R') {
        putc('\n');
        putString("RESET");
        putc('\n');
        count = 0;
    }
    IFS1bits.U2RXIF = 0;
}

