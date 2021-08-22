/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "WTCMD.h"
#include "../gcode/parser.h"
#include "../HAL/STM32F1/watchdog.h"
#include "../sd/cardreader.h"
#include "../module/printcounter.h"
#include "../libs/duration_t.h"
#include "../module/planner.h"
#include "../module/temperature.h"
#include "../module/configuration_store.h"
#include "../MarlinCore.h"
#include "../feature/host_actions.h"
#include "../wtgl/WTGL_Manager.h"
#include "../wtgl/WTGL_Serial.h"
#include "../module/probe.h"
#include "../module/stepper.h"
#include "WTUtilty.h"
#include "../../gcode/gcode.h"
#include "../libs/nozzle.h"
#include "../feature/pause.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/powerloss.h"
#endif

#define SD_CONFIG_FILE		"config.sav"

WT_STATUS wt_machineStatus = WS_IDLE;

WT_MAIN_ACTION wt_mainloop_action = WT_MAIN_ACTION::WMA_IDLE;

uint8_t wt_onlineprinting = SPARK_LOST;		// 0 = not printing, 1 = paused, 2 = printing, 3 = lost

char parsedString[30];

static xyze_pos_t resume_position;

static void wt_pause_print();
static void wt_resume_print();

void GetMachineStatus()
{
	SERIAL_ECHOPGM("MStatus:");
	SERIAL_ECHO(wt_machineStatus);

	SERIAL_ECHOPGM(" MFile:");
	#ifdef SDSUPPORT
	card.printFilename();
	#endif

	char buffer[21];
	duration_t elapsed = print_job_timer.duration();
	elapsed.toString(buffer);

	SERIAL_ECHOPGM(" MTime:");
	SERIAL_ECHO(buffer);

	SERIAL_ECHOPGM(" MPercent:");
	#ifdef SDSUPPORT
	card.report_status();
	#endif

	SERIAL_EOL();

}

void wt_sdcard_stop()
{
	card.endFilePrint();
	print_job_timer.stop();

	#if ENABLED(POWER_LOSS_RECOVERY)
      recovery.purge();
    #endif

	wait_for_heatup = false;

	wtvar_gohome = 1;
	(void)settings.save();

	wt_restart();
}

void wt_sdcard_pause()
{
	card.pauseSDPrint();
	print_job_timer.pause();

	#if ENABLED(POWER_LOSS_RECOVERY)
      if (recovery.enabled) recovery.save(true);
    #endif

	#if ENABLED(PARK_HEAD_ON_PAUSE)
	 queue.enqueue_now_P(PSTR("M125"));
	#endif

	wt_machineStatus = WS_PAUSE;
	wt_mainloop_action = WMA_PAUSE;	
}

void wt_sdcard_resume()
{
	wt_resume_print();

	card.startFileprint();
	print_job_timer.start();

	planner.synchronize();
	
	wtgl.GotoPrintingMenu();

	wt_machineStatus = WS_PRINTING;

  	#ifdef ACTION_ON_RESUMED
    host_action_resumed();
  	#endif
}

void wt_spark_begin()
{
	wt_onlineprinting = SPARK_PRINTING;
	print_job_timer.start();

	#ifdef WTGL_LCD
	wtgl.GotoPrintingMenu();
	#endif

	wt_machineStatus = WS_PRINTING;
}

void wt_spark_end()
{
	wt_machineStatus = WS_FINISH;
	wt_onlineprinting = SPARK_IDLE;
	print_job_timer.stop();
}

void wt_spark_cancel()
{
	wt_sdcard_stop();
}

void wt_spark_pause()
{
	print_job_timer.pause();

	wt_machineStatus = WS_PAUSE;
	wt_onlineprinting = SPARK_PAUSED;

	#ifdef WTGL_LCD
	wtgl.GotoPrintingMenu();
	#endif

	wt_mainloop_action = WMA_PAUSE;	
}

void wt_spark_resume()
{
	wt_resume_print();
	print_job_timer.start();

	planner.synchronize();

	wt_machineStatus = WS_PRINTING;
	wt_onlineprinting = SPARK_PRINTING;

	#ifdef WTGL_LCD
	wtgl.GotoPrintingMenu();
	#endif
}

