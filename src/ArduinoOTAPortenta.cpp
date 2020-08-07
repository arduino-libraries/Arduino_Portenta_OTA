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

#include <AIoTC_Config.h>

#if OTA_STORAGE_PORTENTA
#include <ArduinoOTAPortenta.h>
#include "OTAStorage_Internal_Flash.h"
#include "OTAStorage_Portenta_Qspi_Flash.h"
#include "OTAStorage_Portenta_SD.h"


/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/

  static OTAStorage_Internal_Flash ota_storage_internal_flash;
  //static OTAStorage_Internal_Flash_offset ota_storage_internal_flash_offset;
  static OTAStorage_Portenta_Qspi_Flash ota_storage_qspi_flash;
  static OTAStorage_Portenta_SD ota_storage_sd;

/******************************************************************************
   GLOBAL CONSTANTS
 ******************************************************************************/

static const int TIMEOUT_FOR_LASTVALUES_SYNC = 10000;

/******************************************************************************
   LOCAL MODULE FUNCTIONS
 ******************************************************************************/


/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

ArduinoOTAPortenta::ArduinoOTAPortenta() :
  _ota_error{static_cast<int>(OTAError::None)}
{

}

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

void ArduinoOTAPortenta::begin(storageTypePortenta storage)
{

  Serial1.begin(115200);
  Serial1.print("ArduinoOTAPortenta begin. Storage = ");
  Serial1.println(storage);

  if (storage==INTERNAL_FLASH_OFFSET || storage==INTERNAL_FLASH_FATFS || storage==INTERNAL_FLASH_LITTLEFS) {
    Serial1.println("Internal falsh storage");
    setOTAStorage(ota_storage_internal_flash, storage);
  } else if (storage==QSPI_FLASH_OFFSET || storage==QSPI_FLASH_FATFS || storage==QSPI_FLASH_LITTLEFS || storage==QSPI_FLASH_FATFS_MBR || storage==QSPI_FLASH_LITTLEFS_MBR) {
    Serial1.println("QSPI falsh storage");
    setOTAStorage(ota_storage_qspi_flash, storage);
  } else if (storage==SD_OFFSET || storage==SD_FATFS || storage==SD_LITTLEFS || storage==SD_FATFS_MBR || storage==SD_LITTLEFS_MBR) {
    Serial1.println("SD storage");
    setOTAStorage(ota_storage_sd, storage);
  } else {
    Serial1.println("Invalid storage type");
    delay(200);
  }
}

void ArduinoOTAPortenta::update()
{
  /* If a _ota_logic object has been instantiated then we are spinning its
    * 'update' method here in order to process incoming data and generally
    * to transition to the OTA logic update states.
    */
  OTAError const err = _ota_logic_portenta.update();
  _ota_error = static_cast<int>(err);
}

int ArduinoOTAPortenta::connected()
{
  return 1;
}

void ArduinoOTAPortenta::setOTAStorage(OTAStoragePortenta & ota_storage, storageTypePortenta storageType)
{
  Serial1.println("ArduinoOTAPortenta::setOTAStorage     storageType = ");
  Serial1.println(storageType);
  _ota_logic_portenta.setOTAStorage(ota_storage, storageType);
}

/******************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 ******************************************************************************/

/******************************************************************************
 * EXTERN DEFINITION
 ******************************************************************************/

ArduinoOTAPortenta OTAPortenta;

#endif  /*OTA_STORAGE_PORTENTA*/
