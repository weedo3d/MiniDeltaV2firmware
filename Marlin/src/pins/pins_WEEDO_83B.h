/**
* WEEDO 71A Motherboad pin configuration
* Copyright (C) 2021 WEEDO3D Perron
*
*
*/


#ifndef BOARD_NAME
#define BOARD_NAME "WEEDO 83B"
#endif

// X Endstop
#define X_MIN_PIN           -1
#define X_MAX_PIN           PB3

// Y Endstop
#define Y_MIN_PIN			-1
#define Y_MAX_PIN			PA8

// Z Endstop
#define Z_MIN_PIN			PA15
#define Z_MAX_PIN			PB13

// X Motor
#define X_STEP_PIN         PA4
#define X_DIR_PIN          PA1
#define X_ENABLE_PIN       PA5

// Y Motor
#define Y_STEP_PIN         PA7
#define Y_DIR_PIN          PA6
#define Y_ENABLE_PIN       PC4

// Z Motor
#define Z_STEP_PIN          PB0
#define Z_DIR_PIN           PC5
#define Z_ENABLE_PIN        PB1

// A Motor
#define E0_STEP_PIN         PB11
#define E0_DIR_PIN          PB10
#define E0_ENABLE_PIN       PB12

// Extruder0
#define AFAN_PIN            PC3
#define HEATER_0_PIN        PC2
#define TEMP_0_PIN          PC1

// Heater Bed
#define HEATER_BED_PIN		  PA0
#define TEMP_BED_PIN          PC0 

#define TEMP_CHAMBER_PIN       TEMP_BED_PIN

//#define POWEROFFPIN 28

#define LED_PIN             PC13

// #define B1_PIN              PB14
// #define B2_PIN              PB13
// #define B3_PIN              PB12

// #define STM_BUSY            PB8
// #define STM_ABORT           PB9

// #define FIL_RUNOUT_PIN     PB15
// #define FIL_RUNOUT1_PIN     PB15

#define SD_DETECT_PIN           PB5

//
// Onboard SD support
//
#define SDIO_D0_PIN        PC8
#define SDIO_D1_PIN        PC9
#define SDIO_D2_PIN        PC10
#define SDIO_D3_PIN        PC11
#define SDIO_CK_PIN        PC12
#define SDIO_CMD_PIN       PD2

#define STM_SD_CS         PC7

#define STM_SD_BUSY         PC6

#define SDIO_SUPPORT 

#define SPI_SPEED           SPI_EIGHTH_SPEED

#define SD_DETECT_STATE     LOW

// Motor current PWM pins
#define MOTOR_CURRENT_PWM_X_PIN     PB9
#define MOTOR_CURRENT_PWM_Y_PIN     PB8
#define MOTOR_CURRENT_PWM_Z_PIN     PB7
#define MOTOR_CURRENT_PWM_E_PIN     PB6
#define MOTOR_CURRENT_PWM_RANGE    1500 // (255 * (1000mA / 65535)) * 257 = 1000 is equal 1.6v Vref in turn equal 1Amp
//#define DEFAULT_PWM_MOTOR_CURRENT  { 600, 700, 200, 400 } // 1.05Amp per driver, here is X, Y, Z and E. This values determined empirically.

 #define DEFAULT_PWM_MOTOR_CURRENT  { 600, 600, 600, 400 } // 1.05Amp per driver, here is X, Y, Z and E. This values determined empirically.

#define FLASH_EEPROM_EMULATION
#define EEPROM_PAGE_SIZE     uint16(0x800) // 2KB
#define EEPROM_START_ADDRESS uint32(0x8000000 + 512 * 1024 - 2 * EEPROM_PAGE_SIZE)
#undef E2END
#define E2END                (EEPROM_PAGE_SIZE - 1) // 2KB

// #define RX_BUFFER_SIZE 512

// fake define for notice, you should modify the define in usart.h
// #define USART_RX_BUF_SIZE 512
// #define USART_TX_BUF_SIZE 512