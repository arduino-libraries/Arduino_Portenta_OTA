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

#include "OTAStorage_Portenta_Internal_Flash.h"

#include "stm32h7xx_hal_rtc_ex.h"

using namespace arduino;

/******************************************************************************
   CONSTANTS
 ******************************************************************************/

static char const SD_UPDATE_FILENAME[] = "UPDATE.BIN";

int update_size_Internal_Flash;

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool OTAStorage_Portenta_Internal_Flash::init()
{
    //TO BE DEFINED
}

bool OTAStorage_Portenta_Internal_Flash::open()
{
    //TO BE DEFINED
}

size_t OTAStorage_Portenta_Internal_Flash::write()
{
  //TO BE DEFINED

  return update_size_Internal_Flash;
}

void OTAStorage_Portenta_Internal_Flash::close()
{
    //TO BE DEFINED
}

void OTAStorage_Portenta_Internal_Flash::remove()
{

}

bool OTAStorage_Portenta_Internal_Flash::rename()
{

}

void OTAStorage_Portenta_Internal_Flash::deinit()
{
  /* Nothing to do */
}

#endif /* OTA_STORAGE_PORTENTA */
