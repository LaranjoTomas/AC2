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

void putstr(char *str)
{
    // use putc() function to send each charater ('\0' should not be sent)
    while(*str != '\0') {
        putc(*str);
        str++;
    }

}

int main(void) {
    // Configure UART2:
    // 1 - Configure BaudRate Generator
    U2BRG = (((20000000+(8*115200))/(16*115200)) - 1); // 115200 bps para transmissão/receção 
    U2MODEbits.BRGH = 0; // fator de 16
    // 2 – Configure number of data bits, parity and number of stop bits
    // (see U2MODE register)
    U2MODEbits.PDSEL1 = 0;
    U2MODEbits.PDSEL0 = 1;
    U2MODEbits.STSEL = 1;
    // 3 – Enable the trasmitter and receiver modules (see register U2STA)
    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;
    // 4 – Enable UART2 (see register U2MODE)
    U2MODEbits.ON = 1;

    while(1)
    {
        putstr("String de teste\n");
        delay(1000);
    }
    return 0;
}

void delay (unsigned int ms) {
    unsigned int wait;

    resetCoreTimer();
    wait = msVal * ms;
    while (readCoreTimer() < wait);
}
