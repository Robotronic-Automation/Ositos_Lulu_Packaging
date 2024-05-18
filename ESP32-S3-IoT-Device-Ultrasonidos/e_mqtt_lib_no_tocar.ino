/**
 * @file  e_mqtt_lib_no_tocar.ino
 * @brief Archivo que contiene funciones para la gestión de la conexión MQTT
 */

#define MQTT_CONNECTION_RETRIES 3

PubSubClient mqttClient(espWifiClient);

// MQTT CONFIG
const char* mqttServerIP = MQTT_SERVER_IP; ///< Dirección IP del servidor MQTT
unsigned int mqttServerPort = MQTT_SERVER_PORT; ///< Puerto del servidor MQTT
String mqttClientID; ///< ID del cliente MQTT

/**
 * @brief Función para gestionar la conexión MQTT en el loop
 */
void mqtt_loop() 
{

  if (!mqttClient.connected()) 
  {
    mqtt_reconnect(MQTT_CONNECTION_RETRIES);
    suscribirseATopics();
  }
  mqttClient.loop();

}

/**
 * @brief Función para conectar al servidor MQTT
 * @param clientID. ID del cliente MQTT
 */
void mqtt_connect(String clientID) 
{

    // Configuramos cliente MQTT
    mqttClientID = String(clientID);
    mqttClient.setServer(mqttServerIP, mqttServerPort);

    // Configuramos 'mqttCallback' como la función que se invocará al 
    //  recibir datos por las suscripciones realizadas
    mqttClient.setCallback(mqttCallback);

    // Conectar al servidor MQTT
    mqtt_reconnect(MQTT_CONNECTION_RETRIES);

}

/**
 * @brief Función para reconectar al servidor MQTT si la conexión se pierde
 * @param retries. Número de intentos de conexión
 */
void mqtt_reconnect(int retries) 
{

  if ( !WiFi.isConnected() )
  {
    return;
  }

  if ( !mqttClient.connected() )
  {
    warnln("Disconnected from the MQTT broker");
  }

  // Intentar la reconexión un número determinado de veces
  int r = 0;
  while (!mqttClient.connected() && r<retries) 
  {
    r++;

    trace("Attempting an MQTT connection to: 'mqtt://");
    trace(mqttServerIP);
    trace(":");
    trace(mqttServerPort);
    trace("' with client-id: '");
    trace(mqttClientID);
    traceln("' ... ");


    // Intentar la conexión
    // boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])

  #ifdef MQTT_USERNAME
    if ( mqttClient.connect(mqttClientID.c_str(), MQTT_USERNAME, MQTT_PASSWORD) ) 
    {
  #else
    if ( mqttClient.connect(mqttClientID.c_str()) ) 
    {
  #endif
      debugln("-=- Connected to MQTT Broker");
      // Damos tiempo a que la conexión se establezca por completo
      delay(1000);
    } 
    else 
    {
      debug("-X- failed, rc=");
      debugln(mqttClient.state());
      debugln("-R-   re-trying in 5 seconds");
      // Esperar 5 segundos antes de volver a intentar
      delay(5000);
    }
  }
}

/**
 * @brief Función de devolución de llamada para procesar mensajes MQTT recibidos
 * @param topic. Tema MQTT del mensaje
 * @param message. Contenido del mensaje MQTT
 * @param length. Longitud del mensaje
 */
void mqttCallback(char* topic, byte* message, unsigned int length) 
{
  // Función que se invocará automáticamente al recibir datos por algún topic
  //  sobre el que nos hayamos suscrito

  // Cargamos los datos recibidos en una variable
  String incomingMessage;
  for (int i = 0; i < length; i++) 
  {
    incomingMessage += (char)message[i];
  }

  traceln("<<~~ RECEIVING an MQTT message:");
  traceln(topic);
  traceln(incomingMessage);

  alRecibirMensajePorTopic(topic, incomingMessage);
}

/**
 * @brief Función para publicar un mensaje MQTT en un tema específico
 * @param topic. Tema MQTT donde se publicará el mensaje
 * @param outgoingMessage. Contenido del mensaje a publicar
 */
void mqtt_publish(const char* topic, String outgoingMessage) 
{
  if ( !mqttClient.connected() ) 
  {
    errorln("Cannot send message through the topic ... the MQTT Client is disconnected!!")
    return;
  }

  traceln("~~>> PUBLISHING an MQTT message:");
  traceln(topic);
  traceln(outgoingMessage);
  mqttClient.publish(topic, outgoingMessage.c_str());
}

/**
 * @brief Función para suscribirse a un tema MQTT
 * @param topic. Tema MQTT al que suscribirse
 */
void mqtt_subscribe(const char* topic) 
{
  if ( !mqttClient.connected() ) 
  {
    errorln("Cannot subscribe to topic ... the MQTT Client is disconnected!!")
    return;
  }

  trace("Subscribed to topic: ");
  traceln(topic);
  mqttClient.subscribe(topic);
}

/*** End of file ****/