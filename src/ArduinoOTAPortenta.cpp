/*
   This file is part of ArduinoIoTCloud.

   Copyright 2019 ARDUINO SA (http://www.arduino.cc/)

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
 * INCLUDE
 ******************************************************************************/

#include "ArduinoOTAPortenta.h"

/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

ArduinoOTAPortenta::ArduinoOTAPortenta()
: _storage_type{NONE}
, _program_length{0}
, _data_offset{0}
{

}

ArduinoOTAPortenta::~ArduinoOTAPortenta()
{

}

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

void ArduinoOTAPortenta::setUpdateLen(uint32_t const program_length)
{
  _program_length = program_length;
}

bool ArduinoOTAPortenta::begin(StorageTypePortenta const storage_type, uint32_t const data_offset)
{
  Serial1.begin(115200);
  if (storage_type == INTERNAL_FLASH_OFFSET ||
      storage_type == INTERNAL_FLASH_FATFS  ||
      storage_type == INTERNAL_FLASH_LITTLEFS)
  {
    setOTAStorage(storage_type, data_offset, _program_length);
    return true;
  }
  else if (storage_type == QSPI_FLASH_OFFSET    || 
           storage_type == QSPI_FLASH_FATFS     ||
           storage_type == QSPI_FLASH_LITTLEFS  ||
           storage_type == QSPI_FLASH_FATFS_MBR ||
           storage_type == QSPI_FLASH_LITTLEFS_MBR)
  {
    setOTAStorage(storage_type, data_offset, _program_length);
    return true;
  }
  else if (storage_type == SD_OFFSET    ||
           storage_type == SD_FATFS     ||
           storage_type == SD_LITTLEFS  ||
           storage_type == SD_FATFS_MBR ||
           storage_type == SD_LITTLEFS_MBR)
  {
    setOTAStorage(storage_type, data_offset, _program_length);
    return true;
  }

  return false;
}

ArduinoOTAPortenta::Error ArduinoOTAPortenta::update()
{
  if(!open())
    return Error::OtaStorageOpen;

  write();
  close();
  NVIC_SystemReset();
  return Error::None;
}

ArduinoOTAPortenta::Error ArduinoOTAPortenta::setOTAStorage(StorageTypePortenta const storage_type, uint32_t const data_offset, uint32_t const program_length)
{
  _storage_type = storage_type;
  _data_offset = data_offset;
  _program_length = program_length;

  if (!init())
    return Error::OtaStorageInit;
  else
    return Error::None;
}
