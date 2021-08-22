/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../MarlinCore.h"
#include "../module/temperature.h"
#include "../sd/cardreader.h"
#include "../module/planner.h"
#include "../feature/powerloss.h"
#include "../gcode/queue.h"
#include "../gcode/gcode.h"
#include "../libs/nozzle.h"
#include "WTGL_Screen_Resume.h"
#include "WTGL_Serial.h"
#include "WTGL_Manager.h"

void WTGL_Screen_Resume::Init()
{
	holdontime = getcurrenttime();

	if (card.isMounted())
	{
		recovery.init();
		recovery.load();
		ShowRecoveryInfo();
		state = DSEP_START;
	}
	else
	{
		wtgl.ShowLog("Error: TF card not found");
		state = DSEP_ERROR;
	}
	
}

void WTGL_Screen_Resume::Update()
{
	char headgcode[MAX_CMD_SIZE + 16] = {0}, str_1[16], str_2[16], flchar[10] = {0};
    static uint32_t _spos;

	switch (state)
	{
	case DSEP_START:
	{
		_spos = recovery.info.sdpos;
		wtgl.ShowLog("Homing...");
		queue.enqueue_now_P(PSTR("M109 S150"));
		queue.enqueue_now_P(PSTR("G28"));
		queue.enqueue_now_P(PSTR("M420 S1"));
		state = DSEP_GOHOME;
	}
	break;

	case DSEP_GOHOME:
	{
		if ((planner.has_blocks_queued() == false) && (queue.length == 0))
		{
			current_position[Z_AXIS] += recovery.info.zoffset;
			queue.enqueue_now_P(PSTR("M114"));

			queue.enqueue_now_P(PSTR("G1 X0 Y-50 F3000"));

			dtostrf(recovery.info.current_position.z + 20, 1, 3, str_1);
			sprintf_P(headgcode, PSTR("G1 Z%s F500"), str_1);
			queue.enqueue_one_now(headgcode);

			state = DSEP_HEATING;
		}
	}
	break;

	case DSEP_HEATING:
	{
		if ((planner.has_blocks_queued() == false) && (queue.length == 0))
		{
			SERIAL_ECHO("heat");
			HOTEND_LOOP() 
			{
				thermalManager.setTargetHotend(recovery.info.target_temperature[e], e);
			}
			#if HAS_HEATED_BED
			thermalManager.setTargetBed(recovery.info.target_temperature_bed);
			#endif
			wtgl.ShowLog("Heating...");
			state = DSEP_FEDDING;
		}
	}
	break;

	case DSEP_FEDDING:
	{
		if ((thermalManager.degHotend(0) >= thermalManager.degTargetHotend(0) - 2) &&
			(thermalManager.degHotend(0) <= thermalManager.degTargetHotend(0) + 2) &&
			(thermalManager.degHotend(0) > 180))
		{
			wtgl.ShowLog("Feeding...");
			state = DSEP_GOHOME;

			destination = current_position;
			destination.e += 20 / planner.e_factor[active_extruder];
			planner.buffer_line(destination, 5, active_extruder);
			current_position = destination;

			state = DSEP_SETPOS;
		}
	}
	break;

	case DSEP_SETPOS:
	{
		if ((planner.has_blocks_queued() == false) && (queue.length == 0))
		{
			wtgl.ShowLog("Restoring position...");
			
			sprintf_P(headgcode, PSTR("G1 X%s Y%s F3000"),
				dtostrf(recovery.info.current_position.x, 1, 3, str_1),
				dtostrf(recovery.info.current_position.y, 1, 3, str_2)
			);
			queue.enqueue_one_now(headgcode);

			dtostrf(recovery.info.current_position.z, 1, 3, str_1);
			sprintf_P(headgcode, PSTR("G1 Z%s F500"), str_1);
			queue.enqueue_one_now(headgcode);

			sprintf_P(headgcode, PSTR("G1 F%d"), recovery.info.feedrate);
			queue.enqueue_one_now(headgcode);

			sprintf_P(headgcode, PSTR("G92.9 E%s"), dtostrf(recovery.info.current_position.e, 1, 3, str_1));
			queue.enqueue_one_now(headgcode);

			gcode.axis_relative = recovery.info.axis_relative;

			state = DSEP_OPENFILE;
		}
	}
	break;

	case DSEP_OPENFILE:
	{
		if ((planner.has_blocks_queued() == false) && (queue.length == 0))
		{
			char *fn = recovery.info.sd_filename;
			extern const char M23_STR[];
			sprintf_P(headgcode, M23_STR, fn);
			queue.enqueue_one_now(headgcode);

			sprintf_P(headgcode, PSTR("M24 S%ld T%ld"), _spos, recovery.info.print_job_elapsed);
			queue.enqueue_one_now(headgcode);

			state = DSEP_COMPLETE;
		}
	}
	break;

	case DSEP_COMPLETE:
	case DSEP_ERROR:
		break;

	default:
		break;
	}
}