void wt_save_config()
{
	if (!card.openNewFileWrite((char*)SD_CONFIG_FILE))
	{
		SERIAL_ECHOLNPGM("config file open fail!");

		#ifdef WTGL_LCD
		gserial.SendByte(REG_CONFIG_SAVE_MSG, 1);
		#endif
		return;
	}

	char buffer[100];

	ZERO(buffer);
	sprintf(buffer, ";%s config data begin", MACHINE_NAME);
	if (!card.write_line(buffer))
	{
		SERIAL_ECHOLNPGM("Write to config save file fail.");
		#ifdef WTGL_LCD
		gserial.SendByte(REG_CONFIG_SAVE_MSG, 2);
		#endif
		goto END;
	}

	ZERO(buffer);
	sprintf(buffer, "M665 L%.2f R%.2f H%.2f X%.2f Y%.2f Z%.2f",
					 LINEAR_UNIT(delta_diagonal_rod),
					 LINEAR_UNIT(delta_radius),
					 LINEAR_UNIT(delta_height),
					 LINEAR_UNIT(delta_tower_angle_trim.a),
					 LINEAR_UNIT(delta_tower_angle_trim.b),
					 LINEAR_UNIT(delta_tower_angle_trim.c));
	if (!card.write_line(buffer))
	{
		SERIAL_ECHOLNPGM("Write to config save file fail.");
		#ifdef WTGL_LCD
		gserial.SendByte(REG_CONFIG_SAVE_MSG, 2);
		#endif
		goto END;
	}

	ZERO(buffer);
	sprintf(buffer, "M666 X%.2f Y%.2f Z%.2f",
					 LINEAR_UNIT(delta_endstop_adj.a),
					 LINEAR_UNIT(delta_endstop_adj.b),
					 LINEAR_UNIT(delta_endstop_adj.c));
	if (!card.write_line(buffer))
	{
		SERIAL_ECHOLNPGM("Write to config save file fail.");
		#ifdef WTGL_LCD
		gserial.SendByte(REG_CONFIG_SAVE_MSG, 2);
		#endif
		goto END;
	}

	ZERO(buffer);
	sprintf(buffer, "M851 X%.2f Y%.2f Z%.2f",
					 LINEAR_UNIT(probe.offset_xy.x),
					 LINEAR_UNIT(probe.offset_xy.y),
					 probe.offset.z);
	if (!card.write_line(buffer))
	{
		SERIAL_ECHOLNPGM("Write to config save file fail.");
		#ifdef WTGL_LCD
		gserial.SendByte(REG_CONFIG_SAVE_MSG, 2);
		#endif
		goto END;
	}

	ZERO(buffer);
	sprintf(buffer, "M907 X%ld Y%ld Z%ld E%ld",
					 stepper.motor_current_setting[0],
					 stepper.motor_current_setting[1],
					 stepper.motor_current_setting[2],
					 stepper.motor_current_setting[3]);
	if (!card.write_line(buffer))
	{
		SERIAL_ECHOLNPGM("Write to config save file fail.");
		#ifdef WTGL_LCD
		gserial.SendByte(REG_CONFIG_SAVE_MSG, 2);
		#endif
		goto END;
	}

	ZERO(buffer);
	sprintf(buffer, ";%s config data end", MACHINE_NAME);
	if (!card.write_line(buffer))
	{
		SERIAL_ECHOLNPGM("Write to config save file fail.");
		#ifdef WTGL_LCD
		gserial.SendByte(REG_CONFIG_SAVE_MSG, 2);
		#endif
		goto END;
	}

	SERIAL_ECHOLNPGM("Parameters saved successfully.");
	#ifdef WTGL_LCD
	gserial.SendByte(REG_CONFIG_SAVE_MSG, 3);
	#endif


END:
	card.closefile();
}

void wt_load_config()
{
	if (!card.openSavFileRead((char*)SD_CONFIG_FILE))
	{
		SERIAL_ECHOLNPGM("config file open fail!");
		#ifdef WTGL_LCD
		gserial.SendByte(REG_CONFIG_SAVE_MSG, 1);
		#endif
		return;
	}

	uint8_t sd_count = 0;
	bool card_eof = card.eof();
	bool sd_comment_mode = false;
	uint8_t linecount = 0;
	char buffer[100];

	ZERO(buffer);
	while (!card_eof && linecount < 10)
	{
		const int16_t n = card.get();
		char sd_char = (char)n;
		card_eof = card.eof();

		if (card_eof || n == -1 || sd_char == '\r')
		{	
			buffer[sd_count] = '\0';

			if (!sd_comment_mode)
			{
				queue.enqueue_one_now(buffer);
			}

			ZERO(buffer);

			sd_count = 0; // clear sd line buffer
			linecount++;
			sd_comment_mode = false;
		}
		else if (sd_count >= sizeof(buffer) - 1) 
		{	

		}
		else 
		{	
			if (sd_char == ';')
				sd_comment_mode = true;
			else if (sd_char != '\n')
				buffer[sd_count++] = sd_char;
		}
	}

	queue.enqueue_one_now("M500");

	SERIAL_ECHOLNPGM("Parameter recovery succeeded.");
	#ifdef WTGL_LCD
	gserial.SendByte(REG_CONFIG_SAVE_MSG, 4);
	#endif

	card.closefile();
}

