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

/******************************************************************************
   INCLUDE
 ******************************************************************************/

#include "Arduino_Portenta_OTA_InternalFlash.h"

#include <stm32h7xx_hal_rtc_ex.h>

#include "FlashIAPBlockDevice.h"

#include <assert.h>

using namespace arduino;

/******************************************************************************
 * EXTERN
 ******************************************************************************/

extern RTC_HandleTypeDef RTCHandle;

/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

Arduino_Portenta_OTA_InternalFlash::Arduino_Portenta_OTA_InternalFlash(StorageTypePortenta const storage_type, uint32_t const _data_offset)
: Arduino_Portenta_OTA(storage_type, _data_offset)
, _fs_flash("fs")
, _littlefs_fs_flash("little_fs")
, _update_size_internal_flash{0}
{
  assert(_storage_type == INTERNAL_FLASH_OFFSET ||
         _storage_type == INTERNAL_FLASH_FATFS  ||
         _storage_type == INTERNAL_FLASH_LITTLEFS);
}

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool Arduino_Portenta_OTA_InternalFlash::init()
{
  FlashIAPBlockDevice bd(0x8000000 + _data_offset, 2 * 1024 * 1024 - _data_offset);

  if (_storage_type == INTERNAL_FLASH_FATFS)
  {
    int const err = _fs_flash.mount(&bd);
    if (err)
    {
      Serial1.print("Error while mounting flash filesystem: ");
      Serial1.println(err);
      return false;
    }
    return true;
  }

  if (_storage_type == INTERNAL_FLASH_OFFSET)
    return true;

  if (_storage_type == INTERNAL_FLASH_LITTLEFS)
  {
    int const err = _littlefs_fs_flash.mount(&bd);
    if (err)
    {
      Serial1.print("Error while mounting littlefs filesystem: ");
      Serial1.println(err);
      return false;
    }
    return true;
  }

  return false;
}

bool Arduino_Portenta_OTA_InternalFlash::open()
{
  DIR * dir = NULL;
  struct dirent *entry = NULL;

  if (_storage_type == INTERNAL_FLASH_FATFS)
  {
    if ((dir = opendir("/fs")) != NULL)
    {
      /* print all the files and directories within directory */
      while ((entry = readdir(dir)) != NULL)
      {
        if (String(entry->d_name) == "UPDATE.BIN")
        {
          struct stat stat_buf;
          stat("/fs/UPDATE.BIN", &stat_buf);
          _update_size_internal_flash = stat_buf.st_size;
          closedir(dir);
          return true;
        }
      }
      closedir(dir);
    }
    return false;
  }

  if (_storage_type == INTERNAL_FLASH_OFFSET)
  {
    _update_size_internal_flash = 2 * 1024 * 1024;
    return true;
  }

  if (_storage_type == INTERNAL_FLASH_LITTLEFS)
  {
    if ((dir = opendir("/little_fs")) != NULL)
    {
      /* print all the files and directories within directory */
      while ((entry = readdir(dir)) != NULL)
      {
        if (String(entry->d_name) == "UPDATE.BIN")
        {
          struct stat stat_buf;
          stat("/little_fs/UPDATE.BIN", &stat_buf);
          _update_size_internal_flash = stat_buf.st_size;
          closedir(dir);
          return true;
        }
      }
      closedir(dir);
    }
    return false;
  }

  _update_size_internal_flash = 0;
  return false;
}

bool Arduino_Portenta_OTA_InternalFlash::write()
{
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, 0x07AA);
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR1, _storage_type);
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR2, _data_offset);
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR3, _update_size_internal_flash);
  return true;
}
