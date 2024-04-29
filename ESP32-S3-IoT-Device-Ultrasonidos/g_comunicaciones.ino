/**
 @file g_comunicaciones.ino
*/

/**
 @brief suscribirseATopics. Se suscribe a los topics MQTT para recibir mensajes
 @param  ninguno
 @return ninguno
*/
void suscribirseATopics() 
{
  
  mqtt_subscribe(TOPIC_COLOR);

}

/**
 @brief alRecibirMensajePorTopic. Controlador que gestiona la recepción de datos
 @param  topic. topic del que recibe el mensaje
 @param  incomingMessage. mensaje que se recibe
 @return ninguno
*/
void alRecibirMensajePorTopic(char* topic, String incomingMessage)
{
  
  // Test JSON
  JsonDocument doc;
   // Parse the JSON input
  DeserializationError err = deserializeJson(doc, incomingMessage);
  // Parsing succeeded?
  if (err) 
  {
    warn(F("deserializeJson() returned ")); warnln(err.f_str());
    return;
  }

  const char* msg;
  //info("(JSON) Rebut musica: "); infoln(msg);

  /*int lum = doc["luminosidad"];
  info("(JSON) Rebut luminosidad: "); infoln(lum);

  const char* temp = doc["temperatura"];
  info("(JSON) Rebut temperatura: "); infoln(temp);
  */
  // Test JSON


  // If a message is received on the topic ...
  if (strcmp(topic, TOPIC_COLOR) == 0) 
  {
    msg = doc["color_estado"];
    infoln(msg);
    setColorLed(msg);
  }

}

/**
 @brief enviarMensajePorTopic. Controlador que gestiona la recepción de datos
 @param  topic. topic en el que publica 
 @param  outgoingMessage. mensaje que se publica
 @return ninguno
*/
void enviarMensajePorTopic(const char* topic, String outgoingMessage) 
{

  mqtt_publish(topic, outgoingMessage.c_str());

}

/*** End of file ****/



