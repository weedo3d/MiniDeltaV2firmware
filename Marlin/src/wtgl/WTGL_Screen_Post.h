/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_POST_H
#define WTGL_SCREEN_POST_H

#include "WTGL_ScreenBase.h"

class WTGL_Screen_Post : public WTGL_ScreenBase
{
public:
	void Init(void);
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);

	void NozzleTempError(void);
	void BedTempError(void);
	void EndStopError(void);

private:
	enum SELFTEST_NOZZLE_ENUM
	{
		SNE_READING = 0,
		SNE_HEATING = 1,
		SNE_END = 2,
		SNE_ERROR = 3
	};

	enum SELFTEST_BED_ENUM
	{
		SBE_READING = 0,
		SBE_HEATING = 1,
		SBE_END = 2,
		SBE_ERROR = 3
	};

	enum SELFTEST_AXIS_ENUM
	{
		SAE_BEGIN = 0,
        SAE_HOMING = 1,
		SAE_XMOVING = 2,
		SAE_YMOVING = 3,
		SAE_ZMOVING = 4,
		SAE_END =5,
		SAE_ERROR = 6
	};

	SELFTEST_NOZZLE_ENUM nozzleState;
	SELFTEST_BED_ENUM bedState;
	SELFTEST_AXIS_ENUM axisState;

	uint32_t beginTime;
	uint32_t axisTime;
	int16_t temp_nozzle0;
	int16_t temp_bed;
	int16_t target_nozzle0;
	int16_t target_bed;

};

#endif
