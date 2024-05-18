/**
 * @file  s_setup.ino
 * @brief Configuración de pines, inicialización de variables y creación de tareas
 */


// Crea un task handle para cada tarea 
TaskHandle_t Ultrasonidos_Task_Handle, Controlador_Task_Handle, 
  GestorComMQTT_Task_Handle, Led_Task_Handle, Led_Azul_Task_Handle, Motor_Task_Handle;

// Declaración de funciones de cada tarea
void GestorComMQTT_Task( void * parameter );
void Ultrasonidos_Task( void * parameter );
void Controlador_Task( void * parameter );
void Led_Task( void * parameter );
void Led_Azul_Task( void * parameter );
void Motor_Task( void * parameter );


/**
 * @brief Configuración de pines, inicialización de variables y creación de tareas
 */
void on_setup() 
{

    // Inicialización de pines 
    pinMode(LED_ROJO, OUTPUT);                  // Led rojo como salida
    pinMode(LED_VERDE, OUTPUT);                 // Led verde como salida
    pinMode(LED_AZUL, OUTPUT);                  // Led azul como salida
    pinMode(TRIGGER_PIN_ULTRASONIDOS, OUTPUT);  // Trigger como salida
    pinMode(ECHO_PIN_ULTRASONIDOS, INPUT);      // Echo como entrada   

    // Configuración de la interrupción con el botón y del servo
    config_button();
    servo_set_pin(SERVO_PIN);

    // Mensaje de prueba para comprobar la conexión MQTT
    String hello_msg = String("¡Conectado! Desde dispositivo ") + deviceID;

    // Generar documento JSON con el mensaje y enviar por MQTT
    JsonDocument doc;
    doc["message"] = hello_msg;
    String hello_msg_json;
    serializeJson(doc, hello_msg_json);
    enviarMensajePorTopic(HELLO_TOPIC, hello_msg_json);

    /* Crear "Ultrasonidos_Task" */
    xTaskCreatePinnedToCore(
             Ultrasonidos_Task,             /* Task function. */
             "Ultrasonidos_Task",           /* name of task. */
             10000,                         /* Stack size of task */
             &buffers.Measure_buffer,       /* parameter of the task */
             1,                             /* priority of the task */
             &Ultrasonidos_Task_Handle,     /* Task handle to keep track of created task */
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
    
    /* Crear "GestorComMQTT_Task" */
    xTaskCreatePinnedToCore(
             GestorComMQTT_Task,            /* Task function. */
             "GestorComMQTT_Task",          /* name of task. */
             10000,                         /* Stack size of task */
             &buffers.MQTT_buffer,          /* parameter of the task */
             1,                             /* priority of the task */
             &GestorComMQTT_Task_Handle,    /* Task handle to keep track of created task */
             0);                            /* pin task to core 0 */
    /* Crear "Led_Task" */
    xTaskCreatePinnedToCore(
             Led_Task,                      /* Task function. */
             "Led_Task",                    /* Name of task. */
             10000,                         /* Stack size of task */
             &buffers.Color_Var,            /* Parameter of the task */
             1,                             /* Priority of the task */
             &Led_Task_Handle,              /* Task handle to keep track of created task */
             0);                            /* pin task to core 0 */

    /* Crear "Led_Azul_Task" */
    xTaskCreatePinnedToCore(
             Led_Azul_Task,                 /* Task function. */
             "Led_Azul_Task",               /* Name of task. */
             10000,                         /* Stack size of task */
             &buffers.Azul_Var,             /* Parameter of the task */
             1,                             /* Priority of the task */
             &Led_Azul_Task_Handle,         /* Task handle to keep track of created task */
             0);                            /* pin task to core 0 */

    /* Crear "Motor_Task" */
    xTaskCreatePinnedToCore(
             Motor_Task,                    /* Task function. */
             "Motor_Task",                  /* Name of task. */
             10000,                         /* Stack size of task */
             &buffers.Motor_Var,            /* Parameter of the task */
             1,                             /* Priority of the task */
             &Motor_Task_Handle,            /* Task handle to keep track of created task */
             0);                            /* pin task to core 0 */

    delay(1000);

}

/**
 * @brief Tarea para tomar mediciones del sensor de ultrasonidos
 * @param parameter. Puntero al buffer donde se almacenan las medidas
 */
void Ultrasonidos_Task( void * parameter )
{
  TickType_t xLastWakeTime;
  Buffer_Circ_Measure * buff = (Buffer_Circ_Measure *) parameter;

  // Inicialización del tiempo de espera para la tarea periódica.
  xLastWakeTime = xTaskGetTickCount();

  // Mensaje de inicio de la tarea
  Serial.printf("Hola desde la tarea Ultrasonidos_Task en el Core %d\n", xPortGetCoreID());

  // Bucle principal de la tarea
  while (!PARAR)
  {
    // Leer el sensor de ultrasonidos y guardar la distancia
    uint32_t cm = ping(TRIGGER_PIN_ULTRASONIDOS, ECHO_PIN_ULTRASONIDOS);
    if(put_item(cm, buff) == -1)
    {
        // Manejo de la situación en la que el buffer está lleno
        // En esta situación, el dato no se puede insertar en el buffer
        warnln("**>> Distancia no guardada!");
    }
    else
    {
        // El dato se ha insertado correctamente en el buffer
    }

    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval / portTICK_PERIOD_MS) );

  }

  // Tarea finalizada
  traceln("Finalizando tarea Ultrasonidos_Task");
  vTaskDelete( NULL );
    
}

