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
  else if( strcmp(status,"azul_on") == 0 )
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
  // TODO: Deberíamos publicar el estado del dispositivo cada vez que cambie
}

/**
 * @brief  Realiza la medición de distancia con el sensor ultrasónico
 * @param  TriggerPin. Pin asignado al pin Trigger del sensor ultrasónico
 * @param  EchoPin. Pin asignado al pin Echo del sensor ultrasónico
 * @return Distancia en centímetros al objeto más cercano
 */
int ping(const int TriggerPin, const int EchoPin) 
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

/*** End of file ****/
