/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_ADDR_H
#define WTGL_ADDR_H

#define SCREEN_BOOT					0x00    
#define ADDR_GLOBLE_CURRENT         0x0000  
#define ADDR_GLOBAL_GOTO            0x0001  
#define ADDR_GLOBAL_MOUNT_SD        0x0002  
#define ADDR_GLOBAL_UNMOUNT_SD      0x0003  
#define ADDR_GLOBAL_BOOTLOADER      0x0004  
#define ADDR_GLOBAL_GCODE           0x0005  

#define SCREEN_POST                 0x01    
#define VAR_POST_SKIP               0x0101 
#define VAR_POST_HELP               0x0102  

#define SCREEN_RESUME               0x02    

#define SCREEN_GUIDE                0x03   
#define VAR_GUIDE_SKIP              0x0301  
#define VAR_GUIDE_FEED              0x0302  
#define VAR_GUIDE_END_FEED          0x0303 
#define VAR_GUIDE_PRINT             0x0304  
#define VAR_GUIDE_INDEX_PRINT       0x0305  

#define SCREEN_MAIN                 0x0A   
#define VAR_MAIN_PRINT              0x0A01  
#define VAR_MAIN_PREPARE            0x0A02  
#define VAR_MAIN_SETUP              0x0A03  
#define VAR_MAIN_INFO               0x0A04  
#define VAR_MAIN_TEST               0x0A05  

#define SCREEN_MSG                  0x0B   
#define VAR_MSG_BACK                0x0B01  

#define SCREEN_TEST                 0x0D    
#define VAR_TEST_X1                 0x0D01  
#define VAR_TEST_X2                 0x0D02  
#define VAR_TEST_Y1                 0x0D03  
#define VAR_TEST_Y2                 0x0D04  
#define VAR_TEST_Z1                 0x0D05  
#define VAR_TEST_Z2                 0x0D06  
#define VAR_TEST_E1                 0x0D07  
#define VAR_TEST_E2                 0x0D08  
#define VAR_TEST_FAN0_ON            0x0D09  
#define VAR_TEST_FAN0_OFF           0x0D0A  
#define VAR_TEST_FAN1_ON            0x0D0B  
#define VAR_TEST_FAN1_OFF           0x0D0C  
#define VAR_TEST_FAN2_ON            0x0D0D  
#define VAR_TEST_FAN2_OFF           0x0D0E  
#define VAR_TEST_BACK               0x0DFF  

#define SCREEN_PREVIEW              0x15    
#define VAR_PREVIEW_FILENAME        0x1500 
#define VAR_PREVIEW_BACK            0x1501 
#define VAR_PREVIEW_FILEINDEX       0x1502  


#define SCREEN_PRINT                0x16    
#define VAR_PRINT_CANCEL            0x1601
#define VAR_PRINT_PAUSE             0x1602  
#define VAR_PRINT_SETTING           0x1603  

#define SCREEN_SPEED_SETTING        0x18   
#define VAR_SPEED_SETTING_DATA      0x1800  

#define SCREEN_PREPARE              0x1E  
#define VAR_PREPARE_BACK            0x1E01  

#define SCREEN_FILAMENT             0x1F    
#define VAR_FILAMENT_W1_BACK        0x1F11 
#define VAR_FILAMENT_FEEDIN         0x1F12  
#define VAR_FILAMENT_RETRACT        0x1F13  
#define VAR_FILAMENT_TYPE           0x1F20  
#define VAR_FILAMENT_W3_BACK        0x1F31 

#define SCREEN_TEMP_SETTING         0x20    
#define VAR_TEMP_SETTING_W1_BACK    0x2011  
#define VAR_TEMP_SETTING_W1_START   0x2012 
#define VAR_TEMP_SETTING_NOZZLE     0x2013  
#define VAR_TEMP_SETTING_BED        0x2014 
#define VAR_TEMP_SETTING_W2_BACK    0x2021 
#define VAR_TEMP_SETTING_W2_SETTING 0x2022 
#define VAR_TEMP_SETTING_W3_BACK    0x2031 

#define SCREEN_JOG                  0x21    
#define VAR_JOG_HOME                0x2101  
#define VAR_JOG_STOP                0x2102  
#define VAR_JOG_BACK                0x2103  
#define VAR_JOG_X1                  0x2111  
#define VAR_JOG_X2                  0x2112 
#define VAR_JOG_X3                  0x2113  
#define VAR_JOG_X4                  0x2114  
#define VAR_JOG_X5                  0x2115 
#define VAR_JOG_X6                  0x2116  
#define VAR_JOG_Y1                  0x2121  
#define VAR_JOG_Y2                  0x2122 
#define VAR_JOG_Y3                  0x2123  
#define VAR_JOG_Y4                  0x2124  
#define VAR_JOG_Y5                  0x2125  
#define VAR_JOG_Y6                  0x2126  
#define VAR_JOG_Z1                  0x2131  
#define VAR_JOG_Z2                  0x2132  
#define VAR_JOG_Z3                  0x2133 
#define VAR_JOG_Z4                  0x2134  
#define VAR_JOG_Z5                  0x2135 
#define VAR_JOG_Z6                  0x2136  

#define SCREEN_LEVEL                0x22    
#define VAR_LEVEL_W1_BACK           0x2211  
#define VAR_LEVEL_START             0x2212 
#define VAR_LEVEL_W2_BACK           0x2221  

#define SCREEN_ZOFFSET              0x23    
#define VAR_ZOFFSET_BACK            0x2301  
#define VAR_ZOFFSET_SAVE            0x2302  
#define VAR_ZOFFSET_UP              0x2303 
#define VAR_ZOFFSET_DOWN            0x2304  

