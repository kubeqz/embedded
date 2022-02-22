
#include <inttypes.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "uart.h"

#define BAUD 19200

#define MYUBBR ((F_CPU / (BAUD * 16L)) - 1)
#define BUFFER_SIZE 16

volatile static uint8_t rx_buffer[BUFFER_SIZE] = {};
volatile static uint8_t tx_buffer[BUFFER_SIZE] = {};
volatile static uint8_t rx_head = 0;
volatile static uint8_t rx_tail = 0;
volatile static uint8_t tx_head = 0;
volatile static uint8_t tx_tail = 0;
volatile static uint8_t sent = true;

void UART_init()
{
  // set baud rate
  UBRRH = (uint8_t)(MYUBBR >> 8);
  UBRRL = (uint8_t)(MYUBBR);
  // enable receive and transmit
  UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
  // set frame format
  UCSRC = (1 << USBS) | (3 << UCSZ0); // asynchron 8n1
}

/*
 * send_uart
 * Sends a single char to UART without ISR
 */
void send_uart(uint8_t c)
{
  // wait for empty data register
  while (!(UCSRA & (1 << UDRE)))
    ;
  // set data into data register
  UDR = c;
}

/*
 * receive_uart
 * Receives a single char without ISR
 */
uint8_t receive_uart()
{
  while (!(UCSRA & (1 << RXC)))
    ;
  return UDR;
}

/*
 * uart_getc
 * Gets a single char from the receive buffer.
 * return	uint16_r	the received char or UART_NO_DATA
 */
bool UART_getc(uint8_t* byte)
{
  uint8_t c = 0;
  uint8_t tmp_tail = 0;

  if (rx_head == rx_tail) {
    return false;
  }
  tmp_tail = (rx_tail + 1) % BUFFER_SIZE;
  c = rx_buffer[rx_tail];
  rx_tail = tmp_tail;
  *byte = c;

  return true;
}

void UART_putc(uint8_t c)
{
  uint8_t tmp_head = (tx_head + 1) % BUFFER_SIZE;
  // wait for space in buffer
  while (tmp_head == tx_tail) {}
  tx_buffer[tx_head] = c;
  tx_head = tmp_head;
  // enable uart data interrupt (send data)
  UCSRB |= (1 << UDRIE);
}

extern "C" {

/*
 * ISR User Data Regiser Empty
 * Send a char out of buffer via UART. If sending is complete, the
 * interrupt gets disabled.
 */
ISR(USART_UDRE_vect)
{
  uint8_t tmp_tail = 0;
  if (tx_head != tx_tail)
  {
    tmp_tail = (tx_tail + 1) % BUFFER_SIZE;
    UDR = tx_buffer[tx_tail];
    tx_tail = tmp_tail;
  }
  else
  {
    // disable this interrupt if nothing more to send
    UCSRB &= ~(1 << UDRIE);
  }
}

/*
 * ISR RX complete
 * Receives a char from UART and stores it in ring buffer.
 */
ISR(USART_RX_vect)
{
  uint8_t tmp_head = 0;
  tmp_head = (rx_head + 1) % BUFFER_SIZE;
  if (tmp_head == rx_tail)
  {
    // buffer overflow error!
  }
  else
  {
    rx_buffer[rx_head] = UDR;
    rx_head = tmp_head;
  }
}

} // extern "C"