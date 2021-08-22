/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../MarlinCore.h"
#include "../module/temperature.h"
#include "../module/motion.h"
#include "../gcode/queue.h"
#include "../module/planner.h"
#include "../module/endstops.h"
#include "WTGL_Screen_Post.h"
#include "WTGL_Serial.h"
#include "WTGL_Manager.h"

#define NOZZLE_HEATING_WAITING		20000
#define BED_HEATING_WAITING			30000
#define NOOZLE_LOW_HEATING_TEMP		5
#define NOZZLE_HIGH_HEATING_TEMP	3
#define BED_LOW_HEATING_TEMP		3
#define BED_HIGH_HEATING_TEMP		2
#define NOZZLE_THRESHOLD			150
#define BED_THRESHOLD				50
#define XY_WAITING					20000
#define Z_WAITING					20000

uint8_t wtgl_ahit, wtgl_bhit, wtgl_chit;

void WTGL_Screen_Post::Init()
{
	gserial.LoadScreen(SCREEN_POST);
	holdontime = getcurrenttime();
	beginTime = getcurrenttime();
	axisTime = getcurrenttime();
    posttime = getcurrenttime() + 10000;

	nozzleState = SNE_READING;
	temp_nozzle0 = thermalManager.degHotend(0);
	if (temp_nozzle0 < -10)
	{
		gserial.SendInt16(REG_NOZZLE0_CURRENT, 0xFFFF);
		gserial.SendByte(REG_NOZZLE0_HEATING, 0);
		nozzleState = SNE_ERROR;
	}
	else
	{
		if (temp_nozzle0 < NOZZLE_THRESHOLD)
			target_nozzle0 = temp_nozzle0 + NOOZLE_LOW_HEATING_TEMP;
		else
			target_nozzle0 = temp_nozzle0 + NOZZLE_HIGH_HEATING_TEMP;

		thermalManager.setTargetHotend(target_nozzle0, 0);
		gserial.SendByte(REG_NOZZLE0_HEATING, 1);
		nozzleState = SNE_HEATING;
	}

    axisState = SAE_BEGIN;
    endstops.enable(true); 
	gserial.SendByte(REG_X_HOMED, 0);
	gserial.SendByte(REG_Y_HOMED, 0);
	gserial.SendByte(REG_Z_HOMED, 0);
}

void WTGL_Screen_Post::Update()
{
	temp_nozzle0 = thermalManager.degHotend(0);
	temp_bed = thermalManager.degBed();

	if (nozzleState == SNE_HEATING)
	{
		if ((beginTime + NOZZLE_HEATING_WAITING) > getcurrenttime())
		{
			if (temp_nozzle0 >= target_nozzle0)
			{
        		gserial.SendByte(REG_NOZZLE0_HEATING, 2);
				nozzleState = SNE_END;
				thermalManager.setTargetHotend(0, 0);
			}
		}
		else
		{	
			gserial.SendByte(REG_NOZZLE0_HEATING, 3);
			nozzleState = SNE_ERROR;
			thermalManager.setTargetHotend(0, 0);
		}
	}

    if (axisState == SAE_BEGIN)
    {
        current_position.reset();
        destination.reset();
        sync_plan_position();

        current_position.z = (delta_height + 10);
        line_to_current_position(homing_feedrate(Z_AXIS));

        axisState = SAE_HOMING;
    }
    else if (axisState == SAE_HOMING)
    {
        if (planner.has_blocks_queued()) return; 

        wtgl_ahit = 0;
        axisState = SAE_XMOVING;
        homeaxis(A_AXIS);
    }
	else if (axisState == SAE_XMOVING)
	{
		if ((axisTime + XY_WAITING) > getcurrenttime())
		{
			if (wtgl_ahit)
			{
				gserial.SendByte(REG_X_HOMED, 1);
                wtgl_bhit = 0;
                axisState = SAE_YMOVING;
				homeaxis(B_AXIS);  
			}
		}
		else
		{	
	        gserial.SendByte(REG_X_HOMED, 2);
			axisState = SAE_ERROR;
		}
	}
	else if (axisState == SAE_YMOVING)
	{
		if ((axisTime + XY_WAITING) > getcurrenttime())
		{
			if (wtgl_bhit)
			{
                gserial.SendByte(REG_Y_HOMED, 1);
                wtgl_chit = 0;
                axisState = SAE_ZMOVING;
				homeaxis(C_AXIS);
			}
		}
		else
		{	
			gserial.SendByte(REG_Y_HOMED, 2);
			axisState = SAE_ERROR;
		}
	}
	else if (axisState == SAE_ZMOVING)
	{
		if ((axisTime + Z_WAITING) > getcurrenttime())
		{
			if (wtgl_chit)
			{
                LOOP_XYZ(i) set_axis_is_at_home((AxisEnum)i);
				gserial.SendByte(REG_Z_HOMED, 1);
				axisState = SAE_END;
			}
		}
		else
		{	
			gserial.SendByte(REG_Z_HOMED, 2);
			axisState = SAE_ERROR;
		}
	}

	if (nozzleState == SNE_END && axisState == SAE_END)
	{	
		queue.enqueue_now_P(PSTR("M18"));
		wtgl.QuitTestingMode();
		Goback();
	}
}

void WTGL_Screen_Post::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
    if (addr == VAR_POST_SKIP)
    {
        thermalManager.setTargetHotend(0, 0);
        thermalManager.setTargetBed(0);
        queue.enqueue_now_P(PSTR("M18"));
        wtgl.QuitTestingMode();
        Goback();
    }
}

void WTGL_Screen_Post::NozzleTempError(void)
{
    gserial.SendByte(REG_NOZZLE0_HEATING, 3);
	nozzleState = SNE_ERROR;
}

void WTGL_Screen_Post::BedTempError(void)
{

}

void WTGL_Screen_Post::EndStopError(void)
{
	if (axisState == SAE_XMOVING)
	{
        gserial.SendByte(REG_X_HOMED, 2);
	}
	else if (axisState == SAE_YMOVING)
	{
        gserial.SendByte(REG_Y_HOMED, 2);
	}
	else if (axisState == SAE_ZMOVING)
	{
		gserial.SendByte(REG_Z_HOMED, 2);
	}
    else if (axisState == SAE_END)
    {
        return;
    }
	axisState = SAE_ERROR;
}
