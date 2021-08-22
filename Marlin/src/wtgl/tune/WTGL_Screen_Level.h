/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_LEVEL_H
#define WTGL_SCREEN_LEVEL_H

#include "../WTGL_ScreenBase.h"

class WTGL_Screen_Level : public WTGL_ScreenBase
{
public:
	void Init(void);
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);

private:
	void do_operation(void);

	bool back_enable;
};

#endif

