/*
 * driver_timer.c
 *
 *  Created on: May 19, 2014
 *      Author: madman2
 */
#include <msp430.h>
#include <stdint.h>

#include "clock-arch.h"
#include "driver_timer.h"

void timer_init()
{
	P1DIR |= 0x01;                            // P1.0 output
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = 1024-1;
	TACTL = TASSEL_1 + MC_1;
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	sys_time += 1;
}
