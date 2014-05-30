#include <msp430.h>
#include "driver_uart.h"
#include "driver_timer.h"
#include "uip.h"
#include "timer.h"
#include "clock.h"

u16_t devicedriver_poll();

int main(void) {
	struct uip_conn *conn;
	struct timer periodic_timer;
	int i;

	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

//	init_clocks();
	driver_uart_init();
	uip_init();
	timer_init();
	_BIS_SR(GIE);

	//P1DIR |= BIT0 | BIT4 | BIT5 | BIT6 | BIT7;
	//P1OUT &= ~BIT0 & ~BIT4 & ~BIT5 & ~BIT6 & ~BIT7;
	P1DIR = 0;
	P1OUT = 0;
	P2DIR = 0;
	P2OUT = 0;
	P3DIR = 0;
	P3OUT = 0;

	timer_set(&periodic_timer, CLOCK_SECOND / 10);

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

		// Poll Device Driver
		uip_len = devicedriver_poll();
		if (uip_len > 0) {
//			P1OUT ^= BIT4;
			uip_input();
			if (uip_len > 0) devicedriver_send();
		} else if (timer_expired(&periodic_timer)) {
			//P1OUT ^= BIT4;
			timer_reset(&periodic_timer);
			// Periodic Processing of connections
			for(i = UIP_CONNS; i-- > 0; ) {
				uip_periodic(i);
				if(uip_len > 0) {
					devicedriver_send();
				}
			}
		}
	}
}

u16_t devicedriver_poll() {
	u16_t tmp = devicedriver_uip_len;
	devicedriver_uip_len = 0;
	return tmp;
}
