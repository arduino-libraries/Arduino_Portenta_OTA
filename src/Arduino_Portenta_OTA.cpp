/*
   This file is part of Arduino_Portenta_OTA.

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

#include "Arduino_Portenta_OTA.h"

#include <stm32h7xx_hal_rtc_ex.h>

/******************************************************************************
 * EXTERN
 ******************************************************************************/

extern RTC_HandleTypeDef RTCHandle;

/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

Arduino_Portenta_OTA::Arduino_Portenta_OTA(StorageTypePortenta const storage_type, uint32_t const data_offset)
: _storage_type{storage_type}
, _data_offset{data_offset}
, _program_length{0}
{

}

Arduino_Portenta_OTA::~Arduino_Portenta_OTA()
{

}

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool Arduino_Portenta_OTA::isOtaCapable()
{
  #define BOOTLOADER_ADDR   (0x8000000)
  uint32_t bootloader_data_offset = 0x1F000;
  uint8_t* bootloader_data = (uint8_t*)(BOOTLOADER_ADDR + bootloader_data_offset);
  uint8_t currentBootloaderVersion = bootloader_data[1];
  if (currentBootloaderVersion < 22)
    return false;
  else
    return true;
}

Arduino_Portenta_OTA::Error Arduino_Portenta_OTA::begin()
{
  if (!isOtaCapable())
    return Error::NoCapableBootloader;

  if (!caStorageInit())
    return Error::CaStorageInit;

  if (!caStorageOpen())
    return Error::CaStorageOpen;

  if (!init())
    return Error::OtaStorageInit;

  return Error::None;
}

Arduino_Portenta_OTA::Error Arduino_Portenta_OTA::update()
{
  if(!open())
    return Error::OtaStorageOpen;

  write();

  return Error::None;
}

void Arduino_Portenta_OTA::reset()
{
  NVIC_SystemReset();
}

void Arduino_Portenta_OTA::setFeedWatchdogFunc(ArduinoPortentaOtaWatchdogResetFuncPointer func)
{
  _feed_watchdog_func = func;
}

void Arduino_Portenta_OTA::feedWatchdog()
{
  if (_feed_watchdog_func)
    _feed_watchdog_func();
}

/******************************************************************************
 * PROTECTED MEMBER FUNCTIONS
 ******************************************************************************/

bool Arduino_Portenta_OTA::findProgramLength(DIR * dir, uint32_t & program_length)
{
  struct dirent * entry = NULL;
  while ((entry = readdir(dir)) != NULL)
  {
    if (String(entry->d_name) == "UPDATE.BIN")
    {
      struct stat stat_buf;
      stat("/fs/UPDATE.BIN", &stat_buf);
      program_length = stat_buf.st_size;
      return true;
    }
  }

  return false;
}

/******************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 ******************************************************************************/

void Arduino_Portenta_OTA::write()
{
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, 0x07AA);
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR1, _storage_type);
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR2, _data_offset);
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR3, _program_length);
}

bool Arduino_Portenta_OTA::caStorageInit()
{
  _bd_raw_qspi = mbed::BlockDevice::get_default_instance();

  if (_bd_raw_qspi->init() != QSPIF_BD_ERROR_OK) {
    Debug.print(DBG_ERROR, F("Error: QSPI init failure."));
    return false;
  }

  mbed::MBRBlockDevice * cert_bd_qspi = new mbed::MBRBlockDevice(_bd_raw_qspi, 1);
  mbed::FATFileSystem *  cert_fs_qspi = new mbed::FATFileSystem("wlan");
  int const err_mount =  cert_fs_qspi->mount(cert_bd_qspi);
  if (err_mount) {
    Debug.print(DBG_ERROR, F("Error while mounting the certificate filesystem. Err = %d"), err_mount);
    return false;
  }
  return true;
}

bool Arduino_Portenta_OTA::caStorageOpen()
{
  FILE* fp = fopen("/wlan/cacert.pem", "r");
  if (!fp) {
    Debug.print(DBG_ERROR, F("Error while opening the certificate file."));
    return false;
  }
  fclose(fp);

  return true;
}
