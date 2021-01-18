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

#include "OTAStoragePortenta.h"

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class ArduinoOTAPortenta
{
  public:

    enum class Error : int
    {
      None           = 0,
      NoOtaStorage   = 1,
      OtaStorageInit = 2,
      OtaStorageOpen = 3
    };

    ArduinoOTAPortenta();


    Error update();

    void setUpdateLen(uint32_t length);
    bool begin(StorageTypePortenta storage, uint32_t offset);

    Error setOTAStorage(OTAStoragePortenta & ota_storage, StorageTypePortenta storageType, uint32_t offset, uint32_t length);

    int download(char* url);
    size_t decompress();

  private:

    OTAStoragePortenta * _ota_storage;
    uint32_t program_len = 0;
    StorageTypePortenta PortentaStorageType;
};

/******************************************************************************
 * EXTERN DECLARATION
 ******************************************************************************/

extern ArduinoOTAPortenta OTAPortenta;

#endif /* ARDUINO_OTA_PORTENTA_H */