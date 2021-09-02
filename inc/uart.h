#ifndef UART_H
#define UART_H

#include "uart_defs.h";

int connect_uart();
int write_uart(int filestream, unsigned char code, int send_data);
Interface_number_value read_uart(int filestream, unsigned char code);
void close_uart(int filestream);

#endif /* UART_H */
