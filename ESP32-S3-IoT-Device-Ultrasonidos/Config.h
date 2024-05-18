/**
 * @file Config.h
 * @brief Archivo de configuración del sistema
 */

#ifndef CONFIG_H
#define CONFIG_H

// COMM BAUDS
#define BAUDS 115200

#define LOGGER_ENABLED            // Comentar para deshabilitar el logger por consola serie

#define LOG_LEVEL TRACE           // Niveles en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE

// DEVICE
//#define DEVICE_ESP_ID             "54CE0361421"   // ESP32 ID
#define DEVICE_GIIROB_PR2_ID      "17" //"giirobpr2_00"

// WIFI
//#define NET_SSID                  "UPV-PSK"
//#define NET_PASSWD                "giirob-pr2-2023"
#define NET_SSID                  "topo1234"
#define NET_PASSWD                "every7652"

// MQTT
//#define MQTT_SERVER_IP            "mqtt.dsic.upv.es"
//#define MQTT_SERVER_PORT          1883
//#define MQTT_USERNAME             "giirob"    // Descomentar esta línea (y la siguiente) para que se conecte al broker MQTT usando usuario y contraseña
//#define MQTT_PASSWORD             "UPV2024"

#define MQTT_SERVER_IP            "broker.emqx.io"
#define MQTT_SERVER_PORT          1883

// TOPICS
/**** HELLO_TOPIC ****/
#define HELLO_TOPIC               "A1"
/**** TOPIC_PRESENCIA : manda si se detecta objeto o no ****/
#define TOPIC_PRESENCIA           "A1/sensor/presencia/cinta/cajas/final"
/**** TOPIC_COLOR : manda azul_on si el cobot está operando y azul_off si no ****/
#define TOPIC_COLOR               "A1/actuador/led/cinta/cajas/final"


// PINES
/**** IO ****/
#define LED_ROJO                  2  // Pin led rojo
#define LED_VERDE                 15 // Pin led verde
#define LED_AZUL                  4  // Pin led azul
#define TRIGGER_PIN_ULTRASONIDOS  5  // Pin trigger del ultrasonidos
#define ECHO_PIN_ULTRASONIDOS     6  // Pin echo del ultrasonidos
#define MOTOR_PIN                 10 // Pin para activar el motor DC

/**** INTERRUPCION BOTON ****/
#define PIN_BUTTON 13

#endif

/*** End of file ****/