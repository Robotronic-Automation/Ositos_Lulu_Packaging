/**
 * @file  g_comunicaciones.ino
 * @brief Implementación de funciones para la gestión de comunicaciones MQTT
 */

/**
 * @brief Se suscribe a los topics MQTT para recibir mensajes
 */
void suscribirseATopics() 
{
  mqtt_subscribe(TOPIC_PARADA_EMERGENCIA); // Suscribirse al topic de la parada de emergencia
}

/**
 * @brief Controlador que gestiona la recepción de datos por topic MQTT
 * @param topic. Topic del que se recibe el mensaje
 * @param incomingMessage. Mensaje recibido
 */
void alRecibirMensajePorTopic(char * topic, String incomingMessage)
{
  
  // Parsear el mensaje JSON recibido
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, incomingMessage);
  if (err) 
  {
    warn(F("deserializeJson() returned ")); 
    warnln(err.f_str());
    return;
  }
  
  const char * msg;

  // If a message is received on the topic ...

  // Si se ha pulsado el botón de emergencia en otra ESP32
  if( strcmp(topic, TOPIC_PARADA_EMERGENCIA) == 0 )
  {
    // Extraer el campo "parada_emergencia" del mensaje JSON
    msg = doc["parada_emergencia"];
    warnln(msg);
    // Parar todo el sistema
    PARAR = true;
  }
}

/**
 * @brief Controlador que envía un mensaje por un topic MQTT
 * @param topic. Topic en el que se publica el mensaje
 * @param outgoingMessage. Mensaje a publicar
 */
void enviarMensajePorTopic(const char * topic, String outgoingMessage) 
{
  mqtt_publish(topic, outgoingMessage.c_str()); // Publicar el mensaje en el topic especificado
}

/*** End of file ****/



