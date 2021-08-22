/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_ZOFFSET_H
#define WTGL_SCREEN_ZOFFSET_H

#include "../WTGL_ScreenBase.h"

class WTGL_Screen_ZOffset : public WTGL_ScreenBase
{
public:
	void Init(void);
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);

private:
	enum ZOFFSET_SCREEN_ENUM
	{
		ZOSE_BEGIN = 0,
		ZOSE_INPUT = 1
	};

	void ShowZOffset(void);
	void manage_manual_move(void);
	void manual_move_to_current(AxisEnum axis);

	ZOFFSET_SCREEN_ENUM zState;
	float zoffset_beginZpos;
	float zoffset_current;
	millis_t manual_move_start_time = 0;
	int8_t manual_move_axis = (int8_t)NO_AXIS;
};

#endif

