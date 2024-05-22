/**
 * @file  f_funciones.ino
 * @brief Implementación de funciones para controlar los LEDs y medir distancias con un sensor ultrasónico
 */

/**
 * @brief  Realiza la medición de distancia con el sensor ultrasónico
 * @param  TriggerPin. Pin asignado al pin Trigger del sensor ultrasónico
 * @param  EchoPin. Pin asignado al pin Echo del sensor ultrasónico
 * @return Distancia en centímetros al objeto más cercano
 */
int ping(const uint32_t TriggerPin, const uint32_t EchoPin) 
{
  /*
   * duration: tiempo entre pulsos
   * distanceCm: distancia en centímetros
   */
  long duration, distanceCm;
  
  // Generar un pulso limpio poniendo a LOW durante 4 microsegundos
  digitalWrite(TriggerPin, LOW);   
  delayMicroseconds(4);
  
  // Generar un pulso de Trigger (disparo) de 10 microsegundos
  digitalWrite(TriggerPin, HIGH);  
  delayMicroseconds(10);

  // Cambiar a LOW
  digitalWrite(TriggerPin, LOW);
  
  // Medir el tiempo entre pulsos, en microsegundos
  duration = pulseIn(EchoPin, HIGH);  
  
  // Convertir a distancia, en centímetros
  distanceCm = duration * 10 / 292/ 2;  

  return distanceCm;
}

/**
 * @brief   Enciende el LED correspondiente según el color indicado
 * @param   status. LED que se desea encender (verde o rojo)
 * @details Enciende el LED correspondiente al color proporcionado y apaga los otros
 */
void setColorLed(const char color[]) 
{
  if ( strcmp(color,"verde") == 0 ) 
  {
    digitalWrite( LED_ROJO, LOW );
    digitalWrite( LED_VERDE, HIGH );  
  } 
  else if ( strcmp(color,"rojo") == 0 )
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
 * @param   status. Estado del cobot (operando o inactivo)
 * @details Enciende o apaga el LED azul según el estado proporcionado 
 */
void setAzulLed(const char status[]) 
{
  if( strcmp(status,"operando") == 0 )
  {
    // Led azul encendido si el cobot está operando
    digitalWrite( LED_AZUL, HIGH );
  }
  else if( strcmp(status,"inactivo") == 0 )
  {
    // Led azul apagado si el cobot está inactivo
    digitalWrite( LED_AZUL, LOW );
  }
  else
  {
    warnln("**>> Solicitud no reconocida!");
  }
}

/*** End of file ****/
