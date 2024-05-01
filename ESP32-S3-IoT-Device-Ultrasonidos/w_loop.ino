/**
 @file w_loop.ino
*/

#include <string>

/**
 @brief on_loop. Funcion con las tareas a realizar dentro del 'loop'
 @param  ninguno
 @return ninguno
*/
void on_loop() 
{
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