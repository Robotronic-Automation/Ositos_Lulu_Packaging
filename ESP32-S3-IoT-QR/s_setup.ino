/**
 @file s_setup.ino
*/

#include <string>

#define SensorsUpdateInterval 1000 // 1 segundo de frecuencia de muestreo

/**
 * @brief  Estructura para instanciar un buffer circular protegido para mensajes y otro para medidas 
 * @member Measure_buffer. Buffer para guardar medidas del sensor de ultrasonidos
 * @member Color_buffer. Buffer para guardar el color del LED segun se lea o no qr
 */
typedef struct Buffers
{
	Buffer_Circ QR_buffer;
  Buffer_Circ Color_buffer;
} Buffers;

// creating a task handle
TaskHandle_t  Consumidor_Task, QRCodeReader_Task, Led_Task_handle;


// creating buffer circular protegido
//static Buffer_Circ Mi_buffer;

// Crea una instancia de la estructura buffers que contiene los buffers circulares protegidos
static Buffers buffers;

// Declaracion de las funciones de cada tarea
void QRCodeReader( void * parameter);
void Consumidor( void * parameter);
void Led_Task( void * parameter);


/**
 @brief on_setup. Añadir la configuración de pines, inicialización de variables, 
        configurar interrupciones, crear tasks, etc
 @param  ninguno
 @return ninguno
*/
void on_setup() 
{

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_ROJO, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_AZUL, OUTPUT);

    // Mensaje para comprobar la conexion MQTT
    String hello_msg = String("Hola Mundo! Desde dispositivo ") + deviceID;

    // Test JSON
    JsonDocument doc;
    doc["message"] = hello_msg;
    //doc["luminosidad"] = 450;
    //doc["temperatura"] = 21.5;
    String hello_msg_json;
    serializeJson(doc, hello_msg_json);
    enviarMensajePorTopic(HELLO_TOPIC, hello_msg_json);

    config_camara();
    config_button();

    /* Create "QRCodeReader_Task" using the xTaskCreatePinnedToCore() function */
    xTaskCreatePinnedToCore(
             QRCodeReader,          /* Task function. */
             "QRCodeReader_Task",   /* name of task. */
             10000,                 /* Stack size of task */
             &buffers.QR_buffer,    /* parameter of the task */
             1,                     /* priority of the task */
             &QRCodeReader_Task,    /* Task handle to keep track of created task */
             1);                    /* pin task to core 0 */

    /* Create "Consumidor_Task" using the xTaskCreatePinnedToCore() function */
    xTaskCreatePinnedToCore(
             Consumidor,            /* Task function. */
             "Consumidor",          /* name of task. */
             10000,                 /* Stack size of task */
             &buffers,              /* parameter of the task */
             1,                     /* priority of the task */
             &Consumidor_Task,      /* Task handle to keep track of created task */
             1);                    /* pin task to core 0 */
    
    /* Create "Led_Task" using the xTaskCreatePinnedToCore() function */
    xTaskCreatePinnedToCore(
             Led_Task,              /* Task function. */
             "Led_Task",            /* name of task. */
             10000,                 /* Stack size of task */
             &buffers.Color_buffer, /* parameter of the task */
             1,                     /* priority of the task */
             &Led_Task_handle,      /* Task handle to keep track of created task */
             1);                    /* pin task to core 0 */
    
    delay(1000);

}

