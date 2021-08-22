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

#include "../../inc/MarlinConfig.h"

#include "../gcode.h"

#include "../../module/stepper.h"
#include "../../module/endstops.h"

#if HOTENDS > 1
  #include "../../module/tool_change.h"
#endif

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(SENSORLESS_HOMING)
  #include "../../feature/tmc_util.h"
#endif

#include "../../module/probe.h"

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

#if ENABLED(QUICK_HOME)

  static void quick_home_xy() {

    // Pretend the current position is 0,0
    current_position.set(0.0, 0.0);
    sync_plan_position();

    const int x_axis_home_dir = x_home_dir(active_extruder);

    const float mlx = max_length(X_AXIS),
                mly = max_length(Y_AXIS),
                mlratio = mlx > mly ? mly / mlx : mlx / mly,
                fr_mm_s = _MIN(homing_feedrate(X_AXIS), homing_feedrate(Y_AXIS)) * SQRT(sq(mlratio) + 1.0);

    #if ENABLED(SENSORLESS_HOMING)
      sensorless_t stealth_states {
          tmc_enable_stallguard(stepperX)
        , tmc_enable_stallguard(stepperY)
        , false
        , false
          #if AXIS_HAS_STALLGUARD(X2)
            || tmc_enable_stallguard(stepperX2)
          #endif
        , false
          #if AXIS_HAS_STALLGUARD(Y2)
            || tmc_enable_stallguard(stepperY2)
          #endif
      };
    #endif

    do_blocking_move_to_xy(1.5 * mlx * x_axis_home_dir, 1.5 * mly * home_dir(Y_AXIS), fr_mm_s);

    endstops.validate_homing_move();

    current_position.set(0.0, 0.0);

    #if ENABLED(SENSORLESS_HOMING)
      tmc_disable_stallguard(stepperX, stealth_states.x);
      tmc_disable_stallguard(stepperY, stealth_states.y);
      #if AXIS_HAS_STALLGUARD(X2)
        tmc_disable_stallguard(stepperX2, stealth_states.x2);
      #endif
      #if AXIS_HAS_STALLGUARD(Y2)
        tmc_disable_stallguard(stepperY2, stealth_states.y2);
      #endif
    #endif
  }

#endif // QUICK_HOME

/**
 * G28: Home all axes according to settings
 *
 * Parameters
 *
 *  None  Home to all axes with no parameters.
 *        With QUICK_HOME enabled XY will home together, then Z.
 *
 *  O   Home only if position is unknown
 *
 *  Rn  Raise by n mm/inches before homing
 *
 * Cartesian/SCARA parameters
 *
 *  X   Home to the X endstop
 *  Y   Home to the Y endstop
 *  Z   Home to the Z endstop
 *
 */
void GcodeSuite::G28() {
  if (DEBUGGING(LEVELING)) {
    DEBUG_ECHOLNPGM(">>> G28");
    log_machine_info();
  }

  #if ENABLED(MARLIN_DEV_MODE)
    if (parser.seen('S')) {
      LOOP_XYZ(a) set_axis_is_at_home((AxisEnum)a);
      sync_plan_position();
      SERIAL_ECHOLNPGM("Simulated Homing");
      report_current_position();
      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("<<< G28");
      return;
    }
  #endif

  // Home (O)nly if position is unknown
  if (!homing_needed() && parser.boolval('O')) {
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("> homing not needed, skip\n<<< G28");
    return;
  }

  // Wait for planner moves to finish!
  planner.synchronize();

  // Disable the leveling matrix before homing
  #if HAS_LEVELING

    // Cancel the active G29 session
    #if ENABLED(PROBE_MANUALLY)
      g29_in_progress = false;
    #endif

    #if ENABLED(RESTORE_LEVELING_AFTER_G28)
      const bool leveling_was_active = planner.leveling_active;
    #endif
    set_bed_leveling_enabled(false);
  #endif

  #define HAS_CURRENT_HOME(N) (defined(N##_CURRENT_HOME) && N##_CURRENT_HOME != N##_CURRENT)
  #define HAS_HOMING_CURRENT (HAS_CURRENT_HOME(X) || HAS_CURRENT_HOME(X2) || HAS_CURRENT_HOME(Y) || HAS_CURRENT_HOME(Y2))

  remember_feedrate_scaling_off();

  endstops.enable(true); // Enable endstops for next homing move

  constexpr bool doZ = true; // for NANODLP_Z_SYNC if your DLP is on a DELTA

  home_delta();

  #if ENABLED(IMPROVE_HOMING_RELIABILITY)
    end_slow_homing(slow_homing);
  #endif

  endstops.not_homing();

  #if ENABLED(RESTORE_LEVELING_AFTER_G28)
    set_bed_leveling_enabled(leveling_was_active);
  #endif

  restore_feedrate_and_scaling();

  report_current_position();

  if (ENABLED(NANODLP_Z_SYNC) && (doZ || ENABLED(NANODLP_ALL_AXIS)))
    SERIAL_ECHOLNPGM(STR_Z_MOVE_COMP);

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("<<< G28");
}
