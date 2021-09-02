#include "pid.h"

#include <stdio.h>

double saida_medida, sinal_de_controle;
double reference = 0.0;
double Kp = 0.0;  // Ganho Proporcional
double Ki = 0.0;  // Ganho Integral
double Kd = 0.0;  // Ganho Derivativo
int T = 1.0;      // Período de Amostragem (ms)
unsigned long last_time;
double error_total, previous_error = 0.0;
int signal_MAX = 100.0;
int signal_MIN = -100.0;

void pid_setup_constants(double Kp_, double Ki_, double Kd_) {
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
}

void pid_update_reference(float reference_) {
  reference = (double)reference_;
}

double pid_control(double saida_medida) {
  double error = reference - saida_medida;

  error_total += error;  // Acumula o error (Termo Integral)

  if (error_total >= signal_MAX) {
    error_total = signal_MAX;
  } else if (error_total <= signal_MIN) {
    error_total = signal_MIN;
  }

  double delta_error =
      error - previous_error;  // Diferença entre os erros (Termo Derivativo)

  sinal_de_controle = Kp * error + (Ki * T) * error_total +
                      (Kd / T) * delta_error;  // PID calcula sinal de controle

  if (sinal_de_controle >= signal_MAX) {
    sinal_de_controle = signal_MAX;
  } else if (sinal_de_controle <= signal_MIN) {
    sinal_de_controle = signal_MIN;
  }

  previous_error = error;

  return sinal_de_controle;
}
