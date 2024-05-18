/**
 * @file  var_prot_bool.h
 * @brief Definiciones y funciones para un buffer circular protegido de enteros
 */

#ifndef VAR_PROT_BOOL_H
#define VAR_PROT_BOOL_H

#include "src/quirc/quirc.h" // Inclusión de la librería Quirc
#define true 1 ///< Valor booleano verdadero
#define false 0 ///< Valor booleano falso


/**
 * @struct  Buffer_Circ_Measure
 * @brief   Estructura para instanciar un buffer circular protegido
 * @details Almacena los datos del buffer, la posición de entrada y salida,
 *          el contador de elementos y el mutex para proteger el acceso
 */
typedef struct Var_Prot_Bool 
{
	bool dato; ///< Vector de enteros que almacena los datos del buffer
  portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED; ///< Mutex para proteger el acceso a la variable
} Var_Prot_Bool;

// Funciones para el manejo de la variable

/**
 * @brief Obtiene un elemento del buffer
 * @param data. Puntero al elemento donde se almacenará el dato
 * @param buff. Puntero al buffer circular
 * @return 0 si se obtiene el elemento correctamente, -1 si el buffer está vacío
 */
int8_t get_value(bool * data, Var_Prot_Bool * var_prot);

/**
 * @brief Inserta un elemento en el buffer
 * @param data. Dato a insertar en el buffer
 * @param buff. Puntero al buffer circular
 * @return 0 si se inserta el elemento correctamente, -1 si el buffer está lleno
 */
int8_t set_value(bool data, Var_Prot_Bool * var_prot);

#endif 

/*** End of file ****/