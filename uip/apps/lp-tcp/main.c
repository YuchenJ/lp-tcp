#include <msp430.h>
#include "driver_uart.h"
#include "uip.h"

int main(void) {
	struct uip_conn *conn;
	int i;

	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	driver_uart_init();
	uip_init();

	uip_ipaddr_t dst = {0};
	u16_t port = 10;

	conn = uip_connect(&dst, hton(port));
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
