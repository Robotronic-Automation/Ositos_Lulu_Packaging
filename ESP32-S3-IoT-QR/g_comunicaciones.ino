/**
 * @file  g_comunicaciones.ino
 * @brief Implementación de funciones para la gestión de comunicaciones MQTT
 */

/**
 * @brief Se suscribe a los topics MQTT para recibir mensajes
 */
void suscribirseATopics() 
{
  //mqtt_subscribe(TOPIC_COBOT); // Suscribirse al topic de color
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
  if ( strcmp(topic, TOPIC_COBOT) == 0 ) 
  {
    // Extraer el campo "color_estado" del mensaje JSON
    msg = doc["color_estado"];
    infoln(msg);
    // Llamar a la función para establecer el color del LED
    //setColorLed(msg);
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



