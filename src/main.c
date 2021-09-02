#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bme280.h"
#include "lcd.h"
#include "pid.h"
#include "thermometer.h"
#define true 1
#define false 0
// #include "gpio.h"

struct bme_connection;

void exit_program(int SIGNAL) {
  if (SIGINT) {
    printf("Desligando resistência e ventoinha\n");
    turn_resistance_off();
    turn_fan_off();
    // close_uart();
    exit(0);
  };
}

void init() {
  wiringPiSetup();
  turn_resistance_off();
  turn_fan_off();
  pid_setup_constants(Kp, Ki, Kd);
  lcd_init();
  bme_connection = init_thermometer();
}

void on_off_process() {}

void pid_process() {}

void mechanic_pocess() {}

void start_program() {
  int option;
  float TR, hysteresis;
  printf("Como deseja controlar a temperatura do sistema?\n" +
         "\t1)Controle On-Off\n" + "\t2) PID\n" + "\t3) Controle mecânico\n");
  scanf("%d", &option);
  if (option == 1)
    on_off_process();
  else if (option == 2)
    pid_process();
  else if (option == 3)
    mechanic_process();
  else {
    printf("Opção invalida\n");
    start_program();
  }
}

int main() {
  signal(SIGINT, exit_program);
  init();
  start_program();
  // printf("Defina uma temperatura de referência: ");
  // scanf("%f", &TR);
  // printf("Defina uma histerese: ");
  // scanf("%f", &hysteresis);

  struct bme280_dev dev = init_thermometer();
  stream_sensor_data_forced_mode(&dev);
  return 0;
}
