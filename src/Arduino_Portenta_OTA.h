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

#ifndef ARDUINO_PORTENTA_OTA_H_
#define ARDUINO_PORTENTA_OTA_H_

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
    INTERNAL_FLASH_FATFS    = INTERNAL_FLASH_FLAG | FATFS_FLAG,
    INTERNAL_FLASH_LITTLEFS = INTERNAL_FLASH_FLAG | LITTLEFS_FLAG,
    QSPI_FLASH_FATFS        = QSPI_FLASH_FLAG | FATFS_FLAG,
    QSPI_FLASH_FATFS_MBR    = QSPI_FLASH_FLAG | FATFS_FLAG | MBR_FLAG,
    SD_FATFS                = SDCARD_FLAG | FATFS_FLAG,
    SD_FATFS_MBR            = SDCARD_FLAG | FATFS_FLAG | MBR_FLAG,
};

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class Arduino_Portenta_OTA
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

             Arduino_Portenta_OTA(StorageTypePortenta const storage_type, uint32_t const data_offset);
    virtual ~Arduino_Portenta_OTA();


    Error begin();
    Error update();
    void  reset();

    /* This functionality is intended for usage with the Arduino IoT Cloud for
     * performing OTA firmware updates using the Arduino IoT Cloud servers.
     */
    int download(const char * url);
    int decompress();


  protected:

    StorageTypePortenta _storage_type;
    uint32_t _data_offset;
    uint32_t _program_length;

    virtual bool init() = 0;
    virtual bool open() = 0;

    static bool findProgramLength(DIR * dir, uint32_t & program_length);

  private:

    void write();

};

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "Arduino_Portenta_OTA_SD.h"
#include "Arduino_Portenta_OTA_QSPI.h"
#include "Arduino_Portenta_OTA_InternalFlash.h"

#endif /* ARDUINO_PORTENTA_OTA_H_ */