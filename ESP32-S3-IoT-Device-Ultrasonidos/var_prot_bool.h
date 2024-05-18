/**
 * @file  var_prot_bool.h
 * @brief Definiciones y funciones para una variable protegida booleana
 */

#ifndef VAR_PROT_BOOL_H
#define VAR_PROT_BOOL_H

/**
 * @struct  Var_Prot_Bool 
 * @brief   Estructura para instanciar una variable protegida booleana
 * @details Almacena el contenido de la variable y el mutex para proteger el acceso
 */
typedef struct Var_Prot_Bool 
{
	bool dato; ///< Variable booleana protegida
  portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED; ///< Mutex para proteger el acceso a la variable
} Var_Prot_Bool;

// Funciones para el manejo de la variable protegida

/**
 * @brief Obtiene el valor de la variable protegida
 * @param data. Puntero al elemento donde se almacenará el contenido de la variable
 * @param var_prot. Puntero a la variable protegida 
 * @return 0 si se obtiene el valor correctamente
 */
int8_t get_value(bool * data, Var_Prot_Bool * var_prot);

/**
 * @brief Modifica el valor de la variable protegida
 * @param data. Valor al que se desea establecer la variable
 * @param var_prot. Puntero a la variable protegida 
 * @return 0 si modifica el valor correctamente
 */
int8_t set_value(bool data, Var_Prot_Bool * var_prot);

#endif // VAR_PROT_BOOL_H

/*** End of file ****/