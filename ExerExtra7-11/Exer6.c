#include <detpic32.h>

void delay(unsigned int ms) {
    resetCoreTimer();
    while(readCoreTimer() < 20000*ms);
}

char putc (char byte2send) {
    // wait while UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy byte2send to the UxTXREG register
    U2TXREG = byte2send;
    return byte2send;
}

void putstring(char *str) {
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
    U2BRG = (((20000000 + (8*57600))/(16*57600)) - 1); // 57600 bps para transmissão/receção 
    U2MODEbits.BRGH = 0; // fator de 16
    // Configure number of data bits, parity and number of stop bits
    U2MODEbits.PDSEL = 2; // paridade: 0-> sem paridade || 1-> paridade par || 2-> paridade impar || 3 -> 9 bits sem paridade
    U2MODEbits.STSEL = 0; // bit de stop : 1-> 2 bit de stop || 0-> 1 bits de stop,
    // Enable the trasmitter and receiver modules (see register U2STA)
    U2STAbits.UTXEN = 1; // transmite
    U2STAbits.URXEN = 0; // receiver
    // Enable UART2 (see register U2MODE)
    U2MODEbits.ON = 1;

    EnableInterrupts();
    while(1) {
        delay(666);
    }
    return 0;
}