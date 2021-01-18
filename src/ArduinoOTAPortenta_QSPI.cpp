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

#include "ArduinoOTAPortenta_QSPI.h"

#include <stm32h7xx_hal_rtc_ex.h>

#include <assert.h>

using namespace arduino;

/******************************************************************************
 * EXTERN
 ******************************************************************************/

extern RTC_HandleTypeDef RTCHandle;

/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

ArduinoOTAPortenta_QSPI::ArduinoOTAPortenta_QSPI(StorageTypePortenta const storage_type, uint32_t const data_offset)
: ArduinoOTAPortenta(storage_type, data_offset)
, _bd_qspi{NULL}
, _fs_qspi{NULL}
, _block_device_qspi(PD_11, PD_12, PF_7, PD_13,  PF_10, PG_6, QSPIF_POLARITY_MODE_1, 40000000)
, _update_size_qspi{0}
{
  assert(_storage_type == QSPI_FLASH_OFFSET    ||
         _storage_type == QSPI_FLASH_FATFS     ||
         _storage_type == QSPI_FLASH_LITTLEFS  ||
         _storage_type == QSPI_FLASH_FATFS_MBR ||
         _storage_type == QSPI_FLASH_LITTLEFS_MBR);
}

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool ArduinoOTAPortenta_QSPI::init()
{
  if(_storage_type == QSPI_FLASH_OFFSET)
    return (_block_device_qspi.init() == QSPIF_BD_ERROR_OK);

  if(_storage_type == QSPI_FLASH_FATFS)
  {
    _fs_qspi = new mbed::FATFileSystem("fs");
    int const err_mount = _fs_qspi->mount(&_block_device_qspi);
    if (err_mount)
    {
      Serial1.print("Error while mounting the filesystem. Err = ");
      Serial1.println(err_mount);
      return false;
    }
    return true;
  }

  if (_storage_type == QSPI_FLASH_FATFS_MBR)
  {
    _bd_qspi = &_block_device_qspi;
    mbed::BlockDevice* physical_block_device = _bd_qspi;
    _bd_qspi = new mbed::MBRBlockDevice(physical_block_device, _data_offset);
    _fs_qspi = new mbed::FATFileSystem("fs");
    int const err_mount = _fs_qspi->mount(_bd_qspi);
    if (err_mount) {
      Serial1.print("Error while mounting the filesystem. Err = ");
      Serial1.println(err_mount);
      return false;
    }
    return true;
  }

  return false;
}

bool ArduinoOTAPortenta_QSPI::open()
{
  if (_storage_type == QSPI_FLASH_OFFSET)
  {
    _update_size_qspi = _program_length;
    return true;
  }

  if(_storage_type == QSPI_FLASH_FATFS || _storage_type == QSPI_FLASH_FATFS_MBR)
  {
    DIR * dir = NULL;
    if ((dir = opendir("/fs")) != NULL)
    {
      /* print all the files and directories within directory */
      struct dirent * entry = NULL;
      while ((entry = readdir(dir)) != NULL)
      {
        if (String(entry->d_name) == "UPDATE.BIN")
        {
          struct stat stat_buf;
          stat("/fs/UPDATE.BIN", &stat_buf);
          _update_size_qspi = stat_buf.st_size;
          closedir(dir);
          return true;
        }
      }
      closedir(dir);
    }
  }

  _update_size_qspi = 0;
  return false;
}

size_t ArduinoOTAPortenta_QSPI::write()
{
  if(_storage_type == QSPI_FLASH_OFFSET ||
     _storage_type == QSPI_FLASH_FATFS  ||
     _storage_type == QSPI_FLASH_FATFS_MBR)
  {
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, 0x07AA);
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR1, _storage_type);
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR2, _data_offset);
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR3, _update_size_qspi);
    return _update_size_qspi;
  }
  return 0;
}

void ArduinoOTAPortenta_QSPI::close()
{

}
