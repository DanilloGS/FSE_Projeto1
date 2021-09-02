#include <fcntl.h>  //Used for UART
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>  //Used for UART
#include <unistd.h>   //Used for UART
#include "crc16.h"
#include "uart_defs.h"


int connect_uart() {
  int uart_filestream = -1;
  uart_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (uart_filestream == -1) {
    printf("Erro - Não foi possível iniciar a UART.\n");
  } else {
    printf("UART inicializado!\n");
  }
  struct termios options;
  tcgetattr(uart_filestream, &options);
  options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(uart_filestream, TCIFLUSH);
  tcsetattr(uart_filestream, TCSANOW, &options);
  return uart_filestream;
}

int write_uart(int filestream, unsigned char code, int send_data) {
  unsigned char function_code = 0x23;
  if (send_data) {
    function_code = 0x16;
  }
  unsigned char package[7] = {0x01, function_code, code, 0x09,
                              0x09, 0x08,          0x01};
  short crc = calcula_CRC(package, 7);
  unsigned char msg[9];
  memcpy(msg, &package, 7);
  memcpy(&msg[7], &crc, 2);
  int count = write(filestream, &msg[0], 9);
  sleep(1);
  return count;
}

Interface_number_value read_uart(int filestream, unsigned char code) {
  unsigned char buffer[20];
  Interface_number_value number = {0, 0};
  int count = read(filestream, buffer, 20);
  if (!count) {
    printf("Nenhum dado foi recebido\n");
  } else if (count < 0) {
    printf("Erro ao ler dados\n");
  } else {
    buffer[count] = '\0';
    if (code == 0xC3)
      memcpy(&number.int_value, &buffer[3], sizeof(int));
    else
      memcpy(&number.float_value, &buffer[3], sizeof(float));
    printf("%d - %f\n", number.int_value, number.float_value);
    return number;
  }
}

void close_uart(int filestream) { 
  printf("Conexão UART finalizada\n");
  close(filestream); 
}
