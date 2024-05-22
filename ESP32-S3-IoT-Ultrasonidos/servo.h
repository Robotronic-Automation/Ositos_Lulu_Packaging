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
void config_servo(const uint32_t pin);

/**
 * @brief Función para modificar el ángulo del servo
 * @param angle. Ángulo del servo al que se quiere establecer
 */
void servo_set_angle(const uint32_t angle);

#endif // SERVO_H

/*** End of file ****/