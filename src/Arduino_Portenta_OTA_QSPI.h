/*
   This file is part of Arduino_Portenta_OTA.

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

#ifndef ARDUINO_PORTENTA_OTA_QSPI_H_
#define ARDUINO_PORTENTA_OTA_QSPI_H_

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "Arduino_Portenta_OTA_Config.h"
#if defined(ARDUINO_PORTENTA_OTA_QSPI_SUPPORT)

#include "Arduino_Portenta_OTA.h"

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class Arduino_Portenta_OTA_QSPI : public Arduino_Portenta_OTA
{

public:

           Arduino_Portenta_OTA_QSPI(StorageTypePortenta const storage_type, uint32_t const data_offset);
  virtual ~Arduino_Portenta_OTA_QSPI() { }


protected:

  virtual bool init  () override;
  virtual bool open  () override;


private:

  mbed::BlockDevice * _bd_qspi;
  mbed::FATFileSystem * _fs_qspi;
};

#endif /* ARDUINO_PORTENTA_OTA_QSPI_SUPPORT */
#endif /* ARDUINO_PORTENTA_OTA_QSPI_H_ */
