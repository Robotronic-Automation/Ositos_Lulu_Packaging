/**
 * @file  var_prot_string.h
 * @brief Definiciones y funciones para una variable protegida de tipo cadena de caracteres
 */

#ifndef VAR_PROT_STRING_H
#define VAR_PROT_STRING_H

/**
 * @struct  Var_Prot_String 
 * @brief   Estructura para instanciar una cadena de caracteres protegida
 * @details Almacena el contenido de la cadena de caracteres y el mutex para proteger el acceso
 */
typedef struct Var_Prot_String 
{
	char dato[10]; ///< Cadena de caracteres protegida
  portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED; ///< Mutex para proteger el acceso a la variable
} Var_Prot_String;

// Funciones para el manejo de la variable protegida

/**
 * @brief  Obtiene la la cadena de caracteres protegida
 * @param  data. Puntero a la cadena donde se almacenará el contenido de la cadena protegida
 * @param  var_prot. Puntero a la variable protegida
 * @return 0 si se obtiene el valor correctamente correctamente
 */
int8_t get_value(char data[], Var_Prot_String * var_prot);

/**
 * @brief Modifica el contenido de la cadena de caracteres protegida
 * @param data. Cadena de caracteres a insertar en la cadena protegida
 * @param var_prot. Puntero a la variable protegida
 * @return 0 si se inserta el elemento correctamente
 */
int8_t set_value(char data[], Var_Prot_String * var_prot);

#endif // VAR_PROT_STRING_H

/*** End of file ****/