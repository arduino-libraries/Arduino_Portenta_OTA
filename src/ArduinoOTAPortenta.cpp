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

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <AIoTC_Config.h>

#if OTA_STORAGE_PORTENTA
#include <ArduinoOTAPortenta.h>
/*
#ifdef BOARD_HAS_ECCX08
  #include "tls/BearSSLTrustAnchors.h"
  #include "tls/utility/CryptoUtil.h"
#endif
*/
//#include "utility/ota/FlashSHA256.h"
#include "OTAStorage_Portenta_SD.h"

//#include "cbor/CBOREncoder.h"

/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/
/*
#if defined(OTA_STORAGE_PORTENTA_INTERNAL_FLASH)
    static OTAStorage_Portenta_Internal_Flash ota_storage_portenta_internal_flash;
#elif defined(OTA_STORAGE_PORTENTA_QSPI_FLASH)
    static OTAStorage_Portenta_Qspi_Flash ota_storage_portenta_qspi_flash;
#elif defined(OTA_STORAGE_PORTENTA_SD)
    static OTAStorage_Portenta_SD ota_storage_portenta_sd;
#endif
*/
  static OTAStorage_Portenta_SD ota_storage_portenta_sd;
/******************************************************************************
   GLOBAL CONSTANTS
 ******************************************************************************/

static const int TIMEOUT_FOR_LASTVALUES_SYNC = 10000;

/******************************************************************************
   LOCAL MODULE FUNCTIONS
 ******************************************************************************/
/*
extern "C" unsigned long getTime()
{
  return ArduinoCloud.getInternalTime();
}
*/
/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

ArduinoOTAPortenta::ArduinoOTAPortenta() :
#if OTA_ENABLED
  _ota_error{static_cast<int>(OTAError::None)}
//, _ota_img_sha256{"Inv."}
//, _storagePortenta{SD}
#endif /* OTA_ENABLED */
{

}

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

void ArduinoOTAPortenta::begin(int storage)  //PortentaStorageType PortentaStorage
{

  Serial1.begin(115200);
  //Serial1.println("ArduinoOTAPortenta begin");

  //_storagePortenta = PortentaStorage;
  switch (storage) {
    case 0: //InternalFlash: 
      //#define OTA_STORAGE_PORTENTA_INTERNAL_FLASH
      //static OTAStorage_Portenta_Internal_Flash ota_storage_portenta_internal_flash;
      //setOTAStorage(ota_storage_portenta_internal_flash);
    break;

    case 1: //QSPIFlash: 
      //#define OTA_STORAGE_PORTENTA_QSPI_FLASH
      //static OTAStorage_Portenta_Qspi_Flash ota_storage_portenta_qspi_flash;
      //setOTAStorage(ota_storage_portenta_qspi_flash);
    break;

    case 2: //SD: 
      //Serial1.println("Im going to set the OTA Storage to SD");
      //#define OTA_STORAGE_PORTENTA_SD
      /*static*/ //OTAStorage_Portenta_SD ota_storage_portenta_sd;
      setOTAStorage(ota_storage_portenta_sd);
    break;

    default:
    break;
  }
}

void ArduinoOTAPortenta::update()
{
#if OTA_ENABLED
    /* If a _ota_logic object has been instantiated then we are spinning its
     * 'update' method here in order to process incoming data and generally
     * to transition to the OTA logic update states.
     */
    OTAError const err = _ota_logic_portenta.update();
    _ota_error = static_cast<int>(err);
    //Serial1.print("ArduinoOTAPortenta::update    _ota_error = ");
    //Serial1.println(_ota_error);
#endif /* OTA_ENABLED */
}

int ArduinoOTAPortenta::connected()
{
  //return _mqttClient.connected();
  return 1;
}
/*
void ArduinoOTAPortenta::printDebugInfo()
{
  DBG_INFO("***** Arduino IoT Cloud for Portenta - configuration info *****");
  //DBG_INFO("Device ID: %s", getDeviceId().c_str());
  //DBG_INFO("Thing ID: %s", getThingId().c_str());
  //DBG_INFO("MQTT Broker: %s:%d", _brokerAddress.c_str(), _brokerPort);
}
*/
#if OTA_ENABLED
void ArduinoOTAPortenta::setOTAStorage(OTAStorage & ota_storage)
{
  //Serial1.println("ArduinoOTAPortenta::setOTAStorage");
  _ota_logic_portenta.setOTAStorage(ota_storage);
}
#endif /* OTA_ENABLED */

/******************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 ******************************************************************************/

/******************************************************************************
 * EXTERN DEFINITION
 ******************************************************************************/

ArduinoOTAPortenta OTAPortenta;

#endif  /*OTA_STORAGE_PORTENTA*/
