/**
 * @file button_interrupt.h
 * @brief Definiciones y funciones para gestionar un botón con interrupción
 */

#ifndef BUTTON_INTERRUPT_H
#define BUTTON_INTERRUPT_H

#include <Arduino.h>

/**
 * @struct  Button
 * @brief   Estructura para almacenar el estado de un botón.
 * @details Contiene el pin al que está conectado el botón y 
 *          un indicador de si el botón está actualmente presionado.
 */
struct Button 
{
  const uint8_t PIN;  ///< Pin al que está conectado el botón
  bool pressed;       ///< Indica si el botón está presionado en este momento
};

// Instancia de botón asignando pin y no pulsado
Button button1 = { PIN_BUTTON, false };

// Funciones para gestionar la interrupción del botón
void IRAM_ATTR isr(); ///< Función de la rutina de interrupción
void config_button(); ///< Función para configurar la interrupción del botón

#endif 

/*** End of file ****/