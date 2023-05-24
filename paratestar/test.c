#include <detpic32.h>
#define msVal 200000

void delay (unsigned int ms) {

    resetCoreTimer();
    while(readCoreTimer() < msVal * ms);
}

void send2displays(unsigned char value)
{
	static const char display7Scodes[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };

	static char flag = 0;

	if (flag == 1) {
		// select display high
		// send digit_high (dh) to display:	dh = value >> 4;
		LATD = (LATD & 0xFF9F) | 0xFFDF;
		LATB = (LATB & 0x80FF) | (display7Scodes[(value >> 4)] << 8);
		flag = 0;
	} else {
		// select display low
		// send digit_low (dl) to display:	dl = value & 0x0F;
		LATD = (LATD & 0xFF9F) | 0xFFBF;
		LATB = (LATB & 0x80FF) | (display7Scodes[(value & 0x0F)] << 8);
		flag = 1;
	}
}

void putc(char byte2send) {
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte2send;
}

char getc(void) {
    while (U2STAbits.URXDA == 0);
    return U2RXREG;
}

int main() {

    char c;

    U2BRG = (((20000000 + (8*115200))/(16*115200))-1);
    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL0 = 0;
    U2MODEbits.PDSEL1 = 0;
    U2MODEbits.STSEL = 0;
    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;
    U2MODEbits.ON = 1;

    TRISE = 0xFFF0;
    LATE = 0xFFF0;
    TRISD = TRISD & 0xFF9F;
    LATD = 0x0000;

    while (1) {
        c = getc();

        if (c == '0') {
            LATE = 0xFFF1;
            send2displays(0x12);
        } else if (c == '1') {
            LATE = 0xFFF2;
        } else if (c == '2') {
            LATE = 0xFFF4;
        } else if (c == '3') {
            LATE = 0xFFF8;
        } else if (c == '4') {
            LATE = 0xFFFF;
            delay(1000);
            LATE = 0xFFF0;
        }
    }

    return 0;
}
