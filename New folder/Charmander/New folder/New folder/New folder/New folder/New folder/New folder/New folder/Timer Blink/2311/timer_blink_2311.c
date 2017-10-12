/*
 * timer_blink_2311.c
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
  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
  P1DIR |= BIT0;                            // P1.0 LED
  TB0CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TB0CCR0 = 3200;                           // Approximately 10 Hz, Aclk is like 32 kHz
  TB0CTL = TBSSEL_1 + MC_1 + TBCLR;         // ACLK, upmode, clear TAR

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
  __no_operation();                         // For debugger
}

// Timer0 A0 interrupt service routine
#pragma vector = TIMER0_B0_VECTOR           //Timer counts
__interrupt void TB0_ISR(void)

{
  P1OUT ^= BIT0;                            // Toggle LED 1.0
}