void QRCodeReader( void * parameter )
{
  Serial.println("QRCodeReader is ready.");
  Serial.print("QRCodeReader running on core ");
  Serial.println(xPortGetCoreID());
  Serial.println();
  TickType_t xLastWakeTime;
  Buffer_Circ * buff_prod = (Buffer_Circ *) parameter;
//Serial.printf("Hola desde la tarea 1 en el Core %d\n",xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
    while (!PARAR)
    {
      q = quirc_new();
      if (q == NULL)
      {
        Serial.print("can't create quirc object\r\n");  
        continue;
      }
    
      fb = esp_camera_fb_get();
      if (!fb)
      {
        Serial.println("Camera capture failed");
        continue;
      }   
      
      quirc_resize(q, fb->width, fb->height);
      image = quirc_begin(q, NULL, NULL);
      memcpy(image, fb->buf, fb->len);
      quirc_end(q);
      
      int count = quirc_count(q);
      if (count > 0) 
      {
        quirc_extract(q, 0, &code);
        err = quirc_decode(&code, &data);
    
        if (err)
        {
          Serial.println("Decoding FAILED");
          //QRCodeResult = "Decoding FAILED";
        } 
        else 
        {
          Serial.printf("Decoding successful:\n");
          dumpData_bis(buff_prod, &data);
          vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS));
        } 
        Serial.println();
      } 
      
      esp_camera_fb_return(fb);
      fb = NULL;
      image = NULL;  
      quirc_destroy(q);

        /*
        digitalWrite(LED_1, HIGH);
        vTaskDelayUntil( &xLastWakeTime, (T_espera/ portTICK_PERIOD_MS));
        digitalWrite(LED_1, LOW);
        vTaskDelayUntil( &xLastWakeTime, (T_espera/ portTICK_PERIOD_MS));
        */
    }
    Serial.println("Finalizando tarea 1");
    vTaskDelete( NULL );  
}


/**
 @brief Consumidor. Tarea para interpretar mediciones del sensor y tomar acciones 
 @param buff_prod. Buffer donde se obtiene el codigo QR
*/
void Consumidor( void * parameter )
{
  //String qr;
  char qr[10];
  TickType_t xLastWakeTime;
  Buffers * buff_prod = (Buffers *) parameter;
  Serial.printf("Hola desde la tarea 2 en el Core %d\n", xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
  while (!PARAR)
  {
    if(get_item(qr, &(buff_prod->QR_buffer)) == 0)
    {
      // Si he podido obtener dato...
        // Hacemos documento de json con el mensaje y enviamos por topic
        JsonDocument doc;
        doc["codProducto"] = qr;
        printf("QR: %s\n", qr);
        String QR_msg_json;
        serializeJson(doc, QR_msg_json);
        enviarMensajePorTopic(TOPIC_QR, QR_msg_json);

        put_item("verde", &(buff_prod->Color_buffer));
    } 
    else
    {
      //  Si no se ha podido obtener dato...
        put_item("rojo", &(buff_prod->Color_buffer));
    }

    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS));
    
  }
  Serial.println("Finalizando tarea 2");
  vTaskDelete( NULL );
}

/**
 @brief Led. Tarea para encender el led segun presencia o no de qr:
              "rojo" --> no hay qr    "verde" --> hay qr
 @param buff_prod. Buffer donde se obtiene el color del led
*/
void Led_Task( void * parameter)
{
  char color[10];
  TickType_t xLastWakeTime;
  Buffer_Circ * buff_prod = (Buffer_Circ *) parameter;
  Serial.printf("Hola desde la tarea 3 en el Core %d\n", xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
  while (!PARAR)
  {
    if(get_item(color, buff_prod) == 0)
    {
      // Si he podido obtener dato...
        if(strcmp(color,"rojo") == 0)
        {
            // No se ha leido qr
            printf("Led rojo encendido\n");
            digitalWrite(LED_VERDE, HIGH);
            digitalWrite(LED_ROJO, LOW);

        }
        else if(strcmp(color,"verde") == 0)
        {
            // Se ha leido qr
            printf("Led verde encendido\n");
            digitalWrite(LED_ROJO, HIGH);
            digitalWrite(LED_VERDE, LOW);

        }
        else
        {
            // El mensaje no es valido
            digitalWrite(LED_ROJO, LOW);
            digitalWrite(LED_VERDE, LOW);
        }

    } 
    
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS));
    
  }
  Serial.println("Finalizando tarea 2");
  vTaskDelete( NULL );
}



/*** End of file ****/