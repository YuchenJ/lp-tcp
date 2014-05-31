/*
 * driver_uart.c
 *
 *  Created on: May 7, 2014
 *      Author: kellogg
 */
#include <msp430.h>
#include <stdint.h>

#include "driver_uart.h"
#include "uip.h"

#define SLIP_END 0xC0
#define SLIP_ESC 0xDB
#define SLIP_ESC_END 0xDC
#define SLIP_ESC_ESC 0xDD

u16_t devicedriver_uip_len;
u16_t _devicedriver_uip_len;  // Internal use only
u8_t _slip_esc_flag;

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

int driver_uart_send_buf(u8_t *buf, uint16_t len) {
	uint16_t i;
	uint8_t res;
	for (i = 0; i < len; i++) {
		switch(buf[i]) {
		case SLIP_END:
			driver_uart_send_char(SLIP_ESC);
			driver_uart_send_char(SLIP_ESC_END);
			break;
		case SLIP_ESC:
			driver_uart_send_char(SLIP_ESC);
			driver_uart_send_char(SLIP_ESC_ESC);
			break;
		default:
			res = driver_uart_send_char(buf[i]);
		}

	}
	return res;
}

void devicedriver_send() {
	driver_uart_send_char(SLIP_END);
	driver_uart_send_buf(uip_buf, uip_len);
	driver_uart_send_char(SLIP_END);
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
	u8_t tmp = UCA0RXBUF;
	switch(tmp) {
	case SLIP_ESC:
		_slip_esc_flag = 1;
		break;
	case SLIP_ESC_END:
		if (_slip_esc_flag == 1) {
			_slip_esc_flag = 0;
			uip_buf[_devicedriver_uip_len++] = SLIP_END;
		} else {
			uip_buf[_devicedriver_uip_len++] = SLIP_ESC_END;
		}
		break;
	case SLIP_ESC_ESC:
		if (_slip_esc_flag == 1) {
			_slip_esc_flag = 0;
			uip_buf[_devicedriver_uip_len++] = SLIP_ESC;
		} else {
			uip_buf[_devicedriver_uip_len++] = SLIP_ESC_ESC;
		}
		break;
	case SLIP_END:
		if (_devicedriver_uip_len != 0) {
			devicedriver_uip_len = _devicedriver_uip_len;
			_devicedriver_uip_len = 0;
			//P1OUT ^= BIT0;
		}
		break;
	default:
		uip_buf[_devicedriver_uip_len++] = tmp;
	}
}
