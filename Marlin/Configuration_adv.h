/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Configuration_adv.h
 *
 * Advanced settings.
 * Only change these if you know exactly what you're doing.
 * Some of these settings can damage your printer if improperly set!
 *
 * Basic settings can be found in Configuration.h
 *
 */
#define CONFIGURATION_ADV_H_VERSION 020005

// @section temperature

//===========================================================================
//=============================Thermal Settings  ============================
//===========================================================================

//
// Custom Thermistor 1000 parameters
//
#if TEMP_SENSOR_0 == 1000
  #define HOTEND0_PULLUP_RESISTOR_OHMS 4700    // Pullup resistor
  #define HOTEND0_RESISTANCE_25C_OHMS  100000  // Resistance at 25C
  #define HOTEND0_BETA                 3950    // Beta value
#endif

#if TEMP_SENSOR_1 == 1000
  #define HOTEND1_PULLUP_RESISTOR_OHMS 4700    // Pullup resistor
  #define HOTEND1_RESISTANCE_25C_OHMS  100000  // Resistance at 25C
  #define HOTEND1_BETA                 3950    // Beta value
#endif

#if TEMP_SENSOR_2 == 1000
  #define HOTEND2_PULLUP_RESISTOR_OHMS 4700    // Pullup resistor
  #define HOTEND2_RESISTANCE_25C_OHMS  100000  // Resistance at 25C
  #define HOTEND2_BETA                 3950    // Beta value
#endif

#if TEMP_SENSOR_3 == 1000
  #define HOTEND3_PULLUP_RESISTOR_OHMS 4700    // Pullup resistor
  #define HOTEND3_RESISTANCE_25C_OHMS  100000  // Resistance at 25C
  #define HOTEND3_BETA                 3950    // Beta value
#endif

#if TEMP_SENSOR_4 == 1000
  #define HOTEND4_PULLUP_RESISTOR_OHMS 4700    // Pullup resistor
  #define HOTEND4_RESISTANCE_25C_OHMS  100000  // Resistance at 25C
  #define HOTEND4_BETA                 3950    // Beta value
#endif

#if TEMP_SENSOR_5 == 1000
  #define HOTEND5_PULLUP_RESISTOR_OHMS 4700    // Pullup resistor
  #define HOTEND5_RESISTANCE_25C_OHMS  100000  // Resistance at 25C
  #define HOTEND5_BETA                 3950    // Beta value
#endif

#if TEMP_SENSOR_6 == 1000
  #define HOTEND6_PULLUP_RESISTOR_OHMS 4700    // Pullup resistor
  #define HOTEND6_RESISTANCE_25C_OHMS  100000  // Resistance at 25C
  #define HOTEND6_BETA                 3950    // Beta value
#endif

#if TEMP_SENSOR_7 == 1000
  #define HOTEND7_PULLUP_RESISTOR_OHMS 4700    // Pullup resistor
  #define HOTEND7_RESISTANCE_25C_OHMS  100000  // Resistance at 25C
  #define HOTEND7_BETA                 3950    // Beta value
#endif

#if TEMP_SENSOR_BED == 1000
  #define BED_PULLUP_RESISTOR_OHMS     4700    // Pullup resistor
  #define BED_RESISTANCE_25C_OHMS      100000  // Resistance at 25C
  #define BED_BETA                     3950    // Beta value
#endif

#if TEMP_SENSOR_CHAMBER == 1000
  #define CHAMBER_PULLUP_RESISTOR_OHMS 4700    // Pullup resistor
  #define CHAMBER_RESISTANCE_25C_OHMS  100000  // Resistance at 25C
  #define CHAMBER_BETA                 3950    // Beta value
#endif

//
// Hephestos 2 24V heated bed upgrade kit.
// https://store.bq.com/en/heated-bed-kit-hephestos2
//
//#define HEPHESTOS2_HEATED_BED_KIT
#if ENABLED(HEPHESTOS2_HEATED_BED_KIT)
  #undef TEMP_SENSOR_BED
  #define TEMP_SENSOR_BED 70
  #define HEATER_BED_INVERTING true
#endif

/**
 * Heated Chamber settings
 */
#if TEMP_SENSOR_CHAMBER
  #define CHAMBER_MINTEMP             5
  #define CHAMBER_MAXTEMP            60
  #define TEMP_CHAMBER_HYSTERESIS     1   // (°C) Temperature proximity considered "close enough" to the target
  //#define CHAMBER_LIMIT_SWITCHING
  //#define HEATER_CHAMBER_PIN       44   // Chamber heater on/off pin
  //#define HEATER_CHAMBER_INVERTING false
#endif

#if DISABLED(PIDTEMPBED)
  #define BED_CHECK_INTERVAL 5000 // ms between checks in bang-bang control
  #if ENABLED(BED_LIMIT_SWITCHING)
    #define BED_HYSTERESIS 2 // Only disable heating if T>target+BED_HYSTERESIS and enable heating if T>target-BED_HYSTERESIS
  #endif
#endif

/**
 * Thermal Protection provides additional protection to your printer from damage
 * and fire. Marlin always includes safe min and max temperature ranges which
 * protect against a broken or disconnected thermistor wire.
 *
 * The issue: If a thermistor falls out, it will report the much lower
 * temperature of the air in the room, and the the firmware will keep
 * the heater on.
 *
 * The solution: Once the temperature reaches the target, start observing.
 * If the temperature stays too far below the target (hysteresis) for too
 * long (period), the firmware will halt the machine as a safety precaution.
 *
 * If you get false positives for "Thermal Runaway", increase
 * THERMAL_PROTECTION_HYSTERESIS and/or THERMAL_PROTECTION_PERIOD
 */
#if ENABLED(THERMAL_PROTECTION_HOTENDS)
  #define THERMAL_PROTECTION_PERIOD 40        // Seconds
  #define THERMAL_PROTECTION_HYSTERESIS 4     // Degrees Celsius

  //#define ADAPTIVE_FAN_SLOWING              // Slow part cooling fan if temperature drops
  #if BOTH(ADAPTIVE_FAN_SLOWING, PIDTEMP)
    //#define NO_FAN_SLOWING_IN_PID_TUNING    // Don't slow fan speed during M303
  #endif

  /**
   * Whenever an M104, M109, or M303 increases the target temperature, the
   * firmware will wait for the WATCH_TEMP_PERIOD to expire. If the temperature
   * hasn't increased by WATCH_TEMP_INCREASE degrees, the machine is halted and
   * requires a hard reset. This test restarts with any M104/M109/M303, but only
   * if the current temperature is far enough below the target for a reliable
   * test.
   *
   * If you get false positives for "Heating failed", increase WATCH_TEMP_PERIOD
   * and/or decrease WATCH_TEMP_INCREASE. WATCH_TEMP_INCREASE should not be set
   * below 2.
   */
  #define WATCH_TEMP_PERIOD 20                // Seconds
  #define WATCH_TEMP_INCREASE 2               // Degrees Celsius
#endif

/**
 * Thermal Protection parameters for the bed are just as above for hotends.
 */
#if ENABLED(THERMAL_PROTECTION_BED)
  #define THERMAL_PROTECTION_BED_PERIOD        20 // Seconds
  #define THERMAL_PROTECTION_BED_HYSTERESIS     2 // Degrees Celsius

  /**
   * As described above, except for the bed (M140/M190/M303).
   */
  #define WATCH_BED_TEMP_PERIOD                60 // Seconds
  #define WATCH_BED_TEMP_INCREASE               2 // Degrees Celsius
#endif

/**
 * Thermal Protection parameters for the heated chamber.
 */
#if ENABLED(THERMAL_PROTECTION_CHAMBER)
  #define THERMAL_PROTECTION_CHAMBER_PERIOD    20 // Seconds
  #define THERMAL_PROTECTION_CHAMBER_HYSTERESIS 2 // Degrees Celsius

  /**
   * Heated chamber watch settings (M141/M191).
   */
  #define WATCH_CHAMBER_TEMP_PERIOD            60 // Seconds
  #define WATCH_CHAMBER_TEMP_INCREASE           2 // Degrees Celsius
#endif

#if ENABLED(PIDTEMP)
  // Add an experimental additional term to the heater power, proportional to the extrusion speed.
  // A well-chosen Kc value should add just enough power to melt the increased material volume.
  //#define PID_EXTRUSION_SCALING
  #if ENABLED(PID_EXTRUSION_SCALING)
    #define DEFAULT_Kc (100) //heating power=Kc*(e_speed)
    #define LPQ_MAX_LEN 50
  #endif

  /**
   * Add an experimental additional term to the heater power, proportional to the fan speed.
   * A well-chosen Kf value should add just enough power to compensate for power-loss from the cooling fan.
   * You can either just add a constant compensation with the DEFAULT_Kf value
   * or follow the instruction below to get speed-dependent compensation.
   *
   * Constant compensation (use only with fanspeeds of 0% and 100%)
   * ---------------------------------------------------------------------
   * A good starting point for the Kf-value comes from the calculation:
   *   kf = (power_fan * eff_fan) / power_heater * 255
   * where eff_fan is between 0.0 and 1.0, based on fan-efficiency and airflow to the nozzle / heater.
   *
   * Example:
   *   Heater: 40W, Fan: 0.1A * 24V = 2.4W, eff_fan = 0.8
   *   Kf = (2.4W * 0.8) / 40W * 255 = 12.24
   *
   * Fan-speed dependent compensation
   * --------------------------------
   * 1. To find a good Kf value, set the hotend temperature, wait for it to settle, and enable the fan (100%).
   *    Make sure PID_FAN_SCALING_LIN_FACTOR is 0 and PID_FAN_SCALING_ALTERNATIVE_DEFINITION is not enabled.
   *    If you see the temperature drop repeat the test, increasing the Kf value slowly, until the temperature
   *    drop goes away. If the temperature overshoots after enabling the fan, the Kf value is too big.
   * 2. Note the Kf-value for fan-speed at 100%
   * 3. Determine a good value for PID_FAN_SCALING_MIN_SPEED, which is around the speed, where the fan starts moving.
   * 4. Repeat step 1. and 2. for this fan speed.
   * 5. Enable PID_FAN_SCALING_ALTERNATIVE_DEFINITION and enter the two identified Kf-values in
   *    PID_FAN_SCALING_AT_FULL_SPEED and PID_FAN_SCALING_AT_MIN_SPEED. Enter the minimum speed in PID_FAN_SCALING_MIN_SPEED
   */
  //#define PID_FAN_SCALING
  #if ENABLED(PID_FAN_SCALING)
    //#define PID_FAN_SCALING_ALTERNATIVE_DEFINITION
    #if ENABLED(PID_FAN_SCALING_ALTERNATIVE_DEFINITION)
      // The alternative definition is used for an easier configuration.
      // Just figure out Kf at fullspeed (255) and PID_FAN_SCALING_MIN_SPEED.
      // DEFAULT_Kf and PID_FAN_SCALING_LIN_FACTOR are calculated accordingly.

      #define PID_FAN_SCALING_AT_FULL_SPEED 13.0        //=PID_FAN_SCALING_LIN_FACTOR*255+DEFAULT_Kf
      #define PID_FAN_SCALING_AT_MIN_SPEED 6.0          //=PID_FAN_SCALING_LIN_FACTOR*PID_FAN_SCALING_MIN_SPEED+DEFAULT_Kf
      #define PID_FAN_SCALING_MIN_SPEED 10.0            // Minimum fan speed at which to enable PID_FAN_SCALING

      #define DEFAULT_Kf (255.0*PID_FAN_SCALING_AT_MIN_SPEED-PID_FAN_SCALING_AT_FULL_SPEED*PID_FAN_SCALING_MIN_SPEED)/(255.0-PID_FAN_SCALING_MIN_SPEED)
      #define PID_FAN_SCALING_LIN_FACTOR (PID_FAN_SCALING_AT_FULL_SPEED-DEFAULT_Kf)/255.0

    #else
      #define PID_FAN_SCALING_LIN_FACTOR (0)             // Power loss due to cooling = Kf * (fan_speed)
      #define DEFAULT_Kf 10                              // A constant value added to the PID-tuner
      #define PID_FAN_SCALING_MIN_SPEED 10               // Minimum fan speed at which to enable PID_FAN_SCALING
    #endif
  #endif
