Arduino_Portenta_OTA
====================

[![Compile Examples](https://github.com/arduino-libraries/Arduino_Portenta_OTA/workflows/Compile%20Examples/badge.svg)](https://github.com/arduino-libraries/Arduino_Portenta_OTA/actions?workflow=Compile+Examples)
[![Arduino Lint](https://github.com/arduino-libraries/Arduino_Portenta_OTA/workflows/Arduino%20Lint/badge.svg)](https://github.com/arduino-libraries/Arduino_Portenta_OTA/actions?workflow=Arduino+Lint)
[![Spell Check](https://github.com/arduino-libraries/Arduino_Portenta_OTA/workflows/Spell%20Check/badge.svg)](https://github.com/arduino-libraries/Arduino_Portenta_OTA/actions?workflow=Spell+Check)

This library allows OTA (Over-The-Air) firmware updates for the Arduino Portenta H7. OTA binaries are downloaded via WiFi and stored on a SD card or on the Portenta H7's QSPI flash storage. Next, all information relevant to the firmware update is stored in non-volatile memory. After a reset the Portenta H7 bootloader accesses this information and uses it to perform the firmware update.

### Example
```C++
#include <Arduino_Portenta_OTA.h>
#include <WiFi.h>
#include "arduino_secrets.h"
/* ... */
void setup()
{
  if (WiFi.status() == WL_NO_SHIELD)
    return;

  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(SSID, PASS);
    delay(10000);
  }

  Arduino_Portenta_OTA_QSPI ota(QSPI_FLASH_FATFS_MBR, 2);
  Arduino_Portenta_OTA::Error ota_err = Arduino_Portenta_OTA::Error::None;

  if (!ota.isOtaCapable())
    return;

  if ((ota_err = ota.begin()) != Arduino_Portenta_OTA::Error::None)
    return;

  int const ota_download = ota.download("http://downloads.arduino.cc/ota/OTA_Usage_Portenta.ino.PORTENTA_H7_M7.ota", false /* is_https */);
  if (ota_download <= 0)
    return;

  int const ota_decompress = ota.decompress();
  if (ota_decompress < 0)
    return;

  if ((ota_err = ota.update()) != Arduino_Portenta_OTA::Error::None)
    return;

  ota.reset();
}

void loop()
{

}
```
