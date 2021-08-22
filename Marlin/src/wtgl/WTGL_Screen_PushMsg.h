/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_PUSH_MSG_H
#define WTGL_SCREEN_PUSH_MSG_H

#include "WTGL_ScreenBase.h"

class WTGL_Screen_PushMsg : public WTGL_ScreenBase
{
public:
	void Init(uint32_t delay);
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);
	
private:

};

#endif
