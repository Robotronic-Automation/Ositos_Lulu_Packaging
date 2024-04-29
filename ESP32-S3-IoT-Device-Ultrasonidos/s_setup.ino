/**
 @file s_setup.ino
*/

/**
 @brief on_setup. Añadir la configuración de pines, inicialización de variables, etc
 @param  ninguno
 @return ninguno
*/
void on_setup() 
{

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_ROJO, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_AZUL, OUTPUT);

    // Inicializa los pines del ultrasonidos: 
    // trigger --> output  ;  echo --> input
    pinMode(TRIGGER_PIN_ULTRASONIDOS, OUTPUT);
    pinMode(ECHO_PIN_ULTRASONIDOS, INPUT);

    // Mensaje para comprobar la conexion MQTT
    String hello_msg = String("Hola Mundo! Desde dispositivo ") + deviceID;

    // Test JSON
    /*JsonDocument doc;
    doc["message"] = hello_msg;
    doc["luminosidad"] = 450;
    doc["temperatura"] = 21.5;
    String hello_msg_json;
    serializeJson(doc, hello_msg_json);
    enviarMensajePorTopic(HELLO_TOPIC, hello_msg_json);
    */

}

/*** End of file ****/