/**
 * @brief Tarea para interpretar mediciones del sensor y tomar acciones en función de ellas
 * @param parameter. Puntero a la estructura de buffers y variables protegidas
 *                   que contienen las medidas y mensajes
 */
void Controlador_Task( void * parameter )
{
  uint32_t cm;
  TickType_t xLastWakeTime;
  Buffers * buffers = (Buffers *) parameter;

  // Inicialización del tiempo de espera para la tarea periódica.
  xLastWakeTime = xTaskGetTickCount();

  // Mensaje de inicio de la tarea
  Serial.printf("Hola desde la tarea Controlador_Task en el Core %d\n", xPortGetCoreID());
  
  // Bucle principal de la tarea
  while (!PARAR)
  {
    // Obtenemos la medida del buffer
    if(get_item(&cm, &(buffers->Measure_buffer)) == 0)
    {
      // Si se ha obtenido la medida correctamente...  
      // Asignamos un mensaje según la distancia medida
      Msg_MQTT msg;

      if( cm <= 25 )
      {
        // Acciones a tomar cuando se detecta un objeto cerca...
        // Parar el motor de la cinta, encender led rojo, e indicar al cobot
        //  que hay caja 
        strcpy(msg.msg, "detect");
        set_value("rojo", &(buffers->Color_Var));
        set_value(0, &(buffers->Motor_Var));
      }
      else 
      {
        // Acciones a tomar cuando no se detecta ningún objeto cercano
        // Reactivar motor de la cinta, encender led verde, e indicar al cobot
        //  que no hay caja 
        strcpy(msg.msg, "libre");
        set_value("verde", &(buffers->Color_Var));
        set_value(1, &(buffers->Motor_Var));    
      }

      // Insertamos el mensaje en el buffer correspondiente     
      msg.topic = TOPIC_PRESENCIA;
      put_item(msg, &(buffers->MQTT_buffer));
     
    } 
 
    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval / portTICK_PERIOD_MS));
    
  }

  // Tarea finalizada
  traceln("Finalizando tarea Controlador_Task");
  vTaskDelete( NULL );

}

/**
 * @brief Tarea para gestionar las comunicaciones con el broker MQTT
 * @param parameter. Puntero al buffer donde se obtienen los mensajes para enviarlos por MQTT
 */
void GestorComMQTT_Task( void * parameter )
{
  Msg_MQTT msg_to_publish;
  TickType_t xLastWakeTime;
  Buffer_Circ_MQTT * buff = (Buffer_Circ_MQTT *) parameter;

  // Inicialización del tiempo de espera para la tarea periódica
  xLastWakeTime = xTaskGetTickCount();

  // Mensaje de inicio de la tarea
  Serial.printf("Hola desde la tarea GestorComMQTT_Task en el Core %d\n", xPortGetCoreID());
  
  // Bucle principal de la tarea
  while (!PARAR)
  {
    // Obtenemos un mensaje del buffer
    if(get_item(&msg_to_publish, buff) == 0)
    {
      // Si se ha obtenido el mensaje correctamente...
      // Convertimos el mensaje en formato JSON y lo enviamos por el topic correspondiente
      if(strcmp(msg_to_publish.topic, TOPIC_PRESENCIA) == 0)
      {
        JsonDocument doc;
        doc["presencia"] = msg_to_publish.msg;
        String msg_json;
        serializeJson(doc, msg_json);
        enviarMensajePorTopic(msg_to_publish.topic, msg_json);
      }
    } 
 
    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS) );
    
  }

  // Tarea finalizada
  traceln("Finalizando tarea GestorComMQTT_Task");
  vTaskDelete( NULL );

}

