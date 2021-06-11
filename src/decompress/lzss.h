#ifndef SSU_LZSS_H_
#define SSU_LZSS_H_

/**************************************************************************************
   INCLUDE
 **************************************************************************************/

#include <Arduino.h>
#include <stdint.h>
#include "Arduino_Portenta_OTA.h"

/**************************************************************************************
   FUNCTION DEFINITION
 **************************************************************************************/

void lzss_init(FILE * update_file_ptr, FILE * target_file_ptr, uint32_t const lzss_file_size, ArduinoPortentaOtaWatchdogResetFuncPointer wdog_feed_func_ptr);
void lzss_decode();
void lzss_flush();

#endif /* SSU_LZSS_H_ */
