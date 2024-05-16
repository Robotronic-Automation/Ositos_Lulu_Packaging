/**
 * @file button_interrupt.ino
 * @brief Implementación de funciones para gestionar un botón con interrupción
 */

#include "button_interrupt.h"

/**
 * @brief   Función de la rutina de interrupción
 * @details Establece el indicador de botón presionado
 */
void IRAM_ATTR isr() 
{
  button1.pressed = true;
  PARAR = true; // Activa la variable global para detener el programa en caso de emergencia
}

/**
 * @brief   Función para configurar la interrupción del botón
 * @details Configura el pin conectado al botón como entrada pull-up y
 *          asigna la rutina de servicio 'isr' para la interrupción
 *          cuando se produce un flanco de bajada
 */
void config_button()
{
  // Configurar el pin conectado al botón como entrada pull-up
  pinMode(button1.PIN, INPUT_PULLUP);

  /* 
   * Asignar la rutina de servicio 'isr' para la interrupción  
   * cuando se produce un flanco de bajada
   */
  attachInterrupt(button1.PIN, isr, FALLING);
}

/*** End of file ****/