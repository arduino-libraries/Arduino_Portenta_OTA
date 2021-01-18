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

#include "ArduinoOTAPortenta_SD.h"

#include "BSP.h"
#include "stm32h7xx_hal_sd.h"
#include "stm32h7xx_hal_rtc_ex.h"

#include <assert.h>

using namespace arduino;

/******************************************************************************
 * EXTERN
 ******************************************************************************/

extern RTC_HandleTypeDef RTCHandle;

/******************************************************************************
   CONSTANTS
 ******************************************************************************/

static char const SD_UPDATE_FILENAME[] = "UPDATE.BIN";

/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

ArduinoOTAPortenta_SD::ArduinoOTAPortenta_SD(StorageTypePortenta const storage_type, uint32_t const data_offset)
: ArduinoOTAPortenta(storage_type, data_offset)
, _bd{NULL}
, _block_device()
, _fs_sd{NULL}
, _dir_sd{NULL}
, _update_size_sd{0}
{
  assert(_storage_type == SD_OFFSET    ||
         _storage_type == SD_FATFS     ||
         _storage_type == SD_LITTLEFS  ||
         _storage_type == SD_FATFS_MBR ||
         _storage_type == SD_LITTLEFS_MBR);
}

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool ArduinoOTAPortenta_SD::init()
{
  if (_storage_type == SD_OFFSET)
  {
    int const err = _block_device.init();
    if (err)
    {
      Serial1.print("Error while _block_device.init() : ");
      Serial1.println(err);
      return false;
    }
    return true;
  }

   if(_storage_type == SD_FATFS)
   {
    _fs_sd = new mbed::FATFileSystem("fs");
    int const err =  _fs_sd->mount(&_block_device);
    if (err) {
      Serial1.print("Error while mounting the filesystem. Err = ");
      Serial1.println(err);
      return false;
    }
    return true;
  }

  if (_storage_type == SD_FATFS_MBR)
  {
    _bd = &_block_device;
    mbed::BlockDevice* physical__block_device = _bd;
    _bd = new mbed::MBRBlockDevice(physical__block_device, 1);
    _fs_sd = new mbed::FATFileSystem("fs");
    int const err =  _fs_sd->mount(_bd);
    if (err)
    {
      Serial1.print("Error while mounting the filesystem. Err = ");
      Serial1.println(err);
      return false;
    }
    return true;
  }

  return false;
}

bool ArduinoOTAPortenta_SD::open()
{
  if (_storage_type == SD_OFFSET)
    return true;

  if (_storage_type == SD_FATFS || _storage_type == SD_FATFS_MBR)
  {
    struct dirent * entry = NULL;
    if ((_dir_sd = opendir("/fs")) != NULL)
    {
      /* print all the files and directories within directory */
      while ((entry = readdir(_dir_sd)) != NULL)
      {
        if (String(entry->d_name) == "UPDATE.BIN")
        {
          struct stat stat_buf;
          stat("/fs/UPDATE.BIN", &stat_buf);
          _update_size_sd = stat_buf.st_size;
          return true;
        }
      }
    }
  }

  return false;
}

size_t ArduinoOTAPortenta_SD::write()
{
  if (_storage_type == SD_FATFS  ||
      _storage_type == SD_OFFSET ||
      _storage_type == SD_FATFS_MBR)
  {
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, 0x07AA);
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR1, _storage_type);

    if(_storage_type == SD_OFFSET || _storage_type == SD_FATFS_MBR)
    {
      HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR2, _data_offset);
      _update_size_sd = _program_length;
    }

    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR3, _update_size_sd);
    return _update_size_sd;
  }

  return 0;
}

void ArduinoOTAPortenta_SD::close()
{
  if(_storage_type==SD_FATFS || _storage_type==SD_FATFS_MBR) {
    closedir (_dir_sd);
  }
}
