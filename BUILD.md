## How to Build

1. Install the latest version of [Visual Studio Code](https://code.visualstudio.com/docs/?dv=win).
2. Once installed, install Python 3.9.7 from [here](https://www.python.org/ftp/python/3.9.7/python-3.9.7-amd64.exe) NOTE: DO NOT INSTALL FROM MICROSOFT STORE. IT WILL NOT WORK!
3. Once that is installed, install Visual Studio from [here](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&rel=16) and when the install dialog opens, make sure to check "Desktop Development with C++"
![Screenshot 2021-10-06 080500](https://user-images.githubusercontent.com/49598984/136198801-759aa0e3-85cd-4c56-91bc-0fdd64fd2cd9.png)
4. Once all of these are installed, open Visual Studio Code.
5. Go to extensions, and search up "PlatformIO IDE". Install that
6. Once PlatformIO is installed, click on it on the sidebar.
![Screenshot 2021-10-06 080831](https://user-images.githubusercontent.com/49598984/136199229-168e3b18-5a5c-4dd6-b47d-333a9418b930.png)
7. Then, once in that menu, go to "Platforms", click "Embedded", and search "ST" and look for "ST STM32". It should be the first result.
![Screenshot 2021-10-06 081232](https://user-images.githubusercontent.com/49598984/136199861-3392cea1-ab24-4a84-88da-fed42010f32a.png)
8. Click on ST STM32 and install it.
9. Then, go and download this repository by clicking on "Code" and then "Download ZIP". Unzip the file that downloads.
11. Open the Unzipped file, and then navigate to buildroot/driver/CH341SER.zip. 
12. Unzip that file, and then run setup. Hit install on the dialoge box that pops up.
13. Once done, go back to the root of the unzipped folder and navigate to buildroot/lib
14. Copy the folder framework-arduinoststm32-maple@99.99.99 to C:/Users/(Your username)/.platformio/packages and then once copied, restart Visual Studio Code.
15. Once Visual Studio Code is open, open back up PlatformIO, and click on home.
16. Click on open project, and navigate to the original unzipped folder. Once in that folder, click on "Open MiniDeltaV2firmware-main".
17. Once the project is opened, click on the PlatformIO icon again. Wait for the "Project Tasks" to load. If they don't load, hover over the Project Tasks and then click on the reload button that appears.
18. Once Project Tasks are loaded, click build. This will take a while
19. You're done! You now have a fresh firmware.bin file ready to flash.  
