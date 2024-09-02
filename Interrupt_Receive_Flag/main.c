#include <msp430.h> 

/**
 * main.c
 * Microcontroller: MSP430FR5987
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    UCA1CTLW0 |= UCSWRST;             // Put UART A1 into SW reset
    UCA1CTLW0 |= UCSSEL__SMCLK;       // Choose SMCLK

	UCA1BRW = 8;
	UCA1MCTLW = 0xD600;

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

	return 0;
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




