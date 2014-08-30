/*
 * driver_uart.h
 *
 *  Created on: May 7, 2014
 *      Author: kellogg
 */

#ifndef DRIVER_UART_H_
#define DRIVER_UART_H_

#ifdef __MSP430FR5969__
#define TX_BUF  UCA0TXBUF
#define TX_BUSY (!(UCA0IFG & UCTXIFG))
#endif // __MSP43FR5969__

#include <stdint.h>
#include "uip.h"

extern u16_t devicedriver_uip_len;

void driver_uart_init();

int driver_uart_send_char(char _c);

int driver_uart_send_buf(u8_t *buf, uint16_t len);

void driver_uart_recv();

void devicedriver_send();

#endif /* DRIVER_UART_H_ */
