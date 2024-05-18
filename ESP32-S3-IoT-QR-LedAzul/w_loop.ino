/**
 * @file  w_loop.ino
 * @brief Archivo que contiene la implementación de las tareas a realizar
 *        dentro del ciclo principal
 */

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
    // Tareas repetitivas
    wifi_loop(); // Gestión de la conexión WiFi
    mqtt_loop(); // Gestión de la conexión MQTT
  }

  
  // Enciende ambos LEDs
  digitalWrite( LED_VERDE, HIGH );
  digitalWrite( LED_ROJO, HIGH );
  digitalWrite( LED_AZUL, HIGH );

  // Mensaje de cierre del ciclo principal
  Serial.println("CERRANDO");

  // Breve retraso antes de finalizar
  delay(3000);

  // Apaga ambos LEDs
  digitalWrite( LED_VERDE, LOW );
  digitalWrite( LED_ROJO, LOW );
  digitalWrite( LED_AZUL, LOW );

  // Mensaje de finalización del ciclo principal
  Serial.println("FIN Main");

  // Finaliza el programa
  exit(0);
}

/*** End of file ****/