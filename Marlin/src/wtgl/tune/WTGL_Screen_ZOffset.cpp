/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../../MarlinCore.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../module/planner.h"
#include "../../module/configuration_store.h"
#include "../../module/probe.h"
#include "WTGL_Screen_ZOffset.h"
#include "../WTGL_Serial.h"
#include "../WTGL_Manager.h"

void WTGL_Screen_ZOffset::manual_move_to_current(AxisEnum axis)
{
	manual_move_start_time = millis();
	manual_move_axis = (int8_t)axis;
}

void WTGL_Screen_ZOffset::manage_manual_move() 
{

	if (manual_move_axis != (int8_t)NO_AXIS && ELAPSED(millis(), manual_move_start_time) && !planner.is_full()) 
	{
		planner.buffer_line(current_position, planner.settings.max_feedrate_mm_s[Z_AXIS], active_extruder);
		manual_move_axis = (int8_t)NO_AXIS;
	}
}

void WTGL_Screen_ZOffset::Init()
{
    queue.enqueue_one_now("G28");
	queue.enqueue_one_now("G29");
	queue.enqueue_one_now("G1 X0 Y0 Z10 F2000");
	queue.enqueue_one_now("G1 Z0 F400");
    wtgl.SendBusy(1);

	holdontime = getcurrenttime();
	zState = ZOSE_BEGIN;
    updaterate = 200;
	zoffset_current = probe.offset.z;
	manual_move_start_time = 0;
	manual_move_axis = (int8_t)NO_AXIS;
	ShowZOffset();
}

void WTGL_Screen_ZOffset::Update()
{
	switch (zState)
	{
	case ZOSE_BEGIN:
	{
		if (planner.has_blocks_queued() == false)
		{
			wtgl.SendBusy(0);
			zState = ZOSE_INPUT;
			zoffset_beginZpos = current_position[Z_AXIS];
            ShowZOffset();
		}
	}
	break;

	case ZOSE_INPUT:
		manage_manual_move();
		break;

	default:
		break;
	}
}

void WTGL_Screen_ZOffset::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{   
    if (addr == VAR_ZOFFSET_BACK)   
    {
        Goback();
    } 
    else if (addr == VAR_ZOFFSET_SAVE)  
    {
        probe.offset.z = zoffset_current;
        (void)settings.save();
        queue.enqueue_one_now(PSTR("G28"));
        Goback();
    }
    else if (addr == VAR_ZOFFSET_DOWN)  
    {
        zoffset_current -= 0.1;
        if (zoffset_current >= -9.9) 
            current_position[Z_AXIS] -= 0.1;
        else
            zoffset_current = -9.9;
        manual_move_to_current(Z_AXIS);
        ShowZOffset();
    }
    else if (addr == VAR_ZOFFSET_UP)  
    {
        zoffset_current += 0.1;
        if (zoffset_current <= 9.9)
            current_position[Z_AXIS] += 0.1;
        else
            zoffset_current = 9.9;
        manual_move_to_current(Z_AXIS);
        ShowZOffset();
    }
}

void WTGL_Screen_ZOffset::ShowZOffset(void)
{
	gserial.SendFloat(REG_ZOFFSET, zoffset_current);
}