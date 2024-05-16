/**
 @file button_interrupt.h
*/

#ifndef BUTTON_INTERRUPT_H
#define BUTTON_INTERRUPT_H

#include <Arduino.h>

/**
 @brief  Button . Estructura para alamacenar el estado de un boton
 @member PIN. Pin al que esta conectado el boton
 @member numberKeyPresses. Numero de veces que se presiona el boton
 @member pressed. Variable que indica si se ha presionado el boton 
*/
struct Button 
{
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

// Instancia de boton asignando pin, numVecesPulsado y no pulsado
Button button1 = { PIN_BUTTON, 0, false };

// Funcion para gestion de la interrupcion
void IRAM_ATTR isr();
// Funcion para configurar la interrupcion
void config_button();

#endif 

/*** End of file ****/