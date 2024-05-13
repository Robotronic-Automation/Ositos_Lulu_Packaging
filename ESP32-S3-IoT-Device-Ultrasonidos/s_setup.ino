/**
 * @file s_setup.ino
 * @brief Añadir la configuración de pines, inicialización de variables, etc. 
 */

#include <string>

#define SensorsUpdateInterval 1000 // 1 segundo de frecuencia de muestreo

/**
 * @brief  Estructura para instanciar un buffer circular protegido para mensajes y otro para medidas 
 * @member Measure_buffer. Buffer para guardar medidas del sensor de ultrasonidos
 * @member Message_buffer. Buffer para guardar los mensajes que se publicaran en el broker de MQTT
 */
typedef struct Buffers
{
	Buffer_Circ_Measure Measure_buffer;
  Buffer_Circ_Message Message_buffer;
} Buffers;

// Crea un task handle para cada tarea 
TaskHandle_t Productor_Task, Consumidor_Task, GestorComMQTT_Task;

// Crea una instancia de la estructura buffers que contiene los buffers circulares protegidos
static Buffers buffers;

// Declaracion de las funciones de cada tarea
void GestorComMQTT( void * parameter );
void Productor( void * parameter );
void Consumidor( void * parameter );

/**
 @brief on_setup. Configuración de pines, inicialización de variables, 
        configuración de interrupciones y creación de tareas.
 @param  ninguno
 @return ninguno
*/
void on_setup() 
{

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_ROJO, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_AZUL, OUTPUT);

    // Inicializa los pines del ultrasonidos: 
    // trigger --> output  ;  echo --> input
    pinMode(TRIGGER_PIN_ULTRASONIDOS, OUTPUT);
    pinMode(ECHO_PIN_ULTRASONIDOS, INPUT);

    // Mensaje para comprobar la conexion MQTT
    String hello_msg = String("Hola Mundo! Desde dispositivo ") + deviceID;

    // Test JSON
    JsonDocument doc;
    doc["message"] = hello_msg;
    String hello_msg_json;
    serializeJson(doc, hello_msg_json);
    enviarMensajePorTopic(HELLO_TOPIC, hello_msg_json);
    

    // Configuramos interrupcion con el boton
    config_button();

    /* Create "Productor_Task" using the xTaskCreatePinnedToCore() function */
    xTaskCreatePinnedToCore(
             Productor,                     /* Task function. */
             "Productor",                   /* name of task. */
             10000,                         /* Stack size of task */
             &buffers.Measure_buffer,       /* parameter of the task */
             1,                             /* priority of the task */
             &Productor_Task,               /* Task handle to keep track of created task */
             0);                            /* pin task to core 0 */

    /* Create "Consumidor_Task" using the xTaskCreatePinnedToCore() function */
    xTaskCreatePinnedToCore(
             Consumidor,                    /* Task function. */
             "Consumidor",                  /* name of task. */
             10000,                         /* Stack size of task */
             &buffers,                      /* parameter of the task */
             1,                             /* priority of the task */
             &Consumidor_Task,              /* Task handle to keep track of created task */
             1);                            /* pin task to core 0 */
    
    // Create "GestorComMQTT_Task" using the xTaskCreatePinnedToCore() function 
    xTaskCreate(
             GestorComMQTT,                 /* Task function. */
             "GestorComMQTT",               /* name of task. */
             10000,                         /* Stack size of task */
             &buffers.Message_buffer,       /* parameter of the task */
             1,                             /* priority of the task */
             &GestorComMQTT_Task);          /* Task handle to keep track of created task */
    
    delay(1000);

}

/**
 @brief Productor. Tarea para tomar mediciones del sensor ultrasonidos
 @param buff_prod. Buffer donde se almacenan las medidas
*/
void Productor( void * parameter )
{
  TickType_t xLastWakeTime;
  Buffer_Circ_Measure * buff_prod = (Buffer_Circ_Measure *) parameter;
  Serial.printf("Hola desde la tarea 1 en el Core %d\n", xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
  while (!PARAR)
  {
    // Leemos sensor ultrasonidos y guardamos distancia
    int cm = ping(TRIGGER_PIN_ULTRASONIDOS, ECHO_PIN_ULTRASONIDOS);
    if(put_item(cm, &buff_prod[0]) == -1)
    {
        // si el buffer esta lleno y no puedo insertar dato...
    }
    else
    {
      //Serial.printf("Inserto dato %d\n", cm);
    }
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS) );
  }
    Serial.println("Finalizando tarea 1");
    vTaskDelete( NULL );
    
}

/**
 @brief Consumidor. Tarea para interpretar mediciones del sensor y tomar acciones 
 @param buff_prod. Buffer donde se obtienen las medidas
*/
void Consumidor( void * parameter )
{
  int cm;
  TickType_t xLastWakeTime;
  Buffers * buffers = (Buffers *) parameter;
  //Buffer_Circ_Measure * buff_measure = &(buffers->Measure_buffer);
  //Buffer_Circ_Message * buff_message = &(buffers->Message_buffer);
  Serial.printf("Hola desde la tarea 2 en el Core %d\n", xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
  while (!PARAR)
  {
    //if(get_item(&cm, buff_measure) == 0)
    if(get_item(&cm, &(buffers->Measure_buffer)) == 0)
    {
      // Si he podido obtener dato...
      //Serial.printf("saco dato %d\n", cm);
      // Asignamos mensaje segun la distancia
      char value[10];
      if( cm <= 25 )
      {
        strcpy(value, "detect");
        //value = "detect";
        // Parar el motor dc de la cinta...
      }
      else 
      {
        strcpy(value, "libre");
        //value = "libre";
        // Reactivar motor dc de la cinta...
      }

      put_item(value, &(buffers->Message_buffer));
      /*
      // Hacemos documento de json con el mensaje y enviamos por topic
      JsonDocument doc;
      doc["presencia"] = value;
      String ULTRASONIDOS_msg_json;
      serializeJson(doc, ULTRASONIDOS_msg_json);
      enviarMensajePorTopic(TOPIC_PRESENCIA, ULTRASONIDOS_msg_json);
      */
    } 
 
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS));
    
  }
  Serial.println("Finalizando tarea 2");
  vTaskDelete( NULL );
}

/**
 @brief GestorComMQTT. Tarea para gestionar comunicaciones con el broker MQTT
 @param buff_prod. Buffer donde se obtienen los mensajes
*/

void GestorComMQTT( void * parameter )
{
  char msg[10];
  TickType_t xLastWakeTime;
  Buffer_Circ_Message * buff_prod = (Buffer_Circ_Message *) parameter;
  Serial.printf("Hola desde la tarea 3 en el Core %d\n", xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
  while (!PARAR)
  {
    if(get_item(msg, buff_prod) == 0)
    {
      // Si he podido obtener dato...
      
      // Hacemos documento de json con el mensaje y enviamos por topic
      JsonDocument doc;
      doc["presencia"] = msg;
      String ULTRASONIDOS_msg_json;
      serializeJson(doc, ULTRASONIDOS_msg_json);
      enviarMensajePorTopic(TOPIC_PRESENCIA, ULTRASONIDOS_msg_json);
    } 
 
    //vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS));
    
  }
  Serial.println("Finalizando tarea 3");
  vTaskDelete( NULL );
}


/*** End of file ****/