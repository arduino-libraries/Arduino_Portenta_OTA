/*
   This file is part of Arduino_Portenta_OTA.

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

#include "Arduino_Portenta_OTA_Config.h"
#if defined(ARDUINO_PORTENTA_OTA_QSPI_SUPPORT)
  #include <QSPIFBlockDevice.h>
#endif

#if defined(ARDUINO_PORTENTA_OTA_SDMMC_SUPPORT)
  #include <SDMMCBlockDevice.h>
#endif

#include <BlockDevice.h>
#include <MBRBlockDevice.h>
#include <FATFileSystem.h>
#include <LittleFileSystem.h>
#include <Arduino_DebugUtils.h>

#include "WiFi.h" /* WiFi from ArduinoCore-mbed */
#include <SocketHelpers.h>

/******************************************************************************
 * DEFINE
 ******************************************************************************/

#define APOTA_QSPI_FLASH_FLAG         (1 << 2)
#define APOTA_SDCARD_FLAG             (1 << 3)
#define APOTA_RAW_FLAG                (1 << 4)
#define APOTA_FATFS_FLAG              (1 << 5)
#define APOTA_LITTLEFS_FLAG           (1 << 6)
#define APOTA_MBR_FLAG                (1 << 7)

#define ARDUINO_PORTENTA_OTA_HAS_WATCHDOG_FEED

/******************************************************************************
 * TYPEDEF
 ******************************************************************************/

enum StorageTypePortenta {
    QSPI_FLASH_FATFS        = APOTA_QSPI_FLASH_FLAG | APOTA_FATFS_FLAG,
    QSPI_FLASH_FATFS_MBR    = APOTA_QSPI_FLASH_FLAG | APOTA_FATFS_FLAG | APOTA_MBR_FLAG,
    SD_FATFS                = APOTA_SDCARD_FLAG | APOTA_FATFS_FLAG,
    SD_FATFS_MBR            = APOTA_SDCARD_FLAG | APOTA_FATFS_FLAG | APOTA_MBR_FLAG,
};

typedef void(*ArduinoPortentaOtaWatchdogResetFuncPointer)(void);

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class Arduino_Portenta_OTA
{
  public:

    enum class Error : int
    {
      None                 =  0,
      NoCapableBootloader  = -1,
      NoOtaStorage         = -2,
      OtaStorageInit       = -3,
      OtaStorageOpen       = -4,
      OtaHeaderLength      = -5,
      OtaHeaderCrc         = -6,
      OtaHeaterMagicNumber = -7,
      CaStorageInit        = -8,
      CaStorageOpen        = -9,
    };

             Arduino_Portenta_OTA(StorageTypePortenta const storage_type, uint32_t const data_offset);
    virtual ~Arduino_Portenta_OTA();


    static bool  isOtaCapable();
    Error begin();
    Error update();
    void  reset();

    /* This functionality is intended for usage with the Arduino IoT Cloud for
     * performing OTA firmware updates using the Arduino IoT Cloud servers.
     */
    int download(const char * url, bool const is_https, MbedSocketClass * socket = static_cast<MbedSocketClass*>(&WiFi));
    int decompress();
    void setFeedWatchdogFunc(ArduinoPortentaOtaWatchdogResetFuncPointer func);
    void feedWatchdog();


  protected:

    StorageTypePortenta _storage_type;
    uint32_t _data_offset;
    uint32_t _program_length;
    mbed::BlockDevice * _bd_raw_qspi;

    virtual bool init() = 0;
    virtual bool open() = 0;

    static bool findProgramLength(DIR * dir, uint32_t & program_length);

  private:

    void write();
    bool caStorageInit();
    bool caStorageOpen();
    ArduinoPortentaOtaWatchdogResetFuncPointer _feed_watchdog_func = 0;

};

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#if defined(ARDUINO_PORTENTA_OTA_SDMMC_SUPPORT)
  #include "Arduino_Portenta_OTA_SD.h"
#endif

#if defined(ARDUINO_PORTENTA_OTA_QSPI_SUPPORT)
  #include "Arduino_Portenta_OTA_QSPI.h"
#endif

#endif /* ARDUINO_PORTENTA_OTA_H_ */