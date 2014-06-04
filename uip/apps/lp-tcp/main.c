#include <msp430.h>
#include "driver_uart.h"
#include "driver_timer.h"
#include "driver_btn.h"
#include "uip.h"
#include "timer.h"
#include "clock.h"

u16_t devicedriver_poll();

int main(void) {
	struct uip_conn *conn;
	//struct timer periodic_timer;
	unsigned int i;

	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	P1DIR |= BIT0 + BIT6;
	P1OUT = 0;
	P2DIR = 0;
	P2OUT = 0;
	P3DIR = 0;
	P3OUT = 0;

//	init_clocks();
	driver_uart_init();
	uip_init();
	timer_init();
	btn_init();

	//P1DIR |= BIT0 | BIT4 | BIT5 | BIT6 | BIT7;
	//P1OUT &= ~BIT0 & ~BIT4 & ~BIT5 & ~BIT6 & ~BIT7;
	//P1DIR |= BIT0;

	//timer_set(&periodic_timer, CLOCK_SECOND / 10);

	uip_ipaddr_t dst, ipaddr;

	uip_ipaddr(ipaddr, 192,168,5,2);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, 192,168,5,1);
	uip_setdraddr(ipaddr);
	uip_ipaddr(ipaddr, 255,255,255,0);
	uip_setnetmask(ipaddr);

	uip_ipaddr(dst, 192,168,5,1);
	static const u16_t port = 1234;

	conn = uip_connect(&dst, htons(port));
	if (conn == 0) {
		_nop();
	}

	for(;;) {
		//P1OUT |= BIT0;
		// Poll Device Driver
		uip_len = devicedriver_poll();
		if (uip_len > 0) {
//			P1OUT ^= BIT4;
			uip_input();
			if (uip_len > 0)
				devicedriver_send();
		} else {
		//if (timer_expired(&periodic_timer)) {
			//P1OUT ^= BIT4;
			//timer_reset(&periodic_timer);
			// Periodic Processing of connections
			for(i = UIP_CONNS; i-- > 0; ) {
				uip_periodic(i);
				//P1OUT ^= BIT6;
				if(uip_len > 0) {
					devicedriver_send();
				}
			}
		}
		//P1OUT &= ~BIT0;
		_BIS_SR(LPM3_bits + GIE);
	}
}

u16_t devicedriver_poll() {
	u16_t tmp = devicedriver_uip_len;
	devicedriver_uip_len = 0;
	return tmp;
}
