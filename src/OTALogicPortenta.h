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

#ifndef HOST
  #include <AIoTC_Config.h>
#else
  #define OTA_ENABLED (1)
#endif

#include "OTAStoragePortenta.h"


/******************************************************************************
 * TYPEDEF
 ******************************************************************************/

enum class OTAState
{
  Init, Idle, StartDownload, WaitForHeader, HeaderReceived, WaitForBinary, BinaryReceived, Verify, Rename, Reset, Error
};

enum class OTAError : int
{
  None                   = 0,
  StorageInitFailed      = 1,
  StorageOpenFailed      = 2,
  StorageWriteFailed     = 3,
  ChecksumMismatch       = 4,
  ReceivedDataOverrun    = 5,
  RenameOfTempFileFailed = 6,
  NoOTAStorageConfigured = 7
};

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class OTALogicPortenta
{

public:

  OTALogicPortenta();


  void setOTAStorage(OTAStoragePortenta & ota_storage, storageTypePortenta storageType, uint32_t offset, uint32_t length);


  OTAError update();

#ifdef HOST
  inline OTAState state() const { return _ota_state; }
  inline OTAError error() const { return _ota_error; }
#endif


private:

  bool _is_configured;
  OTAStoragePortenta * _ota_storage;
  OTAState _ota_state;
  OTAError _ota_error;

  OTAState handle_Init();
  OTAState handle_StartDownload();
  OTAState handle_BinaryReceived();
  OTAState handle_Reset();
};

#endif /* ARDUINO_OTA_LOGIC_PORTENTA_H_ */