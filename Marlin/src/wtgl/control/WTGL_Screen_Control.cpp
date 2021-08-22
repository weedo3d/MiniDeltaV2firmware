/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../../MarlinCore.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../module/planner.h"
#include "../../module/configuration_store.h"
#include "../../module/printcounter.h"
#include "../../feature/powerloss.h"
#include "WTGL_Screen_Control.h"
#include "../WTGL_Serial.h"
#include "../WTGL_Manager.h"


void WTGL_Screen_Control::Init()
{
	gserial.LoadScreen(SCREEN_CONTROL);

    gserial.SendString(REG_DEVICE_NAME, MACHINE_NAME);
    gserial.SendString(REG_DEVICE_VERSION, SHORT_BUILD_VERSION);
    printStatistics now = print_job_timer.getStats();
	duration_t elapsed = now.printTime;
    gserial.SendInt32(REG_PRINTED_TIME, elapsed.second());
    gserial.SendByte(REG_OPTION_DIAG, wtvar_enableselftest);
    gserial.SendByte(REG_OPTION_POWERLOSS, wtvar_enablepowerloss);
    
	holdontime = getcurrenttime();
}

void WTGL_Screen_Control::Update()
{
	// do nothing
}

void WTGL_Screen_Control::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
    if (addr == VAR_CONTROL_BACK)
    {
        Goback();
    }
    else if (addr == VAR_OPTION_POST_DATA && data_length == 1)
    {
        wtvar_enableselftest = data[0];
        (void)settings.save();
    }
    else if (addr == VAR_OPTION_POWERLOSS_DATA && data_length == 1)
    {
        wtvar_enablepowerloss = data[0];
        recovery.enable(wtvar_enablepowerloss);
        (void)settings.save();
    }
	
}

