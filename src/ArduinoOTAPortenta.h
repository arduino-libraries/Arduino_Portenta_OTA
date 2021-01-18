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

             ArduinoOTAPortenta(StorageTypePortenta const storage_type, uint32_t const data_offset);
    virtual ~ArduinoOTAPortenta();


    bool begin();
    void setUpdateLen(uint32_t const program_length);
    Error update();

    int download(const char * url);
    size_t decompress();


  protected:

    StorageTypePortenta _storage_type;
    uint32_t _data_offset;
    uint32_t _program_length;

    virtual bool   init  () = 0;
    virtual bool   open  () = 0;
    virtual size_t write () = 0;
    virtual void   close () = 0;

};

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "ArduinoOTAPortenta_QSPI.h"

#endif /* ARDUINO_OTA_PORTENTA_H */