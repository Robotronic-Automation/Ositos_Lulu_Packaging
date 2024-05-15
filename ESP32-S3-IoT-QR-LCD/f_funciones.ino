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

/*** End of file ****/
