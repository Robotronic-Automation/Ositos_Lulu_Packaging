/**
 * @file  buffer_circ_prot.h
 * @brief Definiciones y funciones para un buffer circular protegido de enteros
 */

#ifndef BUFFER_CIRC_PROT_H
#define BUFFER_CIRC_PROT_H

#include "src/quirc/quirc.h" // Inclusión de la librería Quirc
#define BUFSIZE 10 ///< Tamaño del buffer
#define true 1 ///< Valor booleano verdadero
#define false 0 ///< Valor booleano falso

#define T_espera 1000 ///< Tiempo de espera predeterminado

/**
 * @struct  Buffer_Circ_Measure
 * @brief   Estructura para instanciar un buffer circular protegido
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
} Buffer_Circ_Measure;

// Funciones para el manejo del buffer circular protegido

/**
 * @brief  Comprueba si el buffer está vacío
 * @param  buff. Puntero al buffer circular
 * @return true si el buffer está vacío, false en caso contrario
 */
bool isEmpty(Buffer_Circ_Measure * buff);

/**
 * @brief  Comprueba si el buffer está lleno
 * @param  buff. Puntero al buffer circular
 * @return true si el buffer está lleno, false en caso contrario
 */
bool isFull(Buffer_Circ_Measure * buff);

/**
 * @brief Obtiene un elemento del buffer
 * @param data. Puntero al elemento donde se almacenará el dato
 * @param buff. Puntero al buffer circular
 * @return 0 si se obtiene el elemento correctamente, -1 si el buffer está vacío
 */
uint8_t get_item(uint32_t * data, Buffer_Circ_Measure * buff);

/**
 * @brief Inserta un elemento en el buffer
 * @param data. Dato a insertar en el buffer
 * @param buff. Puntero al buffer circular
 * @return 0 si se inserta el elemento correctamente, -1 si el buffer está lleno
 */
uint8_t put_item(uint32_t data, Buffer_Circ_Measure * buff);

/**
 * @brief Obtiene el número de elementos en el buffer
 * @param buff. Puntero al buffer circular
 * @return Número de elementos en el buffer
 */
uint32_t number(Buffer_Circ_Measure * buff);

/**
 * @brief Lista el contenido del buffer
 * @param buff Puntero al buffer circular
 * @return 0 si se lista correctamente, -1 si el buffer está vacío
 */
uint8_t listBuffer(Buffer_Circ_Measure * buff);

#endif 

/*** End of file ****/