/*
   This file is part of Arduino_Portenta_OTA.

   Copyright 2022 ARDUINO SA (http://www.arduino.cc/)

   This software is released under the GNU General Public License version 3,
   which covers the main part of arduino-cli.
   The terms of this license can be found at:
   https://www.gnu.org/licenses/gpl-3.0.en.html

   You can be released from the requirements of the above licenses by purchasing
   a commercial license. Buying such a license is mandatory if you want to modify or
   otherwise use the software for commercial activities involving the Arduino
   software without disclosing the source code of your own applications. To purchase
   a commercial license, send an email to license@arduino.cc.
*/

#ifndef ARDUINO_PORTENTA_OTA_CONFIG_H_
#define ARDUINO_PORTENTA_OTA_CONFIG_H_

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <Arduino.h>

#if defined(ARDUINO_PORTENTA_H7_M7)
  #define ARDUINO_PORTENTA_OTA_MAGIC 0x2341025b
  #define ARDUINO_PORTENTA_OTA_SDMMC_SUPPORT
  #define ARDUINO_PORTENTA_OTA_QSPI_SUPPORT
#endif

#if defined(ARDUINO_NICLA_VISION)
  #define ARDUINO_PORTENTA_OTA_MAGIC 0x2341025f
  #define ARDUINO_PORTENTA_OTA_QSPI_SUPPORT
#endif

#if defined(ARDUINO_OPTA)
  #define ARDUINO_PORTENTA_OTA_MAGIC 0x23410064
  #define ARDUINO_PORTENTA_OTA_QSPI_SUPPORT
#endif

#if defined(ARDUINO_GIGA)
  #define ARDUINO_PORTENTA_OTA_MAGIC 0x23410266
  #define ARDUINO_PORTENTA_OTA_QSPI_SUPPORT
#endif

#endif /* ARDUINO_PORTENTA_OTA_CONFIG_H_ */
