/*
   This file is part of ArduinoIoTCloud.

   Copyright 2020 ARDUINO SA (http://www.arduino.cc/)

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

#ifndef ARDUINO_AIOTC_CONFIG_H_
#define ARDUINO_AIOTC_CONFIG_H_

/******************************************************************************
 * AUTOMATICALLY CONFIGURED DEFINES
 ******************************************************************************/

#if defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4)
  #define OTA_STORAGE_PORTENTA         (1)
#else
  #define OTA_STORAGE_PORTENTA         (0)
#endif

#if OTA_STORAGE_PORTENTA
  #define OTA_ENABLED             (1)
#else
  #define OTA_ENABLED             (0)
#endif

#if defined(ARDUINO_PORTENTA_H7_M7)      ||     defined(ARDUINO_PORTENTA_H7_M4)
  #define BOARD_HAS_ECCX08
  #define HAS_TCP
#endif

#endif /* ARDUINO_AIOTC_CONFIG_H_ */
