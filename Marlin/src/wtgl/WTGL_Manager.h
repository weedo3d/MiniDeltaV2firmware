/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_MANAGER_H
#define WTGL_MANAGER_H

#include "../MarlinCore.h"
#include "WTGL_ScreenBase.h"

#include "WTGL_Screen_Main.h"
#include "tune/WTGL_Screen_Prepare.h"
#include "control/WTGL_Screen_Control.h"
#include "print/WTGL_Screen_Printing.h"
#include "tune/WTGL_Screen_Level.h"
#include "tune/WTGL_Screen_Filament.h"
#include "tune/WTGL_Screen_TempSetting.h"
#include "tune/WTGL_Screen_MotorOff.h"
#include "WTGL_Screen_Resume.h"
#include "WTGL_Screen_Boot.h"
#include "control/WTGL_Screen_Configuration.h"
#include "control/WTGL_Screen_FactoryTest.h"
#include "tune/WTGL_Screen_ZOffset.h"
#include "tune/WTGL_Screen_Jog.h"
#include "WTGL_Screen_Post.h"
#include "WTGL_Screen_Guide.h"
#include "control/WTGL_Screen_TroubleShooting.h"

#define WTGL_PAYLOAD_LENGTH		64
#define TestError

typedef struct 
{
	long printTime;
	long printTimeLeft;
	double completion;
	long filepos;
	long filesize;
	wt_fileinfo_t fileinfo;
	long date;
	uint16_t index;
	bool remote;			
} wt_job_info_t;

class WTGL_Manager
{
public:
	WTGL_Manager();

	void Init(unsigned long baud, bool doreset = true);
	void Update();

	void SetFilename(const char* fname);
	void SetPrintTime(const char* time);

	void GotoScreen(WTGL_ScreenBase* screen);
	void LoadSubScreen(WTGL_ScreenBase* screen, WTGL_ScreenBase* parent);
	void ResetScreen(void);

	void GotoMain(void);
	void GotoFilamentMenu(void) { screenFilament->SetParent(screenCurrent);  GotoScreen(screenFilament); }
	void GotoBootMenu(void) { GotoScreen(screenBoot); }
	void GotoControlMenu(void) { GotoScreen(screenControl);}
	void GotoPrepareMenu(void) { GotoScreen(screenPrepare);}
	void GotoLevelMenu(void) { GotoScreen(screenLevelbed); }
	void GotoPreheatMenu(void) { GotoScreen(screenPreheat); }
    void GotoMotorOffMenu(void) { GotoScreen(screenMotor); }
	void GotoResumePrintingMenu(void) { GotoScreen(screenResumePrinting); }
	void GotoRestoreSettingMenu(void) { GotoScreen(screenRestoreSetting); }
	void GotoTestModeMenu(void) { GotoScreen(screenTestMode); }

	void GotoZOffsetMenu(void) { GotoScreen(screenZOffset); }
	void GotoJogMenu(void) { GotoScreen(screenJog); }
	void GotoSelfTest(void) { testingMode = true; GotoScreen(screenSelftest); }
	void GotoWizardMenu(void) { GotoScreen(screenWizard); }
	void GotoErrorDiaglogMenu(void) { testingMode = true; GotoScreen(screenErrorDiag); }
	void GotoPrintingMenu() { GotoScreen(screenPrinting); }

	void ShowMessageRetrun(const char* title, const char* button, const char* msg);
	void ShowMessage(const char* msg);

	void ShowErrorMessage(const char* msg);
	void ShowRunoutMessage(void);
	void ShowUploadingMenu(char* msg);
	void ShowUploadFailMenu();
	void ShowMovingMessage(void);
	FORCE_INLINE bool isTesting(void) { return testingMode; }
	void QuitTestingMode(void) { testingMode = false; }
	void NozzleTempError(void);
	void BedTempError(void);
	void EndStopError(void);
	void ErrorID(uint8_t id);

    void ShowLog(const char* message);
	void ShowLogPair(const char *format, ...);

    void ShowTemperature(void);

    void PayloadProcess(uint16_t addr, uint8_t *data, uint8_t data_length);
	void SendBusy(uint8_t busy);

	bool busy = false;

    uint8_t currentID;

    wt_job_info_t jobinfo;

private:
	void InitMenu();
	void reset();
	bool testingMode;
    int lasttemp_nozzle0 = -1;
	int lasttemp_bed = -1;
	uint16_t temp_nozzle0;
	uint16_t temp_bed;

	WTGL_Screen_Main* screenMain;
	WTGL_Screen_Prepare* screenPrepare;
	WTGL_Screen_Control* screenControl;
	WTGL_Screen_Level* screenLevelbed;
	WTGL_Screen_Filament* screenFilament;
	WTGL_Screen_TempSetting* screenPreheat;

	WTGL_Screen_Resume* screenResumePrinting;
	WTGL_Screen_Boot* screenBoot;
	WTGL_Screen_Configuration* screenRestoreSetting;
	WTGL_Screen_FactoryTest* screenTestMode;

	WTGL_Screen_ZOffset* screenZOffset;
	WTGL_Screen_Jog* screenJog;
    WTGL_Screen_MotorOff* screenMotor;
	WTGL_Screen_Post* screenSelftest;
	WTGL_Screen_Guide* screenWizard;
	WTGL_Screen_TroubleShooting* screenErrorDiag;
	WTGL_Screen_Printing* screenPrinting;

	WTGL_ScreenBase* screenCurrent;

};

extern WTGL_Manager wtgl;

#endif
