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

#include "OTAStorage_Internal_Flash.h"

#include "stm32h7xx_hal_rtc_ex.h"

using namespace arduino;

/******************************************************************************
   CONSTANTS
 ******************************************************************************/

FlashIAPBlockDevice bd(0x8000000, 2 * 1024 * 1024);

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool OTAStorage_Internal_Flash::init()
{
  return true;
}

bool OTAStorage_Internal_Flash::open()
{
  return true;
}

size_t OTAStorage_Internal_Flash::write()
{
  Serial1.println("OTAStorage_Internal_Flash::write");
  delay(200);

  if(storagePortenta==INTERNAL_FLASH_OFFSET) {
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, 0x07AA);
    Serial1.println("OTAStorage_Internal_Flash::write    1");
    Serial1.print("OTAStorage_Internal_Flash::write    storagePortenta = ");
    Serial1.println(storagePortenta);
    delay(200);
    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR1, storagePortenta);
    Serial1.println("OTAStorage_Internal_Flash::write    2");
    Serial1.print("OTAStorage_Internal_Flash::write    update_size = ");
    int update_size_Internal_Flash = 2 * 1024 * 1024;
    Serial1.println(update_size_Internal_Flash);
    delay(200);

    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR2, 0X80000);

    HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR3, update_size_Internal_Flash);
    Serial1.println("OTAStorage_Internal_Flash::write    3");
    delay(200);

    return update_size_Internal_Flash;
  } else {
    Serial1.println("storageType not implemented yet");
    return 0;
  }
}

void OTAStorage_Internal_Flash::close()
{
  /* Nothing to do */
}

void OTAStorage_Internal_Flash::remove()
{
  /* Nothing to do */
}

bool OTAStorage_Internal_Flash::rename()
{
  /* Nothing to do */
}

void OTAStorage_Internal_Flash::deinit()
{
  /* Nothing to do */
}

#endif /* OTA_STORAGE_PORTENTA */
