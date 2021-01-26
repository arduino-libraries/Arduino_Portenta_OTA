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

#ifndef ARDUINO_PORTENTA_OTA_INTERNAL_FLASH_H_
#define ARDUINO_PORTENTA_OTA_INTERNAL_FLASH_H_

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "Arduino_Portenta_OTA.h"

#include <FlashIAPBlockDevice.h>

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class Arduino_Portenta_OTA_InternalFlash : public Arduino_Portenta_OTA
{
public:

           Arduino_Portenta_OTA_InternalFlash(StorageTypePortenta const storage_type, uint32_t const data_offset);
  virtual ~Arduino_Portenta_OTA_InternalFlash() { }


protected:

  virtual bool init  () override;
  virtual bool open  () override;
  virtual bool write () override;


private:

  FlashIAPBlockDevice _bd;
  mbed::FATFileSystem _fs_flash;
  mbed::LittleFileSystem _littlefs_fs_flash;
  int _update_size_internal_flash;

};

#endif /* ARDUINO_PORTENTA_OTA_INTERNAL_FLASH_H_ */
