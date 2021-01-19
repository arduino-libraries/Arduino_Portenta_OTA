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

#ifndef ARDUINO_OTA_PORTENTA_H
#define ARDUINO_OTA_PORTENTA_H

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <BlockDevice.h>
#include <MBRBlockDevice.h>
#include <SDMMCBlockDevice.h>

#include <FATFileSystem.h>
#include <LittleFileSystem.h>

#include <QSPIFBlockDevice.h>

/******************************************************************************
 * DEFINE
 ******************************************************************************/

#define INTERNAL_FLASH_FLAG     (1 << 1)
#define QSPI_FLASH_FLAG         (1 << 2)
#define SDCARD_FLAG             (1 << 3)
#define RAW_FLAG                (1 << 4)
#define FATFS_FLAG              (1 << 5)
#define LITTLEFS_FLAG           (1 << 6)
#define MBR_FLAG                (1 << 7)

/******************************************************************************
 * TYPEDEF
 ******************************************************************************/

enum StorageTypePortenta {
    INTERNAL_FLASH_OFFSET   = INTERNAL_FLASH_FLAG | RAW_FLAG,
    INTERNAL_FLASH_FATFS    = INTERNAL_FLASH_FLAG | FATFS_FLAG,
    INTERNAL_FLASH_LITTLEFS = INTERNAL_FLASH_FLAG | LITTLEFS_FLAG,
    QSPI_FLASH_OFFSET       = QSPI_FLASH_FLAG | RAW_FLAG,
    QSPI_FLASH_FATFS        = QSPI_FLASH_FLAG | FATFS_FLAG,
    QSPI_FLASH_LITTLEFS     = QSPI_FLASH_FLAG | LITTLEFS_FLAG,
    QSPI_FLASH_FATFS_MBR    = QSPI_FLASH_FLAG | FATFS_FLAG | MBR_FLAG,
    QSPI_FLASH_LITTLEFS_MBR = QSPI_FLASH_FLAG | LITTLEFS_FLAG | MBR_FLAG,
    SD_OFFSET               = SDCARD_FLAG | RAW_FLAG,
    SD_FATFS                = SDCARD_FLAG | FATFS_FLAG,
    SD_LITTLEFS             = SDCARD_FLAG | LITTLEFS_FLAG,
    SD_FATFS_MBR            = SDCARD_FLAG | FATFS_FLAG | MBR_FLAG,
    SD_LITTLEFS_MBR         = SDCARD_FLAG | LITTLEFS_FLAG | MBR_FLAG
};

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class ArduinoOTAPortenta
{
  public:

    enum class Error : int
    {
      None                 =  0,
      NoOtaStorage         = -1,
      OtaStorageInit       = -2,
      OtaStorageOpen       = -3,
      OtaHeaderLength      = -4,
      OtaHeaderCrc         = -5,
      OtaHeaterMagicNumber = -6,
    };

             ArduinoOTAPortenta(StorageTypePortenta const storage_type, uint32_t const data_offset);
    virtual ~ArduinoOTAPortenta();


    Error begin();
    void  setUpdateLen(uint32_t const program_length);
    Error update();

    int download(const char * url);
    int decompress();


  protected:

    StorageTypePortenta _storage_type;
    uint32_t _data_offset;
    uint32_t _program_length;

    virtual bool   init  () = 0;
    virtual bool   open  () = 0;
    virtual size_t write () = 0;

};

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "ArduinoOTAPortenta_SD.h"
#include "ArduinoOTAPortenta_QSPI.h"
#include "ArduinoOTAPortenta_InternalFlash.h"

#endif /* ARDUINO_OTA_PORTENTA_H */