/**
 * @file  var_prot_string.ino
 * @brief Implementación de funciones para un buffer circular protegido
 */

#include "var_prot_string.h"


/**
 * @brief  Obtiene un elemento del buffer
 * @param  buff. Puntero al buffer circular
 * @param  data. Variable en la que se guarda el elemento obtenido
 * @return 0 si se obtiene el elemento correctamente, -1 si el buffer está vacío
 */
int8_t get_value(char data[], Var_Prot_String * var_prot)
{
	
    portENTER_CRITICAL (&(var_prot->taskMux)); // Entrar en la región crítica
    strcpy(data, var_prot->dato);
    portEXIT_CRITICAL (&(var_prot->taskMux)); // Salir de la región crítica

		return 0;	
}

/**
 * @brief  Inserta un elemento en el buffer
 * @param  buff. Puntero al buffer circular
 * @param  data. Elemento a introducir en el buffer
 * @return 0 si se inserta el elemento correctamente, -1 si el buffer está lleno
 */
int8_t set_value(char data[], Var_Prot_String * var_prot)
{

    portENTER_CRITICAL (&(var_prot->taskMux)); // Entrar en la región crítica
    strcpy(var_prot->dato, data);
    portEXIT_CRITICAL (&(var_prot->taskMux)); // Salir de la región crítica

		return 0;	
}

/*** End of file ****/