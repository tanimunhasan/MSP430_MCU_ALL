#include <msp430.h>
#include <stdio.h>

void uart_init(void);
void uart_transmit(char c);
void print(const char *str);
void delay_ms(unsigned int ms);
void clockConfigure(void);
void print_float(float value);

void clockConfigure(void)
{
    CSCTL0_H = CSKEY_H;               // Unlock CS registers
    CSCTL1 = DCOFSEL_0;               // DCO at 1 MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;  // No dividers
    CSCTL0_H = 0;                      // Lock CS registers
}

void uart_init(void)
{
    // Configure UART pins
    P2SEL1 |= BIT0 | BIT1;      // TX: P2.0, RX: P2.1
    P2SEL0 &= ~(BIT0 | BIT1);

    // Put eUSCI_A0 into reset mode before configuring
    UCA0CTLW0 = UCSWRST;

    // Select SMCLK as clock source
    UCA0CTLW0 |= UCSSEL__SMCLK;

    // Baud Rate = 38400 bps @ 1 MHz
    UCA0BRW = 1;                      // UCBRx = 1
    UCA0MCTLW = UCOS16 | (10 << 4) | (0x00 << 8);  // UCBRFx = 10, UCBRSx = 0x00

    // Release from reset
    UCA0CTLW0 &= ~UCSWRST;
}

void uart_transmit(char c)
{
    while (!(UCA0IFG & UCTXIFG));      // Wait until TX buffer is ready
    UCA0TXBUF = c;                     // Transmit character
}

void print(const char *str)
{
    while (*str)
    {
        uart_transmit(*str++);
    }
}

void delay_ms(unsigned int ms)
{
    while (ms--)
    {
        __delay_cycles(1000);          // 1 ms delay at 1 MHz
    }
}

// Function to print float values
void print_float(float value)
{
    char buffer[20];                    // Buffer to hold the float string
    sprintf(buffer, "%.2f\r\n", value); // Convert float to string with 2 decimal points
    print(buffer);
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;         // Stop watchdog timer

    // Unlock GPIO (for FRAM-based MSP430s)
    PM5CTL0 &= ~LOCKLPM5;

    clockConfigure();                  // Configure clock
    uart_init();                       // Initialize UART

    // LED Configuration
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;                   // LED OFF initially

    float sensor_value = 23.45;        // Example float value
    int i = 0;

    while (1)
    {
        P1OUT ^= BIT0;                // Toggle LED

        // Print integer and float values
        char msg[30];
        sprintf(msg, "Count: %d, Value: ", i++);
        print(msg);
        print_float(sensor_value + i * 0.1);   // Incrementing float value

        delay_ms(1000);                // Delay for 1 second
    }
}
