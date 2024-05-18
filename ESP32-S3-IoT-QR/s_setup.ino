/**
 * @file  s_setup.ino
 * @brief Configuración de pines, inicialización de variables y creación de tareas
 */


/**
 * @brief  Estructura para instanciar un buffer circular protegido para mensajes y otro para medidas 
 * @member QR_buffer. Buffer para guardar el codigo de producto que contiene el QR leído
 * @member MQTT_buffer. Buffer para almacenar mensajes a publicar en el broker MQTT
 * @member Color_buffer. Buffer para guardar el color del LED segun se lea o no qr
 */
typedef struct Buffers
{
	Buffer_Circ_String QR_buffer;
  Buffer_Circ_MQTT MQTT_buffer;
  Buffer_Circ_String Color_buffer;
} Buffers;

// Crea un task handle para cada tarea 
TaskHandle_t  Controlador_Task_Handle, GestorComMQTT_Task_Handle, 
  QRCodeReader_Task_Handle, Led_Task_Handle;

// Instancia de estructura Buffers que contiene buffers circulares protegidos
static Buffers buffers;

// Declaración de funciones de cada tarea
void Controlador_Task( void * parameter);
void QRCodeReader_Task( void * parameter);
void GestorComMQTT_Task( void * parameter);
void Led_Task( void * parameter);


/**
 * @brief Configuración de pines, inicialización de variables y creación de tareas
 */
void on_setup() 
{

    // Inicializacion de pines 
    pinMode(LED_ROJO, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);

    // Configurar la cámara y la interrupción con el botón
    config_camara();
    config_button();

    // Mensaje de prueba para comprobar la conexión MQTT
    String hello_msg = String("Hola Mundo! Desde dispositivo ") + deviceID;

    // Generar documento JSON con el mensaje y enviar por MQTT
    JsonDocument doc;
    doc["message"] = hello_msg;
    String hello_msg_json;
    serializeJson(doc, hello_msg_json);
    enviarMensajePorTopic(HELLO_TOPIC, hello_msg_json);

    /* Create "QRCodeReader_Task" */
    xTaskCreatePinnedToCore(
             QRCodeReader_Task,             /* Task function. */
             "QRCodeReader_Task",           /* name of task. */
             10000,                         /* Stack size of task */
             &buffers.QR_buffer,            /* parameter of the task */
             1,                             /* priority of the task */
             &QRCodeReader_Task_Handle,     /* Task handle to keep track of created task */
             0);                            /* pin task to core 0 */

    /* Crear "Controlador_Task" */
    xTaskCreatePinnedToCore(
             Controlador_Task,              /* Task function. */
             "Controlador_Task",            /* name of task. */
             10000,                         /* Stack size of task */
             &buffers,                      /* parameter of the task */
             1,                             /* priority of the task */
             &Controlador_Task_Handle,      /* Task handle to keep track of created task */
             0);                            /* pin task to core 0 */
    
  /* Crear "Led_Task" */
    xTaskCreate(
             Led_Task,                      /* Task function. */
             "Led_Task",                    /* Name of task. */
             10000,                         /* Stack size of task */
             &buffers.Color_buffer,         /* Parameter of the task */
             1,                             /* Priority of the task */
             &Led_Task_Handle);             /* Task handle to keep track of created task */

    /* Crear "GestorComMQTT_Task" */
    xTaskCreate(
             GestorComMQTT_Task,            /* Task function. */
             "GestorComMQTT_Task",          /* name of task. */
             10000,                         /* Stack size of task */
             &buffers.MQTT_buffer,          /* parameter of the task */
             1,                             /* priority of the task */
             &GestorComMQTT_Task_Handle);   /* Task handle to keep track of created task */
  
    delay(1000);

}

/**
 * @brief Tarea que lee códigos QR y procesa las imágenes capturadas por la cámara
 * @param parameter. Puntero a la estructura de buffer circular
 */
void QRCodeReader_Task( void * parameter )
{
  TickType_t xLastWakeTime;
  Buffer_Circ_String * buff = (Buffer_Circ_String *) parameter;

  // Inicialización del tiempo de espera para la tarea periódica
  xLastWakeTime = xTaskGetTickCount();

  // Mensaje de inicio de la tarea
  Serial.printf("Hola desde la tarea QRCodeReader_Task en el Core %d\n",xPortGetCoreID());
  
  // Bucle principal de la tarea
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
        } 
        else 
        {
          Serial.printf("Decoding successful:\n");
          dumpData_bis(buff, &data);
          // Espera hasta el próximo intervalo de tiempo
          vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS));
        } 

        Serial.println();
      } 
      
      esp_camera_fb_return(fb);
      fb = NULL;
      image = NULL;  
      quirc_destroy(q);

    }

  // Tarea finalizada
  Serial.println("Finalizando tarea QRCodeReader_Task");
  vTaskDelete( NULL );  

}


