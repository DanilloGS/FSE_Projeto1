#include <gpio.h>
#include <softPwm.h>
#include <wiringPi.h>

#define RESISTOR_PIN 4
#define FAN_PIN 5

void turn_resistance_on(int new_resistor_value) {
  pinMode(RESISTOR_PIN, OUTPUT);
  softPwmCreate(RESISTOR_PIN, 0, 100);
  softPwmWrite(RESISTOR_PIN, new_resistor_value);
}

void turn_resistance_off() {
  pinMode(RESISTOR_PIN, OUTPUT);
  softPwmCreate(RESISTOR_PIN, 0, 100);
  softPwmWrite(RESISTOR_PIN, 0);
}

void turn_fan_on(int new_fan_value) {
  pinMode(FAN_PIN, OUTPUT);
  softPwmCreate(FAN_PIN, 0, 100);
  softPwmWrite(FAN_PIN, new_fan_value);
}

void turn_fan_off() {
  pinMode(FAN_PIN, OUTPUT);
  softPwmCreate(FAN_PIN, 0, 100);
  softPwmWrite(FAN_PIN, 0);
}

void pwm_control(int intensity_signal) {
  if (intensity_signal > 0) {
    turn_resistance_on(intensity_signal);
    turn_fan_off();
  } else {
    if (intensity_signal <= -40)
      turn_fan_on(intensity_signal * -1);
    else
      turn_fan_off();
    turn_resistance_off();
  }
}
