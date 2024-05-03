/**
 @file qr.h
*/

#ifndef QR_H
#define QR_H

#include <Arduino.h>
#include "src/quirc/quirc.h"
#include "esp_camera.h"

/* GPIO of CAMERA_MODEL_ESP32S3_EYE */
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      15
  #define SIOD_GPIO_NUM     4
  #define SIOC_GPIO_NUM     5
  
  #define Y9_GPIO_NUM       16
  #define Y8_GPIO_NUM       17
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       12
  #define Y5_GPIO_NUM       10
  #define Y4_GPIO_NUM       8
  #define Y3_GPIO_NUM       9
  #define Y2_GPIO_NUM       11
  #define VSYNC_GPIO_NUM    6
  #define HREF_GPIO_NUM     7
  #define PCLK_GPIO_NUM     13
 
/*  Declaracion de Variables */

struct QRCodeData
{
  bool valid;
  int dataType;
  uint8_t payload[1024];
  int payloadLen;
};

struct quirc *q = NULL;
uint8_t *image = NULL;  
camera_fb_t * fb = NULL;
struct quirc_code code;
struct quirc_data data;
quirc_decode_error_t err;
struct QRCodeData qrCodeData;  
String QRCodeResult = "";

// Funcion para configurar la camara 
void config_camara();
// Funcion para mostrar los resultados de lectura del codigo QR en el serial monitor
void dumpData_bis(Buffer_Circ * buff_prod, const struct quirc_data * data);

#endif

/*** End of file ****/