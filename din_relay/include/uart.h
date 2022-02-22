#ifndef UART_H_
#define UART_H_

#include <stdio.h>

void UART_init();

bool UART_getc(uint8_t*);

void UART_putc(uint8_t c);

#endif /*UART_H_*/
