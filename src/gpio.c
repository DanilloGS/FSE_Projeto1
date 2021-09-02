#include <gpio.h>
#include <softPwm.h>
#include <wiringPi.h>

#define MIN_RANGE 0
#define MAX_RANGE 100
#define RESISTOR_PIN 4
#define FAN_PIN 5

void turn_resistance_on(int new_resistor_value) {
  pinMode(RESISTOR_PIN, OUTPUT);
  softPwmCreate(RESISTOR_PIN, MIN_RANGE, MAX_RANGE);
  softPwmWrite(RESISTOR_PIN, new_resistor_value);
}

void turn_resistance_off() {
  pinMode(RESISTOR_PIN, OUTPUT);
  softPwmCreate(RESISTOR_PIN, MIN_RANGE, MAX_RANGE);
  softPwmWrite(RESISTOR_PIN, MIN_RANGE);
}

void turn_fan_on(int new_fan_value) {
  pinMode(FAN_PIN, OUTPUT);
  softPwmCreate(FAN_PIN, MIN_RANGE, MAX_RANGE);
  softPwmWrite(FAN_PIN, new_fan_value);
}

void turn_fan_off() {
  pinMode(FAN_PIN, OUTPUT);
  softPwmCreate(FAN_PIN, MIN_RANGE, MAX_RANGE);
  softPwmWrite(FAN_PIN, MIN_RANGE);
}

void control_system(int control_signal) {
  if (control_signal > 0) {
    turn_resistance_on(control_signal);
    turn_fan_off();
  } else {
    if (control_signal <= -40) {
      turn_fan_on(control_signal * -1);
    } else {
      turn_fan_off();
    }

    turn_resistance_off();
  }
}