#endif

/**
 * Automatic Temperature:
 * The hotend target temperature is calculated by all the buffered lines of gcode.
 * The maximum buffered steps/sec of the extruder motor is called "se".
 * Start autotemp mode with M109 S<mintemp> B<maxtemp> F<factor>
 * The target temperature is set to mintemp+factor*se[steps/sec] and is limited by
 * mintemp and maxtemp. Turn this off by executing M109 without F*
 * Also, if the temperature is set to a value below mintemp, it will not be changed by autotemp.
 * On an Ultimaker, some initial testing worked with M109 S215 B260 F1 in the start.gcode
 */
//#define AUTOTEMP
#if ENABLED(AUTOTEMP)
  #define AUTOTEMP_OLDWEIGHT 0.98
#endif

// Extra options for the M114 "Current Position" report
//#define M114_DETAIL         // Use 'M114` for details to check planner calculations
//#define M114_REALTIME       // Real current position based on forward kinematics
//#define M114_LEGACY         // M114 used to synchronize on every call. Enable if needed.

// Show Temperature ADC value
// Enable for M105 to include ADC values read from temperature sensors.
//#define SHOW_TEMP_ADC_VALUES

/**
 * High Temperature Thermistor Support
 *
 * Thermistors able to support high temperature tend to have a hard time getting
 * good readings at room and lower temperatures. This means HEATER_X_RAW_LO_TEMP
 * will probably be caught when the heating element first turns on during the
 * preheating process, which will trigger a min_temp_error as a safety measure
 * and force stop everything.
 * To circumvent this limitation, we allow for a preheat time (during which,
 * min_temp_error won't be triggered) and add a min_temp buffer to handle
 * aberrant readings.
 *
 * If you want to enable this feature for your hotend thermistor(s)
 * uncomment and set values > 0 in the constants below
 */

// The number of consecutive low temperature errors that can occur
// before a min_temp_error is triggered. (Shouldn't be more than 10.)
//#define MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED 0

// The number of milliseconds a hotend will preheat before starting to check
// the temperature. This value should NOT be set to the time it takes the
// hot end to reach the target temperature, but the time it takes to reach
// the minimum temperature your thermistor can read. The lower the better/safer.
// This shouldn't need to be more than 30 seconds (30000)
//#define MILLISECONDS_PREHEAT_TIME 0

// @section extruder

// Extruder runout prevention.
// If the machine is idle and the temperature over MINTEMP
// then extrude some filament every couple of SECONDS.
//#define EXTRUDER_RUNOUT_PREVENT
#if ENABLED(EXTRUDER_RUNOUT_PREVENT)
  #define EXTRUDER_RUNOUT_MINTEMP 190
  #define EXTRUDER_RUNOUT_SECONDS 30
  #define EXTRUDER_RUNOUT_SPEED 1500  // (mm/m)
  #define EXTRUDER_RUNOUT_EXTRUDE 5   // (mm)
#endif

// @section temperature

// Calibration for AD595 / AD8495 sensor to adjust temperature measurements.
// The final temperature is calculated as (measuredTemp * GAIN) + OFFSET.
#define TEMP_SENSOR_AD595_OFFSET  0.0
#define TEMP_SENSOR_AD595_GAIN    1.0
#define TEMP_SENSOR_AD8495_OFFSET 0.0
#define TEMP_SENSOR_AD8495_GAIN   1.0

/**
 * Controller Fan
 * To cool down the stepper drivers and MOSFETs.
 *
 * The fan will turn on automatically whenever any stepper is enabled
 * and turn off after a set period after all steppers are turned off.
 */
//#define USE_CONTROLLER_FAN
#if ENABLED(USE_CONTROLLER_FAN)
  //#define CONTROLLER_FAN_PIN -1           // Set a custom pin for the controller fan
  #define CONTROLLERFAN_SECS 60             // Duration in seconds for the fan to run after all motors are disabled
  #define CONTROLLERFAN_SPEED 255           // 255 == full speed
  //#define CONTROLLERFAN_SPEED_Z_ONLY 127  // Reduce noise on machines that keep Z enabled
#endif

// When first starting the main fan, run it at full speed for the
// given number of milliseconds.  This gets the fan spinning reliably
// before setting a PWM value. (Does not work with software PWM for fan on Sanguinololu)
//#define FAN_KICKSTART_TIME 100

// Some coolers may require a non-zero "off" state.
//#define FAN_OFF_PWM  1

/**
 * PWM Fan Scaling
 *
 * Define the min/max speeds for PWM fans (as set with M106).
 *
 * With these options the M106 0-255 value range is scaled to a subset
 * to ensure that the fan has enough power to spin, or to run lower
 * current fans with higher current. (e.g., 5V/12V fans with 12V/24V)
 * Value 0 always turns off the fan.
 *
 * Define one or both of these to override the default 0-255 range.
 */
//#define FAN_MIN_PWM 50
//#define FAN_MAX_PWM 128

/**
 * FAST PWM FAN Settings
 *
 * Use to change the FAST FAN PWM frequency (if enabled in Configuration.h)
 * Combinations of PWM Modes, prescale values and TOP resolutions are used internally to produce a
 * frequency as close as possible to the desired frequency.
 *
 * FAST_PWM_FAN_FREQUENCY [undefined by default]
 *   Set this to your desired frequency.
 *   If left undefined this defaults to F = F_CPU/(2*255*1)
 *   i.e., F = 31.4kHz on 16MHz microcontrollers or F = 39.2kHz on 20MHz microcontrollers.
 *   These defaults are the same as with the old FAST_PWM_FAN implementation - no migration is required
 *   NOTE: Setting very low frequencies (< 10 Hz) may result in unexpected timer behavior.
 *
 * USE_OCR2A_AS_TOP [undefined by default]
 *   Boards that use TIMER2 for PWM have limitations resulting in only a few possible frequencies on TIMER2:
 *   16MHz MCUs: [62.5KHz, 31.4KHz (default), 7.8KHz, 3.92KHz, 1.95KHz, 977Hz, 488Hz, 244Hz, 60Hz, 122Hz, 30Hz]
 *   20MHz MCUs: [78.1KHz, 39.2KHz (default), 9.77KHz, 4.9KHz, 2.44KHz, 1.22KHz, 610Hz, 305Hz, 153Hz, 76Hz, 38Hz]
 *   A greater range can be achieved by enabling USE_OCR2A_AS_TOP. But note that this option blocks the use of
 *   PWM on pin OC2A. Only use this option if you don't need PWM on 0C2A. (Check your schematic.)
 *   USE_OCR2A_AS_TOP sacrifices duty cycle control resolution to achieve this broader range of frequencies.
 */
#if ENABLED(FAST_PWM_FAN)
  //#define FAST_PWM_FAN_FREQUENCY 31400
  //#define USE_OCR2A_AS_TOP
#endif

// @section extruder

/**
 * Extruder cooling fans
 *
 * Extruder auto fans automatically turn on when their extruders'
 * temperatures go above EXTRUDER_AUTO_FAN_TEMPERATURE.
 *
 * Your board's pins file specifies the recommended pins. Override those here
 * or set to -1 to disable completely.
 *
 * Multiple extruders can be assigned to the same pin in which case
 * the fan will turn on when any selected extruder is above the threshold.
 */
#define E0_AUTO_FAN_PIN   AFAN_PIN
#define E1_AUTO_FAN_PIN -1
#define E2_AUTO_FAN_PIN -1
#define E3_AUTO_FAN_PIN -1
#define E4_AUTO_FAN_PIN -1
#define E5_AUTO_FAN_PIN -1
#define CHAMBER_AUTO_FAN_PIN -1

#define EXTRUDER_AUTO_FAN_TEMPERATURE 50
#define EXTRUDER_AUTO_FAN_SPEED 255   // 255 == full speed
#define CHAMBER_AUTO_FAN_TEMPERATURE 30
#define CHAMBER_AUTO_FAN_SPEED 255

/**
 * Part-Cooling Fan Multiplexer
 *
 * This feature allows you to digitally multiplex the fan output.
 * The multiplexer is automatically switched at tool-change.
 * Set FANMUX[012]_PINs below for up to 2, 4, or 8 multiplexed fans.
 */
#define FANMUX0_PIN -1
#define FANMUX1_PIN -1
#define FANMUX2_PIN -1

/**
 * M355 Case Light on-off / brightness
 */
//#define CASE_LIGHT_ENABLE
#if ENABLED(CASE_LIGHT_ENABLE)
  //#define CASE_LIGHT_PIN 4                  // Override the default pin if needed
  #define INVERT_CASE_LIGHT false             // Set true if Case Light is ON when pin is LOW
  #define CASE_LIGHT_DEFAULT_ON true          // Set default power-up state on
  #define CASE_LIGHT_DEFAULT_BRIGHTNESS 105   // Set default power-up brightness (0-255, requires PWM pin)
  //#define CASE_LIGHT_MAX_PWM 128            // Limit pwm
  //#define CASE_LIGHT_MENU                   // Add Case Light options to the LCD menu
  //#define CASE_LIGHT_NO_BRIGHTNESS          // Disable brightness control. Enable for non-PWM lighting.
  //#define CASE_LIGHT_USE_NEOPIXEL           // Use Neopixel LED as case light, requires NEOPIXEL_LED.
  #if ENABLED(CASE_LIGHT_USE_NEOPIXEL)
    #define CASE_LIGHT_NEOPIXEL_COLOR { 255, 255, 255, 255 } // { Red, Green, Blue, White }
  #endif
#endif

// @section homing

// If you want endstops to stay on (by default) even when not homing
// enable this option. Override at any time with M120, M121.
//#define ENDSTOPS_ALWAYS_ON_DEFAULT

// @section extras

//#define Z_LATE_ENABLE // Enable Z the last moment. Needed if your Z driver overheats.

// Employ an external closed loop controller. Override pins here if needed.
//#define EXTERNAL_CLOSED_LOOP_CONTROLLER
#if ENABLED(EXTERNAL_CLOSED_LOOP_CONTROLLER)
  //#define CLOSED_LOOP_ENABLE_PIN        -1
  //#define CLOSED_LOOP_MOVE_COMPLETE_PIN -1
#endif

/**
 * Dual Steppers / Dual Endstops
 *
 * This section will allow you to use extra E drivers to drive a second motor for X, Y, or Z axes.
 *
 * For example, set X_DUAL_STEPPER_DRIVERS setting to use a second motor. If the motors need to
 * spin in opposite directions set INVERT_X2_VS_X_DIR. If the second motor needs its own endstop
 * set X_DUAL_ENDSTOPS. This can adjust for "racking." Use X2_USE_ENDSTOP to set the endstop plug
 * that should be used for the second endstop. Extra endstops will appear in the output of 'M119'.
 *
 * Use X_DUAL_ENDSTOP_ADJUSTMENT to adjust for mechanical imperfection. After homing both motors
 * this offset is applied to the X2 motor. To find the offset home the X axis, and measure the error
 * in X2. Dual endstop offsets can be set at runtime with 'M666 X<offset> Y<offset> Z<offset>'.
 */

//#define X_DUAL_STEPPER_DRIVERS
#if ENABLED(X_DUAL_STEPPER_DRIVERS)
  #define INVERT_X2_VS_X_DIR true   // Set 'true' if X motors should rotate in opposite directions
  //#define X_DUAL_ENDSTOPS
  #if ENABLED(X_DUAL_ENDSTOPS)
    #define X2_USE_ENDSTOP _XMAX_
    #define X2_ENDSTOP_ADJUSTMENT  0
  #endif
#endif

//#define Y_DUAL_STEPPER_DRIVERS
#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
  #define INVERT_Y2_VS_Y_DIR true   // Set 'true' if Y motors should rotate in opposite directions
  //#define Y_DUAL_ENDSTOPS
  #if ENABLED(Y_DUAL_ENDSTOPS)
    #define Y2_USE_ENDSTOP _YMAX_
    #define Y2_ENDSTOP_ADJUSTMENT  0
  #endif
