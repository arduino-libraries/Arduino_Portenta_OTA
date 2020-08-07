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

#include "OTAStorage_Portenta_SD.h"

#include "stm32h7xx_hal_rtc_ex.h"

using namespace arduino;

/******************************************************************************
   CONSTANTS
 ******************************************************************************/

static char const SD_UPDATE_FILENAME[] = "UPDATE.BIN";

SDMMCBlockDevice block_device;

mbed::FATFileSystem fs("fs");

DIR *dir;

struct dirent *ent;

int update_size;

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool OTAStorage_Portenta_SD::init()
{
  int err =  fs.mount(&block_device);
  /*
  Serial1.print("OTAStorage_Portenta_SD::init()    err = ");
  Serial1.println(err);
  */
  if (err)
    return false;
  else
    return true;
}

bool OTAStorage_Portenta_SD::open()
{
    if ((dir = opendir("/fs")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) {
            if (String(ent->d_name) == "UPDATE.BIN") {
                struct stat stat_buf;
                stat("/fs/UPDATE.BIN", &stat_buf);
                update_size = stat_buf.st_size;
                /*
                Serial1.print("OTAStorage_Portenta_SD::open()   update_size = ");
                Serial1.println(update_size);
                */
                return true;
            }
        }
    }
    return false;
}

size_t OTAStorage_Portenta_SD::write()
{
  storagePortenta = SD_FATFS;
  Serial1.println("OTAStorage_Portenta_SD::write");
  delay(200);
    // OTA file is already in the FAT partition of the SDCARD
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, 0x07AA);
  Serial1.println("OTAStorage_Portenta_SD::write    1");
  Serial1.print("OTAStorage_Portenta_SD::write    storagePortenta = ");
  Serial1.println(storagePortenta);
  delay(200);
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR1, storagePortenta);
  Serial1.println("OTAStorage_Portenta_SD::write    2");
  Serial1.print("OTAStorage_Portenta_SD::write    update_size = ");
  Serial1.println(update_size);
  delay(200);

  // offset is useless if the storage medium is a partition
  // HAL_RTCEx_BKUPWrite(&RtCHandle, RTC_BKP_DR2, offset);

  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR3, update_size);
  Serial1.println("OTAStorage_Portenta_SD::write    3");
  delay(200);

  return update_size;
}

void OTAStorage_Portenta_SD::close()
{
  closedir (dir);
}

void OTAStorage_Portenta_SD::remove()
{

}

bool OTAStorage_Portenta_SD::rename()
{

}

void OTAStorage_Portenta_SD::deinit()
{
  /* Nothing to do */
}

#endif /* OTA_STORAGE_PORTENTA */
