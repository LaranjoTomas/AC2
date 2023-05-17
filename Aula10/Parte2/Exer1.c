#include <detpic32.h>
#define msVal 20000

void delay(unsigned int);

void putc(char byte)
{
    // wait while UART2 UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy "byte" to the U2TXREG register
    U2TXREG = byte;
}

void putc1(char byte) {
    while (U1STAbits.UTXBF == 1);
    U1TXREG = byte;
}

void putstr(char *str)
{
    // use putc() function to send each charater ('\0' should not be sent)
    while(*str != '\0') {
        putc(*str);
        str++;
    }

}

char getc (void) {
    while(U1STAbits.URXDA == 0);

    return U1RXREG;
}

int main(void) {
    // Configure UART2:
    // 1 - Configure BaudRate Generator
    U1BRG = (((20000000+(8*115200))/(16*115200)) - 1); // 115200 bps para transmissão/receção 
    U1MODEbits.BRGH = 0; // fator de 16
    // 2 – Configure number of data bits, parity and number of stop bits
    // (see U2MODE register)
    U1MODEbits.PDSEL1 = 0;
    U1MODEbits.PDSEL0 = 0;
    U1MODEbits.STSEL = 0;
    // 3 – Enable the trasmitter and receiver modules (see register U2STA)
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    // 4 – Enable UART2 (see register U2MODE)
    U1MODEbits.ON = 1;

    while(1)
    {
        putc1(0x5A); // 0101 101
        delay(10);
    }
    return 0;
}

void delay (unsigned int ms) {
    unsigned int wait;

    resetCoreTimer();
    wait = msVal * ms;
    while (readCoreTimer() < wait);
}
