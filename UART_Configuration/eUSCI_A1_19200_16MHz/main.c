#include <msp430.h>

/**
 * main.c
 * BAUD RATE 19200    16MHz
 *
 */

// Function to initialize UART
void initUART(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //-- Setup UART
    UCA1CTLW0 |= UCSWRST;       // put UART A1 into SW reset
    UCA1CTLW0 |= UCSSEL__SMCLK; // choose SMCLK for UART A1 [if MHz then go for SMCLK, if KHz go for ACLK]

    UCA1BRW = 52;               // Set Prescalar to 52

    // UCOS16 = 1 = 0x8000
    // UCBRSx = 0x49
    // UCBRFx = 1
    UCA1MCTLW |= (0x49 << 8) | (1 << 4) | 0x8000; // Configure modulation setting

    //-- SETUP Ports
    P3SEL1 &= ~BIT4;            // Clear P3SEL1 bit 4
    P3SEL0 |= BIT4;             // Set P3SEL0 bit 4

    PM5CTL0 &= ~LOCKLPM5;       // turn on I/O
    UCA1CTLW0 &= ~UCSWRST;      // Initialize eUSCI_A1 module
}

// Function to initialize the LED on P3.0
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
void uartSendString(char *str)
{
    while(*str != '\0')
    {
        while(!(UCA1IFG & UCTXIFG)); // Wait for the TX buffer to be ready
        UCA1TXBUF = *str;            // Send the character
        str++;                       // Move to the next character
    }

    toggleLED(); // Blink the LED after sending the string
}

void main(void)
{
    initUART(); // Initialize UART
    initLED();  // Initialize LED

    //-- start main loop from here
    while(1)
    {
        uartSendString("Hello, Worked 16MHz  Baud rate 19200!\r\n"); // Send the string
        _delay_cycles(160000); // Delay before next transmission
    }

    return;
}
