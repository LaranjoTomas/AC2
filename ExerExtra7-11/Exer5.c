#include <detpic32.h>

void putc(char byte)
{
    // wait while UART2 UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy "byte" to the U2TXREG register
    U2TXREG = byte;
}

void putstring(char *str)
{
    // use putc() function to send each charater ('\0' should not
    // be sent)
    int i;
    for(i = 0; str[i] != '\0'; i++) {
        putc(str[i]);
    }
}

int main(void) {
    // Configure UART2:
    // Configure BaudRate Generator
    U2BRG = (((20000000 + (8*1200))/(16*1200)) - 1); // 1200 bps para transmissão/receção 
    U2MODEbits.BRGH = 0; // fator de 16
    // Configure number of data bits, parity and number of stop bits
    U2MODEbits.PDSEL = 0; // paridade: 0-> sem paridade || 1-> paridade par || 2-> paridade impar || 3 -> 9 bits sem paridade
    U2MODEbits.STSEL = 0; // bit de stop : 1-> 2 bit de stop || 0-> 1 bits de stop,
    // Enable the trasmitter and receiver modules (see register U2STA)
    U2STAbits.UTXEN = 1; // transmite
    U2STAbits.URXEN = 0; // receiver
    // Enable UART2 (see register U2MODE)
    U2MODEbits.ON = 1;

    TRISB = 0x000F; // RB3, RB2, RB1, RB0 as inputs
    
    // Enable global Interrupts
    EnableInterrupts();
    while(1) {
        putstring("RB30=");
        putc(PORTBbits.RB3 + '0'); // putc precisa de character para imprimir
        putc(PORTBbits.RB2 + '0'); // '0' é o valor ascii do 0 ao somar com outro valor torna o total ascii
        putc(PORTBbits.RB1 + '0'); 
        putc(PORTBbits.RB0 + '0');
        putc('\n');
        delay(500); // 2hz
    }
    return 0;
}
