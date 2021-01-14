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

#include <ArduinoOTAPortenta.h>

#include "OTAStorage_Internal_Flash.h"
#include "OTAStorage_Portenta_Qspi_Flash.h"
#include "OTAStorage_Portenta_SD.h"

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/

static OTAStorage_Internal_Flash ota_storage_internal_flash;
static OTAStorage_Portenta_Qspi_Flash ota_storage_qspi_flash;
static OTAStorage_Portenta_SD ota_storage_sd;

/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

ArduinoOTAPortenta::ArduinoOTAPortenta()
{

}

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

void ArduinoOTAPortenta::setUpdateLen(uint32_t length)
{
  program_len = length;
}

bool ArduinoOTAPortenta::begin(StorageTypePortenta storage, uint32_t offset)
{
  Serial1.begin(115200);
  if (storage==INTERNAL_FLASH_OFFSET || storage==INTERNAL_FLASH_FATFS || storage==INTERNAL_FLASH_LITTLEFS) {
    setOTAStorage(ota_storage_internal_flash, storage, offset, program_len);
    return true;
  } else if (storage==QSPI_FLASH_OFFSET || storage==QSPI_FLASH_FATFS || storage==QSPI_FLASH_LITTLEFS || storage==QSPI_FLASH_FATFS_MBR || storage==QSPI_FLASH_LITTLEFS_MBR) {
    setOTAStorage(ota_storage_qspi_flash, storage, offset, program_len);
    return true;
  } else if (storage==SD_OFFSET || storage==SD_FATFS || storage==SD_LITTLEFS || storage==SD_FATFS_MBR || storage==SD_LITTLEFS_MBR) {
    setOTAStorage(ota_storage_sd, storage, offset, program_len);
    return true;
  } else {
    return false;
  }
}

PortentaOTAError ArduinoOTAPortenta::update()
{
  /* If a _ota_logic object has been instantiated then we are spinning its
    * 'update' method here in order to process incoming data and generally
    * to transition to the OTA logic update states.
    */
  return _ota_logic_portenta.update();
}

void ArduinoOTAPortenta::setOTAStorage(OTAStoragePortenta & ota_storage, StorageTypePortenta storageType, uint32_t offset, uint32_t length)
{
  _ota_logic_portenta.setOTAStorage(ota_storage, storageType, offset, length);
}

/******************************************************************************
 * EXTERN DEFINITION
 ******************************************************************************/

ArduinoOTAPortenta OTAPortenta;
