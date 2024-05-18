/**
 * @file s_setup.ino
 * @brief Configuración de pines, inicialización de variables y creación de tareas
 */

#include <string>
#include <ESP32Servo.h>

#define SensorsUpdateInterval 500 ///< Frecuencia de muestreo en milisegundos (0,5 segundo)

/**
 * @brief  Estructura para instanciar buffers circulares protegidos para medidas y mensajes
 * @member Measure_buffer. Buffer para almacenar medidas del sensor de ultrasonidos
 * @member MQTT_buffer. Buffer para almacenar mensajes a publicar en el broker MQTT
 * @member Color_Var. Variable para almacenar el color del LED
 * @member Motor_Var. Variable para almacenar el estado del motor de la cinta
 */
typedef struct Buffers
{
	Buffer_Circ_Measure Measure_buffer;
  Buffer_Circ_MQTT MQTT_buffer;
  Var_Prot_String Color_Var;
  Var_Prot_Bool Motor_Var;
} Buffers;

// Crea un task handle para cada tarea 
TaskHandle_t Ultrasonidos_Task_Handle, Controlador_Task_Handle, 
  GestorComMQTT_Task_Handle, Led_Task_Handle, Motor_Task_Handle;

// Instancia de estructura Buffers que contiene buffers circulares protegidos
static Buffers buffers;

// Declaración de funciones de cada tarea
void GestorComMQTT_Task( void * parameter );
void Ultrasonidos_Task( void * parameter );
void Controlador_Task( void * parameter );
void Led_Task( void * parameter );
void Motor_Task( void * parameter);


/**
 * @brief on_setup. Configuración de pines, inicialización de variables y creación de tareas
 */
void on_setup() 
{

    // Inicializacion de pines 
    pinMode(LED_ROJO, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_AZUL, OUTPUT);
    pinMode(TRIGGER_PIN_ULTRASONIDOS, OUTPUT);  // Trigger como salida
    pinMode(ECHO_PIN_ULTRASONIDOS, INPUT);      // Echo como entrada
    //pinMode(MOTOR_PIN, OUTPUT);
   

    // Configurar interrupción con el botón
    config_button();

    // Mensaje de prueba para comprobar la conexión MQTT
    String hello_msg = String("¡Hola Mundo! Desde dispositivo ") + deviceID;

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
             1);                            /* pin task to core 0 */
    
    /* Crear "GestorComMQTT_Task" */
    xTaskCreate(
             GestorComMQTT_Task,            /* Task function. */
             "GestorComMQTT_Task",          /* name of task. */
             10000,                         /* Stack size of task */
             &buffers.MQTT_buffer,       /* parameter of the task */
             1,                             /* priority of the task */
             &GestorComMQTT_Task_Handle);   /* Task handle to keep track of created task */
    
    /* Crear "Led_Task" */
    xTaskCreate(
             Led_Task,                    /* Task function. */
             "Led_Task",                  /* Name of task. */
             10000,                       /* Stack size of task */
             &buffers.Color_Var,       /* Parameter of the task */
             1,                           /* Priority of the task */
             &Led_Task_Handle);           /* Task handle to keep track of created task */

    /* Crear "Motor_Task" */
    xTaskCreate(
             Motor_Task,                   /* Task function. */
             "Motor_Task",                 /* Name of task. */
             10000,                        /* Stack size of task */
             &buffers.Motor_Var,           /* Parameter of the task */
             2,                            /* Priority of the task */
             &Motor_Task_Handle);          /* Task handle to keep track of created task */

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
    }
    else
    {
        // El dato se ha insertado correctamente en el buffer
        // Serial.printf("Inserto dato %d\n", cm);
    }

    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval / portTICK_PERIOD_MS) );

  }

  // Tarea finalizada
  Serial.println("Finalizando tarea Ultrasonidos_Task");
  vTaskDelete( NULL );
    
}

/**
 * @brief Tarea para interpretar mediciones del sensor y tomar acciones en función de ellas
 * @param parameter. Puntero a la estructura de buffers que contiene las medidas y mensajes
 */
