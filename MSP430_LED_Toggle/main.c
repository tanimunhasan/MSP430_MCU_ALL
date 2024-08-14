#include <msp430.h>

/**
 * main.c
 * BAUD RATE 19200    8MHz
 * Code Works: Sayed Tanimun Hasan
 */

// Function to configure clocks
void configureClocks(void)
{
    CSCTL0_H = CSKEY >> 8;             // Unlock CS registers`
    CSCTL1 = DCOFSEL_6;      // Set DCO to 8MHz
    CSCTL2 = SELS__DCOCLK;             // Set SMCLK = DCO
    CSCTL3 = DIVS__1;                  // Set SMCLK divider to 1
    CSCTL0_H = 0;                      // Lock CS registers
}

// Function to initialize UART
void initUART(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    UCA1CTLW0 |= UCSWRST;       // Put UART A1 into SW reset
    UCA1CTLW0 |= UCSSEL__SMCLK; // Choose SMCLK

    // Set baud rate to 19200 with 8MHz clock
    UCA1BRW = 26;               // 8MHz / 19200 = 416.67 -> UCBR = 26


    //****************Modulation******************
    // First Modulation (UCBRSx): 0x54       Second Modulation (UCBRFx): 0x00       Oversampling UCOS16: 1
    UCA1MCTLW =  (0x00 << 4)| (0x54 << 8)  | UCOS16;  // UCBRSx = 0x54, UCBRFx = 0, enable UCOS16

    P3SEL1 &= ~BIT4;            // Clear P3SEL1 bit 4
    P3SEL0 |= BIT4;             // Set P3SEL0 bit 4

    PM5CTL0 &= ~LOCKLPM5;       // Turn on I/O
    UCA1CTLW0 &= ~UCSWRST;      // Initialize eUSCI_A1 module
}

void initLED(void)
{
    P3DIR |= BIT0;  // Set P3.0 as output
    P3OUT &= ~BIT0; // Turn off LED initially
}

// Function to toggle the LED
void toggleLED(void)
{
    P3OUT ^= BIT0;  // Toggle the state of P3.0
}

// Function to send a string via UART


void main(void)
{
    configureClocks();  // Configure the clocks for 8MHz operation
    initUART();         // Initialize UART
    initLED();         // Initialize LED

    //-- Test main loop
    while(1)
    {
        toggleLED();
        _delay_cycles(100000); // Adjusted delay for 8MHz clock

    }

}


