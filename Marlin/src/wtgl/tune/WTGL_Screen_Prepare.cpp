/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../../MarlinCore.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../module/planner.h"
#include "WTGL_Screen_Prepare.h"
#include "../WTGL_Serial.h"
#include "../WTGL_Manager.h"

void WTGL_Screen_Prepare::Init()
{
	gserial.LoadScreen(SCREEN_PREPARE);
	holdontime = getcurrenttime();
}

void WTGL_Screen_Prepare::Update()
{
	// do nothing
}

void WTGL_Screen_Prepare::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
    if (addr == VAR_PREPARE_BACK)
    {
        Goback();
    }
    else if (addr == VAR_LEVEL_W2_BACK)
    {
        gserial.LoadScreen(SCREEN_PREPARE);
    }
}
