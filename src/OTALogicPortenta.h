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

#ifndef ARDUINO_OTA_LOGIC_PORTENTA_H_
#define ARDUINO_OTA_LOGIC_PORTENTA_H_

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "OTAStoragePortenta.h"

/******************************************************************************
 * DEFINE
 ******************************************************************************/

#define PORTENTA_OTA_ERROR_BASE (100)

/******************************************************************************
 * TYPEDEF
 ******************************************************************************/

enum class OTAState
{
  Init, Idle, StartDownload, WaitForHeader, HeaderReceived, WaitForBinary, BinaryReceived, Verify, Rename, Reset, Error
};

enum class PortentaOTAError : int
{
  None                   = PORTENTA_OTA_ERROR_BASE + 0,
  StorageInitFailed      = PORTENTA_OTA_ERROR_BASE + 1,
  StorageOpenFailed      = PORTENTA_OTA_ERROR_BASE + 2,
  StorageWriteFailed     = PORTENTA_OTA_ERROR_BASE + 3,
  ChecksumMismatch       = PORTENTA_OTA_ERROR_BASE + 4,
  ReceivedDataOverrun    = PORTENTA_OTA_ERROR_BASE + 5,
  RenameOfTempFileFailed = PORTENTA_OTA_ERROR_BASE + 6,
  NoOTAStorageConfigured = PORTENTA_OTA_ERROR_BASE + 7
};

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class OTALogicPortenta
{

public:

  OTALogicPortenta();

  void setOTAStorage(OTAStoragePortenta & ota_storage, StorageTypePortenta storageType, uint32_t offset, uint32_t length);

  PortentaOTAError update();

private:

  bool _is_configured;
  OTAStoragePortenta * _ota_storage;
  OTAState _ota_state;
  PortentaOTAError _ota_error;

  OTAState handle_Init();
  OTAState handle_StartDownload();
  OTAState handle_BinaryReceived();
  OTAState handle_Reset();
};

#endif /* ARDUINO_OTA_LOGIC_PORTENTA_H_ */