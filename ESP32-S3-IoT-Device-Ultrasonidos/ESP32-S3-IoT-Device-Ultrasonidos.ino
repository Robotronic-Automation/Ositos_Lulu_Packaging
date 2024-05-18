/**
 * Grado en Informatica Industrial y Robótica (GIIROB)
 * Asignatura: PR2
 * Grupo: A1
 * Curso: 2023/24
 *
 * @file ESP32-S3-IoT-Device-Ultrasonidos.ino
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
#include <cstdint> 
#include <string>

#include "button_interrupt.h"
#include "buffer_measure.h"
#include "buffer_mqtt.h"
#include "var_prot_bool.h"
#include "var_prot_string.h"
#include "servo.h"

/**
 * @brief  Estructura para instanciar buffers circulares protegidos y variables 
 *         protegidas para medidas y mensajes
 * @member Measure_buffer. Buffer para almacenar medidas del sensor de ultrasonidos
 * @member MQTT_buffer. Buffer para almacenar mensajes a publicar en el broker MQTT
 * @member Color_Var. Variable para almacenar el color del LED encendido
 * @member Azul_Var. Variable para almacenar si se enciende el LED azul o no
 * @member Motor_Var. Variable para almacenar el estado del motor de la cinta
 */
typedef struct Buffers
{
	Buffer_Circ_Measure Measure_buffer;
  Buffer_Circ_MQTT MQTT_buffer;
  Var_Prot_String Color_Var;
  Var_Prot_String Azul_Var;
  Var_Prot_Bool Motor_Var;
} Buffers;

// Instancia de estructura Buffers que contiene buffers circulares y variables protegidas 
static Buffers buffers;

// Variable global para detener el programa en caso de emergencia
volatile bool PARAR = false;

// ID de Dispositivo 
String deviceID = String("giirobpr2-device-") + String(DEVICE_GIIROB_PR2_ID); 

/**
 * @brief  Configuración inicial del programa. Configura conceptos 'core',
 *         inicializa la conexión WiFi y la conexión con el bróker MQTT,
 *         se suscribe a topics y llama a on_setup
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
void loop() 
{
  // Tareas específicas del dispositivo definidas en on_loop
  on_loop();
}

/*** End of file ****/