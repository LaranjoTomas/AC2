#include <detpic32.h>

void putc(char byte)
{
    // wait while UART2 UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy "byte" to the U2TXREG register
    U2TXREG = byte;
}

int main(void)
{   

    // Configure UART2:
    // Configure BaudRate Generator
    U2BRG = (((20000000+(8*115200))/(16*115200)) - 1); // 115200 bps para transmissão/receção 
    U2MODEbits.BRGH = 0; // fator de 16
    // Configure number of data bits, parity and number of stop bits
    U2MODEbits.PDSEL1 = 0;
    U2MODEbits.PDSEL0 = 0;
    U2MODEbits.STSEL = 1;
    // Enable the trasmitter and receiver modules (see register U2STA)
    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;
    // Enable UART2 (see register U2MODE)
    U2MODEbits.ON = 1;

    // Configure UART2 interrupts, with RX interrupts enabled
    //and TX interrupts disabled:
    //enable U2RXIE, disable U2TXIE (register IEC1)
    //set UART2 priority level (register IPC8)
    //clear Interrupt Flag bit U2RXIF (register IFS1)
    //define RX interrupt mode (URXISEL bits)

    IEC1bits.U2RXIE = 1;
    IEC1bits.U2TXIE = 0;

    IPC8bits.U2IP = 1;
    IPC8bits.U2IS = 2;

    IFS1bits.U2RXIF = 0;

    U2STAbits.URXISEL1 = 0;
    U2STAbits.URXISEL0 = 0;

    // Enable global Interrupts
    EnableInterrupts();


    while(1);
    return 0;
}

void _int_(32) isr_uart2(void)
{
    char cc;
    if (IFS1bits.U2RXIF == 1) {
        // Read character from FIFO (U2RXREG)
        cc = U2RXREG;
        putc(cc);
        // Clear UART2 Rx interrupt flag
        // turn on the led RC14 if the character is 'T' and turn it off if the character is 't'
        if (cc == 'T') {
            LATCbits.LATC14 = 1;
        }else if (cc == 't') {
            LATCbits.LATC14 = 0;
        }
        IFS1bits.U2RXIF = 0;
    }
}