#endif

//
// For Z set the number of stepper drivers
//
#define NUM_Z_STEPPER_DRIVERS 1   // (1-4) Z options change based on how many

#if NUM_Z_STEPPER_DRIVERS > 1
  //#define Z_MULTI_ENDSTOPS
  #if ENABLED(Z_MULTI_ENDSTOPS)
    #define Z2_USE_ENDSTOP          _XMAX_
    #define Z2_ENDSTOP_ADJUSTMENT   0
    #if NUM_Z_STEPPER_DRIVERS >= 3
      #define Z3_USE_ENDSTOP        _YMAX_
      #define Z3_ENDSTOP_ADJUSTMENT 0
    #endif
    #if NUM_Z_STEPPER_DRIVERS >= 4
      #define Z4_USE_ENDSTOP        _ZMAX_
      #define Z4_ENDSTOP_ADJUSTMENT 0
    #endif
  #endif
#endif

/**
 * Dual X Carriage
 *
 * This setup has two X carriages that can move independently, each with its own hotend.
 * The carriages can be used to print an object with two colors or materials, or in
 * "duplication mode" it can print two identical or X-mirrored objects simultaneously.
 * The inactive carriage is parked automatically to prevent oozing.
 * X1 is the left carriage, X2 the right. They park and home at opposite ends of the X axis.
 * By default the X2 stepper is assigned to the first unused E plug on the board.
 *
 * The following Dual X Carriage modes can be selected with M605 S<mode>:
 *
 *   0 : (FULL_CONTROL) The slicer has full control over both X-carriages and can achieve optimal travel
 *       results as long as it supports dual X-carriages. (M605 S0)
 *
 *   1 : (AUTO_PARK) The firmware automatically parks and unparks the X-carriages on tool-change so
 *       that additional slicer support is not required. (M605 S1)
 *
 *   2 : (DUPLICATION) The firmware moves the second X-carriage and extruder in synchronization with
 *       the first X-carriage and extruder, to print 2 copies of the same object at the same time.
 *       Set the constant X-offset and temperature differential with M605 S2 X[offs] R[deg] and
 *       follow with M605 S2 to initiate duplicated movement.
 *
 *   3 : (MIRRORED) Formbot/Vivedino-inspired mirrored mode in which the second extruder duplicates
 *       the movement of the first except the second extruder is reversed in the X axis.
 *       Set the initial X offset and temperature differential with M605 S2 X[offs] R[deg] and
 *       follow with M605 S3 to initiate mirrored movement.
 */
//#define DUAL_X_CARRIAGE
#if ENABLED(DUAL_X_CARRIAGE)
  #define X1_MIN_POS X_MIN_POS   // Set to X_MIN_POS
  #define X1_MAX_POS X_BED_SIZE  // Set a maximum so the first X-carriage can't hit the parked second X-carriage
  #define X2_MIN_POS    80       // Set a minimum to ensure the  second X-carriage can't hit the parked first X-carriage
  #define X2_MAX_POS   353       // Set this to the distance between toolheads when both heads are homed
  #define X2_HOME_DIR    1       // Set to 1. The second X-carriage always homes to the maximum endstop position
  #define X2_HOME_POS X2_MAX_POS // Default X2 home position. Set to X2_MAX_POS.
                      // However: In this mode the HOTEND_OFFSET_X value for the second extruder provides a software
                      // override for X2_HOME_POS. This also allow recalibration of the distance between the two endstops
                      // without modifying the firmware (through the "M218 T1 X???" command).
                      // Remember: you should set the second extruder x-offset to 0 in your slicer.

  // This is the default power-up mode which can be later using M605.
  #define DEFAULT_DUAL_X_CARRIAGE_MODE DXC_AUTO_PARK_MODE

  // Default x offset in duplication mode (typically set to half print bed width)
  #define DEFAULT_DUPLICATION_X_OFFSET 100

#endif // DUAL_X_CARRIAGE

// Activate a solenoid on the active extruder with M380. Disable all with M381.
// Define SOL0_PIN, SOL1_PIN, etc., for each extruder that has a solenoid.
//#define EXT_SOLENOID

// @section homing

// Homing hits each endstop, retracts by these distances, then does a slower bump.
#define X_HOME_BUMP_MM 5
#define Y_HOME_BUMP_MM 5
#define Z_HOME_BUMP_MM 5 // deltas need the same for all three axes
#define HOMING_BUMP_DIVISOR { 4, 4, 4 }  // Re-Bump Speed Divisor (Divides the Homing Feedrate)
//#define QUICK_HOME                     // If homing includes X and Y, do a diagonal move initially
//#define HOMING_BACKOFF_MM { 2, 2, 2 }  // (mm) Move away from the endstops after homing

// When G28 is called, this option will make Y home before X
//#define HOME_Y_BEFORE_X

// Enable this if X or Y can't home without homing the other axis first.
//#define CODEPENDENT_XY_HOMING

/**
 * Z Steppers Auto-Alignment
 * Add the G34 command to align multiple Z steppers using a bed probe.
 */
//#define Z_STEPPER_AUTO_ALIGN
#if ENABLED(Z_STEPPER_AUTO_ALIGN)
  // Define probe X and Y positions for Z1, Z2 [, Z3 [, Z4]]
  // If not defined, probe limits will be used.
  // Override with 'M422 S<index> X<pos> Y<pos>'
  //#define Z_STEPPER_ALIGN_XY { {  10, 190 }, { 100,  10 }, { 190, 190 } }

  /**
   * Orientation for the automatically-calculated probe positions.
   * Override Z stepper align points with 'M422 S<index> X<pos> Y<pos>'
   *
   * 2 Steppers:  (0)     (1)
   *               |       |   2   |
   *               | 1   2 |       |
   *               |       |   1   |
   *
   * 3 Steppers:  (0)     (1)     (2)     (3)
   *               |   3   | 1     | 2   1 |     2 |
   *               |       |     3 |       | 3     |
   *               | 1   2 | 2     |   3   |     1 |
   *
   * 4 Steppers:  (0)     (1)     (2)     (3)
   *               | 4   3 | 1   4 | 2   1 | 3   2 |
   *               |       |       |       |       |
   *               | 1   2 | 2   3 | 3   4 | 4   1 |
   *
   */
  #ifndef Z_STEPPER_ALIGN_XY
    //#define Z_STEPPERS_ORIENTATION 0
  #endif

  // Provide Z stepper positions for more rapid convergence in bed alignment.
  // Requires triple stepper drivers (i.e., set NUM_Z_STEPPER_DRIVERS to 3)
  //#define Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS
  #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
    // Define Stepper XY positions for Z1, Z2, Z3 corresponding to
    // the Z screw positions in the bed carriage.
    // Define one position per Z stepper in stepper driver order.
    #define Z_STEPPER_ALIGN_STEPPER_XY { { 210.7, 102.5 }, { 152.6, 220.0 }, { 94.5, 102.5 } }
  #else
    // Amplification factor. Used to scale the correction step up or down in case
    // the stepper (spindle) position is farther out than the test point.
    #define Z_STEPPER_ALIGN_AMP 1.0       // Use a value > 1.0 NOTE: This may cause instability!
  #endif

  // On a 300mm bed a 5% grade would give a misalignment of ~1.5cm
  #define G34_MAX_GRADE              5    // (%) Maximum incline that G34 will handle
  #define Z_STEPPER_ALIGN_ITERATIONS 5    // Number of iterations to apply during alignment
  #define Z_STEPPER_ALIGN_ACC        0.02 // Stop iterating early if the accuracy is better than this
  #define RESTORE_LEVELING_AFTER_G34      // Restore leveling after G34 is done?
  // After G34, re-home Z (G28 Z) or just calculate it from the last probe heights?
  // Re-homing might be more precise in reproducing the actual 'G28 Z' homing height, especially on an uneven bed.
  #define HOME_AFTER_G34
#endif

// @section motion

#define AXIS_RELATIVE_MODES { false, false, false, false }

// Add a Duplicate option for well-separated conjoined nozzles
//#define MULTI_NOZZLE_DUPLICATION

// By default pololu step drivers require an active high signal. However, some high power drivers require an active low signal as step.
#define INVERT_X_STEP_PIN false
#define INVERT_Y_STEP_PIN false
#define INVERT_Z_STEP_PIN false
#define INVERT_E_STEP_PIN false

// Default stepper release if idle. Set to 0 to deactivate.
// Steppers will shut down DEFAULT_STEPPER_DEACTIVE_TIME seconds after the last move when DISABLE_INACTIVE_? is true.
// Time can be set by M18 and M84.
#define DEFAULT_STEPPER_DEACTIVE_TIME 120
#define DISABLE_INACTIVE_X true
#define DISABLE_INACTIVE_Y true
#define DISABLE_INACTIVE_Z true  // Set to false if the nozzle will fall down on your printed part when print has finished.
#define DISABLE_INACTIVE_E true

#define DEFAULT_MINIMUMFEEDRATE       0.0     // minimum feedrate
#define DEFAULT_MINTRAVELFEEDRATE     0.0

//#define HOME_AFTER_DEACTIVATE  // Require rehoming after steppers are deactivated

// Minimum time that a segment needs to take if the buffer is emptied
#define DEFAULT_MINSEGMENTTIME        20000   // (ms)

// Slow down the machine if the look ahead buffer is (by default) half full.
// Increase the slowdown divisor for larger buffer sizes.
#define SLOWDOWN
#if ENABLED(SLOWDOWN)
  #define SLOWDOWN_DIVISOR 2
#endif

// Frequency limit
// See nophead's blog for more info
// Not working O
//#define XY_FREQUENCY_LIMIT  15

// Minimum planner junction speed. Sets the default minimum speed the planner plans for at the end
// of the buffer and all stops. This should not be much greater than zero and should only be changed
// if unwanted behavior is observed on a user's machine when running at very slow speeds.
#define MINIMUM_PLANNER_SPEED 0.05 // (mm/s)

//
// Backlash Compensation
// Adds extra movement to axes on direction-changes to account for backlash.
//
//#define BACKLASH_COMPENSATION
#if ENABLED(BACKLASH_COMPENSATION)
  // Define values for backlash distance and correction.
  // If BACKLASH_GCODE is enabled these values are the defaults.
  #define BACKLASH_DISTANCE_MM { 0, 0, 0 } // (mm)
  #define BACKLASH_CORRECTION    0.0       // 0.0 = no correction; 1.0 = full correction

  // Set BACKLASH_SMOOTHING_MM to spread backlash correction over multiple segments
  // to reduce print artifacts. (Enabling this is costly in memory and computation!)
  //#define BACKLASH_SMOOTHING_MM 3 // (mm)

  // Add runtime configuration and tuning of backlash values (M425)
  //#define BACKLASH_GCODE

  #if ENABLED(BACKLASH_GCODE)
    // Measure the Z backlash when probing (G29) and set with "M425 Z"
    #define MEASURE_BACKLASH_WHEN_PROBING

    #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
      // When measuring, the probe will move up to BACKLASH_MEASUREMENT_LIMIT
      // mm away from point of contact in BACKLASH_MEASUREMENT_RESOLUTION
      // increments while checking for the contact to be broken.
      #define BACKLASH_MEASUREMENT_LIMIT       0.5   // (mm)
      #define BACKLASH_MEASUREMENT_RESOLUTION  0.005 // (mm)
      #define BACKLASH_MEASUREMENT_FEEDRATE    Z_PROBE_SPEED_SLOW // (mm/m)
    #endif
  #endif
#endif

/**
 * Automatic backlash, position and hotend offset calibration
 *
 * Enable G425 to run automatic calibration using an electrically-
 * conductive cube, bolt, or washer mounted on the bed.
 *
 * G425 uses the probe to touch the top and sides of the calibration object
 * on the bed and measures and/or correct positional offsets, axis backlash
 * and hotend offsets.
 *
 * Note: HOTEND_OFFSET and CALIBRATION_OBJECT_CENTER must be set to within
 *       ±5mm of true values for G425 to succeed.
 */
