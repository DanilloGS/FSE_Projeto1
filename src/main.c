#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bme280.h"
#include "display.h"
#include "pid.h"
#include "thermometer.h"
#include "uart.h"

#define true 1
#define false 0
int continue_measurement = true;
// #include "gpio.h"

struct bme280_dev bme_connection;
int uart_filesystem;

void exit_program(int SIGNAL) {
  switch (SIGNAL) {
    case SIGINT:
      shutdown_program();
      break;
    case SIGTSTP:
      break;
    default:
      break;
  }
}

void shutdown_program() {
  printf("Encerrando programa\n");
  turn_resistance_off();
  turn_fan_off();
  close_uart(uart_filesystem);
  exit(0);
}

void stop_measurement() {
  continue_measurement = false;
}

void on_off_routine() {
  float hysteresis, TI, TR, TE;
  int value_to_send = 0;
  printf("Defina uma Histerese\n");
  scanf("%f", &hysteresis);
  while (continue_measurement) {
    write_uart_get(uart_filesystem, GET_INTERNAL_TEMP);
    TI = read_uart(uart_filesystem, GET_INTERNAL_TEMP).float_value;

    write_uart_get(uart_filesystem, GET_POTENTIOMETER);
    TR = read_uart(uart_filesystem, GET_POTENTIOMETER).float_value;

    TE = get_current_temperature(&bme_connection);

    if (TR - hysteresis >= TI) {
      turn_fan_off();
      turn_resistance_on(100);
      value_to_send = 100;
    } else if (TR + hysteresis <= TI) {
      turn_fan_on(100);
      turn_resistance_off();
      value_to_send = -100;
    }

    print_display(TI, TR, TE);
    write_uart_send(uart_filesystem, value_to_send);
  }
}

void terminal_routine() {
  int option;
  printf(
      "Como deseja controlar a temperatura?\n"
      "\t1) On-Off\n"
      "\t2) PID\n");
  scanf("%d", &option);
  switch (option) {
    case 1:
      on_off_routine();
      break;
    case 2:
      // pid_routine();
      break;
    case 3:
      shutdown_program();
      break;
    default:
      menu();
      break;
  }
}

void init() {
  wiringPiSetup();
  turn_resistance_off();
  turn_fan_off();
  pid_setup_constants(5, 1, 5);
  connect_display();
  bme_connection = connect_bme();
  uart_filesystem = connect_uart();
}

void menu() {
  init();
  int option;
  printf(
      "Como deseja definir a temperatura de referência do sistema?\n"
      "\t1) Terminal\n"
      "\t2) Potenciômetro\n");
  scanf("%d", &option);
  switch (option) {
    case 1:
      terminal_routine();
      break;
    case 2:
      // mechanic_routine();
      break;
    case 3:
      shutdown_program();
      break;
    default:
      menu();
      break;
  }
}

int main() {
  signal(SIGINT, exit_program);
  signal(SIGTSTP, exit_program);
  menu();
  return 0;
}
