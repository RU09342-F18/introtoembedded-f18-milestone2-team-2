#include <msp430.h> 
#include <macros.h>

// Initialize ADC
void initADC(void)
{
    ADC12CTL0 = ADC12SHT0_4 + ADC12ON + ADC12MSC; // turns on adc 12 and samples every 12 ADC12CLK cycles
    ADC12CTL1 |= ADC12SHP + ADC12CONSEQ_2; // ADC12 uses sampling timer
    ADC12MCTL0 |= ADC12INCH_0; // selects channel 0 for ADC input
    ADC12IE |= ADC12IE0; // enables ADC interrupt
    P6SEL |= 0x01; // selects pin 6.0 to use the ADC
    P1DIR |= LED1; // selects LED 1 to be output
    P1OUT |= LED1;
}
// Initialize timer
void initTimer(void)
{
    P1DIR |= BIT2; // sets pin 1.2 as output
    P1SEL |= BIT2; // pin 1.2 gets timer output
    TA0CCTL0 = CCIE; // enables interrupt
    TA0CCR0 = 4096-1; // sets PWM period, 4096 = 3.3v
    TA0CCTL1 = OUTMOD_7; // CCR1 set to reset/set mode
    TA0CCR1 = 2048; // CCR1 PWM duty cycle
    TA0CTL = TASSEL_2 + MC_2 + ID_3; // uses SMCLK, up mode
}
// Initialize UART
void initUART(void)
{
    P3SEL |= BIT3 + BIT4; // sets pin 3.3 and 3.4 for USCI_A0 TXD/RXD
    UCA0CTL1 |= UCSWRST; // resets USCI logic to hold in reset state
    UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
    UCA0BR0 = 0x06;                           // 1 MHz 9600(see User's Guide)
    UCA0BR1 = 0x00;                           // 1 MHz 9600
    UCA0MCTL = UCBRS_0 + UCBRF_13 + UCOS16;   // Modln UCBRSx = 0, UCBRFx = 0, over sampling mode
    UCA0CTL1 &= ~(UCSWRST); // starts USCI state machine
}




unsigned int in;
float volts;
float tempC;
char tempTens;
char tempOnes;
char tempCASCII;
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	initADC(); // initializes ADC
	initTimer(); // initilaizes timer/PWM
	initUART(); // initializes UART
	while(1)
	{
	    __bis_SR_register(LPM0_bits + GIE); // enables low power mode 0 and global interrupts
	}


}
// timer ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    ADC12CTL0 |= ADC12ENC; // enable conversion
    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 |= ADC12SC; // enables sampling and conversion

}
// ADC ISR
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
        ADC12CTL0 &= ~ADC12ENC; // disable conversion
        in=ADC12MEM0; // sets in to ADC conversion results
        volts=in*0.00081; // converts bits to voltage
        tempC=(volts/0.03389)-24; // converts voltage to temp
        tempTens =(tempC / 10); // converts tens to ascii
        tempOnes =(tempC - (tempTens*0x0A)); // converts ones to ascii
        if(tempC >= 20 && tempC <= 40) // if temp is between linear range
        {
            while(!(UCA0IFG&UCTXIFG));
            UCA0TXBUF=tempTens + '0'; // set to converted temp
            while(!(UCA0IFG&UCTXIFG));
            UCA0TXBUF=tempOnes + '0'; // set to converted temp
            while(!(UCA0IFG&UCTXIFG));
            UCA0TXBUF=0x20;
        }

        else if (tempC > 40) // set to flat value if out of linear range
        {
            while(!(UCA0IFG&UCTXIFG));
            tempTens =4 + '0';
            tempOnes = 0 + '0';
            UCA0TXBUF=tempTens; // set to converted temp
            while(!(UCA0IFG&UCTXIFG)); // tx clear
            UCA0TXBUF=tempOnes; // set to converted temp
            while(!(UCA0IFG&UCTXIFG)); // tx clear
            UCA0TXBUF=0x20;
        }

       else if (tempC < 20)
       {
           while(!(UCA0IFG&UCTXIFG)); // tx clear
           tempTens =2 + '0';
           tempOnes =0 + '0';
           UCA0TXBUF=tempTens; // set to converted temp
           while(!(UCA0IFG&UCTXIFG)); // tx clear
           UCA0TXBUF=tempOnes; // set to converted temp
           while(!(UCA0IFG&UCTXIFG)); // tx clear
           UCA0TXBUF=0x20;
       }
}

