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

#include "OTALogicPortenta.h"

/******************************************************************************
 * CTOR/DTOR
 ******************************************************************************/

OTALogicPortenta::OTALogicPortenta()
: _is_configured{false}
, _ota_storage{nullptr}
{
}

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

void OTALogicPortenta::setOTAStorage(OTAStoragePortenta & ota_storage, StorageTypePortenta storageType, uint32_t offset, uint32_t length)
{
  _ota_storage = &ota_storage;
  _ota_storage->storagePortenta = storageType;
  _ota_storage->data_offset = offset;
  _ota_storage->program_len = length;
  _is_configured = true;
  _ota_storage->init();
}

PortentaOTAError OTALogicPortenta::update()
{
  if (!_is_configured)
    return PortentaOTAError::NoOTAStorageConfigured;

  if(!_ota_storage->open())
    return PortentaOTAError::StorageOpenFailed;

  _ota_storage->write();
  _ota_storage->close();
  NVIC_SystemReset();
  return PortentaOTAError::None;
}
