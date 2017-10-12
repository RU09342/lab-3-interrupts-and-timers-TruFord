/*
 * simple_blink_G2253.c
 *
 *  Created on: Oct 10, 2017
 * 	Last Edited: Oct 10, 2017
 *  Original Author: Uknown
 *  Josh Ford
 */

 #include <msp430.h>

 int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= BIT0;                            // P1.0 set to output
  P2DIR |= BIT0;                            // P2.0 set to output

  P1REN = BIT1;                             // Enable resistor on P1.1
  P1OUT = BIT1;                             // Set resistor on P1.1 to pull-up
  P1IES |= BIT1;                            // Set interrupt edge select to low
  P1IE |= BIT1;                             // Enable interrupt on P1.1
  P1IFG &= ~BIT1;                           // Clear the interrupt flag

  TB0CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TB0CCR0 = 3277;                           // Set initial clock frequency to 10 Hz

  TB0CTL = TBSSEL_1 + MC_1;                 // ACLK, up mode

  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                            // Required for Code to Properly Function

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

// Button on P1.1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IES & BIT1)                        // If button on P1.1 is pressed
    {
      TB1CTL = TBSSEL_1 + MC_2;             // Enable timer B1: ACLK, continuous mode
      P1OUT ^= 0x01;                        // Toggle P1.0
      P1IES &= ~BIT1;                       // Flip edge to high
    }
    else
    {
        TB0CCR0 = TB1R;                     // Set CCR0 to amount of time timer B1 is on
        P1OUT &= ~BIT0;                     // Turn off LED on P1.0
        P1IES |= BIT1;                      // Flip edge back to low
        TB1CTL = TBCLR;                     // Clear timer
    }
    P1IFG &= ~BIT1;                         // Clear the interrupt flag
}

// Timer B0 interrupt service routine
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void)
{
       P2OUT ^= 0x01;                       // Toggle P1.1

}
