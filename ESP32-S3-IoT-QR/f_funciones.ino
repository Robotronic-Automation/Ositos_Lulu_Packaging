/**
 * @file  f_funciones.ino
 * @brief Implementación de funciones para controlar los LEDs y medir distancias con un sensor ultrasónico
 */

/**
 * @brief   Modifica el color del LED según el color indicado
 * @param   status Color deseado para el LED (verde o rojo)
 * @details Enciende el LED correspondiente al color proporcionado y apaga los otros
 */
void setColorLed(const char status[]) 
{
  if ( strcmp(status,"verde") == 0 ) 
  {
    printf("Led verde encendido\n");
    digitalWrite( LED_ROJO, LOW );
    digitalWrite( LED_VERDE, HIGH );
    
  } 
  else if ( strcmp(status,"rojo") == 0 )
  {
    printf("Led rojo encendido\n");
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
