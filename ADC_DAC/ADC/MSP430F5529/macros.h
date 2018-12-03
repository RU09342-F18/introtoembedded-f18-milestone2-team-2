#ifndef MACROS_H_
#define MACROS_H_

#ifdef __MSP430G2553__
    #define B1         BIT3
    #define LED1       BIT0
    #define LED2       BIT6
    #define B1DIR      P1DIR
    #define B1IN       P1IN
    #define B1REN      P1REN
    #define B1OUT      P1OUT
    #define B1IE       P1IE
    #define B1IES      P1IES
    #define B1IFG      P1IFG
    #define L1DIR      P1DIR
    #define L1OUT      P1OUT
    #define L2DIR      P1DIR
    #define L2OUT      P1OUT
#endif // __MSP430G2553__

#ifdef __MSP430F5529__
    #define B1         BIT1
    #define B2         BIT1
    #define LED1       BIT0
    #define LED2       BIT7
    #define B1DIR      P1DIR
	#define B1IN	   P1IN
    #define B1REN      P1REN
    #define B1OUT      P1OUT
    #define B1IE       P1IE
    #define B1IES      P1IES
    #define B1IFG      P1IFG
    #define L1DIR      P1DIR
    #define L1OUT      P1OUT
    #define B2DIR      P2DIR
    #define B2REN      P2REN
    #define B2OUT      P2OUT
    #define B2IE       P2IE
    #define B2IES      P2IES
    #define B2IFG      P2IFG
    #define L2DIR      P4DIR
    #define L2OUT      P4OUT
#endif // __MSP430F5529__

#ifdef __MSP430FR2311__
    #define B1         BIT1
    #define LED1       BIT0
    #define LED2       BIT0
    #define B1DIR      P1DIR
	#define B1IN	   P1IN
    #define B1REN      P1REN
    #define B1OUT      P1OUT
    #define B1IE       P1IE
    #define B1IES      P1IES
    #define B1IFG      P1IFG
    #define L1DIR      P1DIR
    #define L1OUT      P1OUT
    #define L2DIR      P2DIR
    #define L2OUT      P2OUT
#endif // __MSP430FR2311__

#ifdef __MSP432P401R__
    #define B1         BIT1
    #define B2         BIT4
    #define LED1       BIT0
    #define LEDR       BIT0
    #define LEDG       BIT1
    #define LEDB       BIT2
    #define B1DIR      P1DIR
    #define B1IN       P1IN
    #define B1REN      P1REN
    #define B1OUT      P1OUT
    #define B1IE       P1IE
    #define B1IES      P1IES
    #define B1IFG      P1IFG
    #define L1DIR      P1DIR
    #define L1OUT      P1OUT
    #define B2DIR      P1DIR
    #define B2IN       P1IN
    #define B2IN       P1IN
    #define B2REN      P1REN
    #define B2OUT      P1OUT
    #define B2IE       P1IE
    #define B2IES      P1IES
    #define B2IFG      P1IFG
    #define LRDIR      P2DIR
    #define LROUT      P2OUT
    #define LGDIR      P2DIR
    #define LGOUT      P2OUT
    #define LBDIR      P2DIR
    #define LBOUT      P2OUT
#endif // __MSP432P401R__

#endif // MACROS_H_



