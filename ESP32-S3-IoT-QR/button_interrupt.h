/**
 @file button_interrupt.h
*/

#ifndef BUTTON_INTERRUPT_H
#define BUTTON_INTERRUPT_H

#include <Arduino.h>

struct Button 
{
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};
Button button1 = { PIN_BUTTON, 0, false };

void IRAM_ATTR isr();
void config_button();
#endif 

/*** End of file ****/