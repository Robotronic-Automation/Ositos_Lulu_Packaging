# ESP32-S3-IoT-QR

---

## Contents
  - [Descripción](#descripción)
  - [Librerías](#librerías)
  - [Entorno](#entorno)
  - [Documentación](#documentación)
  - [Esquema del Circuito](#esquema-circuito)
  - [Disposición en Planta](#disposición-planta)
  - [Instrucciones para la Instalación e Integración](#instrucciones-instalación-integración)

---

## Descripción

Sistema basado en una ESP32-S3 que lee los códigos de producto que contienen los QR de cada caja en la cinta mediante una cámara empleada como lector de QR. Se señaliza la detección de QR mediante 2 LEDs, uno rojo que indica que no se ha podido leer QR y otro verde cuando la lectura se ha realizado de forma satisfactoria. Además, los códigos de las cajas se envían por MQTT para su posterior clasificación en la base de datos de la empresa. La comunicación inalámbrica se realiza mediante el protocolo MQTT y la comunicación local entre tareas haciendo uso de buffers circulares protegidos. Como medida de seguridad se implementa una interrupción activada por la pulsación de un botón que para todo el sistema.

---

## Entorno

El código se ha desarrollado en el entorno de desarrollo integrado de Arduino Arduino IDE. Para compilar y ejecutar el código descargar [aquí](https://www.arduino.cc/en/software).
Para establecer la comunicación MQTT se recomienda el programa [MQTTX](https://mqttx.app/downloads), aunque se puede utilizar cualquier otro.

---

## Librerías

Una vez descargado, descargar las librerías:
* `ArduinoJson` by Benoit Blanchon (Library Manager): [descarga](https://arduinojson.org/?utm_source=meta&utm_medium=library.properties)
* `PubSubClient` by Nick O'Leary (Library Manager): [descarga](https://github.com/knolleary/pubsubclient/releases/tag/v2.8)
* `esp32` by Espressif Systems (Boards Manager): [descarga](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)

---

## Documentación

Puedes usar la [ESP32-S3-IoT-QR Online Documentation](https://github.com/Tamala24/ROBOTRONIC/) para obtener toda la información sobre el código de este proyecto.

---

## Esquema del Circuito


---

## Disposición en Planta


---

## Instrucciones para la Instalación e Integración

### 1. Instalar y configurar el entorno Arduino con las librerías especificadas, además de instalar el programa para la comunicación MQTT.

### 2. Instalar el código fuente descargando la carpeta zip de este repositorio o usando git clone.

### 3. Montar el circuito como se indica en la [figura](esquema-circuito).

### 4. Conectar la placa al ordenador usando el puerto USB y compilar el proyecto y subir a la ESP32-S3.




