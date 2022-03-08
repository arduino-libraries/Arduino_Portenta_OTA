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

#include "Arduino_Portenta_OTA_Config.h"
#if defined(ARDUINO_PORTENTA_OTA_QSPI_SUPPORT)

#include "Arduino_Portenta_OTA_QSPI.h"

#include <assert.h>

using namespace arduino;

/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

Arduino_Portenta_OTA_QSPI::Arduino_Portenta_OTA_QSPI(StorageTypePortenta const storage_type, uint32_t const data_offset)
: Arduino_Portenta_OTA(storage_type, data_offset)
, _bd_qspi{NULL}
, _fs_qspi{NULL}
{
  assert(_storage_type == QSPI_FLASH_FATFS || _storage_type == QSPI_FLASH_FATFS_MBR);
}

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool Arduino_Portenta_OTA_QSPI::init()
{
#if defined (COMPONENT_4343W_FS)
  _bd_raw_qspi = mbed::BlockDevice::get_default_instance();
#else
  _bd_raw_qspi = new QSPIFBlockDevice(PD_11, PD_12, PF_7, PD_13,  PF_10, PG_6, QSPIF_POLARITY_MODE_1, 40000000);
#endif
  if (_bd_raw_qspi->init() != QSPIF_BD_ERROR_OK) {
    Serial1.println("Error: QSPI init failure.");
    return false;
  }

  if(_storage_type == QSPI_FLASH_FATFS)
  {
    _fs_qspi = new mbed::FATFileSystem("fs");
    int const err_mount = _fs_qspi->mount(_bd_raw_qspi);
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
    _bd_qspi = new mbed::MBRBlockDevice(_bd_raw_qspi, _data_offset);
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

bool Arduino_Portenta_OTA_QSPI::open()
{
  DIR * dir = NULL;
  if ((dir = opendir("/fs")) != NULL)
  {
    if (Arduino_Portenta_OTA::findProgramLength(dir, _program_length))
    {
      closedir(dir);
      return true;
    }
    closedir(dir);
  }

  return false;
}

#endif /* ARDUINO_PORTENTA_OTA_QSPI_SUPPORT */
