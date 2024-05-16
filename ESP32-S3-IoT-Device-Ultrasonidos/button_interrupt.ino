/**
 @file button_interrupt.ino
*/

#include "button_interrupt.h"

/**
 @brief isr. Funcion para gestion de la interrupcion
 @param  ninguno
 @return ninguno
*/
void IRAM_ATTR isr() 
{
  button1.numberKeyPresses += 1;
  button1.pressed = true;
  PARAR = true;
}

/**
 @brief isr. Funcion para configurar la interrupcion
 @param  ninguno
 @return ninguno
*/
void config_button()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
}

/*** End of file ****/