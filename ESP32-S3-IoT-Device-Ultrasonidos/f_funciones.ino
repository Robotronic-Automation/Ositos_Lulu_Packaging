/**
 @file f_funciones.ino
*/

/**
 @brief setColorLed. Funcion que modifica el color del led segun el estado del cobot
 @param status. string que contiene el color del led 
 @return ninguno
*/
void setColorLed(String status) 
{
  if ( status == "verde" ) 
  {

    digitalWrite( LED_ROJO, LOW );
    digitalWrite( LED_VERDE, HIGH );
    digitalWrite( LED_AZUL, HIGH );

  } else if ( status == "rojo" )
  {

    digitalWrite( LED_ROJO, HIGH );
    digitalWrite( LED_VERDE, LOW );
    digitalWrite( LED_AZUL, LOW );
    
  } else 
  {
    warnln("**>> Solicitud no reconocida!");
  }
  // TODO: Deberíamos publicar el estado del dispositivo cada vez que cambie
}

/**
 @brief ping. Funcion que realiza la medida del sensor ultrasonido
 @param TriggerPin. Pin asignado a Trigger
 @param EchoPin. Pin asignado a Echo
 @return distanceCm. Devuelve la distancia en cm al objeto más cercano
*/
int ping(int TriggerPin, int EchoPin) 
{
  // Duration : tiempo entre pulsos
  // distanceCm : distancia en cm
  long duration, distanceCm;
  
  // Para generar un pulso limpio ponemos a LOW 4us
  digitalWrite(TriggerPin, LOW);   
  delayMicroseconds(4);
  // Generamos Trigger (disparo) de 10us
  digitalWrite(TriggerPin, HIGH);  
  delayMicroseconds(10);
  // Cambiamos a LOW
  digitalWrite(TriggerPin, LOW);
  
  // Medimos el tiempo entre pulsos, en microsegundos
  duration = pulseIn(EchoPin, HIGH);  
  
  // Convertimos a distancia, en cm
  distanceCm = duration * 10 / 292/ 2;  

  return distanceCm;
}

/*** End of file ****/
