#include <detpic32.h>

int main(void)
{
    
    TRISB = 0x80FF; // 1000 0000 1111 1111
    TRISD = 0xFF9F; // 
    LATB = LATB & 0x80FF;
    char c;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    while(1)
    {
        c = getChar();
        if (c >= 'a' && c <= 'g' ) {
            LATB = (LATB & 0x80FF) | (1 << (8 + (c - 'a'))) ; // 1000 0000 1111 1111
            putChar(c);
        } else if (c >= 'A' && c <= 'G') {
            LATB = (LATB & 0x80FF) | (1 << (8 + (c - 'A'))) ; // 1000 0000 1111 1111
            putChar(c);
        }
    }
}
