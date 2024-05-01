/**
 @file Config.h
*/

#ifndef CONFIG_H
#define CONFIG_H

// COMM BAUDS
#define BAUDS 115200

#define LOGGER_ENABLED            // Comentar para deshabilitar el logger por consola serie

#define LOG_LEVEL TRACE           // nivells en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE

// DEVICE
//#define DEVICE_ESP_ID             "54CE0361421"   // ESP32 ID
#define DEVICE_GIIROB_PR2_ID      "17" //"giirobpr2_00"

// WIFI
//#define NET_SSID                  "UPV-PSK"
//#define NET_PASSWD                "giirob-pr2-2023"
#define NET_SSID                    "topo1234"
#define NET_PASSWD                  "every7652"

// MQTT
//#define MQTT_SERVER_IP            "mqtt.dsic.upv.es"
//#define MQTT_SERVER_PORT          1883
//#define MQTT_USERNAME             "giirob"    // Descomentar esta línea (y la siguiente) para que se conecte al broker MQTT usando usuario y contraseña
//#define MQTT_PASSWORD             "UPV2024"

#define MQTT_SERVER_IP            "broker.emqx.io"
#define MQTT_SERVER_PORT          1883

// TOPICS
// TOPIC_PRESENCIA : manda si se detecta objeto o no
#define TOPIC_PRESENCIA           "A1/sensor/presencia/cinta/cajas/final"
// TOPIC_COLOR : manda rojo si el cobot está operando y verde si no
#define TOPIC_COLOR               "A1/actuador/led/cinta/cajas/final"
// TOPIC_COBOT_1 : el cobot publica si esta operando?
#define TOPIC_COBOT_1             "A1/cobot/cinta/cajas/final"

// PINES
// IO
#define LED_ROJO                  2  // Pin rojo led RGB
#define LED_VERDE                 3  // Pin verde led RGB
#define LED_AZUL                  4  // Pin azul led RGB
#define TRIGGER_PIN_ULTRASONIDOS  5  // Pin trigger del ultrasonidos
#define ECHO_PIN_ULTRASONIDOS     6  // Pin echo del ultrasonidos
// INTERRUPCION BOTON
#define LED_1  2
#define LED_2  3
#define PIN_BUTTON 13

#endif

/*** End of file ****/