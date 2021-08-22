/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../../MarlinCore.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"
#include "../../module/endstops.h"
#include "WTGL_Screen_TroubleShooting.h"
#include "../WTGL_Serial.h"
#include "../WTGL_Manager.h"
#include "../../wtlib/WTCMD.h"

void WTGL_Screen_TroubleShooting::Init()
{
	gserial.LoadScreen(SCREEN_DIAG);
	holdontime = getcurrenttime();
	updaterate = 200;
	pageid = 0;
	endstops.enable(true); 
}

void WTGL_Screen_TroubleShooting::Update()
{
	if ((pageid >= 2 &&  pageid <= 4) || pageid == 9)
	{
		gserial.SendByte(REG_PLANNER_STATE, planner.has_blocks_queued());
	}
	else if (pageid >= 5 && pageid <= 7)
	{
		gserial.SendByte(REG_PLANNER_STATE, planner.has_blocks_queued());
		ShowEndstop();
	}
	else if (pageid == 8)
	{
		ShowEndstop();
	}
	else if (pageid == 10 || pageid == 11)
	{
		wtgl.ShowTemperature();
	}
	// switch (pageid)
	// {
	// case 2:
	// case 4:
	// case 6:
	// 	if (planner.has_blocks_queued() == false && moving)
	// 	{
	// 		moving = 0;
	// 		gserial.SendString(ADDR_HELP2_TEXT_BUTTON2, MMSG_NEXT[wtvar_language], 20);
	// 	}
	// 	break;

	// case 3:
	// 	if (READ(X_MAX_PIN) && moving)
	// 	{
	// 		moving = 0;
	// 		gserial.SendString(ADDR_HELP2_TEXT_BUTTON2, MMSG_NEXT[wtvar_language], 20);
	// 	}
	// 	break;

	// case 5:
	// 	if (READ(Y_MAX_PIN) && moving)
	// 	{
	// 		moving = 0;
	// 		gserial.SendString(ADDR_HELP2_TEXT_BUTTON2, MMSG_NEXT[wtvar_language], 20);
	// 	}
	// 	break;

	// case 7:
	// 	if (READ(Z_MAX_PIN) && moving)
	// 	{
	// 		moving = 0;
	// 		gserial.SendString(ADDR_HELP2_TEXT_BUTTON2, MMSG_NEXT[wtvar_language], 20);
	// 	}
	// 	break;

	// case 8:
	// 	if (planner.has_blocks_queued() == false && moving)
	// 	{
	// 		moving = 0;
	// 		gserial.SendString(ADDR_HELP2_TEXT_BUTTON2, MMSG_NEXT[wtvar_language], 20);
	// 	}

	// 	if (READ(Z_MIN_PIN))
	// 		gserial.SendString(ADDR_HELP2_TEXT_LINE2, MMSG_SWITCH_OFF[wtvar_language], 20);
	// 	else
	// 		gserial.SendString(ADDR_HELP2_TEXT_LINE2, MMSG_SWITCH_ON[wtvar_language], 20);

	// 	break;

	// case 9:
	// 	if (thermalManager.degHotend(0) > 208 && moving)
	// 	{
	// 		moving = 0;
	// 		gserial.SendString(ADDR_HELP2_TEXT_BUTTON2, MMSG_END[wtvar_language], 20);

	// 		queue.enqueue_now_P(PSTR("G92 E0"));
	// 		queue.enqueue_now_P(PSTR("G1 E50 F200"));
	// 		queue.enqueue_now_P(PSTR("M104 S0"));

	// 		ShowDialog(DIAG_PAGE9_TITLE[wtvar_language],
	// 			DIAG_PAGE9_LINE2[wtvar_language],
	// 			DIAG_PAGE9_LINE3[wtvar_language],
	// 			DIAG_PAGE9_LINE4[wtvar_language],
	// 			DIAG_PAGE9_LINE5[wtvar_language],
	// 			MMSG_EMPTY,
	// 			DIAG_PAGE9_LINE7[wtvar_language],
	// 			MMSG_EMPTY);
	// 	}
	// 	else if (moving)
	// 	{
	// 		temp_nozzle0 = thermalManager.degHotend(0);
	// 		gserial.SendString(ADDR_HELP2_TEXT_LINE4, temp_nozzle0, 3);
	// 	}

	// 	break;

	// //case 10:
	// //	temp_nozzle0 = thermalManager.degHotend(0);
	// //	if (temp_nozzle0 > 55 && moving)
	// //	{
	// //		moving = 0;

	// //		ShowDialog(DIAG_PAGE10_TITLE[wtvar_language],
	// //			DIAG_PAGE10_LINE1[wtvar_language],
	// //			DIAG_PAGE10_LINE2[wtvar_language],
	// //			DIAG_PAGE10_LINE3[wtvar_language],
	// //			DIAG_PAGE10_LINE4[wtvar_language],
	// //			DIAG_PAGE10_LINE5[wtvar_language],
	// //			DIAG_PAGE10_LINE6[wtvar_language],
	// //			DIAG_PAGE10_LINE7[wtvar_language]);
	// //	}

	// //	break;

	// default:
	// 	break;
	// }
	
}

