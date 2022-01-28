/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include <stdarg.h>

#include "../MarlinCore.h"
#include "../sd/cardreader.h"
#include "../module/temperature.h"
#include "WTGL_Manager.h"
#include "WTGL_Serial.h"
#include "../wtlib/WTCMD.h"
#include "../gcode/queue.h"
#include "../wtlib/WTGcodeinfo.h"
#include "../wtlib/WTUtilty.h"

WTGL_Manager wtgl;

enum TouchCmdSts
{
	NOTHING = 0,
	FOUNDHEAD1 = 1,
	FOUNDHEAD2 = 2,
	FOUNDLEN = 3,
	FOUNDFUN = 4,
	FOUNDDATA = 5,
	CMDVALID = 6
};

WTGL_Manager::WTGL_Manager()
{

}

void WTGL_Manager::Init(unsigned long baud, bool doreset)
{
	gserial.Init(baud);

	InitMenu();

	testingMode = false;
}

void WTGL_Manager::Update()
{
	gserial.Process();

	nowtime = getcurrenttime();
	if (screenCurrent != NULL && nowtime > screenCurrent->holdontime + screenCurrent->updaterate)
	{
		screenCurrent->holdontime = nowtime;
		screenCurrent->Update();
		gserial.SendByte(REG_PRINTER_STATE,  (uint8_t)wt_machineStatus);
		gserial.SendByte(REG_OCTOPRINT_STATE,  (uint8_t)wt_onlineprinting);
	}
}

void WTGL_Manager::InitMenu()
{
    screenBoot = new WTGL_Screen_Boot();
	screenMain = new WTGL_Screen_Main();

	screenPrepare = new WTGL_Screen_Prepare();
    screenPrepare->SetParent(screenMain);
    
	screenControl = new WTGL_Screen_Control();
    screenControl->SetParent(screenMain);

	screenPrinting = new WTGL_Screen_Printing();

    screenJog = new WTGL_Screen_Jog();
    screenJog->SetParent(screenPrepare);

    screenFilament = new WTGL_Screen_Filament();
    screenFilament->SetParent(screenPrepare);
    
    screenPreheat = new WTGL_Screen_TempSetting();
    screenPreheat->SetParent(screenPrepare);

	screenLevelbed = new WTGL_Screen_Level();
	screenLevelbed->SetParent(screenPrepare);

    screenZOffset = new WTGL_Screen_ZOffset();
    screenZOffset->SetParent(screenPrepare);

    screenMotor = new WTGL_Screen_MotorOff();
    screenMotor->SetParent(screenPrepare);

    screenSelftest = new WTGL_Screen_Post();
    screenSelftest->SetParent(screenMain);

    screenRestoreSetting = new WTGL_Screen_Configuration();
    screenRestoreSetting->SetParent(screenControl);

    screenErrorDiag = new WTGL_Screen_TroubleShooting();
    screenErrorDiag->SetParent(screenControl);

    screenTestMode = new WTGL_Screen_FactoryTest();
    screenTestMode->SetParent(screenMain);

	screenWizard = new WTGL_Screen_Guide();
	screenWizard->SetParent(screenMain);

	screenResumePrinting = new WTGL_Screen_Resume();
	screenResumePrinting->SetParent(screenMain);

	screenCurrent = NULL;
}

void WTGL_Manager::GotoMain(void)
{
    screenCurrent = screenMain;
	screenCurrent->Init();
}

void WTGL_Manager::GotoScreen(WTGL_ScreenBase* screen)
{
    screenCurrent = screen;
	screenCurrent->Init();
}

void WTGL_Manager::LoadSubScreen(WTGL_ScreenBase* screen, WTGL_ScreenBase* parent)
{
	screenCurrent = screen;
	screenCurrent->SetParent(parent);
	screenCurrent->Init();
}

void WTGL_Manager::ResetScreen(void)
{
	gserial.SendCmd(REG_RESET);
}

void WTGL_Manager::ShowErrorMessage(const char* msg)
{
	gserial.SendString(REG_ERROR_MSG, msg);
}


void WTGL_Manager::NozzleTempError(void)
{
	thermalManager.disable_all_heaters();

    if(screenCurrent == screenSelftest)
	    screenSelftest->NozzleTempError();
    else if (screenCurrent == screenErrorDiag)
	    screenErrorDiag->NozzleTempError();
}

void WTGL_Manager::BedTempError(void)
{
	thermalManager.disable_all_heaters();
}

void WTGL_Manager::EndStopError(void)
{
    if(screenCurrent == screenSelftest)
	    screenSelftest->EndStopError();
    else if (screenCurrent == screenErrorDiag)
	    screenErrorDiag->EndStopError();
}