void Controlador_Task( void * parameter )
{
  uint32_t cm;
  TickType_t xLastWakeTime;
  Buffers * buffers = (Buffers *) parameter;

  // Inicialización del tiempo de espera para la tarea periódica.
  xLastWakeTime = xTaskGetTickCount();

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
        strcpy(msg.msg, "detect");
        //put_item("rojo", &(buffers->Color_buffer));
        set_value("rojo", &(buffers->Color_Var));
        set_value(0, &(buffers->Motor_Var));
        //put_item("off", &(buffers->Motor_buffer));
        // Acciones a tomar cuando se detecta un objeto cerca...
        // Parar el motor dc de la cinta...
      }
      else 
      {
        strcpy(msg.msg, "libre");
        //put_item("verde", &(buffers->Color_buffer));
        set_value("verde", &(buffers->Color_Var));
        set_value(1, &(buffers->Motor_Var));
        //put_item("on", &(buffers->Motor_buffer));
        // Acciones a tomar cuando no se detecta ningún objeto cercano...
        // Reactivar motor dc de la cinta...
      }

      // Insertamos el mensaje en el buffer correspondiente     
      msg.topic = TOPIC_PRESENCIA;
      put_item(msg, &(buffers->MQTT_buffer));
     
    } 
 
    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS));
    
  }

  // Tarea finalizada
  Serial.println("Finalizando tarea Controlador_Task");
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
  Serial.println("Finalizando tarea GestorComMQTT_Task");
  vTaskDelete( NULL );

}

/**
 * @brief Tarea para encender el LED según se haya detectad caja o no
 * @param parameter Puntero al buffer donde se obtiene el color del LED
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
    // Intenta obtener un color del buffer
    if(get_value(color, var_prot) == 0)
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
 * @brief Tarea para encender el motor DC según se haya detectad caja o no
 * @param parameter Puntero al buffer donde se obtiene el estado del motor
 */
void Motor_Task( void * parameter)
{
  //char status[10];
  bool status;
  Servo myservo;            // create servo object to control a servo 
  int posVal = 0;           // variable to store the servo position
  TickType_t xLastWakeTime;
  //Buffer_Circ_Message * buff = (Buffer_Circ_Message *) parameter;
  Var_Prot_Bool * var_prot = (Var_Prot_Bool *) parameter;
  
  // Inicialización del tiempo de espera para la tarea periódica
  xLastWakeTime = xTaskGetTickCount();

  // Configuracion del objeto servo
  myservo.setPeriodHertz(50);           // standard 50 hz servo 
  myservo.attach(MOTOR_PIN, 500, 2500);  // attaches the servo on servoPin to the servo 

  // Mensaje de inicio de la tarea
  Serial.printf("Hola desde la tarea Motor_Task en el Core %d\n", xPortGetCoreID());

  // Bucle principal de la tarea
  while (!PARAR)
  {
    // Intenta obtener un color del buffer
    if(get_value(&status, var_prot) == 0)
    {
      // Si se ha obtenido el color correctamente, establece el color del LED
      //if(strcmp(status, "on") == 0)
      if(status == 1)
      {
        Serial.printf("Motor on\n");
        //digitalWrite( MOTOR_PIN, HIGH );
        for (posVal = 0; posVal <= 180; posVal += 1) // goes from 0 degrees to 180 degrees 
        { 
          // in steps of 1 degree 
          myservo.write(posVal);       // tell servo to go to position in variable 'pos' 
          delay(15);                   // waits 15ms for the servo to reach the position 
        } 
        for (posVal = 180; posVal >= 0; posVal -= 1) // goes from 180 degrees to 0 degrees 
        { 
          myservo.write(posVal);       // tell servo to go to position in variable 'pos' 
          delay(15);                   // waits 15ms for the servo to reach the position 
        }
      }
      //else if(strcmp(status, "off") == 0)
      else if(status == 0)
      {
        Serial.printf("Motor off\n");
        //digitalWrite( MOTOR_PIN, LOW );

      }
    } 

    // Espera hasta el próximo intervalo de tiempo
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval / portTICK_PERIOD_MS) );
    
  }

  // Tarea finalizada
  Serial.println("Finalizando tarea Motor_Task");
  vTaskDelete( NULL );

}
/*** End of file ****/