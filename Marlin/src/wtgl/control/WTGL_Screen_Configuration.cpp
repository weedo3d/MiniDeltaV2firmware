/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../../MarlinCore.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../module/planner.h"
#include "../../module/configuration_store.h"
#include "../../wtlib/WTCMD.h"
#include "../../gcode/parser.h"
#include "../../module/probe.h"
#include "../../module/stepper.h"
#include "WTGL_Screen_Configuration.h"
#include "../WTGL_Serial.h"
#include "../WTGL_Manager.h"

//
void WTGL_Screen_Configuration::Init()
{
	gserial.LoadScreen(SCREEN_CONFIG);

	ShowReport();
	holdontime = getcurrenttime();
}

void WTGL_Screen_Configuration::Update()
{


}

void WTGL_Screen_Configuration::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
    if (addr == VAR_CONFIG_BACK)
    {
        Goback();
    }
    else if (addr == VAR_CONFIG_RESTORE_BACK)
    {
        ShowReport();
    }
    else if (addr == VAR_CONFIG_RESTORE_OK)
    {
        settings.reset();
        settings.save();			
        safe_delay(500);
        wt_restart();
    }
    else if (addr == VAR_CONFIG_SAVE)
    {
        queue.enqueue_one_now("W216");
    }
    else if (addr == VAR_CONFIG_LOAD)
    {
        queue.enqueue_one_now("W217");
    }
}

void WTGL_Screen_Configuration::ShowReport(void)
{
    safe_delay(300);
    wtgl.ShowLog("Delta settings:");
    safe_delay(300);

    wtgl.ShowLogPair("M665 L%.2f R%.2f H%.2f X%.2f Y%.2f Z%.2f",
                     LINEAR_UNIT(delta_diagonal_rod),
					 LINEAR_UNIT(delta_radius),
					 LINEAR_UNIT(delta_height),
					 LINEAR_UNIT(delta_tower_angle_trim.a),
					 LINEAR_UNIT(delta_tower_angle_trim.b),
					 LINEAR_UNIT(delta_tower_angle_trim.c));
    safe_delay(300);

    wtgl.ShowLog("Endstop adjustment:");
    safe_delay(300);

    wtgl.ShowLogPair("M666 X%.2f Y%.2f Z%.2f",
                     LINEAR_UNIT(delta_endstop_adj.a),
					 LINEAR_UNIT(delta_endstop_adj.b),
					 LINEAR_UNIT(delta_endstop_adj.c));
    safe_delay(300);

    wtgl.ShowLog("Z-Probe Offset:");
    safe_delay(300);

    wtgl.ShowLogPair("M851 X%.2f Y%.2f Z%.2f",
                     LINEAR_UNIT(probe.offset_xy.x),
					 LINEAR_UNIT(probe.offset_xy.y),
					 probe.offset.z);
    safe_delay(300);

    wtgl.ShowLog("Stepper motor currents:");
    safe_delay(300);

    wtgl.ShowLogPair("M907 X%ld Y%ld Z%ld E%ld",
                     stepper.motor_current_setting[0],
					 stepper.motor_current_setting[1],
					 stepper.motor_current_setting[2],
					 stepper.motor_current_setting[3]);
    safe_delay(300);
}
