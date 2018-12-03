#include <msp430.h> 
#include <macros.h>

unsigned int in;
float volts;
unsigned int tempC;
unsigned int Tset;
int error;
int errorPrev = 0;
float integral = 0;
float output;
float PWM = 0;
const int Kp = 60;
const float Ki = 0;
const int Kd = 0;
int derivative;
char tempHund;
char tempTens;
char tempOnes;
char tempCASCII;



// Initialize ADC
void initADC(void)
{
    ADC12CTL0 = ADC12SHT0_4 + ADC12ON + ADC12MSC; // turns on adc 12 and samples every 12 ADC12CLK cycles
    ADC12CTL1 |= ADC12SHP + ADC12CONSEQ_2; // ADC12 uses sampling timer
    ADC12MCTL0 |= ADC12INCH_0; // selects channel 0 for ADC input
    ADC12IE |= ADC12IE0; // enables ADC interrupt
    P6SEL |= 0x01; // selects pin 6.0 to use the ADC
    P4DIR |= BIT0; // selects LED 1 to be output
    P4OUT |= BIT0;
}
// Initialize timer
void initTimer(void)
{
    //Timer A0 setup
    TA0CCTL0 = CCIE; // enables interrupt
    TA0CCR0 = 12000; // sets clock period
    TA0CTL = TASSEL_1 + MC_1; // uses SMCLK, continuous mode
    //Timer A1 setup
    P2DIR |= BIT0+BIT1;                       // P2.0 and P2.1 output
    P2SEL |= BIT0+BIT1;                       // P2.0 and P2.1 options select
    TA1CCR0 = 512-1;                          // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = 0;                            // CCR1 PWM duty cycle
    TA1CTL = TASSEL_1 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR
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
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
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
        while(!(UCA0IFG & UCTXIFG));
        Tset = UCA0RXBUF; // holds recieved charater
        ADC12CTL0 &= ~ADC12ENC; // disable conversion
        in=ADC12MEM0; // sets in to ADC conversion results
        volts=in*0.00081; // converts bits to voltage
        tempC=(49.24956*volts) - 52.2713; // converts voltage to temp
        tempTens =(tempC / 10); // converts tens to ascii
        tempOnes =(tempC - (tempTens*0x0A)); // converts ones to ascii
        error = tempC - Tset;
        integral = integral + error;
        derivative = (error - errorPrev);
        output = (Kp * error) + (Ki* integral) + (Kd * derivative);
        errorPrev = error;
        TA1CCR1 = TA1CCR1 + output;
        if(output >= 0)
        {
            PWM = PWM + output;
            TA1CCR1 = PWM;
        }
        else if (output <= 0)
        {
            PWM = PWM + output;
            TA1CCR1 = PWM;
        }
        if(PWM >= 511)
        {
            PWM = 511;
            TA1CCR1 = PWM;
        }
        else if(PWM <= 0)
        {
            PWM = 0;
            TA1CCR1 = PWM;
        }
        if(tempC > 20 && tempC < 100) // if temp is between linear range
        {
            while(!(UCA0IFG&UCTXIFG));
            UCA0TXBUF=tempTens + '0'; // set to converted temp
            while(!(UCA0IFG&UCTXIFG));
            UCA0TXBUF=tempOnes + '0'; // set to converted temp
            while(!(UCA0IFG&UCTXIFG));
            UCA0TXBUF=0x20;
        }

        else if (tempC >= 100) // set to flat value if out of linear range
        {
            while(!(UCA0IFG&UCTXIFG));
            tempHund =1 + '0';
            tempTens =0 + '0';
            tempOnes =0 + '0';
            UCA0TXBUF=tempHund; // set to converted temp
            while(!(UCA0IFG&UCTXIFG)); // tx clear
            UCA0TXBUF=tempTens; // set to converted temp
            while(!(UCA0IFG&UCTXIFG)); // tx clear
            UCA0TXBUF=tempOnes; // set to converted temp
            while(!(UCA0IFG&UCTXIFG)); // tx clear
            UCA0TXBUF=0x20;
        }

       else if (tempC <= 20)
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
