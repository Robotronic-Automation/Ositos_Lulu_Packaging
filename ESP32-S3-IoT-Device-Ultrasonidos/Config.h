/**
 * @file  Config.h
 * @brief Archivo de configuración del sistema
 */

#ifndef CONFIG_H
#define CONFIG_H

// COMM BAUDS
#define BAUDS 115200

#define LOGGER_ENABLED            // Comentar para deshabilitar el logger por consola serie

#define LOG_LEVEL TRACE           // Niveles en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE

// DEVICE
#define DEVICE_GIIROB_PR2_ID      "17" //"giirobpr2_00"

// WIFI
#define NET_SSID                  "UPV-PSK"
#define NET_PASSWD                "giirob-pr2-2023"

// MQTT
#define MQTT_SERVER_IP            "mqtt.dsic.upv.es"
#define MQTT_SERVER_PORT          1883
#define MQTT_USERNAME             "giirob"    // Descomentar esta línea (y la siguiente) para que se conecte al broker MQTT usando usuario y contraseña
#define MQTT_PASSWORD             "UPV2024"

// TOPICS
/**** HELLO_TOPIC ****/
#define HELLO_TOPIC               "A1"
/**** TOPIC_PRESENCIA : manda si se detecta objeto o no ****/
#define TOPIC_PRESENCIA           "A1/sensor/presencia/cinta/cajas/final"
/**** TOPIC_COBOT : manda azul_on si el cobot está operando y azul_off si no ****/
#define TOPIC_COBOT               "A1/actuador/led/cinta/cajas/final"

// DEFINICION DE CONSTANTES
#define true 1                       ///< Valor booleano verdadero
#define false 0                      ///< Valor booleano falso
#define SensorsUpdateInterval 1000   ///< Frecuencia de muestreo en milisegundos (1 segundo)

// PINES
/**** IO ****/
#define LED_ROJO                  2  ///< Pin led rojo
#define LED_VERDE                 15 ///< Pin led verde
#define LED_AZUL                  4  ///< Pin led azul
#define TRIGGER_PIN_ULTRASONIDOS  5  ///< Pin trigger del ultrasonidos
#define ECHO_PIN_ULTRASONIDOS     6  ///< Pin echo del ultrasonidos

/**** INTERRUPCION BOTON ****/
#define PIN_BUTTON 13                ///< Pin del botón

/**** SERVO ****/
#define SERVO_PIN 10                 ///< PIN para la salida PWM
#define SERVO_CHN 0                  ///< Canal PWM
#define SERVO_FRQ 50                 ///< Frecuencia de PWM
#define SERVO_BIT 12                 ///< Bits usados para codificar el PWM

#endif // CONFIG_H

/*** End of file ****/