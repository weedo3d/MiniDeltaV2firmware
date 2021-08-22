/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_JOB_H
#define WTGL_SCREEN_JOB_H

#include "../WTGL_ScreenBase.h"

class WTGL_Screen_Jog : public WTGL_ScreenBase
{
public:
	void Init(void);
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);

private:
	void ShowHelp(void);

    xyze_pos_t lpos;
};

#endif
