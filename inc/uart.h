#ifndef UART_H
#define UART_H

#include "uart_defs.h"

int connect_uart();
void write_uart_get(int filestream, unsigned char code);
void write_uart_send(int filestream, int value);
Number_type read_uart(int filestream, unsigned char code);
void close_uart(int filestream);

#endif /* UART_H */
