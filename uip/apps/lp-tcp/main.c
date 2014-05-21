#include <msp430.h>
#include "driver_uart.h"
#include "driver_timer.h"
#include "uip.h"

int main(void) {
	struct uip_conn *conn;
	int i;

	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

//	init_clocks();
	driver_uart_init();
	uip_init();
	timer_init();
	_BIS_SR(GIE);

	uip_ipaddr_t dst, ipaddr;

	uip_ipaddr(ipaddr, 192,168,5,2);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, 192,168,5,1);
	uip_setdraddr(ipaddr);
	uip_ipaddr(ipaddr, 255,255,255,0);
	uip_setnetmask(ipaddr);

	uip_ipaddr(dst, 173,194,33,112);
	u16_t port = 80;

	conn = uip_connect(&dst, htons(port));
	if (conn == 0) {
		_nop();
	}

	for(;;) {
		for(i = 0; i < UIP_CONNS; ++i) {
		    uip_periodic(i);
		    if(uip_len > 0) {
		      devicedriver_send();
		    }
		}
	}
}

/*void init_clocks() {

}*/
