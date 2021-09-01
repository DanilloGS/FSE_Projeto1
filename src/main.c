#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "bme280.h"
// #include "gpio.h"

void exit_program(int SIGNAL) {
  if (SIGINT) {
    printf("Desligando resistência e ventoinha\n");
    // turn_resistance_off();
    // turn_fan_off();
  };
}

int main() {
  signal(SIGINT, exit_program);
  float TR, hysteresis;
  printf("Defina uma temperatura de referência: ");
  scanf("%f", &TR);
  printf("Defina uma histerese: ");
  scanf("%f", &hysteresis);

  return 0;
}
