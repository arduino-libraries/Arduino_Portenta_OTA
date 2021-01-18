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
  int err;

  if(_storage_type==SD_OFFSET) {
    err = _block_device.init();
    if (err)
      return false;
    else
      return true;
  } else if(_storage_type==SD_FATFS) {
    _fs_sd = new mbed::FATFileSystem("fs");
    err =  _fs_sd->mount(&_block_device);
    if (err) {
      Serial1.print("Error while mounting the filesystem. Err = ");
      Serial1.println(err);
      return false;
    } else {
      Serial1.println("Filesystem correcly mounted");
      return true;
    }
  } else if (_storage_type==SD_FATFS_MBR) {
    _bd = &_block_device;
    mbed::BlockDevice* physical__block_device = _bd;
    _bd = new mbed::MBRBlockDevice(physical__block_device, 1);
    _fs_sd = new mbed::FATFileSystem("fs");
    err =  _fs_sd->mount(_bd);
    if (err) {
      Serial1.print("Error while mounting the filesystem. Err = ");
      Serial1.println(err);
      return false;
    } else {
      Serial1.println("Filesystem correcly mounted");
      return true;
    }
  } else {
    Serial1.println("storageType not implemented yet");
    return false;
  }
}

bool ArduinoOTAPortenta_SD::open()
{
  if(_storage_type==SD_OFFSET) {
    return true;
  } else if(_storage_type==SD_FATFS || _storage_type==SD_FATFS_MBR) {
    struct dirent * ent_SD = NULL;
    if ((_dir_sd = opendir("/fs")) != NULL) {
      /* print all the files and directories within directory */
      while ((ent_SD = readdir(_dir_sd)) != NULL) {
        if (String(ent_SD->d_name) == "UPDATE.BIN") {
            struct stat stat_buf;
            stat("/fs/UPDATE.BIN", &stat_buf);
            _update_size_sd = stat_buf.st_size;
            return true;
        }
      }
    }
  } else {
    Serial1.println("storageType not implemented yet");
    return false;
  }
}

size_t ArduinoOTAPortenta_SD::write()
{
  Serial1.println("ArduinoOTAPortenta_SD::write");
  delay(200);

  if(_storage_type==SD_FATFS || _storage_type==SD_OFFSET || _storage_type==SD_FATFS_MBR) {
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, 0x07AA);
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR1, _storage_type);
    Serial1.println("ArduinoOTAPortenta_SD::write    1");
    Serial1.print("ArduinoOTAPortenta_SD::write    _storage_type = ");
    Serial1.println(_storage_type);
    delay(200);

    if(_storage_type==SD_OFFSET || _storage_type==SD_FATFS_MBR) {
      HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR2, _data_offset);
      _update_size_sd = _program_length;
    }

    Serial1.println("ArduinoOTAPortenta_SD::write    2");
    Serial1.print("ArduinoOTAPortenta_SD::write    update_size = ");
    Serial1.println(_update_size_sd);
    Serial1.print("ArduinoOTAPortenta_SD::write    _data_offset = ");
    Serial1.println(_data_offset);
    delay(200);
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR3, _update_size_sd);
    Serial1.println("ArduinoOTAPortenta_SD::write    3");
    delay(200);
    return _update_size_sd;
  } else {
    Serial1.println("storageType not implemented yet");
    return 0;
  }
  
}

void ArduinoOTAPortenta_SD::close()
{
  if(_storage_type==SD_FATFS || _storage_type==SD_FATFS_MBR) {
    closedir (_dir_sd);
  }
}
