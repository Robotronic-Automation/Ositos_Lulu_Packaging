/**
 * @file  var_prot_string.h
 * @brief Definiciones y funciones para un buffer circular protegido de enteros
 */

#ifndef VAR_PROT_STRING_H
#define VAR_PROT_STRING_H

/**
 * @struct  Var_Prot_String 
 * @brief   Estructura para instanciar una string protegida
 * @details Almacena el contenido de la string y el mutex para proteger el acceso
 */
typedef struct Var_Prot_String 
{
	char dato[10]; ///< Cadena de caracteres que almacena el mensaje
  portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED; ///< Mutex para proteger el acceso a la variable
} Var_Prot_String;

// Funciones para el manejo de la variable

/**
 * @brief  Obtiene la string protegida
 * @param  data. Puntero a la cadena donde se almacenará la string
 * @param  var_prot. Puntero a la variable protegida
 * @return 0 si se obtiene el elemento correctamente, -1 si no
 */
int8_t get_value(char data[], Var_Prot_String * var_prot);

/**
 * @brief Modifica el contenido de la string protegida
 * @param data. Cadena a insertar en la string
 * @param var_prot. Puntero a la variable protegida
 * @return 0 si se inserta el elemento correctamente, -1 si no
 */
int8_t set_value(char data[], Var_Prot_String * var_prot);

#endif 

/*** End of file ****/