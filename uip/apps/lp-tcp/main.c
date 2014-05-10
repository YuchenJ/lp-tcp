#include <msp430.h>
#include "driver_uart.h"
#include "uip.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	driver_uart_init();
	uip_init();

	for(;;) {
		uip_ipaddr_t dst = {0};
		u16_t port = 10;

		uip_connect(&dst, port);
		uip_input();

		_nop();

		volatile unsigned int i;	// volatile to prevent optimization

		driver_uart_send_char('H');

		i = 10000;					// SW Delay
		do i--;
		while(i != 0);
	}
}
