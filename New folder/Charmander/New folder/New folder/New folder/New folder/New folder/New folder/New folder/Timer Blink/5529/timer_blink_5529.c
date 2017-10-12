/*
 * timer_blink_5529.c
 *
 *  Created on: Oct 10, 2017
 * 	Last Edited: Oct 10, 2017
 *  Original Author: Uknown
 *  Josh Ford
 */

 #include <msp430.h>

 {
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P4DIR |= BIT7;                            // P4.7 LED
  TA0CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TA0CCR0 = 3200;                           // Approximately 10 Hz, Aclk is like 32 kHz
  TA0CTL = TASSEL_1 + MC_1 + TACLR;         // ACLK, upmode, clear TAR

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
  __no_operation();                         // For debugger
}

// Timer0 A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR           //Timer counts
__interrupt void TA0_ISR(void)
{
  P4OUT ^= BIT7;                            // Toggle LED 4.7
}
