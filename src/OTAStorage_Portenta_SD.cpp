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

// /******************************************************************************
//    INCLUDE
//  ******************************************************************************/

// #include "OTAStorage_Portenta_SD.h"

// #include "stm32h7xx_hal_rtc_ex.h"
// #include "stm32h7xx_hal_sd.h"
// #include "BSP.h"

// using namespace arduino;

// /******************************************************************************
//    CONSTANTS
//  ******************************************************************************/

// static char const SD_UPDATE_FILENAME[] = "UPDATE.BIN";

// mbed::BlockDevice* bd = NULL;

// SDMMCBlockDevice block_device;

// mbed::FATFileSystem* fs_SD = NULL;

// DIR *dir_SD;

// struct dirent *ent_SD;

// int update_size_SD;

// BSP_SD_CardInfo card_info;

// /******************************************************************************
//    PUBLIC MEMBER FUNCTIONS
//  ******************************************************************************/

// bool OTAStorage_Portenta_SD::init()
// {
//   int err;

//   if(storagePortenta==SD_OFFSET) {
//     err = block_device.init();
//     if (err)
//       return false;
//     else
//       return true;
//   } else if(storagePortenta==SD_FATFS) {
//     fs_SD = new mbed::FATFileSystem("fs");
//     err =  fs_SD->mount(&block_device);
//     if (err) {
//       Serial1.print("Error while mounting the filesystem. Err = ");
//       Serial1.println(err);
//       return false;
//     } else {
//       Serial1.println("Filesystem correcly mounted");
//       return true;
//     }
//   } else if (storagePortenta==SD_FATFS_MBR) {
//     bd = &block_device;
//     mbed::BlockDevice* physical_block_device = bd;
//     bd = new mbed::MBRBlockDevice(physical_block_device, 1);
//     fs_SD = new mbed::FATFileSystem("fs");
//     err =  fs_SD->mount(bd);
//     if (err) {
//       Serial1.print("Error while mounting the filesystem. Err = ");
//       Serial1.println(err);
//       return false;
//     } else {
//       Serial1.println("Filesystem correcly mounted");
//       return true;
//     }
//   } else {
//     Serial1.println("storageType not implemented yet");
//     return false;
//   }
// }

// bool OTAStorage_Portenta_SD::open()
// {
//   if(storagePortenta==SD_OFFSET) {
//     BSP_SD_GetCardInfo(&card_info);
//     Serial1.print("OTAStorage_Portenta_SD::open    CardType = ");
//     Serial1.println(card_info.CardType);
//     Serial1.print("OTAStorage_Portenta_SD::open    CardVersion = ");
//     Serial1.println(card_info.CardVersion);
//     Serial1.print("OTAStorage_Portenta_SD::open    Class = ");
//     Serial1.println(card_info.Class);
//     Serial1.print("OTAStorage_Portenta_SD::open    relative card address = ");
//     Serial1.println(card_info.RelCardAdd);
//     Serial1.print("OTAStorage_Portenta_SD::open    BlockNbr = ");
//     Serial1.println(card_info.BlockNbr);
//     Serial1.print("OTAStorage_Portenta_SD::open    BlockSize = ");
//     Serial1.println(card_info.BlockSize);
//     Serial1.print("OTAStorage_Portenta_SD::open    LogBlockNbr = ");
//     Serial1.println(card_info.LogBlockNbr);
//     Serial1.print("OTAStorage_Portenta_SD::open    LogBlockLogBlockSizeNbr = ");
//     Serial1.println(card_info.LogBlockSize);
//     return true;
//   } else if(storagePortenta==SD_FATFS || storagePortenta==SD_FATFS_MBR) {
//     if ((dir_SD = opendir("/fs")) != NULL) {
//       /* print all the files and directories within directory */
//       while ((ent_SD = readdir(dir_SD)) != NULL) {
//         if (String(ent_SD->d_name) == "UPDATE.BIN") {
//             struct stat stat_buf;
//             stat("/fs/UPDATE.BIN", &stat_buf);
//             update_size_SD = stat_buf.st_size;
//             return true;
//         }
//       }
//     }
//   } else {
//     Serial1.println("storageType not implemented yet");
//     return false;
//   }
// }

// size_t OTAStorage_Portenta_SD::write()
// {
//   Serial1.println("OTAStorage_Portenta_SD::write");
//   delay(200);

//   if(storagePortenta==SD_FATFS || storagePortenta==SD_OFFSET || storagePortenta==SD_FATFS_MBR) {
//     HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, 0x07AA);
//     HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR1, storagePortenta);
//     Serial1.println("OTAStorage_Portenta_SD::write    1");
//     Serial1.print("OTAStorage_Portenta_SD::write    storagePortenta = ");
//     Serial1.println(storagePortenta);
//     delay(200);

//     if(storagePortenta==SD_OFFSET || storagePortenta==SD_FATFS_MBR) {
//       HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR2, data_offset);
//       update_size_SD = program_len;
//     }

//     Serial1.println("OTAStorage_Portenta_SD::write    2");
//     Serial1.print("OTAStorage_Portenta_SD::write    update_size = ");
//     Serial1.println(update_size_SD);
//     Serial1.print("OTAStorage_Portenta_SD::write    data_offset = ");
//     Serial1.println(data_offset);
//     delay(200);
//     HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR3, update_size_SD);
//     Serial1.println("OTAStorage_Portenta_SD::write    3");
//     delay(200);
//     return update_size_SD;
//   } else {
//     Serial1.println("storageType not implemented yet");
//     return 0;
//   }
  
// }

// void OTAStorage_Portenta_SD::close()
// {
//   if(storagePortenta==SD_FATFS || storagePortenta==SD_FATFS_MBR) {
//     closedir (dir_SD);
//   }
// }
