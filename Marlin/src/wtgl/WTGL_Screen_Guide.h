/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_GUIDE_H
#define WTGL_SCREEN_GUIDE_H

#include "WTGL_ScreenBase.h"

class WTGL_Screen_Guide : public WTGL_ScreenBase
{
public:
	WTGL_Screen_Guide(void) { }
	void Init(void);
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);
private:
	enum FILAMENT_SCREEN_STATUS_ENUM
	{
		FSSE_CHOICETYPE = 0,
		FSSE_HEATTING = 1,
		FSSE_WAITTING = 2,
		FSSE_LOADING = 3,
		FSSE_COMPLETE = 4
	};

	void DisableWizard(void);

	int targetTemp;

	enum FILAMENT_SCREEN_STATUS_ENUM filaopsts;
};

#endif