//#define CALIBRATION_GCODE
#if ENABLED(CALIBRATION_GCODE)

  #define CALIBRATION_MEASUREMENT_RESOLUTION     0.01 // mm

  #define CALIBRATION_FEEDRATE_SLOW             60    // mm/m
  #define CALIBRATION_FEEDRATE_FAST           1200    // mm/m
  #define CALIBRATION_FEEDRATE_TRAVEL         3000    // mm/m

  // The following parameters refer to the conical section of the nozzle tip.
  #define CALIBRATION_NOZZLE_TIP_HEIGHT          1.0  // mm
  #define CALIBRATION_NOZZLE_OUTER_DIAMETER      2.0  // mm

  // Uncomment to enable reporting (required for "G425 V", but consumes PROGMEM).
  //#define CALIBRATION_REPORTING

  // The true location and dimension the cube/bolt/washer on the bed.
  #define CALIBRATION_OBJECT_CENTER     { 264.0, -22.0,  -2.0 } // mm
  #define CALIBRATION_OBJECT_DIMENSIONS {  10.0,  10.0,  10.0 } // mm

  // Comment out any sides which are unreachable by the probe. For best
  // auto-calibration results, all sides must be reachable.
  #define CALIBRATION_MEASURE_RIGHT
  #define CALIBRATION_MEASURE_FRONT
  #define CALIBRATION_MEASURE_LEFT
  #define CALIBRATION_MEASURE_BACK

  // Probing at the exact top center only works if the center is flat. If
  // probing on a screwhead or hollow washer, probe near the edges.
  //#define CALIBRATION_MEASURE_AT_TOP_EDGES

  // Define the pin to read during calibration
  #ifndef CALIBRATION_PIN
    //#define CALIBRATION_PIN -1            // Define here to override the default pin
    #define CALIBRATION_PIN_INVERTING false // Set to true to invert the custom pin
    //#define CALIBRATION_PIN_PULLDOWN
    #define CALIBRATION_PIN_PULLUP
  #endif
#endif

/**
 * Adaptive Step Smoothing increases the resolution of multi-axis moves, particularly at step frequencies
 * below 1kHz (for AVR) or 10kHz (for ARM), where aliasing between axes in multi-axis moves causes audible
 * vibration and surface artifacts. The algorithm adapts to provide the best possible step smoothing at the
 * lowest stepping frequencies.
 */
//#define ADAPTIVE_STEP_SMOOTHING

/**
 * Custom Microstepping
 * Override as-needed for your setup. Up to 3 MS pins are supported.
 */
//#define MICROSTEP1 LOW,LOW,LOW
//#define MICROSTEP2 HIGH,LOW,LOW
//#define MICROSTEP4 LOW,HIGH,LOW
//#define MICROSTEP8 HIGH,HIGH,LOW
//#define MICROSTEP16 LOW,LOW,HIGH
//#define MICROSTEP32 HIGH,LOW,HIGH

// Microstep setting (Only functional when stepper driver microstep pins are connected to MCU.
#define MICROSTEP_MODES { 16, 16, 16, 16, 16, 16 } // [1,2,4,8,16]

/**
 *  @section  stepper motor current
 *
 *  Some boards have a means of setting the stepper motor current via firmware.
 *
 *  The power on motor currents are set by:
 *    PWM_MOTOR_CURRENT - used by MINIRAMBO & ULTIMAIN_2
 *                         known compatible chips: A4982
 *    DIGIPOT_MOTOR_CURRENT - used by BQ_ZUM_MEGA_3D, RAMBO & SCOOVO_X9H
 *                         known compatible chips: AD5206
 *    DAC_MOTOR_CURRENT_DEFAULT - used by PRINTRBOARD_REVF & RIGIDBOARD_V2
 *                         known compatible chips: MCP4728
 *    DIGIPOT_I2C_MOTOR_CURRENTS - used by 5DPRINT, AZTEEG_X3_PRO, AZTEEG_X5_MINI_WIFI, MIGHTYBOARD_REVE
 *                         known compatible chips: MCP4451, MCP4018
 *
 *  Motor currents can also be set by M907 - M910 and by the LCD.
 *    M907 - applies to all.
 *    M908 - BQ_ZUM_MEGA_3D, RAMBO, PRINTRBOARD_REVF, RIGIDBOARD_V2 & SCOOVO_X9H
 *    M909, M910 & LCD - only PRINTRBOARD_REVF & RIGIDBOARD_V2
 */
//#define PWM_MOTOR_CURRENT { 1300, 1300, 1250 }          // Values in milliamps
//#define DIGIPOT_MOTOR_CURRENT { 135,135,135,135,135 }   // Values 0-255 (RAMBO 135 = ~0.75A, 185 = ~1A)
//#define DAC_MOTOR_CURRENT_DEFAULT { 70, 80, 90, 80 }    // Default drive percent - X, Y, Z, E axis

// Use an I2C based DIGIPOT (e.g., Azteeg X3 Pro)
//#define DIGIPOT_I2C
#if ENABLED(DIGIPOT_I2C) && !defined(DIGIPOT_I2C_ADDRESS_A)
  /**
   * Common slave addresses:
   *
   *                        A   (A shifted)   B   (B shifted)  IC
   * Smoothie              0x2C (0x58)       0x2D (0x5A)       MCP4451
   * AZTEEG_X3_PRO         0x2C (0x58)       0x2E (0x5C)       MCP4451
   * AZTEEG_X5_MINI        0x2C (0x58)       0x2E (0x5C)       MCP4451
   * AZTEEG_X5_MINI_WIFI         0x58              0x5C        MCP4451
   * MIGHTYBOARD_REVE      0x2F (0x5E)                         MCP4018
   */
  #define DIGIPOT_I2C_ADDRESS_A 0x2C  // unshifted slave address for first DIGIPOT
  #define DIGIPOT_I2C_ADDRESS_B 0x2D  // unshifted slave address for second DIGIPOT
#endif

//#define DIGIPOT_MCP4018          // Requires library from https://github.com/stawel/SlowSoftI2CMaster
#define DIGIPOT_I2C_NUM_CHANNELS 8 // 5DPRINT: 4     AZTEEG_X3_PRO: 8     MKS SBASE: 5
// Actual motor currents in Amps. The number of entries must match DIGIPOT_I2C_NUM_CHANNELS.
// These correspond to the physical drivers, so be mindful if the order is changed.
#define DIGIPOT_I2C_MOTOR_CURRENTS { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }  //  AZTEEG_X3_PRO

//===========================================================================
//=============================Additional Features===========================
//===========================================================================

// @section lcd

// Change values more rapidly when the encoder is rotated faster
#define ENCODER_RATE_MULTIPLIER
#if ENABLED(ENCODER_RATE_MULTIPLIER)
  #define ENCODER_10X_STEPS_PER_SEC   30  // (steps/s) Encoder rate for 10x speed
  #define ENCODER_100X_STEPS_PER_SEC  80  // (steps/s) Encoder rate for 100x speed
#endif

// Play a beep when the feedrate is changed from the Status Screen
//#define BEEP_ON_FEEDRATE_CHANGE
#if ENABLED(BEEP_ON_FEEDRATE_CHANGE)
  #define FEEDRATE_CHANGE_BEEP_DURATION   10
  #define FEEDRATE_CHANGE_BEEP_FREQUENCY 440
#endif


// Scroll a longer status message into view
//#define STATUS_MESSAGE_SCROLLING

// On the Info Screen, display XY with one decimal place when possible
//#define LCD_DECIMAL_SMALL_XY

// The timeout (in ms) to return to the status screen from sub-menus
//#define LCD_TIMEOUT_TO_STATUS 15000

// Add an 'M73' G-code to set the current percentage
#define LCD_SET_PROGRESS_MANUALLY

// Show the E position (filament used) during printing
//#define LCD_SHOW_E_TOTAL

#if ENABLED(SHOW_BOOTSCREEN)
  #define BOOTSCREEN_TIMEOUT 4000        // (ms) Total Duration to display the boot screen(s)
#endif


#if ENABLED(SDSUPPORT)

  // The standard SD detect circuit reads LOW when media is inserted and HIGH when empty.
  // Enable this option and set to HIGH if your SD cards are incorrectly detected.
  //#define SD_DETECT_STATE HIGH

  #define SD_FINISHED_STEPPERRELEASE true          // Disable steppers when SD Print is finished
  #define SD_FINISHED_RELEASECOMMAND "M84 X Y Z E" // You might want to keep the Z enabled so your bed stays in place.

  // Reverse SD sort to show "more recent" files first, according to the card's FAT.
  // Since the FAT gets out of order with usage, SDCARD_SORT_ALPHA is recommended.
  #define SDCARD_RATHERRECENTFIRST

  #define SD_MENU_CONFIRM_START             // Confirm the selected SD file before printing

  //#define MENU_ADDAUTOSTART               // Add a menu option to run auto#.g files

  #define EVENT_GCODE_SD_STOP "G28XY"       // G-code to run on Stop Print (e.g., "G28XY" or "G27")

  /**
   * Continue after Power-Loss (Creality3D)
   *
   * Store the current state to the SD Card at the start of each layer
   * during SD printing. If the recovery file is found at boot time, present
   * an option on the LCD screen to continue the print from the last-known
   * point in the file.
   *
   * If the machine reboots when resuming a print you may need to replace or
   * reformat the SD card. (Bad sectors delay startup triggering the watchdog.)
   */
  #define POWER_LOSS_RECOVERY
  #if ENABLED(POWER_LOSS_RECOVERY)
    #define PLR_ENABLED_DEFAULT   false // Power Loss Recovery enabled by default. (Set with 'M413 Sn' & M500)
    //#define BACKUP_POWER_SUPPLY       // Backup power / UPS to move the steppers on power loss
    //#define POWER_LOSS_ZRAISE       2 // (mm) Z axis raise on resume (on power loss with UPS)
    //#define POWER_LOSS_PIN         44 // Pin to detect power loss. Set to -1 to disable default pin on boards without module.
    //#define POWER_LOSS_STATE     HIGH // State of pin indicating power loss
    //#define POWER_LOSS_PULL           // Set pullup / pulldown as appropriate
    //#define POWER_LOSS_PURGE_LEN   20 // (mm) Length of filament to purge on resume
    //#define POWER_LOSS_RETRACT_LEN 10 // (mm) Length of filament to retract on fail. Requires backup power.

    // Without a POWER_LOSS_PIN the following option helps reduce wear on the SD card,
    // especially with "vase mode" printing. Set too high and vases cannot be continued.
    #define POWER_LOSS_MIN_Z_CHANGE 0.05 // (mm) Minimum Z change before saving power-loss data
  #endif

  /**
   * Sort SD file listings in alphabetical order.
   *
   * With this option enabled, items on SD cards will be sorted
   * by name for easier navigation.
   *
   * By default...
   *
   *  - Use the slowest -but safest- method for sorting.
   *  - Folders are sorted to the top.
   *  - The sort key is statically allocated.
   *  - No added G-code (M34) support.
   *  - 40 item sorting limit. (Items after the first 40 are unsorted.)
   *
   * SD sorting uses static allocation (as set by SDSORT_LIMIT), allowing the
   * compiler to calculate the worst-case usage and throw an error if the SRAM
   * limit is exceeded.
   *
   *  - SDSORT_USES_RAM provides faster sorting via a static directory buffer.
   *  - SDSORT_USES_STACK does the same, but uses a local stack-based buffer.
   *  - SDSORT_CACHE_NAMES will retain the sorted file listing in RAM. (Expensive!)
   *  - SDSORT_DYNAMIC_RAM only uses RAM when the SD menu is visible. (Use with caution!)
   */
  //#define SDCARD_SORT_ALPHA

  // SD Card Sorting options
  #if ENABLED(SDCARD_SORT_ALPHA)
    #define SDSORT_LIMIT       40     // Maximum number of sorted items (10-256). Costs 27 bytes each.
    #define FOLDER_SORTING     -1     // -1=above  0=none  1=below
    #define SDSORT_GCODE       false  // Allow turning sorting on/off with LCD and M34 g-code.
    #define SDSORT_USES_RAM    false  // Pre-allocate a static array for faster pre-sorting.
    #define SDSORT_USES_STACK  false  // Prefer the stack for pre-sorting to give back some SRAM. (Negated by next 2 options.)
    #define SDSORT_CACHE_NAMES false  // Keep sorted items in RAM longer for speedy performance. Most expensive option.
    #define SDSORT_DYNAMIC_RAM false  // Use dynamic allocation (within SD menus). Least expensive option. Set SDSORT_LIMIT before use!
    #define SDSORT_CACHE_VFATS 2      // Maximum number of 13-byte VFAT entries to use for sorting.
                                      // Note: Only affects SCROLL_LONG_FILENAMES with SDSORT_CACHE_NAMES but not SDSORT_DYNAMIC_RAM.
  #endif

  // This allows hosts to request long names for files and folders with M33
  #define LONG_FILENAME_HOST_SUPPORT

  // Enable this option to scroll long filenames in the SD card menu
  //#define SCROLL_LONG_FILENAMES

  // Leave the heaters on after Stop Print (not recommended!)
  //#define SD_ABORT_NO_COOLDOWN

  /**
   * This option allows you to abort SD printing when any endstop is triggered.
   * This feature must be enabled with "M540 S1" or from the LCD menu.
   * To have any effect, endstops must be enabled during SD printing.
   */
  //#define SD_ABORT_ON_ENDSTOP_HIT

  /**
   * This option makes it easier to print the same SD Card file again.
   * On print completion the LCD Menu will open with the file selected.
   * You can just click to start the print, or navigate elsewhere.
   */
  //#define SD_REPRINT_LAST_SELECTED_FILE

  /**
   * Auto-report SdCard status with M27 S<seconds>
   */
  //#define AUTO_REPORT_SD_STATUS

  /**
   * Support for USB thumb drives using an Arduino USB Host Shield or
   * equivalent MAX3421E breakout board. The USB thumb drive will appear
   * to Marlin as an SD card.
   *
   * The MAX3421E can be assigned the same pins as the SD card reader, with
   * the following pin mapping:
   *
   *    SCLK, MOSI, MISO --> SCLK, MOSI, MISO
   *    INT              --> SD_DETECT_PIN [1]
   *    SS               --> SDSS
   *
   * [1] On AVR an interrupt-capable pin is best for UHS3 compatibility.
   */
  //#define USB_FLASH_DRIVE_SUPPORT
  #if ENABLED(USB_FLASH_DRIVE_SUPPORT)
    #define USB_CS_PIN    SDSS
    #define USB_INTR_PIN  SD_DETECT_PIN

    /**
     * USB Host Shield Library
     *
     * - UHS2 uses no interrupts and has been production-tested
     *   on a LulzBot TAZ Pro with a 32-bit Archim board.
     *
     * - UHS3 is newer code with better USB compatibility. But it
     *   is less tested and is known to interfere with Servos.
     *   [1] This requires USB_INTR_PIN to be interrupt-capable.
     */
    //#define USE_UHS3_USB
  #endif

  /**
   * When using a bootloader that supports SD-Firmware-Flashing,
   * add a menu item to activate SD-FW-Update on the next reboot.
   *
   * Requires ATMEGA2560 (Arduino Mega)
   *
   * Tested with this bootloader:
   *   https://github.com/FleetProbe/MicroBridge-Arduino-ATMega2560
   */
  //#define SD_FIRMWARE_UPDATE
  #if ENABLED(SD_FIRMWARE_UPDATE)
    #define SD_FIRMWARE_UPDATE_EEPROM_ADDR    0x1FF
    #define SD_FIRMWARE_UPDATE_ACTIVE_VALUE   0xF0
    #define SD_FIRMWARE_UPDATE_INACTIVE_VALUE 0xFF
  #endif

  // Add an optimized binary file transfer mode, initiated with 'M28 B1'
  //#define BINARY_FILE_TRANSFER

  /**
   * Set this option to one of the following (or the board's defaults apply):
   *
   *           LCD - Use the SD drive in the external LCD controller.
   *       ONBOARD - Use the SD drive on the control board. (No SD_DETECT_PIN. M21 to init.)
   *  CUSTOM_CABLE - Use a custom cable to access the SD (as defined in a pins file).
   *
   * :[ 'LCD', 'ONBOARD', 'CUSTOM_CABLE' ]
   */
  //#define SDCARD_CONNECTION LCD

