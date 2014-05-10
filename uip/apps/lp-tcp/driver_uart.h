/*
 * driver_uart.h
 *
 *  Created on: May 7, 2014
 *      Author: kellogg
 */

#ifndef DRIVER_UART_H_
#define DRIVER_UART_H_

#ifdef __MSP430G2553__
#define TX_BUF  UCA0TXBUF
#define TX_BUSY (!(UCA0TXIFG & IFG2))
#endif // __MSP430G2553__

#include <stdint.h>

void driver_uart_init();

int driver_uart_send_char(char _c);

int driver_uart_send_buf(char *buf, uint16_t len);

void driver_uart_recv();

#endif /* DRIVER_UART_H_ */
