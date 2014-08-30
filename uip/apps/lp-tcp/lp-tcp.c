/*#include "lp-tcp.h"
#include "uip.h"
#include "msp430.h"
#include "memb.h"
#include <string.h>

int i = 0;
char handshake[21]= {0xff, 0xfb, 0x1f, 0xff, 0xfb, 0x20, 0xff, 0xfb,0x18,0xff,0xfb,0x27,0xff,0xfd,0x01,0xff,0xfb,0x03,0xff,0xfd,0x03};


void lp_tcp_appcall(void) {
	struct lp_tcp_state *s;
	s = &(uip_conn->appstate);

	if (uip_connected()) {
		s->state = WELCOME_SENT;
		//uip_send(handshake, 21);
		//uip_send("welcome", 7);
		return;
	}

	if (uip_acked() && s->state == WELCOME_SENT) {
		s->state = WELCOME_ACKED;
	}

	if (uip_newdata()) {

		char negotiate1[12] = {0xff, 0xfc, 0x23, 0xff, 0xfa, 0x1f, 0x00, 0x50, 0x00, 0x18,0xff, 0xf0} ;
        char negotiate2[34] = {0xff, 0xfa, 0x20, 0x00, 0x33, 0x38, 0x34, 0x30, 0x30, 0x2c, 0x33, 0x38, 0x34, 0x30, 0x30, 0xff, 0xf0, 0xff, 0xfa, 0x27, 0x00, 0xff, 0xf0, 0xff,0xfa,0x18,0x00,0x58,0x54,0x45,0x52,0x4d,0xff,0xf0};
        char negotiate3[9] = {0xff, 0xfc, 0x01, 0xff, 0xfe, 0x05, 0xff, 0xfc, 0x21};

		if(i == 0)
		uip_send(handshake, 21);
		if(i == 1)
		uip_send(negotiate1, 12);
		if(i == 2)
		uip_send(negotiate2, 34);
		if(i == 3)
		uip_send(negotiate3, 9);
		if(i == 4)
		uip_send("king", 4);    //Login
		if(i == 5)
		uip_send("\r\n", 2);
		if(i == 6)
		uip_send("111666", 6);  //Password
		if(i == 7)
		uip_send("\r\n", 2);
		if(i == 8)
		uip_send("ls", 2);      //Command
		if(i == 9)
		uip_send("\r\n", 2);

		//P1OUT ^= BIT0;
		//uip_send("Received packet\n", 16);
		if(i ++ == 10)
		uip_close();           //close telnet connection
	}

	if (uip_rexmit()) {
		switch(s->state) {
		case WELCOME_SENT:
			uip_send(handshake, 21);
			break;
		case WELCOME_ACKED:
			uip_send("ACK", 3);
			break;
		}
	}
}*/




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
		P1OUT ^= BIT0;
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
