/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../MarlinCore.h"
#include "../module/temperature.h"
#include "WTGL_Screen_Main.h"
#include "WTGL_Serial.h"
#include "WTGL_Manager.h"
#include "../wtlib/WTCMD.h"
#include "../wtlib/WTGcodeinfo.h"
#include "../sd/cardreader.h"
#include "../wtlib/WTUtilty.h"

void WTGL_Screen_Main::Init()
{
	gserial.LoadScreen(SCREEN_MAIN);

	wtgl.ShowTemperature();

	holdontime = getcurrenttime();

    wt_machineStatus = WS_IDLE;
}

void WTGL_Screen_Main::Update()
{
	wtgl.ShowTemperature();
}

void WTGL_Screen_Main::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
    if (addr == VAR_MAIN_PREPARE)
    {
        wtgl.GotoPrepareMenu();
    }
    else if (addr == VAR_MAIN_SETUP)
    {
        wtgl.GotoControlMenu();
    }
    else if (addr == VAR_PREVIEW_FILENAME)
    {
        gcodeinfo.parse((char*)data);
        card.openAndPrintFile((const char*)data);
    }
    else if (addr == VAR_PREVIEW_FILEINDEX && data_length == 2)
    {
        card.selectFileByIndex(str2uint16(data));
        gcodeinfo.parse(card.filename);
        card.selectFileByIndex(str2uint16(data));
        card.openAndPrintFile(card.filename);
    }
    else if (addr == VAR_MAIN_INFO)
    {

    }
    else if (addr == VAR_MAIN_TEST)
    {
        wtgl.GotoTestModeMenu();
    }
}

