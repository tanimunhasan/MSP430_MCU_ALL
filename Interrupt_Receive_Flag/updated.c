#include <msp430.h> 

/**
 * main.c
 */

void configureClocks(void)
{
    CSCTL0_H = CSKEY >> 8;             // Unlock CS registers
    CSCTL1 = DCOFSEL_3|DCORSEL;                // Set DCO to 8MHz
    CSCTL2 = SELS__DCOCLK;             // Set SMCLK = DCO
    CSCTL3 = DIVS__1;                  // Set SMCLK divider to 1
    CSCTL0_H = 0;                      // Lock CS registers
}

void initUART(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    UCA1CTLW0 |= UCSWRST;             // Put UART A1 into SW reset
    UCA1CTLW0 |= UCSSEL__SMCLK;       // Choose SMCLK

    UCA1BRW = 26;               // 8MHz / 19200 = 416.67 -> UCBR = 26
    UCA1MCTLW = (0x54 << 8) | (0x00 << 4) | UCOS16;  // UCBRSx = 0x54, UCBRFx = 0, enable UCOS16

	// ---Setup ports
    P3SEL1 &= ~BIT5;             // P3SEL1.3: P3SEL0.4 = 01       [P3.4 for mine]
    P3SEL0 |= BIT5;             // puts UART A1 TX on P3.4

    //----------LED---------
    P3DIR |= BIT0;  // Set P3.0 as output
    P3OUT &= ~BIT0; // Turn off LED initially

    PM5CTL0 &= ~LOCKLPM5;       //turn on I/O
    UCA1CTLW0 &= ~UCSWRST;       // take UART A1 out of SW reset

    //Setup IRQ A1 RXIFG
    UCA1IE |= UCRXIE;   //local enable for A1 RXIFG
    __enable_interrupt(); //global en for maskable .... (GIE bit in SR)


    //--main loop

    while(1)
    {

    }  //do nothing

}

void main()
{
    configureClocks();
    initUART();


}
//-------------------------------------
//ISRs
#pragma vector = USCI_A1_VECTOR  // EUSCI_A1_VECTOR  aliased to USCI_A1_VECTOR in the header files
__interrupt void USCI_A1_RX_ISR(void)
{
    if(UCA1RXBUF == 't')
    {
        P3OUT ^= BIT0;  // toggle led
    }
}

