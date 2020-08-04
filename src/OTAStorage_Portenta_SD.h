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

#ifndef ARDUINO_OTA_STORAGE_PORTENTA_SD_H_
#define ARDUINO_OTA_STORAGE_PORTENTA_SD_H_

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <AIoTC_Config.h>
#if OTA_STORAGE_PORTENTA
//#ifdef OTA_STORAGE_PORTENTA_SD

#include "OTAStorage.h"

#include "SDMMCBlockDevice.h"

#include "FATFileSystem.h"

#define INTERNAL_FLASH_FLAG     (1 << 1)
#define QSPI_FLASH_FLAG         (1 << 2)
#define SDCARD_FLAG             (1 << 3)
#define RAW_FLAG                (1 << 4)
#define FATFS_FLAG              (1 << 5)
#define LITTLEFS_FLAG           (1 << 6)
#define MBR_FLAG                (1 << 7)

extern RTC_HandleTypeDef RTCHandle;

enum /*class*/ storageTypePortenta {
    INTERNAL_FLASH_OFFSET = INTERNAL_FLASH_FLAG | RAW_FLAG,
    INTERNAL_FLASH_FATFS = INTERNAL_FLASH_FLAG | FATFS_FLAG,
    INTERNAL_FLASH_LITTLEFS = INTERNAL_FLASH_FLAG | LITTLEFS_FLAG,
    QSPI_FLASH_OFFSET = QSPI_FLASH_FLAG | RAW_FLAG,
    QSPI_FLASH_FATFS = QSPI_FLASH_FLAG | FATFS_FLAG,
    QSPI_FLASH_LITTLEFS = QSPI_FLASH_FLAG | LITTLEFS_FLAG,
    QSPI_FLASH_FATFS_MBR = QSPI_FLASH_FLAG | FATFS_FLAG | MBR_FLAG,
    QSPI_FLASH_LITTLEFS_MBR = QSPI_FLASH_FLAG | LITTLEFS_FLAG | MBR_FLAG,
    SD_OFFSET = SDCARD_FLAG | RAW_FLAG,
    SD_FATFS = SDCARD_FLAG | FATFS_FLAG,
    SD_LITTLEFS = SDCARD_FLAG | LITTLEFS_FLAG,
    SD_FATFS_MBR = SDCARD_FLAG | FATFS_FLAG | MBR_FLAG,
    SD_LITTLEFS_MBR = SDCARD_FLAG | LITTLEFS_FLAG | MBR_FLAG
};

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class OTAStorage_Portenta_SD : public OTAStorage
{
public:
           OTAStorage_Portenta_SD();
  virtual ~OTAStorage_Portenta_SD() { }


  virtual bool   init  () override;
  virtual bool   open  () override;
  virtual size_t write (uint8_t const * const buf, size_t const num_bytes) override;
  virtual void   close () override;
  virtual void   remove() override;
  virtual bool   rename() override;
  virtual void   deinit() override;

private:

    storageTypePortenta storagePortenta;

    //mbed::FATFileSystem fs;

    //SDMMCBlockDevice block_device;

    //RTC_HandleTypeDef RTCHandle;

    int started;

    //DIR *dir;

    //struct dirent *ent;

    int update_size;


};

//extern OTAStorage_Portenta_SD OTAPortenta_SD;

//#endif /* OTA_STORAGE_PORTENTA_SD */

#endif /* OTA_STORAGE_PORTENTA */

#endif /* ARDUINO_OTA_STORAGE_PORTENTA_SD_H_ */
