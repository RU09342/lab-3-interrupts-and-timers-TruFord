/*
 * button_delay_G2553.c
 *
 *  Created on: Oct 10, 2017
 * 	Last Edited: Oct 10, 2017
 *  Original Author: Uknown
 *  Josh Ford
 */

 #include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                // Stop WDT
  P1DIR |= BIT0 + BIT6;                    // P1.0 and P1.6 set to output

  P1REN = BIT3;                            // Enable Resistor on P1.3
  P1OUT = BIT3;                            // Set resistor on P1.3 to pull-up
  P1IES |= BIT3;                           // Set P1.3 interrupt edge select low
  P1IE |= BIT3;                            // Set P1 interrupt enable equal to P1.3
  P1IFG &= ~BIT3;                          // Clear the interrupt flag

  TA0CCTL0 = CCIE;                         // CCR0 interrupt enabled
  TA0CCR0 = 3277;                          // Set initial blink frequency to 10 Hz
  TA0CTL = TASSEL_1 + MC_1;                // ACLK, upmode

  __bis_SR_register(LPM0_bits + GIE);      // Enter LPM0 w/ interrupt
}

// Button P1.3 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IES & BIT3)                           // If the button on P1.3 is pressed
    {
      TA1CTL = TASSEL_1 + MC_2;                // Initialize A1 timer: ACLK, continuous mode
      P1OUT ^= 0x01;                           // Toggle P1.0
      P1IES &= ~BIT3;                          // Flip edge to high
    }
    else
    {
        TA0CCR0 = TA1R;                        // Set CCR0 to amount of time A1 timer counts
        P1OUT &= ~BIT0;                        // Turn off LED on P1.0
        P1IES |= BIT3;                         // Flip edge back to low
        TA1CTL = TACLR;                        // Clear timer
    }
    P1IFG &= ~BIT3;                            // Clear the interrupt flag
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
       P1OUT ^= 0x40;                      // toggle P1.6

}