void WTGL_Screen_TroubleShooting::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
	if (addr == VAR_DIAG_QUIT || addr == VAR_DIAG_END)
	{
		Finish();
	}
	else if (addr == VAR_DIAG_W1_START)
	{
		pageid = 2;
		gserial.SendByte(REG_PLANNER_STATE, 1);
		wt_move_axis(A_AXIS, 50, get_homing_bump_feedrate(A_AXIS) * 2);
		wt_move_axis(A_AXIS, -20, get_homing_bump_feedrate(A_AXIS) * 2);
	}
	else if (addr == VAR_DIAG_W2_NEXT)
	{
		pageid = 3;
		gserial.SendByte(REG_PLANNER_STATE, 1);
		wt_move_axis(B_AXIS, 50, get_homing_bump_feedrate(B_AXIS) * 2);
		wt_move_axis(B_AXIS, -20, get_homing_bump_feedrate(B_AXIS) * 2);
	}
	else if (addr == VAR_DIAG_W3_NEXT)
	{
		pageid = 4;
		gserial.SendByte(REG_PLANNER_STATE, 1);
		wt_move_axis(C_AXIS, 50, get_homing_bump_feedrate(C_AXIS) * 2);
		wt_move_axis(C_AXIS, -20, get_homing_bump_feedrate(C_AXIS) * 2);
	}
	else if (addr == VAR_DIAG_W4_NEXT)
	{
		pageid = 5;
		gserial.SendByte(REG_PLANNER_STATE, 1);
		ShowEndstop();

		current_position.reset();
		destination.reset();
		sync_plan_position();
		current_position.z = (delta_height + 10);
		line_to_current_position(homing_feedrate(Z_AXIS));
		planner.synchronize();

		homeaxis(A_AXIS);
	}
	else if (addr == VAR_DIAG_W5_NEXT)
	{
		pageid = 6;
		gserial.SendByte(REG_PLANNER_STATE, 1);
		ShowEndstop();

		homeaxis(B_AXIS);
	}
	else if (addr == VAR_DIAG_W6_NEXT)
	{
		pageid = 7;
		gserial.SendByte(REG_PLANNER_STATE, 1);
		ShowEndstop();

		homeaxis(C_AXIS);
	}
	else if (addr == VAR_DIAG_W7_NEXT)
	{
		pageid = 8;
	}
	else if (addr == VAR_DIAG_W8_NEXT)
	{
		pageid = 9;
		thermalManager.allow_cold_extrude = 1;
		wt_move_axis(E_AXIS, -90, 10);
		wt_move_axis(E_AXIS, 90, 10);
		thermalManager.allow_cold_extrude = 0;
	}
	else if (addr == VAR_DIAG_W9_NEXT)
	{
		pageid = 10;
		queue.enqueue_now_P(PSTR("M104 T0 S60"));
		wtgl.ShowTemperature();
	}
	else if (addr == VAR_DIAG_W10_NEXT)
	{
		pageid = 11;
		queue.enqueue_now_P(PSTR("M104 T0 S0"));
		queue.enqueue_now_P(PSTR("M140 S40"));
		wtgl.ShowTemperature();
	}
	
}

void WTGL_Screen_TroubleShooting::NozzleTempError(void)
{
	// gserial.SendString(ADDR_HELP2_TEXT_LINE2, MMSG_SELF_TEST_NOTEMP[wtvar_language], 64);
}

void WTGL_Screen_TroubleShooting::BedTempError(void)
{

}

void WTGL_Screen_TroubleShooting::EndStopError(void)
{
	// switch (pageid)
	// {
	// case 3:
	// case 5:
	// case 7:
	// 	gserial.SendString(ADDR_HELP2_TEXT_LINE7, MMSG_SELF_TEST_NOHOMING[wtvar_language], 64);
	// 	break;

	// default:
	// 	break;
	// }
}

void WTGL_Screen_TroubleShooting::Finish(void)
{
	thermalManager.setTargetHotend(0, 0);
	thermalManager.setTargetBed(0);
	queue.enqueue_now_P(PSTR("M84"));
	queue.enqueue_now_P(PSTR("M104 T0 S0"));
	queue.enqueue_now_P(PSTR("M140 S0"));
	wtgl.QuitTestingMode();
	Goback();
}

void WTGL_Screen_TroubleShooting::ShowEndstop(void)
{
    uint8_t xmax, ymax, zmax, zmin, estate = 0;

    xmax = READ(X_MAX_PIN);
    ymax = READ(Y_MAX_PIN);
    zmax = READ(Z_MAX_PIN);
    zmin = !READ(Z_MIN_PIN);

    estate = (xmax << 6) + (ymax << 4) + (zmax << 2) + (zmin << 3);

    gserial.SendByte(REG_ENDSTOP_STATE, estate);

}