/**
 @file w_loop.ino
*/

#include <string>

//long now, lastMsg = 0;             // now --> current time  ; lastMsg --> last time a message was sent
//long sensorsUpdateInterval = 1000; // Tiempo de actualización de los sensores

/**
 @brief on_loop. Funcion con las tareas a realizar dentro del 'loop'
 @param  ninguno
 @return ninguno
*/
void on_loop() 
{
/*
  now = millis();
  if ( now - lastMsg > sensorsUpdateInterval ) 
  {
    lastMsg = now;
    
    //
    // Read and process sensors
    //

    // Leemos sensor ultrasonidos y guardamos distancia
    int cm = ping(TRIGGER_PIN_ULTRASONIDOS, ECHO_PIN_ULTRASONIDOS);
    Serial.println(cm);
    //enviarMensajePorTopic(LDR_STATUS, String(value));

    // Asignamos mensaje segun la distancia
    String value;
    if( cm <= 25 )
    {
      value = "detectado";
    }
    else 
    {
      value = "libre";
    }

    // Hacemos documento de json con el mensaje y enviamos por topic
    JsonDocument doc;
    doc["presencia"] = value;
    String ULTRASONIDOS_msg_json;
    serializeJson(doc, ULTRASONIDOS_msg_json);
    enviarMensajePorTopic(TOPIC_PRESENCIA, ULTRASONIDOS_msg_json);
    
/*
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    enviarMensajePorTopic("esp32/temperature", tempString);


  }
*/  
 // put your main code here, to run repeatedly:
  Serial.println("Inicio Main");
  while (!PARAR)
  {
    if (button1.pressed) {
      Serial.printf("El botón se ha pulsado %u veces\n", button1.numberKeyPresses);
      digitalWrite( LED_1, HIGH );
      digitalWrite( LED_2, HIGH );
      button1.pressed = false;
    }
    delay(100);
  }
  Serial.println("CERRANDO");
  delay(3000);
  Serial.println("FIN Main");
  exit(0);

}

/*** End of file ****/