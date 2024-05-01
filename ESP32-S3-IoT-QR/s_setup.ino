/**
 @file s_setup.ino
*/

#include <string>

#define SensorsUpdateInterval 1000 // 1 segundo de frecuencia de muestreo

void QRCodeReader( void * parameter);
void Consumidor( void * parameter);

// creating a task handle
TaskHandle_t  Consumidor_Task, QRCodeReader_Task;
// creating buffer circular protegido
static Buffer_Circ Mi_buffer;

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
             &Mi_buffer,            /* parameter of the task */
             1,                     /* priority of the task */
             &QRCodeReader_Task,    /* Task handle to keep track of created task */
             0);                    /* pin task to core 0 */

    /* Create "Consumidor_Task" using the xTaskCreatePinnedToCore() function */
    xTaskCreatePinnedToCore(
             Consumidor,            /* Task function. */
             "Consumidor",          /* name of task. */
             10000,                 /* Stack size of task */
             &Mi_buffer,            /* parameter of the task */
             1,                     /* priority of the task */
             &Consumidor_Task,      /* Task handle to keep track of created task */
             1);                    /* pin task to core 0 */
    
    delay(1000);

}

/**
 @brief QRCodeReader. Tarea para leer codigo QR
 @param buff_prod. Buffer donde se almacena el codigo QR
*/
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
      if (q == NULL){
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
      if (count > 0) {
        quirc_extract(q, 0, &code);
        err = quirc_decode(&code, &data);
    
        if (err){
          Serial.println("Decoding FAILED");
          //QRCodeResult = "Decoding FAILED";
        } else {
          Serial.printf("Decoding successful:\n");
          dumpData_bis(buff_prod, &data);
          //put_item(data,buff_prod);
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
  String qr, qr_last = "";
  TickType_t xLastWakeTime;
  Buffer_Circ * buff_prod = (Buffer_Circ *) parameter;
  Serial.printf("Hola desde la tarea 2 en el Core %d\n", xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
  while (!PARAR)
  {
    if(get_item(qr, buff_prod) == 0)
    {
      // Si he podido obtener dato...
      //Serial.printf("saco dato %d\n", cm);
      // Asignamos mensaje segun la distancia
      String value;
      if( !qr.equals(qr_last) )
      {
        value = "detectado";
        /*
        // Hacemos documento de json con el mensaje y enviamos por topic
        JsonDocument doc;
        doc["codProducto"] = qr;
        String QR_msg_json;
        serializeJson(doc, QR_msg_json);
        enviarMensajePorTopic(TOPIC_QR, QR_msg_json);
        */
        qr_last = qr;
        // Parar el motor dc de la cinta...
      }
      else 
      {
        value = "libre";
        // Reactivar motor dc de la cinta...
      }
      
      // Hacemos documento de json con el mensaje y enviamos por topic
      JsonDocument doc;
      doc["presencia"] = value;
      String QR_msg_json;
      serializeJson(doc, QR_msg_json);
      enviarMensajePorTopic(TOPIC_QR, QR_msg_json);
    } 
 
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS));
    
  }
  Serial.println("Finalizando tarea 2");
  vTaskDelete( NULL );
}

/*
// Tarea para gestionar comunicaciones con el broker MQTT
void GestorComunicMQTT( void * parameter )
{
  int cm;
  TickType_t xLastWakeTime;
  Buffer_Circ * buff_prod = (Buffer_Circ *) parameter;
  Serial.printf("Hola desde la tarea 2 en el Core %d\n", xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
  while (!PARAR)
  {
    if(get_item(&cm, buff_prod) == 0)
    {
      // Si he podido obtener dato...
      Serial.printf("saco dato %d\n", cm);
      // Asignamos mensaje segun la distancia
      String value;
      if( cm <= 25 )
      {
        value = "detectado";
        // Parar el motor dc de la cinta...
      }
      else 
      {
        value = "libre";
        // Reactivar motor dc de la cinta...
      }

      // Hacemos documento de json con el mensaje y enviamos por topic
      JsonDocument doc;
      doc["presencia"] = value;
      String ULTRASONIDOS_msg_json;
      serializeJson(doc, ULTRASONIDOS_msg_json);
      enviarMensajePorTopic(TOPIC_PRESENCIA, ULTRASONIDOS_msg_json);
    } 
 
    //vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS));
    
  }
  Serial.println("Finalizando tarea 2");
  vTaskDelete( NULL );
}
*/

/*** End of file ****/