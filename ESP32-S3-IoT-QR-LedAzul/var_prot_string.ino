/**
 * @file  var_prot_string.ino
 * @brief Implementación de funciones para un buffer circular protegido
 */

#include "var_prot_string.h"

/**
 * @brief  Obtiene la string protegida
 * @param  data. Puntero a la cadena donde se almacenará la string
 * @param  var_prot. Puntero a la variable protegida
 * @return 0 si se obtiene el elemento correctamente, -1 si no
 */
int8_t get_value(char data[], Var_Prot_String * var_prot)
{
	
    portENTER_CRITICAL (&(var_prot->taskMux)); // Entrar en la región crítica
    strcpy(data, var_prot->dato);
    portEXIT_CRITICAL (&(var_prot->taskMux)); // Salir de la región crítica

		return 0;	
}

/**
 * @brief Modifica el contenido de la string protegida
 * @param data. Cadena a insertar en la string
 * @param var_prot. Puntero a la variable protegida
 * @return 0 si se inserta el elemento correctamente, -1 si no
 */
int8_t set_value(char data[], Var_Prot_String * var_prot)
{

    portENTER_CRITICAL (&(var_prot->taskMux)); // Entrar en la región crítica
    strcpy(var_prot->dato, data);
    portEXIT_CRITICAL (&(var_prot->taskMux)); // Salir de la región crítica

		return 0;	
}

/*** End of file ****/