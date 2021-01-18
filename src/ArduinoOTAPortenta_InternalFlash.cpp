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

/******************************************************************************
   INCLUDE
 ******************************************************************************/

#include "ArduinoOTAPortenta_InternalFlash.h"

#include <stm32h7xx_hal_rtc_ex.h>

#include <assert.h>

using namespace arduino;

/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

ArduinoOTAPortenta_InternalFlash::ArduinoOTAPortenta_InternalFlash(StorageTypePortenta const storage_type, uint32_t const _data_offset)
: ArduinoOTAPortenta(storage_type, _data_offset)
, _fs_flash("fs")
, _littlefs_fs_flash("little_fs")
, _dir_flash{NULL}
, _update_size_internal_flash{0}
{
  assert(_storage_type == INTERNAL_FLASH_OFFSET ||
         _storage_type == INTERNAL_FLASH_FATFS  ||
         _storage_type == INTERNAL_FLASH_LITTLEFS);
}

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool ArduinoOTAPortenta_InternalFlash::init()
{
  FlashIAPBlockDevice bd(0x8000000 + _data_offset, 2 * 1024 * 1024 - _data_offset);

  int err;

  if(_storage_type==INTERNAL_FLASH_FATFS) {
    err =  _fs_flash.mount(&bd);
    if (err)
      return false;
    else
      return true;
  } else if (_storage_type==INTERNAL_FLASH_OFFSET) {
    return true;
  } else if (_storage_type==INTERNAL_FLASH_LITTLEFS) {
    err =  _littlefs_fs_flash.mount(&bd);
    if (err)
      return false;
    else
      return true;
  } else {
    Serial1.println("ERROR: storageType not available");
    return false;
  }
}

bool ArduinoOTAPortenta_InternalFlash::open()
{
  struct dirent *ent_Flash = NULL;
  if(_storage_type==INTERNAL_FLASH_FATFS) {
    if ((_dir_flash = opendir("/fs")) != NULL) {
      /* print all the files and directories within directory */
      while ((ent_Flash = readdir(_dir_flash)) != NULL) {
        if (String(ent_Flash->d_name) == "UPDATE.BIN") {
            struct stat stat_buf;
            stat("/fs/UPDATE.BIN", &stat_buf);
            _update_size_internal_flash = stat_buf.st_size;
            return true;
        }
      }
    }
  } else if (_storage_type==INTERNAL_FLASH_OFFSET) {
    _update_size_internal_flash = 2 * 1024 * 1024;
    return true;
  } else if (_storage_type==INTERNAL_FLASH_LITTLEFS) {
    if ((_dir_flash = opendir("/little_fs")) != NULL) {
      /* print all the files and directories within directory */
      while ((ent_Flash = readdir(_dir_flash)) != NULL) {
        if (String(ent_Flash->d_name) == "UPDATE.BIN") {
            struct stat stat_buf;
            stat("/little_fs/UPDATE.BIN", &stat_buf);
            _update_size_internal_flash = stat_buf.st_size;
            return true;
        }
      }
    }
  } else {
    Serial1.println("ERROR: storageType not available");
    _update_size_internal_flash = 0;
    return false;
  }
}

size_t ArduinoOTAPortenta_InternalFlash::write()
{
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, 0x07AA);
  Serial1.println("ArduinoOTAPortenta_InternalFlash::write    1");
  Serial1.print("ArduinoOTAPortenta_InternalFlash::write    _storage_type = ");
  Serial1.println(_storage_type);
  delay(200);
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR1, _storage_type);
  Serial1.println("ArduinoOTAPortenta_InternalFlash::write    2");
  Serial1.print("ArduinoOTAPortenta_InternalFlash::write    update_size = ");
  Serial1.println(_update_size_internal_flash);
  delay(200);

  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR2, _data_offset);

  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR3, _update_size_internal_flash);
  Serial1.println("ArduinoOTAPortenta_InternalFlash::write    3");
  delay(200);

  return _update_size_internal_flash;
}

void ArduinoOTAPortenta_InternalFlash::close()
{
  if(_storage_type==INTERNAL_FLASH_FATFS || _storage_type==INTERNAL_FLASH_LITTLEFS) {
    closedir (_dir_flash);
  }
}
