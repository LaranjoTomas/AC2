#include <detpic32.h>

typedef struct{
    char mem[100];
    int nchar;
    int posrd;
} t_buf;

volatile t_buf txbuf;

void putc(char byte)
{
    // wait while UART2 UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy "byte" to the U2TXREG register
    U2TXREG = byte;
}

void putstrInt(char *str)
{
    while(txbuf.nchar > 0);
    // copy all character of the string "str" to the buffer
    while(*str != '\0'){
        txbuf.mem[txbuf.nchar] = *str;
        txbuf.nchar++;
        str++;
    }
    // Initialize "posrd" variable with 0
    txbuf.posrd = 0;
    // Enable UART2 TX interrupt    
    IEC1bits.U2TXIE = 1;
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

    // Initialize buffer variable (nchar) with 0
    txbuf.nchar = 0;

    while(1) {
        putstrInt("Test string which can be as long as you like, up to a maximum of 100 characters\n");
    }
    return 0;
}

void _int_(32) isr_uart2(void)
{
    if (IFS1bits.U2TXIF == 1)
    {
        if(txbuf.nchar > 0) { // At least one character to be transmitted
            // Read 1 character from the buffer and send it
            U2TXREG = txbuf.mem[txbuf.posrd];
            // Update buffer "posrd" and "nchar" variables
            txbuf.posrd++;
            txbuf.nchar--;
        } else {
            // Disable UART2 Tx interrupts
            IEC1bits.U2TXIE = 0;
        }
        // Clear UART2 Tx interrupt flag
        IFS1bits.U2TXIF = 0;
    }
}