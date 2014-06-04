#include "msp430.h"
#include "uip.h"

void btn_init() {
	P1DIR &= ~BIT3;
	P1REN |= BIT3;
	P1OUT |= BIT3;
	//P1IES |= BIT3;
	P1IFG &= ~BIT3;
	P1IE |= BIT3;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR()
{
	static int down = 1;
	switch(P1IFG & BIT3) {
	case BIT3:
		P1IFG &= ~BIT3;
		//P1OUT ^= BIT6;
		if (down) {
			uip_send("Button down\n", 12);
		} else {
			uip_send("Button up\n", 10);
		}
		down ^= 1;
		P1IES ^= BIT3;
		break;
	default:
		P1IFG = 0;
	}
}
