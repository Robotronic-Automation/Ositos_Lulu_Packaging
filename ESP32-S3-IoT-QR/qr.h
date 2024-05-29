/**
 * @file  qr.h
 * @brief Definiciones y declaraciones para la configuración de la cámara y lectura de códigos QR
 */

#ifndef QR_H
#define QR_H

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

/**
 * @struct QRCodeData
 * @brief  Estructura para datos de códigos QR
 */
struct QRCodeData
{
  bool valid;
  int dataType;
  uint8_t payload[1024];
  int payloadLen;
};

// Puntero al objeto quirc para el manejo del QR
struct quirc * q = NULL;
uint8_t * image = NULL;         // Puntero a la imagen capturada
camera_fb_t * fb = NULL;       // Puntero al frame buffer de la cámara
struct quirc_code code;        // Estructura para el código QR
struct quirc_data data;        // Estructura para los datos del QR
quirc_decode_error_t err;      // Variable para errores de decodificación
struct QRCodeData qrCodeData;  // Estructura para almacenar datos del QR

// Prototipos de funciones

/**
 * @brief Configura e inicializa la cámara
 */
void config_camara();

/**
 * @brief Muestra los resultados de lectura del código QR en el monitor serial 
 *        y guarda el contenido del QR en el buffer 
 * @param buff. Buffer para guardar el payload del código QR
 * @param data. Estructura con la información del código QR
 */
void dumpData_bis(Buffer_Circ_String_t * buff, const struct quirc_data * data);

#endif // QR_H

/*** End of file ****/