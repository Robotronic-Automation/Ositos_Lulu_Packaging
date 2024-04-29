/**
 @file button_interrupt.h
*/

#ifndef BUTTON_INTERRUPT_H
#define BUTTON_INTERRUPT_H
#include <Arduino.h>

#define LED_1  2
#define LED_2  3
#define PIN_BUTTON 13
#define T_espera 100
// the setup function runs once when you press reset or power the board

struct Button 
{
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button1 = {PIN_BUTTON, 0, false};
volatile bool PARAR = false;

void IRAM_ATTR isr();
void config_button();

#endif 

/*** End of file ****/