#endif // SDSUPPORT

/**
 * By default an onboard SD card reader may be shared as a USB mass-
 * storage device. This option hides the SD card from the host PC.
 */
//#define NO_SD_HOST_DRIVE   // Disable SD Card access over USB (for security).


//
// ADC Button Debounce
//
#if HAS_ADC_BUTTONS
  #define ADC_BUTTON_DEBOUNCE_DELAY 16  // (ms) Increase if buttons bounce or repeat too fast
#endif

// @section safety

/**
 * The watchdog hardware timer will do a reset and disable all outputs
 * if the firmware gets too overloaded to read the temperature sensors.
 *
 * If you find that watchdog reboot causes your AVR board to hang forever,
 * enable WATCHDOG_RESET_MANUAL to use a custom timer instead of WDTO.
 * NOTE: This method is less reliable as it can only catch hangups while
 * interrupts are enabled.
 */
#define USE_WATCHDOG
#if ENABLED(USE_WATCHDOG)
  //#define WATCHDOG_RESET_MANUAL
#endif

// @section lcd

/**
 * Babystepping enables movement of the axes by tiny increments without changing
 * the current position values. This feature is used primarily to adjust the Z
 * axis in the first layer of a print in real-time.
 *
 * Warning: Does not respect endstops!
 */
#define BABYSTEPPING
#if ENABLED(BABYSTEPPING)
  //#define INTEGRATED_BABYSTEPPING         // EXPERIMENTAL integration of babystepping into the Stepper ISR
  //#define BABYSTEP_WITHOUT_HOMING
  //#define BABYSTEP_XY                     // Also enable X/Y Babystepping. Not supported on DELTA!
  #define BABYSTEP_INVERT_Z false           // Change if Z babysteps should go the other way
  #define BABYSTEP_MULTIPLICATOR_Z  1       // Babysteps are very small. Increase for faster motion.
  #define BABYSTEP_MULTIPLICATOR_XY 1

  //#define DOUBLECLICK_FOR_Z_BABYSTEPPING  // Double-click on the Status Screen for Z Babystepping.
  #if ENABLED(DOUBLECLICK_FOR_Z_BABYSTEPPING)
    #define DOUBLECLICK_MAX_INTERVAL 1250   // Maximum interval between clicks, in milliseconds.
                                            // Note: Extra time may be added to mitigate controller latency.
    //#define BABYSTEP_ALWAYS_AVAILABLE     // Allow babystepping at all times (not just during movement).
    //#define MOVE_Z_WHEN_IDLE              // Jump to the move Z menu on doubleclick when printer is idle.
    #if ENABLED(MOVE_Z_WHEN_IDLE)
      #define MOVE_Z_IDLE_MULTIPLICATOR 1   // Multiply 1mm by this factor for the move step size.
    #endif
  #endif

  //#define BABYSTEP_DISPLAY_TOTAL          // Display total babysteps since last G28

  #define BABYSTEP_ZPROBE_OFFSET          // Combine M851 Z and Babystepping
  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    //#define BABYSTEP_HOTEND_Z_OFFSET      // For multiple hotends, babystep relative Z offsets
    //#define BABYSTEP_ZPROBE_GFX_OVERLAY   // Enable graphical overlay on Z-offset editor
  #endif
#endif

// @section extruder

/**
 * Linear Pressure Control v1.5
 *
 * Assumption: advance [steps] = k * (delta velocity [steps/s])
 * K=0 means advance disabled.
 *
 * NOTE: K values for LIN_ADVANCE 1.5 differ from earlier versions!
 *
 * Set K around 0.22 for 3mm PLA Direct Drive with ~6.5cm between the drive gear and heatbreak.
 * Larger K values will be needed for flexible filament and greater distances.
 * If this algorithm produces a higher speed offset than the extruder can handle (compared to E jerk)
 * print acceleration will be reduced during the affected moves to keep within the limit.
 *
 * See http://marlinfw.org/docs/features/lin_advance.html for full instructions.
 * Mention @Sebastianv650 on GitHub to alert the author of any issues.
 */
//#define LIN_ADVANCE
#if ENABLED(LIN_ADVANCE)
  //#define EXTRA_LIN_ADVANCE_K // Enable for second linear advance constants
  #define LIN_ADVANCE_K 0.22    // Unit: mm compression per 1mm/s extruder speed
  //#define LA_DEBUG            // If enabled, this will generate debug information output over USB.
#endif

// @section leveling

/**
 * Points to probe for all 3-point Leveling procedures.
 * Override if the automatically selected points are inadequate.
 */
#if EITHER(AUTO_BED_LEVELING_3POINT, AUTO_BED_LEVELING_UBL)
  #define PROBE_PT_1_X 30.31
  #define PROBE_PT_1_Y -17.5
  #define PROBE_PT_2_X -30.31
  #define PROBE_PT_2_Y -17.5
  #define PROBE_PT_3_X 0
  #define PROBE_PT_3_Y 35
#endif

/**
 * Override MIN_PROBE_EDGE for each side of the build plate
 * Useful to get probe points to exact positions on targets or
 * to allow leveling to avoid plate clamps on only specific
 * sides of the bed. With NOZZLE_AS_PROBE negative values are
 * allowed, to permit probing outside the bed.
 *
 * If you are replacing the prior *_PROBE_BED_POSITION options,
 * LEFT and FRONT values in most cases will map directly over
 * RIGHT and REAR would be the inverse such as
 * (X/Y_BED_SIZE - RIGHT/BACK_PROBE_BED_POSITION)
 *
 * This will allow all positions to match at compilation, however
 * should the probe position be modified with M851XY then the
 * probe points will follow. This prevents any change from causing
 * the probe to be unable to reach any points.
 */
#if PROBE_SELECTED && !IS_KINEMATIC
  //#define MIN_PROBE_EDGE_LEFT MIN_PROBE_EDGE
  //#define MIN_PROBE_EDGE_RIGHT MIN_PROBE_EDGE
  //#define MIN_PROBE_EDGE_FRONT MIN_PROBE_EDGE
  //#define MIN_PROBE_EDGE_BACK MIN_PROBE_EDGE
#endif

#if EITHER(MESH_BED_LEVELING, AUTO_BED_LEVELING_UBL)
  // Override the mesh area if the automatic (max) area is too large
  //#define MESH_MIN_X MESH_INSET
  //#define MESH_MIN_Y MESH_INSET
  //#define MESH_MAX_X X_BED_SIZE - (MESH_INSET)
  //#define MESH_MAX_Y Y_BED_SIZE - (MESH_INSET)
#endif

/**
 * Repeatedly attempt G29 leveling until it succeeds.
 * Stop after G29_MAX_RETRIES attempts.
 */
//#define G29_RETRY_AND_RECOVER
#if ENABLED(G29_RETRY_AND_RECOVER)
  #define G29_MAX_RETRIES 3
  #define G29_HALT_ON_FAILURE
  /**
   * Specify the GCODE commands that will be executed when leveling succeeds,
   * between attempts, and after the maximum number of retries have been tried.
   */
  #define G29_SUCCESS_COMMANDS "M117 Bed leveling done."
  #define G29_RECOVER_COMMANDS "M117 Probe failed. Rewiping.\nG28\nG12 P0 S12 T0"
  #define G29_FAILURE_COMMANDS "M117 Bed leveling failed.\nG0 Z10\nM300 P25 S880\nM300 P50 S0\nM300 P25 S880\nM300 P50 S0\nM300 P25 S880\nM300 P50 S0\nG4 S1"

#endif

/**
 * Thermal Probe Compensation
 * Probe measurements are adjusted to compensate for temperature distortion.
 * Use G76 to calibrate this feature. Use M871 to set values manually.
 * For a more detailed explanation of the process see G76_M871.cpp.
 */
