/**
 * Grado en Informatica Industrial y Robótica (GIIROB)
 * Asignatura: PR2
 * Grupo: A1
 * Curso: 2023/24
 *
 * @file ESP32-S3-IoT-QR.ino
 *
 * Control de versiones
 * Version: 1.0   (2024/05/18) Prototipo completo
*/

#include "Config.h"

#include <WiFi.h>
#ifdef SSL_ROOT_CA
  #include <WiFiClientSecure.h>
#endif

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <string>

#include "src/quirc/quirc.h"  // Inclusión de la librería Quirc
#include "button_interrupt.h"
#include "buffer_circ_prot.h"
#include "buffer_string.h"
#include "buffer_mqtt.h"
#include "qr.h"

// Variable global para detener el programa en caso de emergencia
volatile bool PARAR = false;

// ID de Dispositivo 
String deviceID = String("giirobpr2-device-") + String(DEVICE_GIIROB_PR2_ID); 

/**
 * @brief  Configuración inicial del programa. Configura conceptos 'core',
 *         inicializa la conexión WiFi y la conexión con el bróker MQTT,
 *         se suscribe a topics y llama a on_setup
 */
void setup() {
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
  Serial.setDebugOutput(true);
  delay(1000);
  Serial.println();
#endif

  // Nos conectamos a la wifi
  wifi_connect();

  // Nos conectamos al broker MQTT, indicando un 'client-id'
  mqtt_connect(deviceID);

  // Suscribimos al dispositivo a los topics MQTT relevantes
  suscribirseATopics();

  // Configuración adicional específica del dispositivo
  on_setup();

}

/**
 * @brief  Bucle principal del programa. Gestiona las tareas repetitivas
 *         como la conexión WiFi, la conexión MQTT y las acciones definidas
 *         en on_loop
 */
void loop() {
  // Tareas específicas del dispositivo definidas en on_loop
  on_loop();
}

/*** End of file ****/