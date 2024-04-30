/*
 * Grado GIIROB
 * Asignatura PR2
 * Grupo A1
 * Curso 2023/24
 *
 * @file Plantilla Dispositivo IoT
 *
 * Control de versiones
 * Version: 0.1   (2024/04/15) Prototipo inicial
*/
#include "Config.h"

#include <WiFi.h>
#ifdef SSL_ROOT_CA
  #include <WiFiClientSecure.h>
#endif
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "button_interrupt.h"
#include "buffer_circ_prot.h"

// Variable global para detener el programa en caso de emergencia
volatile bool PARAR = false;

// ID de Dispositivo : se proporcionan varias alternativas, a modo de ejemplo
String deviceID = String("giirobpr2-device-") + String(DEVICE_GIIROB_PR2_ID); 
  // Versión usando el ID asignado en la asignatura GIIROB-PR2
//String deviceID = String("device-") + String(WiFi.macAddress());            
  // Versión usando la dirección MAC del dispositivo
//String deviceID = String("device-esp32s3-") + String(DEVICE_ESP_ID);        
  // Versión usando el ID de ESP del dispositivo

/**
 @brief setup. Configura conceptos 'core', inicializa la wifi y la conexión con 
        el bróker MQTT, se suscribe a topics, y llama a on_setup. 
 @param  ninguno
 @return ninguno
*/
void setup() 
{
  // Este setup 
  // Métodos:
  //  - suscribirseATopics()  -> topics MQTT a suscribir para recibir mensajes 
  //                            (g_comunicaciones.ino)
  //  - on_setup()            -> añadir la configuración de pines, inicialización
  //                            de variables, etc. (s_setup.ino)
  //  - on_loop()             -> tareas a realizar dentro del 'loop' (w_loop.ino)

#ifdef LOGGER_ENABLED
  // Inicializamos comunicaciones serial
  Serial.begin(BAUDS);
  delay(1000);
  Serial.println();
#endif

  // Nos conectamos a la wifi
  wifi_connect();

  // Nos conectamos al broker MQTT, indicando un 'client-id'
  mqtt_connect(deviceID);

  suscribirseATopics();

  on_setup();

}

void loop() 
{

  // NO QUITAR (jjfons)
  wifi_loop();
  mqtt_loop();

  on_loop();
}

/*** End of file ****/