#if HAS_BED_PROBE && TEMP_SENSOR_PROBE && TEMP_SENSOR_BED
  // Enable thermal first layer compensation using bed and probe temperatures
  #define PROBE_TEMP_COMPENSATION

  // Add additional compensation depending on hotend temperature
  // Note: this values cannot be calibrated and have to be set manually
  #if ENABLED(PROBE_TEMP_COMPENSATION)
    // Max temperature that can be reached by heated bed.
    // This is required only for the calibration process.
    #define PTC_MAX_BED_TEMP BED_MAXTEMP

    // Park position to wait for probe cooldown
    #define PTC_PARK_POS_X 0.0F
    #define PTC_PARK_POS_Y 0.0F
    #define PTC_PARK_POS_Z 100.0F

    // Probe position to probe and wait for probe to reach target temperature
    #define PTC_PROBE_POS_X  90.0F
    #define PTC_PROBE_POS_Y 100.0F

    // Enable additional compensation using hotend temperature
    // Note: this values cannot be calibrated automatically but have to be set manually
    //#define USE_TEMP_EXT_COMPENSATION
  #endif
#endif

// @section extras

//
// G60/G61 Position Save and Return
//
//#define SAVED_POSITIONS 1         // Each saved position slot costs 12 bytes

//
// G2/G3 Arc Support
//
//#define ARC_SUPPORT                 // Disable this feature to save ~3226 bytes
#if ENABLED(ARC_SUPPORT)
  #define MM_PER_ARC_SEGMENT      1 // (mm) Length (or minimum length) of each arc segment
  //#define ARC_SEGMENTS_PER_R    1 // Max segment length, MM_PER = Min
  #define MIN_ARC_SEGMENTS       24 // Minimum number of segments in a complete circle
  //#define ARC_SEGMENTS_PER_SEC 50 // Use feedrate to choose segment length (with MM_PER_ARC_SEGMENT as the minimum)
  #define N_ARC_CORRECTION       25 // Number of interpolated segments between corrections
  //#define ARC_P_CIRCLES           // Enable the 'P' parameter to specify complete circles
  //#define CNC_WORKSPACE_PLANES    // Allow G2/G3 to operate in XY, ZX, or YZ planes
#endif

// Support for G5 with XYZE destination and IJPQ offsets. Requires ~2666 bytes.
//#define BEZIER_CURVE_SUPPORT

/**
 * G38 Probe Target
 *
 * This option adds G38.2 and G38.3 (probe towards target)
 * and optionally G38.4 and G38.5 (probe away from target).
 * Set MULTIPLE_PROBING for G38 to probe more than once.
 */
//#define G38_PROBE_TARGET
#if ENABLED(G38_PROBE_TARGET)
  //#define G38_PROBE_AWAY        // Include G38.4 and G38.5 to probe away from target
  #define G38_MINIMUM_MOVE 0.0275 // (mm) Minimum distance that will produce a move.
#endif

// Moves (or segments) with fewer steps than this will be joined with the next move
#define MIN_STEPS_PER_SEGMENT 6

/**
 * Minimum delay before and after setting the stepper DIR (in ns)
 *     0 : No delay (Expect at least 10µS since one Stepper ISR must transpire)
 *    20 : Minimum for TMC2xxx drivers
 *   200 : Minimum for A4988 drivers
 *   400 : Minimum for A5984 drivers
 *   500 : Minimum for LV8729 drivers (guess, no info in datasheet)
 *   650 : Minimum for DRV8825 drivers
 *  1500 : Minimum for TB6600 drivers (guess, no info in datasheet)
 * 15000 : Minimum for TB6560 drivers (guess, no info in datasheet)
 *
 * Override the default value based on the driver type set in Configuration.h.
 */
#define MINIMUM_STEPPER_POST_DIR_DELAY 200
#define MINIMUM_STEPPER_PRE_DIR_DELAY 200

/**
 * Minimum stepper driver pulse width (in µs)
 *   0 : Smallest possible width the MCU can produce, compatible with TMC2xxx drivers
 *   0 : Minimum 500ns for LV8729, adjusted in stepper.h
 *   1 : Minimum for A4988 and A5984 stepper drivers
 *   2 : Minimum for DRV8825 stepper drivers
 *   3 : Minimum for TB6600 stepper drivers
 *  30 : Minimum for TB6560 stepper drivers
 *
 * Override the default value based on the driver type set in Configuration.h.
 */
#define MINIMUM_STEPPER_PULSE 1

/**
 * Maximum stepping rate (in Hz) the stepper driver allows
 *  If undefined, defaults to 1MHz / (2 * MINIMUM_STEPPER_PULSE)
 *  5000000 : Maximum for TMC2xxx stepper drivers
 *  1000000 : Maximum for LV8729 stepper driver
 *  500000  : Maximum for A4988 stepper driver
 *  250000  : Maximum for DRV8825 stepper driver
 *  150000  : Maximum for TB6600 stepper driver
 *   15000  : Maximum for TB6560 stepper driver
 *
 * Override the default value based on the driver type set in Configuration.h.
 */
#define MAXIMUM_STEPPER_RATE 500000

// @section temperature

// Control heater 0 and heater 1 in parallel.
//#define HEATERS_PARALLEL

//===========================================================================
//================================= Buffers =================================
//===========================================================================

// @section hidden

// The number of linear motions that can be in the plan at any give time.
// THE BLOCK_BUFFER_SIZE NEEDS TO BE A POWER OF 2 (e.g. 8, 16, 32) because shifts and ors are used to do the ring-buffering.
#if ENABLED(SDSUPPORT)
  #define BLOCK_BUFFER_SIZE 16 // SD,LCD,Buttons take more memory, block buffer needs to be smaller
#else
  #define BLOCK_BUFFER_SIZE 16 // maximize block buffer
#endif

// @section serial

// The ASCII buffer for serial input
#define MAX_CMD_SIZE 96
#define BUFSIZE 10

// Transmission to Host Buffer Size
// To save 386 bytes of PROGMEM (and TX_BUFFER_SIZE+3 bytes of RAM) set to 0.
// To buffer a simple "ok" you need 4 bytes.
// For ADVANCED_OK (M105) you need 32 bytes.
// For debug-echo: 128 bytes for the optimal speed.
// Other output doesn't need to be that speedy.
// :[0, 2, 4, 8, 16, 32, 64, 128, 256]
#define TX_BUFFER_SIZE 0

// Host Receive Buffer Size
// Without XON/XOFF flow control (see SERIAL_XON_XOFF below) 32 bytes should be enough.
// To use flow control, set this buffer size to at least 1024 bytes.
// :[0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048]
//#define RX_BUFFER_SIZE 1024

#if RX_BUFFER_SIZE >= 1024
  // Enable to have the controller send XON/XOFF control characters to
  // the host to signal the RX buffer is becoming full.
  //#define SERIAL_XON_XOFF
#endif

// Add M575 G-code to change the baud rate
//#define BAUD_RATE_GCODE

#if ENABLED(SDSUPPORT)
  // Enable this option to collect and display the maximum
  // RX queue usage after transferring a file to SD.
  //#define SERIAL_STATS_MAX_RX_QUEUED

  // Enable this option to collect and display the number
  // of dropped bytes after a file transfer to SD.
  //#define SERIAL_STATS_DROPPED_RX
#endif

// Enable an emergency-command parser to intercept certain commands as they
// enter the serial receive buffer, so they cannot be blocked.
// Currently handles M108, M112, M410
// Does not work on boards using AT90USB (USBCON) processors!
//#define EMERGENCY_PARSER

// Bad Serial-connections can miss a received command by sending an 'ok'
// Therefore some clients abort after 30 seconds in a timeout.
// Some other clients start sending commands while receiving a 'wait'.
// This "wait" is only sent when the buffer is empty. 1 second is a good value here.
//#define NO_TIMEOUTS 1000 // Milliseconds

// Some clients will have this feature soon. This could make the NO_TIMEOUTS unnecessary.
//#define ADVANCED_OK

// Printrun may have trouble receiving long strings all at once.
// This option inserts short delays between lines of serial output.
#define SERIAL_OVERRUN_PROTECTION

// @section extras

/**
 * Extra Fan Speed
 * Adds a secondary fan speed for each print-cooling fan.
 *   'M106 P<fan> T3-255' : Set a secondary speed for <fan>
 *   'M106 P<fan> T2'     : Use the set secondary speed
 *   'M106 P<fan> T1'     : Restore the previous fan speed
 */
//#define EXTRA_FAN_SPEED

/**
 * Firmware-based and LCD-controlled retract
 *
 * Add G10 / G11 commands for automatic firmware-based retract / recover.
 * Use M207 and M208 to define parameters for retract / recover.
 *
 * Use M209 to enable or disable auto-retract.
 * With auto-retract enabled, all G1 E moves within the set range
 * will be converted to firmware-based retract/recover moves.
 *
 * Be sure to turn off auto-retract during filament change.
 *
 * Note that M207 / M208 / M209 settings are saved to EEPROM.
 *
 */
//#define FWRETRACT
#if ENABLED(FWRETRACT)
  #define FWRETRACT_AUTORETRACT           // Override slicer retractions
  #if ENABLED(FWRETRACT_AUTORETRACT)
    #define MIN_AUTORETRACT 0.1           // (mm) Don't convert E moves under this length
    #define MAX_AUTORETRACT 10.0          // (mm) Don't convert E moves over this length
  #endif
  #define RETRACT_LENGTH 3                // (mm) Default retract length (positive value)
  #define RETRACT_LENGTH_SWAP 13          // (mm) Default swap retract length (positive value)
  #define RETRACT_FEEDRATE 45             // (mm/s) Default feedrate for retracting
  #define RETRACT_ZRAISE 0                // (mm) Default retract Z-raise
  #define RETRACT_RECOVER_LENGTH 0        // (mm) Default additional recover length (added to retract length on recover)
  #define RETRACT_RECOVER_LENGTH_SWAP 0   // (mm) Default additional swap recover length (added to retract length on recover from toolchange)
  #define RETRACT_RECOVER_FEEDRATE 8      // (mm/s) Default feedrate for recovering from retraction
  #define RETRACT_RECOVER_FEEDRATE_SWAP 8 // (mm/s) Default feedrate for recovering from swap retraction
  #if ENABLED(MIXING_EXTRUDER)
    //#define RETRACT_SYNC_MIXING         // Retract and restore all mixing steppers simultaneously
  #endif
#endif

/**
 * Universal tool change settings.
 * Applies to all types of extruders except where explicitly noted.
 */
#if EXTRUDERS > 1
  // Z raise distance for tool-change, as needed for some extruders
  #define TOOLCHANGE_ZRAISE     2  // (mm)
  //#define TOOLCHANGE_NO_RETURN   // Never return to the previous position on tool-change
  #if ENABLED(TOOLCHANGE_NO_RETURN)
    //#define EVENT_GCODE_AFTER_TOOLCHANGE "G12X"   // G-code to run after tool-change is complete
  #endif

  // Retract and prime filament on tool-change
  //#define TOOLCHANGE_FILAMENT_SWAP
  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
    #define TOOLCHANGE_FIL_SWAP_LENGTH          12  // (mm)
    #define TOOLCHANGE_FIL_EXTRA_PRIME           2  // (mm)
    #define TOOLCHANGE_FIL_SWAP_RETRACT_SPEED 3600  // (mm/m)
    #define TOOLCHANGE_FIL_SWAP_PRIME_SPEED   3600  // (mm/m)
  #endif

  /**
   * Position to park head during tool change.
   * Doesn't apply to SWITCHING_TOOLHEAD, DUAL_X_CARRIAGE, or PARKING_EXTRUDER
   */
  //#define TOOLCHANGE_PARK
  #if ENABLED(TOOLCHANGE_PARK)
    #define TOOLCHANGE_PARK_XY    { X_MIN_POS + 10, Y_MIN_POS + 10 }
    #define TOOLCHANGE_PARK_XY_FEEDRATE 6000  // (mm/m)
  #endif
#endif

/**
 * Advanced Pause
 * Experimental feature for filament change support and for parking the nozzle when paused.
 * Adds the GCode M600 for initiating filament change.
 * If PARK_HEAD_ON_PAUSE enabled, adds the GCode M125 to pause printing and park the nozzle.
 *
 * Requires an LCD display.
 * Requires NOZZLE_PARK_FEATURE.
 * This feature is required for the default FILAMENT_RUNOUT_SCRIPT.
 */
