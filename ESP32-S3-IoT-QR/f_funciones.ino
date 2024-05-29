/**
 * @file  f_funciones.ino
 * @brief Implementación de funciones para controlar los LEDs 
 */

/**
 * @brief   Enciende el LED correspondiente según el color indicado
 * @param   status. LED que se desea encender (verde o rojo)
 * @details Enciende el LED correspondiente al color proporcionado y apaga los otros
 */
void setColorLed(const char status[]) 
{
  if ( strcmp(status,"verde") == 0 ) 
  {
    traceln("Led verde encendido");
    digitalWrite( LED_ROJO, LOW );
    digitalWrite( LED_VERDE, HIGH );
    
  } 
  else if ( strcmp(status,"rojo") == 0 )
  {
    traceln("Led rojo encendido");
    digitalWrite( LED_ROJO, HIGH );
    digitalWrite( LED_VERDE, LOW );
    
  }
  else 
  {
    warnln("**>> Solicitud no reconocida!");
  }
  // TODO: Deberíamos publicar el estado del dispositivo cada vez que cambie
}

/*** End of file ****/
