#include <detpic32.h>

volatile unsigned char c;
volatile unsigned int count;

void delay{unsigned int ms} {
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void putc(char byte2send)
{
 // wait while UTXBF == 1
    while(U2STAbits.UTXBF == 1);
 // Copy byte2send to the UxTXREG register
    U2TXREG = byte2send; 
}

void putstr(char *str) {
    int i;
    for(i = 0; str[i] != '\0'; i++) {
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

int main (void) {

    U2BRG = (( 20000000 + 8 * 9600 ) / ( 16 * 9600 )) - 1 ; 
    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL = 1; // even
    U2MODEbits.STSEL = 0; // stop bits

    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;

    U2MODEbits.ON = 1;

    IFS1bits.U2RXIF = 0;

    IEC1bits.U2RXIE = 1;
    IEC1bits.U2TXIE = 0;

    IPC8bits.U2IP = 1;

    U2STAbits.URXISEL = 0;
    
    EnableInterrupts();

    while(1);
    return 0;
}

void _int_(32) isr_UART2(void) {

    if(IFS1bits.U2RXIF == 1) {
        c = getc();
        if(c == 0x0A) {
            putc('\n');
            putstr("O número de minúsculas digitado foi ");
            putc(count + '0');
            putc('\n');
            count = 0;
        } else {
            putc('\n');
            putc(c);
            if ((c >= 'a') && (c <= 'z')) {
                count++;
            }
        }
    }
    IFS1bits.U2RXIF == 0;
}
