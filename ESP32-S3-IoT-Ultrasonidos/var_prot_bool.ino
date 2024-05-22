/**
 * @file  var_prot_bool.ino
 * @brief Implementación de funciones para una variable protegida booleana
 */

#include "var_prot_bool.h"


/**
 * @brief Obtiene el valor de la variable protegida
 * @param data. Puntero al elemento donde se almacenará el contenido de la variable
 * @param var_prot. Puntero a la variable protegida 
 * @return 0 si se obtiene el valor correctamente
 */
int8_t get_value(bool * data, Var_Prot_Bool_t * var_prot)
{
	
    portENTER_CRITICAL (&(var_prot->taskMux)); // Entrar en la región crítica
		*data = var_prot->dato;
    portEXIT_CRITICAL (&(var_prot->taskMux)); // Salir de la región crítica

		return 0;	
}

/**
 * @brief Modifica el valor de la variable protegida
 * @param data. Valor al que se desea establecer la variable
 * @param var_prot. Puntero a la variable protegida 
 * @return 0 si modifica el valor correctamente
 */
int8_t set_value(bool data, Var_Prot_Bool_t * var_prot)
{

    portENTER_CRITICAL (&(var_prot->taskMux)); // Entrar en la región crítica
		var_prot->dato = data; 
    portEXIT_CRITICAL (&(var_prot->taskMux)); // Salir de la región crítica

		return 0;	
}

/*** End of file ****/