/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_RESUME_H
#define WTGL_SCREEN_RESUME_H

#include "WTGL_ScreenBase.h"

class WTGL_Screen_Resume : public WTGL_ScreenBase
{
public:
	void Init(void);
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);

private:
	void ShowRecoveryInfo(void);

	enum DSEP_ENUM
	{
		DSEP_START = 0,
		DSEP_GOHOME = 1,
		DSEP_HEATING = 2,
		DSEP_FEDDING = 3,
		DSEP_SETPOS = 4,
		DSEP_OPENFILE = 11,
		DSEP_COMPLETE = 12,
		DSEP_ERROR = 13
	};

	DSEP_ENUM state;
};

#endif

