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
#if OTA_ENABLED
//#if OTA_STORAGE_PORTENTA

#include "OTALogicPortenta.h"

#ifndef HOST
  #include <Arduino.h>
  //#include <Arduino_DebugUtils.h>
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
, _ota_state{OTAState::Init}
, _ota_error{OTAError::None}
//, _storagePortenta{storageTypePortenta::SD_FATFS}
{
  //init_mqtt_ota_buffer();
  //init_ota_binary_data();
}

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

void OTALogicPortenta::setOTAStorage(OTAStorage & ota_storage)
{
  _ota_storage = &ota_storage;
  _is_configured = true;
  /*
  Serial1.print("OTALogicPortenta::setOTAStorage    _is_configured = ");
  Serial1.println(_is_configured);
  */

}

OTAError OTALogicPortenta::update()
{
  /* This if clause should never happen. None the less we
   * should insure ourselves against this scenario because
   * otherwise we'll have a nullptr dereferencing.
   */
  if (!_is_configured) {
    //Serial1.println("OTALogicPortenta NOT configured!");
    _ota_state = OTAState::Error;
    _ota_error = OTAError::NoOTAStorageConfigured;
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
/*
void OTALogic::onOTADataReceived(uint8_t const * const data, size_t const length)
{
  size_t const bytes_available = (MQTT_OTA_BUF_SIZE - _mqtt_ota_buf.num_bytes);
  if(length <= bytes_available)
  {
    memcpy(_mqtt_ota_buf.buf + _mqtt_ota_buf.num_bytes, data, length);
    _mqtt_ota_buf.num_bytes += length;
  }
  else
  {
    _ota_state = OTAState::Error;
    _ota_error = OTAError::ReceivedDataOverrun;
  }
}
*/
/******************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 ******************************************************************************/

OTAState OTALogicPortenta::handle_Init()
{/*
#ifndef HOST
  DBG_VERBOSE("OTALogic::%s", __FUNCTION__);
#endif
*/
  if (_ota_storage->init()) {
    //Serial1.println("OTALogicPortenta::handle_Init -> StartDownload");
    return OTAState::StartDownload;
    //return OTAState::Idle;
  } else {
    //Serial1.println("OTALogicPortenta::handle_Init -> INCORRECT STATE");
    _ota_error = OTAError::StorageInitFailed;
    return OTAState::Error;
  }
}
/*
OTAState OTALogic::handle_Idle()
{
  if(_mqtt_ota_buf.num_bytes > 0) {
    return OTAState::StartDownload;
  }
  return OTAState::Idle;
}
*/
OTAState OTALogicPortenta::handle_StartDownload()
{/*
#ifndef HOST
  DBG_VERBOSE("OTALogic::%s", __FUNCTION__);
#endif
*/
  if(_ota_storage->open()) {
    //Serial1.println("OTALogicPortenta::handle_StartDownload -> BinaryReceived");
    return OTAState::BinaryReceived;
  } else {
    //Serial1.println("OTALogicPortenta::handle_StartDownload -> INCORRECT STATE");
    _ota_error = OTAError::StorageOpenFailed;
    return OTAState::Error;
  }
}
/*
OTAState OTALogic::handle_WaitForBinary()
{
  if (_mqtt_ota_buf.num_bytes > 0) {
    return OTAState::BinaryReceived;
  }
  return OTAState::WaitForBinary;
}
*/
OTAState OTALogicPortenta::handle_BinaryReceived()
{
    //Serial1.println("OTALogicPortenta::handle_BinaryReceived -> Reset");
    uint8_t buf[10];
  /* Write to OTA storage */
  //Serial1.println("Im going to start the write");
  int size = _ota_storage->write(buf, 10);
  /*
  Serial1.print("Write completed! Size = ");
  Serial1.println(size);
  Serial1.println("Im going to close the file");
  */
  _ota_storage->close();
  return OTAState::Reset;
}
/*
OTAState OTALogic::handle_Verify()
{
#ifndef HOST
  DBG_VERBOSE("OTALogic::%s", __FUNCTION__);
#endif
  if(_ota_bin_data.crc32 == _ota_bin_data.hdr_crc32) {
    return OTAState::Rename;
  } else {
    _ota_storage->remove();
    _ota_error = OTAError::ChecksumMismatch;
    return OTAState::Error;
  }
}

OTAState OTALogic::handle_Rename()
{
#ifndef HOST
  DBG_VERBOSE("OTALogic::%s", __FUNCTION__);
#endif
  if(_ota_storage->rename()) {
    _ota_storage->deinit();
    return OTAState::Reset;
  }
  else {
    _ota_error = OTAError::RenameOfTempFileFailed;
    return OTAState::Error;
  }
}
*/
OTAState OTALogicPortenta::handle_Reset()
{
  //Serial1.println("OTALogicPortenta::handle_Reset");
//#if !defined(HOST) && !defined(ESP8266)
  /* After completion of the reset a second stage bootloader
   * such as the SFU is examining the OTA storage medium for
   * the existence of a binary update file. Should such a file
   * exist the 2nd stage bootloader if performing the firmware
   * update before starting the application, otherwise the app
   * is started directly.
   */
  /*
#ifndef HOST
  DBG_VERBOSE("OTALogic::%s", __FUNCTION__);
  delay(250);
#endif
*/
  NVIC_SystemReset();
//#endif /* HOST */
  return OTAState::Reset;
}
/*
void OTALogic::init_mqtt_ota_buffer()
{
  memset(_mqtt_ota_buf.buf, 0U, sizeof(_mqtt_ota_buf.buf));
  _mqtt_ota_buf.num_bytes = 0;
}

void OTALogic::init_ota_binary_data()
{
  _ota_bin_data.hdr_len        = 0;
  _ota_bin_data.hdr_crc32      = 0;
  _ota_bin_data.bytes_received = 0;
  _ota_bin_data.crc32          = crc_init();
}
*/
//#endif /* OTA_STORAGE_PORTENTA */

#endif /* OTA_ENABLED */
