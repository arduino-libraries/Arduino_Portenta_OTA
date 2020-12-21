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

#include <AIoTC_Config.h>
#if OTA_STORAGE_PORTENTA

#include "OTAStorage_Portenta_Qspi_Flash.h"

#include "stm32h7xx_hal_rtc_ex.h"

using namespace arduino;

/******************************************************************************
   CONSTANTS
 ******************************************************************************/

mbed::BlockDevice* bd_Qspi = NULL;

QSPIFBlockDevice blockDevice_qspi(PD_11, PD_12, PF_7, PD_13,  PF_10, PG_6, QSPIF_POLARITY_MODE_1, 40000000);

mbed::FATFileSystem* fs_QSPI = NULL;

DIR *dir_QSPI;

struct dirent *ent_QSPI;

int update_size_Qspi;

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool OTAStorage_Portenta_Qspi_Flash::init()
{
  int err;

  if(storagePortenta==QSPI_FLASH_OFFSET) {
    err = blockDevice_qspi.init();
    if (err)
      return false;
    else
      return true;
  } else if(storagePortenta==QSPI_FLASH_FATFS) {
    fs_QSPI = new mbed::FATFileSystem("fs");
    err =  fs_QSPI->mount(&blockDevice_qspi);
    if (err) {
      Serial1.print("Error while mounting the filesystem. Err = ");
      Serial1.println(err);
      return false;
    } else {
      Serial1.println("Filesystem correcly mounted");
      return true;
    }
  } else if (storagePortenta==QSPI_FLASH_FATFS_MBR) {
    Serial1.println("Storage type = QSPI_FLASH_FATFS_MBR");
    bd_Qspi = &blockDevice_qspi;
    mbed::BlockDevice* physical_block_device = bd_Qspi;
    bd_Qspi = new mbed::MBRBlockDevice(physical_block_device, data_offset);
    fs_QSPI = new mbed::FATFileSystem("fs");
    err =  fs_QSPI->mount(bd_Qspi);
    if (err) {
      Serial1.print("Error while mounting the filesystem. Err = ");
      Serial1.println(err);
      return false;
    } else {
      Serial1.println("Filesystem correcly mounted");
      return true;
    }
  } else {
    Serial1.println("ERROR: storageType not available");
    return false;
  }
}

bool OTAStorage_Portenta_Qspi_Flash::open()
{
  if(storagePortenta==QSPI_FLASH_OFFSET) {
    update_size_Qspi = program_len;
    return true;
  } else if(storagePortenta==QSPI_FLASH_FATFS || storagePortenta==QSPI_FLASH_FATFS_MBR) {
    if ((dir_QSPI = opendir("/fs")) != NULL) {
      /* print all the files and directories within directory */
      while ((ent_QSPI = readdir(dir_QSPI)) != NULL) {
        if (String(ent_QSPI->d_name) == "UPDATE.BIN") {
            struct stat stat_buf;
            stat("/fs/UPDATE.BIN", &stat_buf);
            update_size_Qspi = stat_buf.st_size;
            return true;
        }
      }
    }
  } else {
    Serial1.println("ERROR: storageType not available");
    update_size_Qspi = 0;
    return false;
  }
}

size_t OTAStorage_Portenta_Qspi_Flash::write()
{
  if(storagePortenta==QSPI_FLASH_OFFSET || storagePortenta==QSPI_FLASH_FATFS || storagePortenta==QSPI_FLASH_FATFS_MBR) {
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, 0x07AA);
    Serial1.println("OTAStorage_Internal_Flash::write    1");
    Serial1.print("OTAStorage_Internal_Flash::write    storagePortenta = ");
    Serial1.println(storagePortenta);
    delay(200);
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR1, storagePortenta);
    Serial1.println("OTAStorage_Internal_Flash::write    2");
    Serial1.print("OTAStorage_Internal_Flash::write    update_size = ");
    Serial1.println(update_size_Qspi);
    delay(200);

    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR2, data_offset);

    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR3, update_size_Qspi);
    Serial1.println("OTAStorage_Internal_Flash::write    3");
    delay(200);

    return update_size_Qspi;
  } else {
    Serial1.println("storageType not implemented yet");
    return 0;
  }
}

void OTAStorage_Portenta_Qspi_Flash::close()
{
  if(storagePortenta==QSPI_FLASH_FATFS || storagePortenta==QSPI_FLASH_FATFS_MBR) {
    closedir (dir_QSPI);
  }
}

void OTAStorage_Portenta_Qspi_Flash::remove()
{

}

bool OTAStorage_Portenta_Qspi_Flash::rename()
{

}

void OTAStorage_Portenta_Qspi_Flash::deinit()
{
  /* Nothing to do */
}

#endif /* OTA_STORAGE_PORTENTA */