#define SCREEN_MOTOR                0x24    
#define VAR_MOTOR_BACK              0x2401  
#define VAR_MOTOR_X                 0x2402  
#define VAR_MOTOR_Y                 0x2403  
#define VAR_MOTOR_Z                 0x2404  
#define VAR_MOTOR_E                 0x2405  
#define VAR_MOTOR_ALL               0x2406  

#define SCREEN_CONTROL              0x28   
#define VAR_CONTROL_BACK            0x2801  

#define SCREEN_DEVICE_INFO          0x29   
#define VAR_DEVICE_BACK             0x2911  
#define VAR_DEVICE_SHOW_QCODE       0x2912  
#define VAR_DEVICE_QCODE_BACK       0x2921 

#define SCREEN_OPTION_POST          0x2C    
#define VAR_OPTION_POST_DATA        0x2C00 
#define VAR_OPTION_POST_BACK        0x2C01  

#define SCREEN_OPTION_POWERLOSS     0x2D    
#define VAR_OPTION_POWERLOSS_DATA   0x2D00  
#define VAR_OPTION_POWERLOSS_BACK   0x2D01 

#define SCREEN_DIAG                 0x2E    
#define VAR_DIAG_QUIT               0x2E01  
#define VAR_DIAG_END                0x2E02 
#define VAR_DIAG_W1_START           0x2E03 
#define VAR_DIAG_W2_NEXT            0x2E04  
#define VAR_DIAG_W3_NEXT            0x2E05 
#define VAR_DIAG_W4_NEXT            0x2E06  
#define VAR_DIAG_W5_NEXT            0x2E07 
#define VAR_DIAG_W6_NEXT            0x2E08  
#define VAR_DIAG_W7_NEXT            0x2E09  
#define VAR_DIAG_W8_NEXT            0x2E0A  
#define VAR_DIAG_W9_NEXT            0x2E0B 
#define VAR_DIAG_W10_NEXT           0x2E0C  

#define SCREEN_CONFIG               0x2F   
#define VAR_CONFIG_BACK             0x2F11  
#define VAR_CONFIG_SAVE             0x2F12  
#define VAR_CONFIG_LOAD             0x2F13 
#define VAR_CONFIG_RESTORE          0x2F14  
#define VAR_CONFIG_SAVE_BACK        0x2F21  
#define VAR_CONFIG_LOAD_BACK        0x2F31 
#define VAR_CONFIG_RESTORE_BACK     0x2F41  
#define VAR_CONFIG_RESTORE_OK       0x2F42  

#define SCREEN_UPDATE               0x30   
#define VAR_UPDATE_W1_BACK          0x3011  
#define VAR_UPDATE_W1_MOTOR         0x3012  
#define VAR_UPDATE_W1_LCD           0x3013  
#define VAR_UPDATE_W2_BACK          0x3021 
#define VAR_UPDATE_W2_ONLINE        0x3022  
#define VAR_UPDATE_W2_LOCAL         0x3023  
#define VAR_UPDATE_W3_BACK          0x3031  
#define VAR_UPDATE_W3_ONLINE        0x3032  
#define VAR_UPDATE_W3_LOCAL         0x3033 

#define REG_RESET                   0xFFAA  
#define REG_CURRENT_ID              0xFF2F 
#define REG_SCREEN_ID               0xFF30  
#define REG_NOZZLE0_HEATING         0xFF31  
#define REG_NOZZLE1_HEATING         0xFF32  
#define REG_BED_HEATING             0xFF33  
#define REG_X_HOMED                 0xFF34  
#define REG_Y_HOMED                 0xFF35  
#define REG_Z_HOMED                 0xFF36  
#define REG_NOZZLE0_CURRENT         0xFF37  
#define REG_NOZZLE1_CURRENT         0xFF38  
#define REG_BED_CURRENT             0xFF39  
#define REG_NOZZLE0_TARGET          0xFF3A  
#define REG_NOZZLE1_TARGET          0xFF3B  
#define REG_BED_TARGET              0xFF3C  
#define REG_ENDSTOP_STATE           0xFF3D  
#define REG_PRINTER_STATE           0xFF3E  
#define REG_PRINTING_PERCENT        0xFF3F  
#define REG_PRINTING_ELAPSE         0xFF40  
#define REG_PRINTING_REMAIN         0xFF41  
#define REG_PRINTING_SPEED          0xFF42  
#define REG_FILAMENT_OPERATION      0xFF43  
#define REG_POSITION                0xFF44 
#define REG_MESSAGE                 0xFF45  
#define REG_ZOFFSET                 0xFF46 
#define REG_MOTOR_STATE             0xFF47  
#define REG_DEVICE_NAME             0xFF48  
#define REG_DEVICE_VERSION          0xFF49  
#define REG_PRINTED_TIME            0xFF4A  
#define REG_OPTION_DIAG             0xFF4B  
#define REG_OPTION_POWERLOSS        0xFF4C  
#define REG_PLANNER_STATE           0xFF4D 
#define REG_QUEUE_LENGTH            0xFF4E  
#define REG_BUSY                    0xFF4F  
#define REG_ERROR_MSG               0xFF50  
#define REG_CONFIG_SAVE_MSG         0xFF51  
#define REG_ERROR_ID                0xFF52  
#define REG_WIFI_SSID               0xFF53  
#define REG_WIFI_PWD                0xFF54  
#define REG_WIFI_JOIN               0xFF55 
#define REG_OPTION_RUNOUT           0xFF56  
#define REG_OPTION_AUTOPAUSE        0xFF57  
#define REG_OCTOPRINT_STATE         0xFF58 

#endif
