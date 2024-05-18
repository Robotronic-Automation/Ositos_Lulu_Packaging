/**
 * @file  servo.h
 * @brief Funciones para manejar un servomotor
 */

#ifndef SERVO_H
#define SERVO_H

// Funciones para el manejo del servo

/**
 * @brief Función para configurar el servo
 * @param pin. Pin conectado al servo
 */
void servo_set_pin(int pin);

/**
 * @brief Función para modificar el ángulo del servo
 * @param angle. Ángulo del servo al que se quiere establecer
 */
void servo_set_angle(int angle);

#endif // SERVO_H

/*** End of file ****/