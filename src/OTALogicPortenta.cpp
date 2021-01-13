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
 * INCLUDE
 ******************************************************************************/

#ifndef HOST
  #include <AIoTC_Config.h>
#else
  #define OTA_ENABLED (1)
#endif

#include "OTALogicPortenta.h"

#ifndef HOST
  #include <Arduino.h>
#else
  #include <algorithm> // for std::min, otherwise Arduino defines min() 
  using namespace std;
#endif

#include <string.h>

/******************************************************************************
 * CTOR/DTOR
 ******************************************************************************/

OTALogicPortenta::OTALogicPortenta()
: _is_configured{false}
, _ota_storage{nullptr}
, _ota_state{OTAState::StartDownload}
, _ota_error{PortentaOTAError::None}
{
}

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

void OTALogicPortenta::setOTAStorage(OTAStoragePortenta & ota_storage, storageTypePortenta storageType, uint32_t offset, uint32_t length)
{
  _ota_storage = &ota_storage;
  Serial1.print("OTALogicPortenta::setOTAStorage    storageType = ");
  Serial1.println(storageType);
  _ota_storage->storagePortenta = storageType;
  _ota_storage->data_offset = offset;
  _ota_storage->program_len = length;
  _is_configured = true;
  handle_Init();
}

PortentaOTAError OTALogicPortenta::update()
{
  /* This if clause should never happen. None the less we
   * should insure ourselves against this scenario because
   * otherwise we'll have a nullptr dereferencing.
   */
  if (!_is_configured) {
    _ota_state = OTAState::Error;
    _ota_error = PortentaOTAError::NoOTAStorageConfigured;
    return _ota_error;
  }

  OTAState prev_ota_state;
  /* The purpose of this loop is to allow the transition of
   * more than one state per a singular call of 'update'. If
   * no state transitition takes place then the loop is exited
   * because it means that external input (such as the arrival)
   * of new data is required.
   */
  do
  {
    prev_ota_state = _ota_state;
    switch(_ota_state)
    {
    case OTAState::Init:           _ota_state = handle_Init          (); break;
    case OTAState::Idle:           /*_ota_state = handle_Idle      ()*/; break;
    case OTAState::StartDownload:  _ota_state = handle_StartDownload (); break;
    case OTAState::WaitForHeader:  /*_ota_state = handle_WaitForHeader ()*/; break;
    case OTAState::HeaderReceived: /*_ota_state = handle_HeaderReceived()*/; break;
    case OTAState::WaitForBinary:  /*_ota_state = handle_WaitForBinary ()*/; break;
    case OTAState::BinaryReceived: _ota_state = handle_BinaryReceived(); break;
    case OTAState::Verify:         /*_ota_state = handle_Verify        ()*/; break;
    case OTAState::Rename:         /*_ota_state = handle_Rename        ()*/; break;
    case OTAState::Reset:          _ota_state = handle_Reset         (); break;
    case OTAState::Error:                                                break;
    }
  } while(_ota_state != prev_ota_state);

  return _ota_error;
}

/******************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 ******************************************************************************/

OTAState OTALogicPortenta::handle_Init()
{
  if (_ota_storage->init()) {
    return OTAState::StartDownload;
  } else {
    _ota_error = PortentaOTAError::StorageInitFailed;
    return OTAState::Error;
  }
}

OTAState OTALogicPortenta::handle_StartDownload()
{
  if(_ota_storage->open()) {
    return OTAState::BinaryReceived;
  } else {
    _ota_error = PortentaOTAError::StorageOpenFailed;
    return OTAState::Error;
  }
}

OTAState OTALogicPortenta::handle_BinaryReceived()
{
  int size = _ota_storage->write();

  _ota_storage->close();
  return OTAState::Reset;
}

OTAState OTALogicPortenta::handle_Reset()
{
  /* After completion of the reset a second stage bootloader
   * such as the SFU is examining the OTA storage medium for
   * the existence of a binary update file. Should such a file
   * exist the 2nd stage bootloader if performing the firmware
   * update before starting the application, otherwise the app
   * is started directly.
   */
  NVIC_SystemReset();
  return OTAState::Reset;
}