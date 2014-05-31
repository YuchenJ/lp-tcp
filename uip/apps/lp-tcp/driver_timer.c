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
#include "driver_uart.h"
#include "uip.h"

void timer_init()
{
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = 512-1;
	TACTL = TASSEL_1 + MC_1;
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	static int count = 0;
	sys_time += 1;

	if (++count == CLOCK_CONF_SECOND / CLOCK_SECOND_FRAC) {
		count = 0;
		_bic_SR_register_on_exit(LPM3_bits + GIE);
	}
}
