/**
 * @file  var_prot_string.ino
 * @brief Implementación de funciones para una variable protegida de tipo cadena de caracteres
 */

#include "var_prot_string.h"

/**
 * @brief  Obtiene la la cadena de caracteres protegida
 * @param  data. Puntero a la cadena donde se almacenará el contenido de la cadena protegida
 * @param  var_prot. Puntero a la variable protegida
 * @return 0 si se obtiene el valor correctamente correctamente
 */
int8_t get_value(char data[], Var_Prot_String_t * var_prot)
{
	
    portENTER_CRITICAL (&(var_prot->taskMux)); // Entrar en la región crítica
    strcpy(data, var_prot->dato);
    portEXIT_CRITICAL (&(var_prot->taskMux)); // Salir de la región crítica

		return 0;	
}

/**
 * @brief Modifica el contenido de la cadena de caracteres protegida
 * @param data. Cadena de caracteres a insertar en la cadena protegida
 * @param var_prot. Puntero a la variable protegida
 * @return 0 si se inserta el elemento correctamente
 */
int8_t set_value(char data[], Var_Prot_String_t * var_prot)
{

    portENTER_CRITICAL (&(var_prot->taskMux)); // Entrar en la región crítica
    strcpy(var_prot->dato, data);
    portEXIT_CRITICAL (&(var_prot->taskMux)); // Salir de la región crítica

		return 0;	
}

/*** End of file ****/