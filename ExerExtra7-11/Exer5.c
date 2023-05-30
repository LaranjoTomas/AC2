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


    T2CONbits.TCKPS = 7; //20*10^6 / 65536 * 2 = 152 
    PR2 = 39062; // Fout = 20MHz / 256 = /2 = 39063
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the


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

    TRISB = TRISB | 0x000F; // RB3, RB2, RB1, RB0 as inputs

    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag
    
    // Enable global Interrupts
    EnableInterrupts();
    while(1) {
        putstring("RB30=");
        putc(PORTBbits.RB3 + '0'); // putc precisa de character para imprimir
        putc(PORTBbits.RB2 + '0'); // '0' é o valor ascii do 0 ao somar com outro valor torna o total ascii
        putc(PORTBbits.RB1 + '0'); 
        putc(PORTBbits.RB0 + '0');
        putc('\n');
    }
    return 0;
}

void _int_(8) isr_T2(void){
    IFS0bits.T2IF = 0;
}