void wt_reset_param(void)
{
	wtvar_gohome = 0;
	wtvar_showWelcome = 1;
	wtvar_enablepowerloss = 0;
	wtvar_enableselftest = 1;
	(void)settings.save();

	safe_delay(200);
	wt_restart();
}

// W Command Process
void WTCMD_Process()
{
	switch (parser.codenum)
	{
	case 1:		
		wt_spark_begin();
		break;

	case 2:		
		wt_spark_end();
		break;

	case 3:		
		wt_spark_cancel();
		break;

	case 4:		
		wt_spark_pause();
		break;

	case 5:		
		wt_spark_resume();
		break;

	case 6:		
		wt_onlineprinting = SPARK_IDLE;
		break;

	case 7:		
		wt_onlineprinting = SPARK_LOST;
		break;

	case 201:	
		GetMachineStatus();
		break;

	case 203:	
		wt_restart();
		while (1);
		break;

	case 209:		
		wt_sdcard_stop();
		while (1);
		break;

	case 216:		
		wt_save_config();
		break;

	case 217:		
		wt_load_config();
		break;

	case 230:		
		wt_reset_param();
		break;

	case 300:		
		gserial.SendString(REG_WIFI_SSID, parser.string_arg);
		break;

	case 301:		
		gserial.SendString(REG_WIFI_PWD, parser.string_arg);
		break;

	case 302:		
		gserial.SendCmd(REG_WIFI_JOIN);
		break;

	}

}

// restart
void wt_restart()
{
	nvic_sys_reset();
};


static void wt_pause_print()
{
	xyz_pos_t park_point = NOZZLE_PARK_POINT;

  	resume_position = current_position;

  	planner.synchronize();

	current_position.e += -PAUSE_PARK_RETRACT_LENGTH / planner.e_factor[active_extruder];
	line_to_current_position(PAUSE_PARK_RETRACT_FEEDRATE);
	planner.synchronize();

	nozzle.park(2, park_point);

	#if HAS_FILAMENT_SENSOR
    runout.reset();
  	#endif
}

static void wt_resume_print()
{
	bool nozzle_timed_out = false;
	HOTEND_LOOP() {
		nozzle_timed_out |= thermalManager.hotend_idle[e].timed_out;
		thermalManager.reset_hotend_idle_timer(e);
	}

	if (nozzle_timed_out || thermalManager.hotEnoughToExtrude(active_extruder)) 
		load_filament(0, 0, ADVANCED_PAUSE_PURGE_LENGTH, 0, false, false);

	if (resume_position.e < 0) do_pause_e_move(resume_position.e, feedRate_t(PAUSE_PARK_RETRACT_FEEDRATE));

	do_blocking_move_to_xy(resume_position, feedRate_t(NOZZLE_PARK_XY_FEEDRATE));

	do_blocking_move_to_z(resume_position.z, feedRate_t(NOZZLE_PARK_Z_FEEDRATE));

  	planner.set_e_position_mm((destination.e = current_position.e = resume_position.e));

	#if HAS_FILAMENT_SENSOR
    runout.reset();
  	#endif
}

// main loop action
void wt_loopaction(void)
{
	if (wt_mainloop_action == WMA_IDLE) return;

	if (wt_mainloop_action == WMA_PAUSE)
	{
		if (queue.length > 0) return;
		
		wt_pause_print();

		#ifdef ACTION_ON_RESUMED
		host_action_paused();
		#endif

		wt_mainloop_action = WMA_IDLE;
	}
	else if (wt_mainloop_action == WMA_RESUME)
	{

	}
}

// load sd card
void wt_load_sd(void)
{
    if ((uint8_t)IS_SD_INSERTED())
    {
        digitalWrite(STM_SD_BUSY, HIGH);
        card.mount();
        SERIAL_ECHOLNPGM("mount sd");
    }
}

// unload sd card
void wt_unload_sd(void)
{
    if ((uint8_t)IS_SD_INSERTED())
    {
        card.release();
        safe_delay(100);
        digitalWrite(STM_SD_BUSY, LOW);
        SERIAL_ECHOLNPGM("unmount sd");

    }
}

void wt_send_queue_length(void)
{
    gserial.SendByte(REG_QUEUE_LENGTH, queue.length);
}

void wt_move_axis(const uint8_t axis, const float distance, const float fr_mm_s)
{
  const feedRate_t real_fr_mm_s = fr_mm_s ?: homing_feedrate((AxisEnum)axis);

    abce_pos_t target = planner.get_axis_positions_mm();
    target[axis] = 0;
    planner.set_machine_position_mm(target);
    target[axis] = distance;

    // Set delta/cartesian axes directly
    planner.buffer_segment(target, real_fr_mm_s, 0);


  planner.synchronize();
}
