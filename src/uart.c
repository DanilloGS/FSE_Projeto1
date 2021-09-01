// #include <fcntl.h>  //Used for UART
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <termios.h>  //Used for UART
// #include <unistd.h>   //Used for UART

// #include "crc16.h"

// char *substring(unsigned char *string, int position, int length) {
//   char *p;
//   int c;
//   printf("---> %s\n", string);
//   p = malloc(length + 1);

//   if (p == NULL) {
//     printf("Unable to allocate memory.\n");
//     exit(1);
//   }

//   for (c = 0; c < length; c++) {
//     *(p + c) = *(string + position - 1);
//     string++;
//   }

//   *(p + c) = '\0';

//   return p;
// }

// int connect_uart() {
//   int uart_filestream = -1;
//   uart_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
//   if (uart_filestream == -1) {
//     printf("Erro - Não foi possível iniciar a UART.\n");
//   } else {
//     printf("UART inicializado!\n");
//   }
//   struct termios options;
//   tcgetattr(uart_filestream, &options);
//   options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
//   options.c_iflag = IGNPAR;
//   options.c_oflag = 0;
//   options.c_lflag = 0;
//   tcflush(uart_filestream, TCIFLUSH);
//   tcsetattr(uart_filestream, TCSANOW, &options);
//   return uart_filestream;
// }

// int write_uart(int filestream, unsigned char code, int send_data) {
//   unsigned char function_code = 0x23;
//   if (send_data) {
//     function_code = 0x16;
//   }
//   unsigned char package[7] = {0x01, function_code, code, 0x09, 0x09, 0x08, 0x01};
//   short crc = calcula_CRC(package, 7);
//   unsigned char msg[9];
//   memcpy(msg, &package, 7);
//   memcpy(&msg[7], &crc, 2);
//   int count = write(filestream, &msg[0], 9);
//   sleep(1);
//   return count;
// }

// void read_uart(int filestream, unsigned char code) {
//   unsigned char rx_buffer[256];
//   int rx_length = read(filestream, (void *)rx_buffer, 255);

//   char *new_string = malloc(sizeof(char) * rx_length - 4);

//   printf("%d\n", rx_length);
//   if (rx_length < 0) {
//     printf("Error during read of UART - RX\n");
//   } else if (rx_length == 0) {
//     printf("There is no content available\n");
//   } else {
//     rx_buffer[rx_length] = '\0';
//     new_string = substring(rx_buffer, 4, rx_length - 2);
//     if (code == 0xC3) {
//       int currentInteger = 0;
//       memcpy(&currentInteger, new_string, strlen(new_string));

//       printf("int: %d\n", currentInteger);
//     } else if (code == 0xC1 || code == 0xC2) {
//       float currentFloat = 0;
//       memcpy(&currentFloat, new_string, strlen(new_string));
//       printf("float: %f\n", currentFloat);
//     }
//   }

//   free(new_string);
// }

// int main(int argc, char const *argv[]) {
//   int uart_device = -1;

//   uart_device = connect_uart();
//   write_uart(uart_device, 0xA1, 0);
//   read_uart(uart_device, 0xA1);

//   close(uart_device);
//   return 0;
// }