/**
 * @brief Tarea para encender el LED correspondiente según se haya detectado caja o no
 * @param parameter. Puntero a la variable donde se obtiene el color del LED a encender
 */
void Led_Task( void * parameter)
{
  char color[10];
  TickType_t xLastWakeTime;
  Var_Prot_String * var_prot = (Var_Prot_String *) parameter;

  // Inicialización del tiempo de espera para la tarea periódica
  xLastWakeTime = xTaskGetTickCount();

  // Mensaje de inicio de la tarea
  Serial.printf("Hola desde la tarea Led_Task en el Core %d\n", xPortGetCoreID());

  // Bucle principal de la tarea
  while (!PARAR)
  {
    // Intenta obtener un color de la variable protegida
    if(get_value(color, var_prot) == 0)
    {
      // Si se ha obtenido el color correctamente, enciende el LED correspondiente
      setColorLed((const char *)color);  
    } 

    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval / portTICK_PERIOD_MS) );
    
  }

  // Tarea finalizada
  traceln("Finalizando tarea Led_Task");
  vTaskDelete( NULL );

}

/**
 * @brief Tarea para encender el LED azul según el cobot esté operando o no  
 * @param parameter. Puntero a la variable donde se obtiene el estado del LED azul
 */
void Led_Azul_Task( void * parameter )
{
  char status[10];
  TickType_t xLastWakeTime;
  Var_Prot_String * var_prot = (Var_Prot_String *) parameter;

  // Inicialización del tiempo de espera para la tarea periódica
  xLastWakeTime = xTaskGetTickCount();

  // Mensaje de inicio de la tarea
  Serial.printf("Hola desde la tarea Led_Azul_Task en el Core %d", xPortGetCoreID());

  // Estado inicial del cobot: inactivo
  set_value("azul_off", var_prot);

  // Bucle principal de la tarea
  while (!PARAR)
  {
    // Intenta obtener un color de la variable protegida
    if(get_value(status, var_prot) == 0)
    {
      // Si se ha obtenido el estado correctamente, enciende o apaga el LED azul
      setAzulLed((const char *)status);  
    } 

    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval / portTICK_PERIOD_MS) );
    
  }

  // Tarea finalizada
  traceln("Finalizando tarea Led_Azul_Task");
  vTaskDelete( NULL );

}


/**
 * @brief Tarea para encender el motor de la cinta según se haya detectado caja o no
 * @param parameter. Puntero a la variable donde se obtiene el estado del motor
 */
void Motor_Task( void * parameter)
{
  bool status;
  TickType_t xLastWakeTime;
  Var_Prot_Bool * var_prot = (Var_Prot_Bool *) parameter;
  
  // Inicialización del tiempo de espera para la tarea periódica
  xLastWakeTime = xTaskGetTickCount();

  // Mensaje de inicio de la tarea
  Serial.printf("Hola desde la tarea Motor_Task en el Core %d\n", xPortGetCoreID());

  // Bucle principal de la tarea
  while (!PARAR)
  {
    // Intenta obtener un estado de la variable protegida
    if(get_value(&status, var_prot) == 0)
    {
      // Si se ha obtenido el estado correctamente, enciende o apaga el motor
      if(status == 1)
      {
        infoln("Motor on");

        for (int i = 0; i < 180; i++) 
        {
          servo_set_angle(i);
          delay(10);
        }
        for (int i = 180; i > 0; i--) 
        {
          servo_set_angle(i);
          delay(10);
        }
      }
      else if(status == 0)
      {
        infoln("Motor off");
      }
    } 

    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval / portTICK_PERIOD_MS) );
    
  }

  // Tarea finalizada
  traceln("Finalizando tarea Motor_Task");
  vTaskDelete( NULL );

}

/*** End of file ****/