void WTGL_Manager::ErrorID(uint8_t id)
{
	gserial.SendByte(REG_ERROR_ID, id);
}

void WTGL_Manager::ShowLog(const char* message)
{
	if (screenCurrent == screenLevelbed || screenCurrent == screenRestoreSetting || screenCurrent == screenResumePrinting)
	{
        gserial.SendString(REG_MESSAGE, message);
	}
}

void WTGL_Manager::ShowLogPair(const char *format, ...)
{
	char buffer[50];
	va_list arg_ptr;
	va_start(arg_ptr, format); 

	ZERO(buffer);
	vsprintf(buffer, format, arg_ptr);

	va_end(arg_ptr);

	if (screenCurrent == screenLevelbed || screenCurrent == screenRestoreSetting || screenCurrent == screenResumePrinting)
	{
        gserial.SendString(REG_MESSAGE, buffer);
	}
}

void WTGL_Manager::ShowTemperature(void)
{
    temp_nozzle0 = thermalManager.degHotend(0);
	temp_bed = thermalManager.degBed();

	if (lasttemp_nozzle0 != temp_nozzle0)
	{
		if (temp_nozzle0 >= 0)
			gserial.SendInt16(REG_NOZZLE0_CURRENT, temp_nozzle0);
		else
			gserial.SendInt16(REG_NOZZLE0_CURRENT, 0);
	}

    if (lasttemp_bed != temp_bed)
	{
		if (temp_bed >= 0)
			gserial.SendInt16(REG_BED_CURRENT, temp_bed);
		else
			gserial.SendInt16(REG_BED_CURRENT, 0);
	}

	lasttemp_nozzle0 = temp_nozzle0;
	lasttemp_bed = temp_bed;
}

void WTGL_Manager::PayloadProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
	if (addr == ADDR_GLOBLE_CURRENT && data_length == 1)
	{	
		if (data_length == 1)
		{
			currentID = data[0];
			if (screenCurrent == screenBoot)
				screenBoot->KeyProcess(addr, data, data_length);
		}
	}
	else if (addr == ADDR_GLOBAL_GOTO)
	{	
		if (data_length == 1)
		{
			switch (data[0])
			{
			case 0x00:		
				wtgl.GotoBootMenu();
				break;

			case 0x02:		
				wtgl.GotoResumePrintingMenu();
				break;

			case 0x0A:		
                wtgl.GotoMain();
				break;

            case 0x1E:	
                wtgl.GotoPrepareMenu();
				break;

			case 0x1F:		
                wtgl.GotoFilamentMenu();
				break;

			case 0x20:		
                wtgl.GotoPreheatMenu();
				break;

			case 0x21:		
                wtgl.GotoJogMenu();
				break;

			case 0x22:		
                wtgl.GotoLevelMenu();
				break;

			case 0x23:		
                wtgl.GotoZOffsetMenu();
				break;

			case 0x24:		
                wtgl.GotoMotorOffMenu();
				break;  

            case 0x28:		
                wtgl.GotoControlMenu();
				break;

			case 0x2E:		
                wtgl.GotoErrorDiaglogMenu();
				break;

			case 0x2F:		
                wtgl.GotoRestoreSettingMenu();
				break;

			}
		}
	}
    else if (addr == ADDR_GLOBAL_MOUNT_SD)
    {
        wt_load_sd();
    }
    else if (addr == ADDR_GLOBAL_UNMOUNT_SD)
    {
        wt_unload_sd();
    }
    else if (addr == ADDR_GLOBAL_BOOTLOADER)
    {
		gserial.SendString(REG_MESSAGE, "Community firmware not support IAP");
    }
	else if (addr == ADDR_GLOBAL_GCODE)
	{
		queue.enqueue_one_now((const char*)data);
	}
	else if (addr == VAR_GUIDE_PRINT && screenCurrent != screenWizard)
	{
		if (wt_machineStatus == WS_IDLE || wt_machineStatus == WS_FINISH)
		{
			gcodeinfo.parse((char*)data);
        	card.openAndPrintFile((const char*)data);
		}
	}
	else if (addr == VAR_GUIDE_INDEX_PRINT && screenCurrent != screenWizard)
	{
		if (wt_machineStatus == WS_IDLE || wt_machineStatus == WS_FINISH)
		{
			card.selectFileByIndex(str2uint16(data));
        	card.openAndPrintFile(card.filename);
		}
	}
	else
	{
		screenCurrent->KeyProcess(addr, data, data_length);
	}
}

void WTGL_Manager::SendBusy(uint8_t busy)
{
	gserial.SendByte(REG_BUSY, busy);
}