/**
 * @brief Tarea para interpretar mediciones del sensor y tomar acciones en función de ellas
 * @param parameter. Puntero a la estructura de buffers que contiene los codigos de producto 
 *                   leídos de QRs y el color del LED a encender
 */
void Controlador_Task( void * parameter )
{
  char qr[10];
  TickType_t xLastWakeTime;
  Buffers * buff = (Buffers *) parameter;
  Serial.printf("Hola desde la tarea Controlador_Task en el Core %d\n", xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
  while (!PARAR)
  {
    if(get_item(qr, &(buff->QR_buffer)) == 0)
    {
      // Si he podido obtener dato...
      // Guardamos mensaje a publicar y su topic en el buffer de mensajes MQTT  
        Msg_MQTT msg_qr;
        msg_qr.topic = TOPIC_QR; 
        strcpy(msg_qr.msg, qr);
        put_item(msg_qr, &(buff->MQTT_buffer));
      // Guardamos el color verde en el buffer para cambiar de LED
        put_item("verde", &(buff->Color_buffer));
    } 
    else
    {
      //  Si no se ha podido obtener dato...
      // Guardamos el color rojo en el buffer para cambiar de LED
        put_item("rojo", &(buff->Color_buffer));
    }

    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval / portTICK_PERIOD_MS) );
    
  }

  // Tarea finalizada
  Serial.println("Finalizando tarea Controlador_Task");
  vTaskDelete( NULL );

}

/**
 * @brief Tarea para encender el LED correspondiente según se haya realizado 
 *        la lectura de un código QR o no
 * @param parameter. Puntero al buffer donde se obtiene el color del LED a encender
 */
void Led_Task( void * parameter)
{
  char color[10];
  TickType_t xLastWakeTime;
  Buffer_Circ_String * buff = (Buffer_Circ_String *) parameter;

  // Inicialización del tiempo de espera para la tarea periódica
  xLastWakeTime = xTaskGetTickCount();

  // Mensaje de inicio de la tarea
  Serial.printf("Hola desde la tarea Led_Task en el Core %d\n", xPortGetCoreID());

  // Bucle principal de la tarea
  while (!PARAR)
  {
    // Intenta obtener un color del buffer
    if(get_item(color, buff) == 0)
    {
      // Si se ha obtenido el color correctamente, establece el color del LED
      setColorLed((const char *)color);
    } 
    
    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval / portTICK_PERIOD_MS) );
    
  }

  // Tarea finalizada
  Serial.println("Finalizando tarea Led_Task");
  vTaskDelete( NULL );

}

/**
 * @brief Tarea para gestionar las comunicaciones con el broker MQTT
 * @param parameter. Puntero al buffer donde se obtienen los mensajes 
 *                   para enviarlos por MQTT
 */
void GestorComMQTT_Task( void * parameter )
{
  Msg_MQTT msg_to_publish;
  TickType_t xLastWakeTime;
  Buffer_Circ_MQTT * buff = (Buffer_Circ_MQTT *) parameter;

  // Inicialización del tiempo de espera para la tarea periódica
  xLastWakeTime = xTaskGetTickCount();

  Serial.printf("Hola desde la tarea GestorComMQTT_Task en el Core %d\n", xPortGetCoreID());
  
  // Bucle principal de la tarea
  while (!PARAR)
  {
    // Obtenemos un mensaje del buffer
    if(get_item(&msg_to_publish, buff) == 0)
    {
      // Si se ha obtenido el mensaje correctamente...
      if(strcmp(msg_to_publish.topic, TOPIC_QR) == 0)
      {
        // Convertimos el mensaje en formato JSON y lo enviamos por el topic correspondiente
        JsonDocument doc;
        doc["codProducto"] = msg_to_publish.msg;
        String msg_json;
        serializeJson(doc, msg_json);
        enviarMensajePorTopic(msg_to_publish.topic, msg_json);
      }

    } 
 
    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval / portTICK_PERIOD_MS) );
    
  }

  // Tarea finalizada
  Serial.println("Finalizando tarea GestorComMQTT_Task");
  vTaskDelete( NULL );

}

/*** End of file ****/