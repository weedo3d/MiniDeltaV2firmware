# MiniDeltaV2 Community Firmware
![image](http://www.weedo.ltd/wp-content/uploads/2021/08/minideltav2.jpg)

## Summary
This is the repository that contains the community version firmware for the Monoprice MiniDeltaV2 3D Printer. 

The framework of the firmware is based on the Marlin 2.0.x version. 

## Compile requirements

- Download and install [VSCode](https://code.visualstudio.com/)
- Search and install PlatformIO IDE from store marketplace
- Search and install ST STM32 embedded platform from PIO Home
- Copy framework-arduinoststm32-maple@99.99.99 from /buildroot/lib to PlatformIO library location (user)/.platformio/packages/
- Install the USB driver from /buildroot/driver/CH341SER.ZIP

## Upload firmware

MiniDeltaV2 uses a customized bootloader, which requires a customized download program for firmware update.  

The Windows version of the download program WEEDOIAP.exe is located in the /buildroot/upload/ directory. The macos version is still under development.

Lanuch the WEEDOIAP.exe, open the firmware.bin file from build directory, choice the com port, then click the update.

![image](http://www.weedo.ltd/wp-content/uploads/2021/04/weedoiap.png)

Wait for about 1 minute, the printer will automatically restart after the firmware update is completed.

## The difference between the community version firmware and the official version firmware

The official firmware has the function of upgrading the firmware via TF card. Use the IAP function in the control menu to read the flash.wfm file on the TF card to upgrade the firmware.

The community version firmware does not support the wfm format, so it does not support firmware upgrade via TF card.

## Return to the official vesrion firmware

Use WEEDOIAP.exe to flash MiniDeltaV2_Vxxx_Community_to_Official.bin in the /Release directory.