#define ADVANCED_PAUSE_FEATURE
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #define PAUSE_PARK_RETRACT_FEEDRATE         60  // (mm/s) Initial retract feedrate.
  #define PAUSE_PARK_RETRACT_LENGTH           2  // (mm) Initial retract.
                                                  // This short retract is done immediately, before parking the nozzle.
  #define FILAMENT_CHANGE_UNLOAD_FEEDRATE     10  // (mm/s) Unload filament feedrate. This can be pretty fast.
  #define FILAMENT_CHANGE_UNLOAD_ACCEL        25  // (mm/s^2) Lower acceleration may allow a faster feedrate.
  #define FILAMENT_CHANGE_UNLOAD_LENGTH      100  // (mm) The length of filament for a complete unload.
                                                  //   For Bowden, the full length of the tube and nozzle.
                                                  //   For direct drive, the full length of the nozzle.
                                                  //   Set to 0 for manual unloading.
  #define FILAMENT_CHANGE_SLOW_LOAD_FEEDRATE   6  // (mm/s) Slow move when starting load.
  #define FILAMENT_CHANGE_SLOW_LOAD_LENGTH     0  // (mm) Slow length, to allow time to insert material.
                                                  // 0 to disable start loading and skip to fast load only
  #define FILAMENT_CHANGE_FAST_LOAD_FEEDRATE   6  // (mm/s) Load filament feedrate. This can be pretty fast.
  #define FILAMENT_CHANGE_FAST_LOAD_ACCEL     25  // (mm/s^2) Lower acceleration may allow a faster feedrate.
  #define FILAMENT_CHANGE_FAST_LOAD_LENGTH     0  // (mm) Load length of filament, from extruder gear to nozzle.
                                                  //   For Bowden, the full length of the tube and nozzle.
                                                  //   For direct drive, the full length of the nozzle.
  //#define ADVANCED_PAUSE_CONTINUOUS_PURGE       // Purge continuously up to the purge length until interrupted.
  #define ADVANCED_PAUSE_PURGE_FEEDRATE        3  // (mm/s) Extrude feedrate (after loading). Should be slower than load feedrate.
  #define ADVANCED_PAUSE_PURGE_LENGTH         50  // (mm) Length to extrude after loading.
                                                  //   Set to 0 for manual extrusion.
                                                  //   Filament can be extruded repeatedly from the Filament Change menu
                                                  //   until extrusion is consistent, and to purge old filament.
  #define ADVANCED_PAUSE_RESUME_PRIME          0  // (mm) Extra distance to prime nozzle after returning from park.
  //#define ADVANCED_PAUSE_FANS_PAUSE             // Turn off print-cooling fans while the machine is paused.

                                                  // Filament Unload does a Retract, Delay, and Purge first:
  #define FILAMENT_UNLOAD_PURGE_RETRACT       13  // (mm) Unload initial retract length.
  #define FILAMENT_UNLOAD_PURGE_DELAY       5000  // (ms) Delay for the filament to cool after retract.
  #define FILAMENT_UNLOAD_PURGE_LENGTH         8  // (mm) An unretract is done, then this length is purged.
  #define FILAMENT_UNLOAD_PURGE_FEEDRATE      25  // (mm/s) feedrate to purge before unload

  #define PAUSE_PARK_NOZZLE_TIMEOUT           45  // (seconds) Time limit before the nozzle is turned off for safety.
  #define FILAMENT_CHANGE_ALERT_BEEPS         10  // Number of alert beeps to play when a response is needed.
  #define PAUSE_PARK_NO_STEPPER_TIMEOUT           // Enable for XYZ steppers to stay powered on during filament change.

  // #define PARK_HEAD_ON_PAUSE                    // Park the nozzle during pause and filament change.
  //#define HOME_BEFORE_FILAMENT_CHANGE           // Ensure homing has been completed prior to parking for filament change

  //#define FILAMENT_LOAD_UNLOAD_GCODES           // Add M701/M702 Load/Unload G-codes, plus Load/Unload in the LCD Prepare menu.
  //#define FILAMENT_UNLOAD_ALL_EXTRUDERS         // Allow M702 to unload all extruders above a minimum target temp (as set by M302)
#endif


// @section i2cbus

//
// I2C Master ID for LPC176x LCD and Digital Current control
// Does not apply to other peripherals based on the Wire library.
//
//#define I2C_MASTER_ID  1  // Set a value from 0 to 2

/**
 * TWI/I2C BUS
 *
 * This feature is an EXPERIMENTAL feature so it shall not be used on production
 * machines. Enabling this will allow you to send and receive I2C data from slave
 * devices on the bus.
 *
 * ; Example #1
 * ; This macro send the string "Marlin" to the slave device with address 0x63 (99)
 * ; It uses multiple M260 commands with one B<base 10> arg
 * M260 A99  ; Target slave address
 * M260 B77  ; M
 * M260 B97  ; a
 * M260 B114 ; r
 * M260 B108 ; l
 * M260 B105 ; i
 * M260 B110 ; n
 * M260 S1   ; Send the current buffer
 *
 * ; Example #2
 * ; Request 6 bytes from slave device with address 0x63 (99)
 * M261 A99 B5
 *
 * ; Example #3
 * ; Example serial output of a M261 request
 * echo:i2c-reply: from:99 bytes:5 data:hello
 */

//#define EXPERIMENTAL_I2CBUS
#if ENABLED(EXPERIMENTAL_I2CBUS)
  #define I2C_SLAVE_ADDRESS  0  // Set a value from 8 to 127 to act as a slave
#endif

// @section extras

/**
 * Photo G-code
 * Add the M240 G-code to take a photo.
 * The photo can be triggered by a digital pin or a physical movement.
 */
//#define PHOTO_GCODE
#if ENABLED(PHOTO_GCODE)
  // A position to move to (and raise Z) before taking the photo
  //#define PHOTO_POSITION { X_MAX_POS - 5, Y_MAX_POS, 0 }  // { xpos, ypos, zraise } (M240 X Y Z)
  //#define PHOTO_DELAY_MS   100                            // (ms) Duration to pause before moving back (M240 P)
  //#define PHOTO_RETRACT_MM   6.5                          // (mm) E retract/recover for the photo move (M240 R S)

  // Canon RC-1 or homebrew digital camera trigger
  // Data from: http://www.doc-diy.net/photo/rc-1_hacked/
  //#define PHOTOGRAPH_PIN 23

  // Canon Hack Development Kit
  // http://captain-slow.dk/2014/03/09/3d-printing-timelapses/
  //#define CHDK_PIN        4

  // Optional second move with delay to trigger the camera shutter
  //#define PHOTO_SWITCH_POSITION { X_MAX_POS, Y_MAX_POS }  // { xpos, ypos } (M240 I J)

  // Duration to hold the switch or keep CHDK_PIN high
  //#define PHOTO_SWITCH_MS   50 // (ms) (M240 D)

  /**
   * PHOTO_PULSES_US may need adjustment depending on board and camera model.
   * Pin must be running at 48.4kHz.
   * Be sure to use a PHOTOGRAPH_PIN which can rise and fall quick enough.
   * (e.g., MKS SBase temp sensor pin was too slow, so used P1.23 on J8.)
   *
   *  Example pulse data for Nikon: https://bit.ly/2FKD0Aq
   *                     IR Wiring: https://git.io/JvJf7
   */
  //#define PHOTO_PULSES_US { 2000, 27850, 400, 1580, 400, 3580, 400 }  // (µs) Durations for each 48.4kHz oscillation
  #ifdef PHOTO_PULSES_US
    #define PHOTO_PULSE_DELAY_US 13 // (µs) Approximate duration of each HIGH and LOW pulse in the oscillation
  #endif
#endif

/**
 * Spindle & Laser control
 *
 * Add the M3, M4, and M5 commands to turn the spindle/laser on and off, and
 * to set spindle speed, spindle direction, and laser power.
 *
 * SuperPid is a router/spindle speed controller used in the CNC milling community.
 * Marlin can be used to turn the spindle on and off. It can also be used to set
 * the spindle speed from 5,000 to 30,000 RPM.
 *
 * You'll need to select a pin for the ON/OFF function and optionally choose a 0-5V
 * hardware PWM pin for the speed control and a pin for the rotation direction.
 *
 * See http://marlinfw.org/docs/configuration/laser_spindle.html for more config details.
 */
//#define SPINDLE_FEATURE
//#define LASER_FEATURE
#if EITHER(SPINDLE_FEATURE, LASER_FEATURE)
  #define SPINDLE_LASER_ACTIVE_HIGH     false  // Set to "true" if the on/off function is active HIGH
  #define SPINDLE_LASER_PWM             true   // Set to "true" if your controller supports setting the speed/power
  #define SPINDLE_LASER_PWM_INVERT      true   // Set to "true" if the speed/power goes up when you want it to go slower
  #define SPINDLE_LASER_POWERUP_DELAY   5000   // (ms) Delay to allow the spindle/laser to come up to speed/power
  #define SPINDLE_LASER_POWERDOWN_DELAY 5000   // (ms) Delay to allow the spindle to stop

  #if ENABLED(SPINDLE_FEATURE)
    //#define SPINDLE_CHANGE_DIR               // Enable if your spindle controller can change spindle direction
    #define SPINDLE_CHANGE_DIR_STOP            // Enable if the spindle should stop before changing spin direction
    #define SPINDLE_INVERT_DIR          false  // Set to "true" if the spin direction is reversed

    /**
     *  The M3 & M4 commands use the following equation to convert PWM duty cycle to speed/power
     *
     *  SPEED/POWER = PWM duty cycle * SPEED_POWER_SLOPE + SPEED_POWER_INTERCEPT
     *    where PWM duty cycle varies from 0 to 255
     *
     *  set the following for your controller (ALL MUST BE SET)
     */
    #define SPEED_POWER_SLOPE    118.4
    #define SPEED_POWER_INTERCEPT  0
    #define SPEED_POWER_MIN     5000
    #define SPEED_POWER_MAX    30000    // SuperPID router controller 0 - 30,000 RPM
  #else
    #define SPEED_POWER_SLOPE      0.3922
    #define SPEED_POWER_INTERCEPT  0
    #define SPEED_POWER_MIN       10
    #define SPEED_POWER_MAX      100    // 0-100%
  #endif
#endif

/**
 * Coolant Control
 *
 * Add the M7, M8, and M9 commands to turn mist or flood coolant on and off.
 *
 * Note: COOLANT_MIST_PIN and/or COOLANT_FLOOD_PIN must also be defined.
 */
//#define COOLANT_CONTROL
#if ENABLED(COOLANT_CONTROL)
  #define COOLANT_MIST                // Enable if mist coolant is present
  #define COOLANT_FLOOD               // Enable if flood coolant is present
  #define COOLANT_MIST_INVERT  false  // Set "true" if the on/off function is reversed
  #define COOLANT_FLOOD_INVERT false  // Set "true" if the on/off function is reversed
#endif

/**
 * Filament Width Sensor
 *
 * Measures the filament width in real-time and adjusts
 * flow rate to compensate for any irregularities.
 *
 * Also allows the measured filament diameter to set the
 * extrusion rate, so the slicer only has to specify the
 * volume.
 *
 * Only a single extruder is supported at this time.
 *
 *  34 RAMPS_14    : Analog input 5 on the AUX2 connector
 *  81 PRINTRBOARD : Analog input 2 on the Exp1 connector (version B,C,D,E)
 * 301 RAMBO       : Analog input 3
 *
 * Note: May require analog pins to be defined for other boards.
 */
//#define FILAMENT_WIDTH_SENSOR

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  #define FILAMENT_SENSOR_EXTRUDER_NUM 0    // Index of the extruder that has the filament sensor. :[0,1,2,3,4]
  #define MEASUREMENT_DELAY_CM        14    // (cm) The distance from the filament sensor to the melting chamber

  #define FILWIDTH_ERROR_MARGIN        1.0  // (mm) If a measurement differs too much from nominal width ignore it
  #define MAX_MEASUREMENT_DELAY       20    // (bytes) Buffer size for stored measurements (1 byte per cm). Must be larger than MEASUREMENT_DELAY_CM.

  #define DEFAULT_MEASURED_FILAMENT_DIA DEFAULT_NOMINAL_FILAMENT_DIA // Set measured to nominal initially

  // Display filament width on the LCD status line. Status messages will expire after 5 seconds.
  //#define FILAMENT_LCD_DISPLAY
