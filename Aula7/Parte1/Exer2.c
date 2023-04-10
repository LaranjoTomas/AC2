#include <detpic32.h>
volatile unsigned char voltage = 0; // Global variable

#define msVal 20000

void delay(unsigned int);

void send2displays(unsigned char value)
{
    static const char display7Scodes[] = {0x3F, 0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
    static char displayFlag = 0;
    // select display high
    // send digit_high (dh) to display: dh = value >> 4
    // select display low
    // send digit_low (dl) to display: dl = value & 0x0F
    if(displayFlag == 0) {
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | (display7Scodes[value & 0x0F] << 8);
        displayFlag = 1;
    } else {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | (display7Scodes[value >> 4] << 8);
        displayFlag = 0;
    }
}

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

int main(void)
{
    unsigned int cnt = 0;
    TRISB = 0x80FF; // configure RB8-RB14 as outputs
    TRISD = 0xFF9F; // configure RD5-RD6 as outputs
    // Configure all (digital I/O, analog input, A/D module, interrupts)
    TRISBbits.TRISB4 = 1; // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0; // RB4 configured as analog input (AN
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
    
    AD1CON2bits.SMPI = 7;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;
    
    IPC6bits.AD1IP = 2;
    IFS1bits.AD1IF = 0; 
    IEC1bits.AD1IE = 1;

    EnableInterrupts(); // Global Interrupt Enable
    while(1)
    {
        if(cnt == 0) // 0, 200 ms, 400 ms, ... (5 samples/second)
        {
            // Start A/D conversion
            AD1CON1bits.ASAM = 1; // Start conversion
        }
        // Send "voltage" value to displays
        send2displays(toBcd(voltage));
        cnt = (cnt + 1) % 25; // 25 ns comes from the 40 MHZ
        // Wait 10 ms
        delay(10);
    }  
    return 0;
}
void _int_(27) isr_adc(void)
{
    // Read 8 samples (ADC1BUF0, ..., ADC1BUF7) and calculate average
    int *p = (int *)(&ADC1BUF0);
    int i;
    int media = 0;
    int sum = 0;
    for(i = 0; i < 8; i++)
    {
        media += p[i*4];
    }
    sum = media / 8;
    // Calculate voltage amplitude
    // Convert voltage amplitude to decimal and store the result in the global variable "voltage"
    voltage = ((sum * 33 + 511 )/ 1023);
    // Start A/D conversion
    AD1CON1bits.ASAM = 1; // Start conversion
    IFS1bits.AD1IF = 0; // Reset AD1IF flag
}

void delay (unsigned int ms)
{
    unsigned int wait;

    resetCoreTimer();
    wait = msVal * ms;
    while (readCoreTimer() < wait);
}