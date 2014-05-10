/*
 * driver_uart.c
 *
 *  Created on: May 7, 2014
 *      Author: kellogg
 */
#include <msp430.h>
#include <stdint.h>

#include "driver_uart.h"

void driver_uart_init() {
	/*** Configure Clock ***/
	DCOCTL = 0;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	/*** Configure UART ***/
	P1SEL |= BIT1 | BIT2;          // P1.1,2 = USCI_A0 TXD/RXD
	P1SEL2 |= BIT1 | BIT2;          // P1.1,2 = USCI_A0 TXD/RXD
	UCA0CTL1 |= UCSWRST;           // Put UART in reset
	UCA0CTL1 |= UCSSEL_2;          // SMCLK
	UCA0BR0 = 104;                 // 9600
	UCA0BR1 = 0;                   // 9600
	UCA0MCTL = UCBRS0;             // Modulation, Oversampling, UCBRSx = 0, UCBRFx = 1
	UCA0CTL1 &= ~UCSWRST;          // Initialize USCI state machine

	IE2 |= UCA0RXIE;
}

int driver_uart_send_char(char _c) {
	while(TX_BUSY);
	return TX_BUF = (unsigned char) _c;
}

int driver_uart_send_buf(char *buf, uint16_t len) {
	uint16_t i;
	uint8_t res;
	for (i = 0; i < len; i++) {
		res = driver_uart_send_char(buf[i]);
	}
	return res;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
	_nop();
	driver_uart_send_char(UCA0RXBUF);
}
