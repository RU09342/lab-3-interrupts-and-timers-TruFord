/*
 * button_delay_5994.c
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
  P1DIR |= BIT0 + BIT1;                     // P1.0 and P1.1 set as outputs

  P5REN = BIT6;                             // Enable resistor on P5.6
  P5OUT = BIT6;                             // Set resistor on P5.6 to pull-up
  P5IES |= BIT6;                            // Set interrupt edge select to low
  P5IE |= BIT6;                             // Enable interrupt for P5.6
  P5IFG &= ~BIT6;                           // Clear the interrupt flag

  TA0CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TA0CCR0 = 3277;                           // Set initial frequency to 10 Hz
  TA0CTL = TASSEL_1 + MC_1;                 // ACLK, up mode

  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                            // Required for Code to Properly Function
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

// Button on P5.6 interrupt service routine
#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{
    if(P5IES & BIT6)                        // If button on P5.6 is pressed
    {
      TA1CTL = TASSEL_1 + MC_2;             // Enable A1 timer: ACLK, continuous mode
      P1OUT ^= 0x01;                        // Toggle P1.0
      P5IES &= ~BIT6;                       // Flip edge to high
    }
    else
    {
        TA0CCR0 = TA1R;                     // Set CCR0 to amount of time A1 timer is on
        P1OUT &= ~BIT0;                     // Turn off LED on P1.0
        P5IES |= BIT6;                      // Flip edge back to low
        TA1CTL = TACLR;                     // Clear timer
    }
    P5IFG &= ~BIT6;                         // Clear interrupt flag
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
       P1OUT ^= 0x02;                      // Toggle P1.1

}
