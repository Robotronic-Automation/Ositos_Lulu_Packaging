/**
 * @file  buffer_measure.h
 * @brief Definiciones y funciones para un buffer circular protegido de enteros
 */

#ifndef BUFFER_MEASURE_H
#define BUFFER_MEASURE_H

#define BUFSIZE 10  ///< Tamaño del buffer

/**
 * @brief   Estructura para instanciar un buffer circular protegido de enteros
 * @details Almacena los datos del buffer, la posición de entrada y salida,
 *          el contador de elementos y el mutex para proteger el acceso
 */
typedef struct Buffer_Measure 
{
	uint32_t datos[BUFSIZE]; ///< Vector de enteros que almacena los datos del buffer
	uint32_t bufIN = 0; ///< Posición de entrada del próximo elemento
  uint32_t bufOUT = 0; ///< Posición de salida del próximo elemento 
  uint32_t contador = 0; ///< Número de elementos en el buffer
  portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED; ///< Mutex para proteger el acceso al buffer
} Buffer_Circ_Measure_t;

// Funciones para el manejo del buffer circular protegido

/**
 * @brief  Comprueba si el buffer está vacío
 * @param  buff. Puntero al buffer circular
 * @return true si el buffer está vacío, false en caso contrario
 */
bool isEmpty(Buffer_Circ_Measure_t * buff);

/**
 * @brief  Comprueba si el buffer está lleno
 * @param  buff. Puntero al buffer circular
 * @return true si el buffer está lleno, false en caso contrario
 */
bool isFull(Buffer_Circ_Measure_t * buff);

/**
 * @brief  Obtiene un elemento del buffer
 * @param  buff. Puntero al buffer circular
 * @param  data. Variable en la que se guarda el elemento obtenido
 * @return 0 si se obtiene el elemento correctamente, -1 si el buffer está vacío
 */
int8_t get_item(uint32_t * data, Buffer_Circ_Measure_t * buff);

/**
 * @brief  Inserta un elemento en el buffer
 * @param  buff. Puntero al buffer circular
 * @param  data. Elemento a introducir en el buffer
 * @return 0 si se inserta el elemento correctamente, -1 si el buffer está lleno
 */
int8_t put_item(uint32_t data, Buffer_Circ_Measure_t * buff);

/**
 * @brief Obtiene el número de elementos en el buffer
 * @param buff. Puntero al buffer circular
 * @return Número de elementos en el buffer
 */
uint32_t number(Buffer_Circ_Measure_t * buff);

/**
 * @brief Lista el contenido del buffer
 * @param buff. Puntero al buffer circular
 * @return 0 si se lista correctamente, -1 si el buffer está vacío
 */
int8_t listBuffer(Buffer_Circ_Measure_t * buff);

#endif // BUFFER_MEASURE_H

/*** End of file ****/