/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../../MarlinCore.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../module/planner.h"
#include "WTGL_Screen_TempSetting.h"
#include "../WTGL_Serial.h"
#include "../WTGL_Manager.h"
#include "../../wtlib/WTUtilty.h"

#define PREHEAT_TARGET_NOZZLE	200
#define PREHEAT_TARGET_BED		80

void WTGL_Screen_TempSetting::Init()
{
    gserial.SendInt16(REG_NOZZLE0_TARGET, thermalManager.degTargetHotend(0));
    gserial.SendInt16(REG_BED_TARGET, thermalManager.degTargetBed());
	gserial.LoadScreen(SCREEN_TEMP_SETTING);

	isHeating = false;

	holdontime = getcurrenttime();
}

void WTGL_Screen_TempSetting::Update()
{
	if (isHeating)
	{
		wtgl.ShowTemperature();
	}
}

void WTGL_Screen_TempSetting::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
    if (addr == VAR_TEMP_SETTING_W1_BACK)
    {
        thermalManager.disable_all_heaters();
		Goback();
    }
    else if (addr == VAR_TEMP_SETTING_W1_START)
    {
        thermalManager.setTargetHotend(target_nozzle0, 0);
        thermalManager.setTargetBed(target_bed);
        wtgl.ShowTemperature();
        isHeating = true;
    }
    else if (addr == VAR_TEMP_SETTING_NOZZLE && data_length == 2)
    {
        target_nozzle0 = str2uint16(data);
    }
    else if (addr == VAR_TEMP_SETTING_BED && data_length == 2)
    {
        target_bed = str2uint16(data);
    }
    else if (addr == VAR_TEMP_SETTING_W2_BACK)
    {
        thermalManager.disable_all_heaters();
		Goback();
    }
    else if (addr == VAR_TEMP_SETTING_W2_SETTING)
    {
        thermalManager.disable_all_heaters();
        isHeating = false;
    }
    else if (addr == VAR_TEMP_SETTING_W3_BACK)
    {
        thermalManager.disable_all_heaters();
		Goback();
    }
}


