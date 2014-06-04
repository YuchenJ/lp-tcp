/*
 * lp-tcp.c
 *
 *  Created on: May 12, 2014
 *      Author: kellogg
 */

#include "lp-tcp.h"
#include "uip.h"
#include "msp430.h"

void lp_tcp_appcall(void) {
	struct lp_tcp_state *s;

	s = &(uip_conn->appstate);

	if (uip_connected()) {
		s->state = WELCOME_SENT;
		uip_send("Handshake complete!\n", 20);
		return;
	}

	if (uip_acked() && s->state == WELCOME_SENT) {
		s->state = WELCOME_ACKED;
	}

	if (uip_newdata()) {
		uip_send("Received packet\n", 16);
	}

	if (uip_rexmit()) {
		switch(s->state) {
		case WELCOME_SENT:
			uip_send("Handshake complete!\n", 20);
			break;
		case WELCOME_ACKED:
			uip_send("Received packet\n", 16);
			break;
		}
	}
}
