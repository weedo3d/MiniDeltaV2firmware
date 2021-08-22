/**
* Copyright (C) 2021 WEEDO3D Perron
*/
#include "../../MarlinCore.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../module/planner.h"
#include "WTGL_Screen_Level.h"
#include "../WTGL_Serial.h"
#include "../WTGL_Manager.h"

void WTGL_Screen_Level::Init()
{
	activated = true;
	wtgl.busy = true;
	back_enable = false;
	queue.enqueue_one_now("G33");
    gserial.SendByte(REG_BUSY, 1);
}

void WTGL_Screen_Level::Update()
{
	if (wtgl.busy == false && back_enable == false)
	{	
		back_enable = true;
		queue.enqueue_one_now("M500");
		wtgl.ShowLog("Calibration setting saved.");
        gserial.SendByte(REG_BUSY, 0);
	}
}

void WTGL_Screen_Level::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
    if ((addr == VAR_LEVEL_W2_BACK) && back_enable)
    {
        Goback();
    }
}