void WTGL_Screen_Resume::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{

}


void WTGL_Screen_Resume::ShowRecoveryInfo()
{
    SERIAL_ECHOLNPAIR(" Job Recovery Info...\nvalid_head:", int(recovery.info.valid_head), " valid_foot:", int(recovery.info.valid_foot));
    if (recovery.info.valid_head) {
      if (recovery.info.valid_head == recovery.info.valid_foot) {
        SERIAL_ECHOPGM("current_position: ");
        LOOP_XYZE(i) {
          if (i) SERIAL_CHAR(',');
          SERIAL_ECHO(recovery.info.current_position[i]);
        }
        SERIAL_EOL();

		SERIAL_ECHOLNPAIR("zoffset: ", recovery.info.zoffset);

        #if HAS_HOME_OFFSET
          SERIAL_ECHOPGM("home_offset: ");
          LOOP_XYZ(i) {
            if (i) SERIAL_CHAR(',');
            SERIAL_ECHO(recovery.info.home_offset[i]);
          }
          SERIAL_EOL();
        #endif

        #if HAS_POSITION_SHIFT
          SERIAL_ECHOPGM("position_shift: ");
          LOOP_XYZ(i) {
            if (i) SERIAL_CHAR(',');
            SERIAL_ECHO(recovery.info.position_shift[i]);
          }
          SERIAL_EOL();
        #endif

        SERIAL_ECHOLNPAIR("feedrate: ", recovery.info.feedrate);

        #if EXTRUDERS > 1
          SERIAL_ECHOLNPAIR("active_extruder: ", int(info.active_extruder));
        #endif

        #if HOTENDS
          SERIAL_ECHOPGM("target_temperature: ");
          HOTEND_LOOP() {
            SERIAL_ECHO(recovery.info.target_temperature[e]);
            if (e < HOTENDS - 1) SERIAL_CHAR(',');
          }
          SERIAL_EOL();
        #endif

        #if HAS_HEATED_BED
          SERIAL_ECHOLNPAIR("target_temperature_bed: ", recovery.info.target_temperature_bed);
        #endif

        #if FAN_COUNT
          SERIAL_ECHOPGM("fan_speed: ");
          FANS_LOOP(i) {
            SERIAL_ECHO(int(recovery.info.fan_speed[i]));
            if (i < FAN_COUNT - 1) SERIAL_CHAR(',');
          }
          SERIAL_EOL();
        #endif

        #if HAS_LEVELING
          SERIAL_ECHOLNPAIR("leveling: ", int(recovery.info.leveling), "\n fade: ", int(recovery.info.fade));
        #endif

        SERIAL_ECHOLNPAIR("sd_filename: ", recovery.info.sd_filename);
        SERIAL_ECHOLNPAIR("sdpos: ", recovery.info.sdpos);
        SERIAL_ECHOLNPAIR("print_job_elapsed: ", recovery.info.print_job_elapsed);
      }
      else
        SERIAL_ECHOLNPGM("INVALID DATA");
    }
    SERIAL_ECHOLNPGM("---");
}

