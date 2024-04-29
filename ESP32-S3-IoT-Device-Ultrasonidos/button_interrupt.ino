/**
 @file button_interrupt.h
*/

#include <Arduino.h>
#include "button_interrupt.h"

void IRAM_ATTR isr() 
{
  button1.numberKeyPresses += 1;
  button1.pressed = true;
  PARAR = true;
}

void config_button()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
}

/*** End of file ****/