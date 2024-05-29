/**
 * @file  buffer_mqtt.h
 * @brief Definiciones y prototipos de funciones relacionadas con 
 *        el buffer circular para mensajes MQTT 
 */

#ifndef BUFFER_MQTT_H
#define BUFFER_MQTT_H

#define BUFSIZE 10 ///< Tamaño del buffer

/**
 * @brief   Estructura para representar un mensaje MQTT
 * @details Almacena el topic y el mensaje a publicar en MQTT
 */
typedef struct Msg_MQTT
{
	char * topic; ///< Cadena de caracteres que almacena el topic en el que publicar el mensaje
	char msg[10]; ///< Cadena de caracteres que almacena el mensaje a publicar en el topic
} Msg_MQTT_t;


/**
 * @brief   Estructura para representar un buffer circular protegido de mensajes
 * @details Almacena los datos del buffer, la posición de entrada y salida,
 *          el contador de elementos y el mutex para proteger el acceso
 */
typedef struct Buffer_MQTT
{
	Msg_MQTT datos[BUFSIZE]; ///< Vector que almacena los mensajes MQTT del buffer
	uint32_t bufIN = 0; ///< Posición de entrada del próximo elemento
  uint32_t bufOUT = 0; ///< Posición de salida del próximo elemento 
  uint32_t contador = 0; ///< Número de elementos en el buffer
  portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED; ///< Mutex para proteger el acceso al buffer
} Buffer_Circ_MQTT_t;

// Funciones para el manejo del buffer circular protegido

/**
 * @brief  Comprueba si el buffer está vacío
 * @param  buff. Puntero al buffer circular
 * @return true si el buffer está vacío, false en caso contrario
 */
bool isEmpty(Buffer_Circ_MQTT_t * buff);

/**
 * @brief  Comprueba si el buffer está lleno
 * @param  buff. Puntero al buffer circular
 * @return true si el buffer está lleno, false en caso contrario
 */
bool isFull(Buffer_Circ_MQTT_t * buff);

/**
 * @brief  Obtiene un elemento del buffer
 * @param  buff. Puntero al buffer circular
 * @param  data. Variable en la que se guarda el elemento obtenido
 * @return 0 si se obtiene el elemento correctamente, -1 si el buffer está vacío
 */
int8_t get_item(Msg_MQTT_t * data, Buffer_Circ_MQTT_t * buff );

/**
 * @brief  Inserta un elemento en el buffer
 * @param  buff. Puntero al buffer circular
 * @param  data. Elemento a introducir en el buffer
 * @return 0 si se inserta el elemento correctamente, -1 si el buffer está lleno
 */
int8_t put_item(Msg_MQTT_t data, Buffer_Circ_MQTT_t * buff );

/**
 * @brief Obtiene el número de elementos en el buffer
 * @param buff. Puntero al buffer circular
 * @return Número de elementos en el buffer
 */
uint32_t number(Buffer_Circ_MQTT_t * buff);

/**
 * @brief Lista el contenido del buffer
 * @param buff Puntero al buffer circular
 * @return 0 si se lista correctamente, -1 si el buffer está vacío
 */
int8_t listBuffer(Buffer_Circ_MQTT_t * buff);

#endif // BUFFER_MQTT_H

/*** End of file ****/