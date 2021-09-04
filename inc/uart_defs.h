#ifndef UART_DEFS_H
#define UART_DEFS_H

#define GET_INTERNAL_TEMP 0xC1
#define GET_POTENTIOMETER 0xC2
#define GET_KEY_VALUE 0xC3
#define SEND_SIGNAL 0xD1

typedef struct Number_type {
  int int_value;
  float float_value;
} Number_type;

#endif /* UART_DEFS_H */
