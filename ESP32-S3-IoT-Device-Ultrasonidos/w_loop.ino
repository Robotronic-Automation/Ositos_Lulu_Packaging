/**
 * @file  w_loop.ino
 * @brief Archivo que contiene la implementación de las tareas a realizar
 *        dentro del ciclo principal
 */

#include <string>

/**
 * @brief Función que define las tareas a realizar dentro del ciclo principal (loop)
 */
void on_loop() 
{
  // Mensaje de inicio del ciclo principal
  Serial.println("Inicio Main");

  // Bucle principal
  while (!PARAR)
  {
    // Verifica si el botón ha sido pulsado
    if (button1.pressed) 
    {
      
      // Restablece el estado del botón pulsado
      button1.pressed = false;

    }

    // Breve retraso para evitar bucle de lecturas intensivas
    delay(100);
  }

  
  // Enciende ambos LEDs
  digitalWrite( LED_VERDE, HIGH );
  digitalWrite( LED_ROJO, HIGH );

  // Mensaje de cierre del ciclo principal
  Serial.println("CERRANDO");

  // Breve retraso antes de finalizar
  delay(3000);

  // Apaga ambos LEDs
  digitalWrite( LED_VERDE, LOW );
  digitalWrite( LED_ROJO, LOW );

  // Mensaje de finalización del ciclo principal
  Serial.println("FIN Main");

  // Finaliza el programa
  exit(0);
}

/*** End of file ****/