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
	//DCOCTL = 0;
	//BCSCTL1 = CALBC1_1MHZ;
	//DCOCTL = CALDCO_1MHZ;

	/*** Configure UART ***/
	/*P1SEL0 |= BIT1 | BIT2;          // P1.1,2 = USCI_A0 TXD/RXD
	P1SEL1 |= BIT1 | BIT2;          // P1.1,2 = USCI_A0 TXD/RXD*/
/*
	UCA0CTL1 |= UCSWRST;           // Put UART in reset
	UCA0CTL1 |= UCSSEL__ACLK;          // SMCLK
	UCA0BR0 = 3;                 // 9600
	UCA0BR1 = 0;                   // 9600
	UCA0MCTLW = 0x5300;             // Modulation, Oversampling, UCBRSx = 0, UCBRFx = 1
	UCA0CTL1 &= ~UCSWRST;          // Initialize USCI state machine
*/
	// Configure GPIO
	  P2SEL1 |= BIT0 | BIT1;                    // USCI_A0 UART operation
	  P2SEL0 &= ~(BIT0 | BIT1);
	  PJSEL0 |= BIT4 | BIT5;                    // For XT1

	  // Disable the GPIO power-on default high-impedance mode to activate
	  // previously configured port settings
	  PM5CTL0 &= ~LOCKLPM5;

	  // XT1 Setup
	  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
	  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
	  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
	  CSCTL4 &= ~LFXTOFF;
	  do
	  {
	    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
	    SFRIFG1 &= ~OFIFG;
	  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
	  CSCTL0_H = 0;                             // Lock CS registers

	  UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
	  UCA0CTLW0 |= UCSSEL__ACLK;                // CLK = ACLK
	  UCA0BR0 = 3;                              // 9600 baud
	  UCA0MCTLW |= 0x5300;                      // 32768/9600 - INT(32768/9600)=0.41
	                                             // UCBRSx value = 0x53 (See UG)
	  UCA0BR1 = 0;
	  UCA0CTL1 &= ~UCSWRST;                     // Initialize eUSCI
	  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

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

#pragma vector=USCI_A0_VECTOR
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
