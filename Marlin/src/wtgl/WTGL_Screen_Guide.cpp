/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../MarlinCore.h"
#include "../module/configuration_store.h"
#include "../gcode/queue.h"
#include "../module/planner.h"
#include "../module/temperature.h"
#include "../sd/cardreader.h"
#include "../wtlib/WTGcodeinfo.h"
#include "WTGL_Screen_Guide.h"
#include "WTGL_Serial.h"
#include "WTGL_Manager.h"

void WTGL_Screen_Guide::Init()
{
	gserial.LoadScreen(SCREEN_GUIDE);

	filaopsts = FSSE_CHOICETYPE;
	holdontime = getcurrenttime();

}

void WTGL_Screen_Guide::Update()
{
	switch (filaopsts)
	{
	case FSSE_CHOICETYPE:
		break;

	case FSSE_HEATTING:
		queue.enqueue_now_P(PSTR("M104 S210"));
		targetTemp = 210;
		filaopsts = FSSE_WAITTING;
		wtgl.ShowTemperature();
		gserial.SendByte(REG_FILAMENT_OPERATION, 0);
		break;

	case FSSE_WAITTING:
		if (thermalManager.degHotend(0) > targetTemp)
		{
			queue.enqueue_now_P(PSTR("G92 E0"));
			queue.enqueue_now_P(PSTR("G1 E100 F200"));
			gserial.SendByte(REG_FILAMENT_OPERATION, 1);
			queue.enqueue_now_P(PSTR("M104 S210"));
			targetTemp = 210;
			filaopsts = FSSE_LOADING;
		}
		wtgl.ShowTemperature();
		break;

	case FSSE_LOADING:
		if (planner.has_blocks_queued() == false)
		{
			queue.enqueue_now_P(PSTR("G92 E0"));
			filaopsts = FSSE_COMPLETE;
		}
        wtgl.ShowTemperature();
		break;

	case FSSE_COMPLETE:
		filaopsts = FSSE_CHOICETYPE;
		queue.enqueue_now_P(PSTR("M104 S0"));
		gserial.SendByte(REG_FILAMENT_OPERATION, 3);
		break;
	}
}

void WTGL_Screen_Guide::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
	if (addr == VAR_GUIDE_SKIP)
	{
		DisableWizard();
		queue.enqueue_now_P(PSTR("M104 S0"));
        queue.enqueue_now_P(PSTR("M410"));
        Goback();
	}
	else if (addr == VAR_GUIDE_PRINT || addr == VAR_PREVIEW_FILENAME)
    {
        gcodeinfo.parse((char*)data);
        card.openAndPrintFile((const char*)data);
    }
	else if (addr == VAR_GUIDE_FEED)
	{
		DisableWizard();
		filaopsts = FSSE_HEATTING;
		Update();
	}
	else if (addr == VAR_GUIDE_END_FEED)
	{
		queue.enqueue_now_P(PSTR("M104 S0"));
        queue.enqueue_now_P(PSTR("M410"));
	}
}

void WTGL_Screen_Guide::DisableWizard(void)
{
	if (wtvar_showWelcome == 1)
	{
		wtvar_showWelcome = 0;
		(void)settings.save();
	}
}
