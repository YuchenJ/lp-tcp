#include <msp430.h>
#include "driver_uart.h"
#include "driver_timer.h"
#include "uip.h"
#include "timer.h"

extern u16_t devicedriver_uip_len;

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

	timer_set(&periodic_timer, CLOCK_SECOND / 2);

	uip_ipaddr_t dst, ipaddr;

	uip_ipaddr(ipaddr, 192,168,5,2);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, 192,168,5,1);
	uip_setdraddr(ipaddr);
	uip_ipaddr(ipaddr, 255,255,255,0);
	uip_setnetmask(ipaddr);

	uip_ipaddr(dst, 192,168,5,1);
	u16_t port = 1234;

	conn = uip_connect(&dst, htons(port));
	if (conn == 0) {
		_nop();
	}

	for(;;) {
		// Poll Device Driver
		uip_len = devicedriver_poll();
		if (uip_len > 0) {
			uip_input();
			if (uip_len > 0) devicedriver_send();
		} else if (timer_expired(&periodic_timer)) {
			timer_reset(&periodic_timer);
			// Periodic Processing of connections
			for(i = 0; i < UIP_CONNS; ++i) {
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

/*void init_clocks() {

}*/
