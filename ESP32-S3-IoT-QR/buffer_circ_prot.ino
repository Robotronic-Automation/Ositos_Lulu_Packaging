/**
 * @file buffer_circ_prot.ino
 * @brief Definiciones y prototipos de funciones relacionadas con el buffer circular para mensajes
 */

#include "buffer_circ_prot.h"

/**
 * @brief  Comprueba si el buffer está vacío
 * @param  buff. Puntero al buffer circular
 * @return true si el buffer está vacío, false en caso contrario
 */
bool isEmpty(Buffer_Circ * buff)
{
	if(buff->contador == 0)
  {
		return true;
	}
  else
  {
		return false;
	}	
}

/**
 * @brief  Comprueba si el buffer está lleno
 * @param  buff. Puntero al buffer circular
 * @return true si el buffer está lleno, false en caso contrario
 */
bool isFull(Buffer_Circ * buff)
{
	if(buff->contador == BUFSIZE)
  {
		return true;
	}
  else
  {
		return false;
	}	
}

/**
 * @brief  Obtiene un elemento del buffer
 * @param  buff. Puntero al buffer circular
 * @param  data. Variable en la que se guarda el elemento obtenido
 * @return 0 si se obtiene el elemento correctamente, -1 si el buffer está vacío
 */
int8_t get_item(char data[], Buffer_Circ * buff )
{
	if(isEmpty(buff))
  {
		//Serial.printf("Error: No hay datos en el vector\n"); 
		return -1;
	}
	else
  {
    portENTER_CRITICAL (&(buff->taskMux)); // Entrar en la región crítica
    strcpy(data, buff->datos[buff->bufOUT]);
		buff->contador--;
		//Aritmetica en módulo del índice del vector
		buff->bufOUT = (buff->bufOUT+1) % BUFSIZE;
    portEXIT_CRITICAL (&(buff->taskMux)); // Salir de la región crítica
		return 0;	
	}
}

/**
 * @brief  Inserta un elemento en el buffer
 * @param  buff. Puntero al buffer circular
 * @param  data. Elemento a introducir en el buffer
 * @return 0 si se inserta el elemento correctamente, -1 si el buffer está lleno
 */
int8_t put_item(char data[], Buffer_Circ * buff )
{
	if(isFull(buff))
  {
		Serial.printf("Error: El vector esta lleno\n"); 
		return -1;
	}
	else
  {
    portENTER_CRITICAL (&(buff->taskMux)); // Entrar en la región crítica
		strcpy(buff->datos[buff->bufIN], data);
		buff->contador++;
		//Aritmetica en módulo del índice del vector
		buff->bufIN = (buff->bufIN+1) % BUFSIZE;
    portEXIT_CRITICAL (&(buff->taskMux)); // Salir de la región crítica
		return 0;	
	}	
}

/**
 * @brief  Obtiene el número de elementos en el buffer
 * @param  buff. Puntero al buffer circular
 * @return Número de elementos en el buffer
 */
uint32_t number(Buffer_Circ * buff)
{
	return buff->contador;	
}

/**
 * @brief  Lista el contenido del buffer
 * @param  buff. Puntero al buffer circular
 * @return 0 si se lista correctamente, -1 si el buffer está vacío
 */
int8_t listBuffer(Buffer_Circ * buff)
{
	if(isEmpty(buff))
  {
		Serial.printf("Error: No hay datos en el vector\n"); 

		return -1;
	}
	else
  {
    Serial.printf("Tu buffer contiene: ");
    for(uint32_t i = 0; i < BUFSIZE; i++)
    {
      Serial.printf("\t%s", buff->datos[i]);
    }
    Serial.printf("\n");

    return 0;
  }
}

/*** End of file ****/