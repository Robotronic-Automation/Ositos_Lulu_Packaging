# ESP32-S3-IoT-Ultrasonidos

## Contenidos
  - [Descripción](#descripción)
  - [Librerías](#librerías)
  - [Entorno](#entorno)
  - [Documentación](#documentación)
  - [Esquema del Circuito](#esquema-del-circuito)
  - [Disposición en Planta](#disposición-en-planta)
  - [Diagramas de comunicación MQTT](#diagramas-de-comunicación-mqtt)
  - [Funcionalidades Locales del Dispositivo](#funcionalidades-locales-del-dispositivo)
  - [Instrucciones para la Instalación e Integración](#instrucciones-para-la-instalación-e-integración)

---

## Descripción

Sistema basado en una ESP32-S3 que detecta presencia de caja en la cinta mediante un sensor ultrasonidos, parando la cinta y avisando al cobot para que paletice en caso de detectar caja. Además, se señaliza el estado de la cinta mediante 2 LEDs, uno rojo que indica que está parada (detección de caja) y otro verde cuando está activa (no peresencia de caja), y un LED azul para indicar cuando el cobot está operando (LED encendido) o no (LED apagado). La comunicación inalámbrica se realiza mediante el protocolo MQTT y la comunicación local entre tareas haciendo uso de buffers circulares protegidos y variables protegidas. Como medida de seguridad se implementa una interrupción activada por la pulsación de un botón que para todo el sistema.

<br>

---

## Entorno

El código se ha desarrollado en el entorno de desarrollo integrado de Arduino, [Arduino IDE](https://www.arduino.cc/en/software). Para establecer la comunicación MQTT se recomienda el programa [MQTTX](https://mqttx.app/downloads), aunque se puede utilizar cualquier otro.

<br>

<p align="center">
		<img height=100 width=100 align="center" src="./images/arduino_logo.jpg" />
		<img height=100 width=100 align="center" src="./images/mqttx_logo.png" />
</p>

<br>

---

## Librerías

Una vez instalados los programas, descargar las siguientes librerías:
* `ArduinoJson` by Benoit Blanchon (Library Manager): [descarga](https://arduinojson.org/?utm_source=meta&utm_medium=library.properties)
* `PubSubClient` by Nick O'Leary (Library Manager): [descarga](https://github.com/knolleary/pubsubclient/releases/tag/v2.8)
* `esp32` by Espressif Systems (Boards Manager): [descarga](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)

<br>

---

## Documentación

Puedes usar la [ESP32-S3-IoT-Ultrasonidos Online Documentation](https://tamala24.github.io/ESP32-S3-Ultrasonidos-Doc/) para obtener toda la información sobre el código de este proyecto.

<br>

---

## Esquema del Circuito

A continuación, se muestra la imagen del esquema de montaje del circuito: 

<br>

<p align="center">
		<img height=500 width=850 align="center" src="./images/esq_circ.png" />
</p>

<br>

---

## Disposición en Planta

El montaje anterior se dispondrá en planta encima de un soporte unido a la cinta en la siguiente posición:

<br>

<p align="center">
		<img height=500 width=1000 align="center" src="./images/disp_planta.jpeg" />
</p>

<br>

---

## Diagramas de comunicación MQTT

A continuación se describen los diagramas de comunicación MQTT de las interacciones indirectas presentes en la estación robotizada:

### Interacción botón emergencia - estación
  Esta interacción es la relativa a la parada de emergencia de todo el sistema robótico presente en la automatización. La ESP32-S3 publicará en el topic “A1/sensor/boton/emergencia/cinta/cajas” el mensaje “PARAR” en caso de que se pulse el botón. A este topic se encontrarán suscritos todos los dispositivos electrónicos de la célula (robot UR, delta, todas las ESP32-S3), deteniendo inmediatamente su ejecución en caso de recibir el mensaje “PARAR”.

<br>

  <p align="center">
		<img height=400 width=600 align="center" src="./images/boton-pe.png" />
  </p>
  
<br>

### Interacción sensor ultrasonidos - actuador cobot
  El sensor ultrasónico conectado a la ESP32-S3 se encarga de detectar la presencia o ausencia de una caja al final de la cinta transportadora de paletizado. En caso de encontrar una caja, publicará un JSON con el campo "presencia" conteniendo el mensaje “detect” en el topic “A1/sensor/presencia/cinta/cajas/final”, en caso contrario, publicará "libre" en su lugar. El cobot analizará el mensaje: en el caso de ser "detect", procederá a coger la caja y paletizarla y, en el caso de ser "libre", el cobot esperará en la posición home hasta ser demandado.

<br>

  <p align="center">
		<img height=400 width=600 align="center" src="./images/ultrasonidos-cobot.png" />
  </p>

<br>

### Interacción actuador cobot - actuador LED
Cuando el cobot inicia su operación de paletizado de una caja tras ser detectada por el sensor de ultrasonidos, publica “operando” en el topic “A1/actuador/led/cinta/cajas/final” y el LED azul se enciende. Mientras el cobot no se encuentre en movimiento se publicará en el topic “inactivo” y el LED permanecerá apagado. 

<br>

  <p align="center">
		<img height=400 width=600 align="center" src="./images/cobot-led.png" />
  </p>

<br>

---

## Funcionalidades Locales del Dispositivo

<br>

 El módulo incluye un semáforo, con 2 LEDs, uno verde y uno rojo, y el control de un servomotor para simular el motor de la cinta. Estas funcionalidades no se contemplan en las interacciones MQTT, ya que se comunican internamente mediante buffers y variables protegidas. El semáforo LED indica cuando el sensor ultrasonidos detecta caja y para el motor de la cinta, haciendo que el LED rojo se encienda y, por el contrario, si no hay presencia de caja y la cinta está activa, que se encienda el LED verde. 
<br> Suponemos que este motor es el que controla la cinta transportadora de cajas y está conectado a la ESP32-S3 que tiene el sensor ultrasónico.

<br>

---

## Instrucciones para la Instalación e Integración

  ### 1. Instalar y configurar el entorno [Arduino](#entorno) con las [librerías](#librerías) especificadas, además de instalar el programa para la comunicación [MQTT](#entorno).
  
 <br>
 
 Para descargar las librerías también se puede hacer desde el _library manager_ y _board manager_ de Arduino IDE

 <br>

 <p align="center">
		<img height=400 width=700 align="center" src="./images/library_manager.png" />
 </p>

 <br>

 <p align="center">
		<img height=400 width=700 align="center" src="./images/boards_manager.png" />
 </p>

 <br>

 Una vez descargadas las librerías necesarías para compilar el proyecto, seleccionar la placa en el cuadro de selección de placa

 <br>

 <p align="center">
		<img height=400 width=700 align="center" src="./images/selecc_placa_arduino.png" />
 </p>

 <br>
  
  ### 2. Instalar el código fuente descargando la carpeta zip de este repositorio o usando git clone.
  
  <br>
  
  #### Opción 1 
  Click en el boton _Download ZIP_ 

  <p align="center">
		<img height=400 width=700 align="center" src="./images/down_zip.png" />
  </p>

  <br>

  #### Opción 2 
  Clonar el repositorio desde el terminal

  <br>

  ```sh
  git clone https://github.com/Tamala24/ROBOTRONIC.git
  ```

  <br>
  
  ### 3. Modificar valores del código por los de la empresa.

  Para conectar la ESP32-S3 al wifi y al broker MQTT de la emmpresa, modificar las constantes NET_SSID, NET_PASSWD, MQTT_SERVER_IP, MQTT_USERNAME y MQTT_PASSWORD del archivo `Config.h` 
  
		/**
		 * @file  Config.h
		 * @brief Archivo de configuración del sistema
		 */
		#ifndef CONFIG_H
		#define CONFIG_H
	
  		...

      		// WIFI
		#define NET_SSID                  "UPV-PSK"
		#define NET_PASSWD                "giirob-pr2-2023"
		
		// MQTT
		#define MQTT_SERVER_IP            "mqtt.dsic.upv.es"
		#define MQTT_SERVER_PORT          1883
		#define MQTT_USERNAME             "giirob"    // Descomentar esta línea (y la siguiente) para que se conecte al broker MQTT usando usuario y contraseña
		#define MQTT_PASSWORD             "UPV2024"

  		...

    		#endif // CONFIG_H

<br> Si se desea modificar los topics por unos más acordes para su integración en la flota de la empresa modificar los valores de HELLO_TOPIC, TOPIC_PRESENCIA, TOPIC_COBOT y TOPIC_PARADA_EMERGENCIA del archivo `Config.h` 
  
		/**
		 * @file  Config.h
		 * @brief Archivo de configuración del sistema
		 */
		#ifndef CONFIG_H
		#define CONFIG_H
	
  		...

      		// TOPICS
		/**** HELLO_TOPIC ****/
		#define HELLO_TOPIC               "A1"
		/**** TOPIC_PRESENCIA : manda si detecta objeto "detect", si no, "libre" ****/
		#define TOPIC_PRESENCIA           "A1/sensor/presencia/cinta/cajas/final"
		/**** TOPIC_COBOT : recibe "operando" si el cobot está operando y "inactivo" si no ****/
		#define TOPIC_COBOT               "A1/actuador/led/cinta/cajas/final"
		/**** TOPIC_PARADA_EMERGENCIA : manda o recibe "PARAR" si se ha pulsado el botón de emergencia ****/
		#define TOPIC_PARADA_EMERGENCIA   "A1/sensor/boton/emergencia/cinta/cajas"

  		...

    		#endif // CONFIG_H
      		
  <br>
 
  ### 4. Montar el circuito como se indica en la [figura](#esquema-del-circuito).
  
  <br>
  
  ### 5. Conectar la placa al ordenador usando el puerto UART, compilar el proyecto y subir a la ESP32-S3.
  
 <br>

 Después de conectar la placa, seleccionar el puerto que aparezca en el seleccionador de puertos en Tools + Port  

 <br>

 <p align="center">
		<img height=400 width=700 align="center" src="./images/puerto.png" />
 </p>

 <br>

 Compilar primero el código y, si no aparece ningún error, ejecutar en la placa

 <br>

 <p align="center">
		<img height=400 width=700 align="center" src="./images/comp_ejec.png" />
 </p>

 <br>
  
  ### 6. Desconectar la placa y situarla en su posición en [planta](#disposición-en-planta) conectada a una fuente de alimentación externa.
  
 <br>

