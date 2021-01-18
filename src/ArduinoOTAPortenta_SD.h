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

#ifndef ARDUINO_OTA_PORTENTA_SD_H_
#define ARDUINO_OTA_PORTENTA_SD_H_

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "ArduinoOTAPortenta.h"

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class ArduinoOTAPortenta_SD : public ArduinoOTAPortenta
{
public:

           ArduinoOTAPortenta_SD(StorageTypePortenta const storage_type, uint32_t const data_offset);
  virtual ~ArduinoOTAPortenta_SD() { }


protected:

  virtual bool   init  () override;
  virtual bool   open  () override;
  virtual size_t write () override;
  virtual void   close () override;


private:

  mbed::BlockDevice * _bd;
  SDMMCBlockDevice _block_device;
  mbed::FATFileSystem * _fs_sd;
  int _update_size_sd;

};

#endif /* ARDUINO_OTA_PORTENTA_SD_H_ */
