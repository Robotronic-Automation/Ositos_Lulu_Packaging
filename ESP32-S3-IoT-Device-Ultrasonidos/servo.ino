/**
 * @file  servo.ino
 * @brief Implementación de funciones para manejar un servomotor
 */

#include "servo.h"

/**
 * @brief Función para configurar el servo
 * @param pin. Pin conectado al servo
 */
void servo_set_pin(int pin)
{
  ledcSetup(SERVO_CHN, SERVO_FRQ, SERVO_BIT);
  ledcAttachPin(pin, SERVO_CHN);
}

/**
 * @brief Función para modificar el ángulo del servo
 * @param angle. Ángulo del servo al que se quiere establecer
 */
void servo_set_angle(int angle)
{
  if (angle > 180 || angle < 0)
  {
    errorln("Servo: Angulo no valido");
    return;
  }
  // La función map cambia la escala
  //  de 0 180 (angulo) a los ticks del temporizador PWM
  // El temporizador es de 12 bits (definido en el SERVO_BIT )
  //  por tanto pwm_value en ticks es
  // (duty(microseg)*(2**12))/20000 (periodo en microseg)
  //  duty*4096/20000
  long pwm_value = map(angle, 0, 180, 103, 512);
  ledcWrite(SERVO_CHN, pwm_value);
}

/*** End of file ****/