/**
 * @file  button_interrupt.h
 * @brief Funciones para gestionar un botón con interrupción
 */

#ifndef BUTTON_INTERRUPT_H
#define BUTTON_INTERRUPT_H

// Funciones para gestionar la interrupción del botón

/**
 * @brief   Función de la rutina de interrupción
 * @details Establece el indicador de botón presionado
 */
void IRAM_ATTR isr(); ///< Función de la rutina de interrupción

/**
 * @brief   Función para configurar la interrupción del botón
 * @details Configura el pin conectado al botón como entrada pull-up y
 *          asigna la rutina de servicio 'isr' para la interrupción
 *          cuando se produce un flanco de bajada
 */
void config_button(); ///< Función para configurar la interrupción del botón

#endif // BUTTON_INTERRUPT_H

/*** End of file ****/