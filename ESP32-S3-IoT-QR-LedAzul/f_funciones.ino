/**
 * @file  f_funciones.ino
 * @brief Implementación de funciones para controlar los LEDs y medir distancias con un sensor ultrasónico
 */

/**
 * @brief   Modifica el color del LED según el color indicado
 * @param   status. Color deseado para el LED (verde o rojo)
 * @details Enciende el LED correspondiente al color proporcionado y apaga los otros
 */
void setColorLed(const char status[]) 
{
  if ( strcmp(status,"verde") == 0 ) 
  {
    digitalWrite( LED_ROJO, LOW );
    digitalWrite( LED_VERDE, HIGH );
  } 
  else if ( strcmp(status,"rojo") == 0 )
  {
    digitalWrite( LED_ROJO, HIGH );
    digitalWrite( LED_VERDE, LOW ); 
  }
  else 
  {
    warnln("**>> Solicitud no reconocida!");
  }

}

/**
 * @brief   Modifica el estado del LED azul 
 * @param   status. Estado del LED azul (azul_on o azul_off)
 * @details Enciende o apaga el LED azul según el estado proporcionado 
 */
void setAzulLed(const char status[]) 
{
  if( strcmp(status,"azul_on") == 0 )
  {
    digitalWrite( LED_AZUL, HIGH );
  }
  else if( strcmp(status,"azul_off") == 0 )
  {
    digitalWrite( LED_AZUL, LOW );
  }
  else
  {
    warnln("**>> Solicitud no reconocida!");
  }
}

/*** End of file ****/