#endif

/**
 * CNC Coordinate Systems
 *
 * Enables G53 and G54-G59.3 commands to select coordinate systems
 * and G92.1 to reset the workspace to native machine space.
 */
//#define CNC_COORDINATE_SYSTEMS

/**
 * Auto-report temperatures with M155 S<seconds>
 */
#define AUTO_REPORT_TEMPERATURES

/**
 * Include capabilities in M115 output
 */
//#define EXTENDED_CAPABILITIES_REPORT

/**
 * Expected Printer Check
 * Add the M16 G-code to compare a string to the MACHINE_NAME.
 * M16 with a non-matching string causes the printer to halt.
 */
//#define EXPECTED_PRINTER_CHECK

/**
 * Disable all Volumetric extrusion options
 */
//#define NO_VOLUMETRICS

#if DISABLED(NO_VOLUMETRICS)
  /**
   * Volumetric extrusion default state
   * Activate to make volumetric extrusion the default method,
   * with DEFAULT_NOMINAL_FILAMENT_DIA as the default diameter.
   *
   * M200 D0 to disable, M200 Dn to set a new diameter.
   */
  //#define VOLUMETRIC_DEFAULT_ON
#endif

/**
 * Enable this option for a leaner build of Marlin that removes all
 * workspace offsets, simplifying coordinate transformations, leveling, etc.
 *
 *  - M206 and M428 are disabled.
 *  - G92 will revert to its behavior from Marlin 1.0.
 */
//#define NO_WORKSPACE_OFFSETS

/**
 * Set the number of proportional font spaces required to fill up a typical character space.
 * This can help to better align the output of commands like `G29 O` Mesh Output.
 *
 * For clients that use a fixed-width font (like OctoPrint), leave this set to 1.0.
 * Otherwise, adjust according to your client and font.
 */
#define PROPORTIONAL_FONT_RATIO 1.0

/**
 * Spend 28 bytes of SRAM to optimize the GCode parser
 */
#define FASTER_GCODE_PARSER

#if ENABLED(FASTER_GCODE_PARSER)
  //#define GCODE_QUOTED_STRINGS  // Support for quoted string parameters
#endif

//#define GCODE_CASE_INSENSITIVE  // Accept G-code sent to the firmware in lowercase

/**
 * CNC G-code options
 * Support CNC-style G-code dialects used by laser cutters, drawing machine cams, etc.
 * Note that G0 feedrates should be used with care for 3D printing (if used at all).
 * High feedrates may cause ringing and harm print quality.
 */
//#define PAREN_COMMENTS      // Support for parentheses-delimited comments
//#define GCODE_MOTION_MODES  // Remember the motion mode (G0 G1 G2 G3 G5 G38.X) and apply for X Y Z E F, etc.

// Enable and set a (default) feedrate for all G0 moves
//#define G0_FEEDRATE 3000 // (mm/m)
#ifdef G0_FEEDRATE
  //#define VARIABLE_G0_FEEDRATE // The G0 feedrate is set by F in G0 motion mode
#endif

/**
 * Startup commands
 *
 * Execute certain G-code commands immediately after power-on.
 */
//#define STARTUP_COMMANDS "M17 Z"

/**
 * G-code Macros
 *
 * Add G-codes M810-M819 to define and run G-code macros.
 * Macros are not saved to EEPROM.
 */
//#define GCODE_MACROS
#if ENABLED(GCODE_MACROS)
  #define GCODE_MACROS_SLOTS       5  // Up to 10 may be used
  #define GCODE_MACROS_SLOT_SIZE  50  // Maximum length of a single macro
#endif

/**
 * Host Action Commands
 *
 * Define host streamer action commands in compliance with the standard.
 *
 * See https://reprap.org/wiki/G-code#Action_commands
 * Common commands ........ poweroff, pause, paused, resume, resumed, cancel
 * G29_RETRY_AND_RECOVER .. probe_rewipe, probe_failed
 *
 * Some features add reason codes to extend these commands.
 *
 * Host Prompt Support enables Marlin to use the host for user prompts so
 * filament runout and other processes can be managed from the host side.
 */
#define HOST_ACTION_COMMANDS
#if ENABLED(HOST_ACTION_COMMANDS)
  //#define HOST_PROMPT_SUPPORT
#endif

/**
 * Cancel Objects
 *
 * Implement M486 to allow Marlin to skip objects
 */
//#define CANCEL_OBJECTS

/**
 * I2C position encoders for closed loop control.
 * Developed by Chris Barr at Aus3D.
 *
 * Wiki: http://wiki.aus3d.com.au/Magnetic_Encoder
 * Github: https://github.com/Aus3D/MagneticEncoder
 *
 * Supplier: http://aus3d.com.au/magnetic-encoder-module
 * Alternative Supplier: http://reliabuild3d.com/
 *
 * Reliabuild encoders have been modified to improve reliability.
 */

//#define I2C_POSITION_ENCODERS
#if ENABLED(I2C_POSITION_ENCODERS)

  #define I2CPE_ENCODER_CNT         1                       // The number of encoders installed; max of 5
                                                            // encoders supported currently.

  #define I2CPE_ENC_1_ADDR          I2CPE_PRESET_ADDR_X     // I2C address of the encoder. 30-200.
  #define I2CPE_ENC_1_AXIS          X_AXIS                  // Axis the encoder module is installed on.  <X|Y|Z|E>_AXIS.
  #define I2CPE_ENC_1_TYPE          I2CPE_ENC_TYPE_LINEAR   // Type of encoder:  I2CPE_ENC_TYPE_LINEAR -or-
                                                            // I2CPE_ENC_TYPE_ROTARY.
  #define I2CPE_ENC_1_TICKS_UNIT    2048                    // 1024 for magnetic strips with 2mm poles; 2048 for
                                                            // 1mm poles. For linear encoders this is ticks / mm,
                                                            // for rotary encoders this is ticks / revolution.
  //#define I2CPE_ENC_1_TICKS_REV     (16 * 200)            // Only needed for rotary encoders; number of stepper
                                                            // steps per full revolution (motor steps/rev * microstepping)
  //#define I2CPE_ENC_1_INVERT                              // Invert the direction of axis travel.
  #define I2CPE_ENC_1_EC_METHOD     I2CPE_ECM_MICROSTEP     // Type of error error correction.
  #define I2CPE_ENC_1_EC_THRESH     0.10                    // Threshold size for error (in mm) above which the
                                                            // printer will attempt to correct the error; errors
                                                            // smaller than this are ignored to minimize effects of
                                                            // measurement noise / latency (filter).

  #define I2CPE_ENC_2_ADDR          I2CPE_PRESET_ADDR_Y     // Same as above, but for encoder 2.
  #define I2CPE_ENC_2_AXIS          Y_AXIS
  #define I2CPE_ENC_2_TYPE          I2CPE_ENC_TYPE_LINEAR
  #define I2CPE_ENC_2_TICKS_UNIT    2048
  //#define I2CPE_ENC_2_TICKS_REV   (16 * 200)
  //#define I2CPE_ENC_2_INVERT
  #define I2CPE_ENC_2_EC_METHOD     I2CPE_ECM_MICROSTEP
  #define I2CPE_ENC_2_EC_THRESH     0.10

  #define I2CPE_ENC_3_ADDR          I2CPE_PRESET_ADDR_Z     // Encoder 3.  Add additional configuration options
  #define I2CPE_ENC_3_AXIS          Z_AXIS                  // as above, or use defaults below.

  #define I2CPE_ENC_4_ADDR          I2CPE_PRESET_ADDR_E     // Encoder 4.
  #define I2CPE_ENC_4_AXIS          E_AXIS

  #define I2CPE_ENC_5_ADDR          34                      // Encoder 5.
  #define I2CPE_ENC_5_AXIS          E_AXIS

  // Default settings for encoders which are enabled, but without settings configured above.
  #define I2CPE_DEF_TYPE            I2CPE_ENC_TYPE_LINEAR
  #define I2CPE_DEF_ENC_TICKS_UNIT  2048
  #define I2CPE_DEF_TICKS_REV       (16 * 200)
  #define I2CPE_DEF_EC_METHOD       I2CPE_ECM_NONE
  #define I2CPE_DEF_EC_THRESH       0.1

  //#define I2CPE_ERR_THRESH_ABORT  100.0                   // Threshold size for error (in mm) error on any given
                                                            // axis after which the printer will abort. Comment out to
                                                            // disable abort behavior.

  #define I2CPE_TIME_TRUSTED        10000                   // After an encoder fault, there must be no further fault
                                                            // for this amount of time (in ms) before the encoder
                                                            // is trusted again.

  /**
   * Position is checked every time a new command is executed from the buffer but during long moves,
   * this setting determines the minimum update time between checks. A value of 100 works well with
   * error rolling average when attempting to correct only for skips and not for vibration.
   */
  #define I2CPE_MIN_UPD_TIME_MS     4                       // (ms) Minimum time between encoder checks.

  // Use a rolling average to identify persistant errors that indicate skips, as opposed to vibration and noise.
  #define I2CPE_ERR_ROLLING_AVERAGE

#endif // I2C_POSITION_ENCODERS

/**
 * Analog Joystick(s)
 */
//#define JOYSTICK
#if ENABLED(JOYSTICK)
  #define JOY_X_PIN    5  // RAMPS: Suggested pin A5  on AUX2
  #define JOY_Y_PIN   10  // RAMPS: Suggested pin A10 on AUX2
  #define JOY_Z_PIN   12  // RAMPS: Suggested pin A12 on AUX2
  #define JOY_EN_PIN  44  // RAMPS: Suggested pin D44 on AUX2

  //#define INVERT_JOY_X  // Enable if X direction is reversed
  //#define INVERT_JOY_Y  // Enable if Y direction is reversed
  //#define INVERT_JOY_Z  // Enable if Z direction is reversed

  // Use M119 with JOYSTICK_DEBUG to find reasonable values after connecting:
  #define JOY_X_LIMITS { 5600, 8190-100, 8190+100, 10800 } // min, deadzone start, deadzone end, max
  #define JOY_Y_LIMITS { 5600, 8250-100, 8250+100, 11000 }
  #define JOY_Z_LIMITS { 4800, 8080-100, 8080+100, 11550 }
#endif


/**
 * NanoDLP Sync support
 *
 * Add support for Synchronized Z moves when using with NanoDLP. G0/G1 axis moves will output "Z_move_comp"
 * string to enable synchronization with DLP projector exposure. This change will allow to use
 * [[WaitForDoneMessage]] instead of populating your gcode with M400 commands
 */
//#define NANODLP_Z_SYNC
#if ENABLED(NANODLP_Z_SYNC)
  //#define NANODLP_ALL_AXIS  // Enables "Z_move_comp" output on any axis move.
                              // Default behavior is limited to Z axis only.
#endif


/**
 * Advanced Print Counter settings
 */
#if ENABLED(PRINTCOUNTER)
  #define SERVICE_WARNING_BUZZES  3
  // Activate up to 3 service interval watchdogs
  //#define SERVICE_NAME_1      "Service S"
  //#define SERVICE_INTERVAL_1  100 // print hours
  //#define SERVICE_NAME_2      "Service L"
  //#define SERVICE_INTERVAL_2  200 // print hours
  //#define SERVICE_NAME_3      "Service 3"
  //#define SERVICE_INTERVAL_3    1 // print hours
#endif

// @section develop

//
// M100 Free Memory Watcher to debug memory usage
//
//#define M100_FREE_MEMORY_WATCHER

//
// M43 - display pin status, toggle pins, watch pins, watch endstops & toggle LED, test servo probe
//
#define PINS_DEBUGGING

// Enable Marlin dev mode which adds some special commands
//#define MARLIN_DEV_MODE
