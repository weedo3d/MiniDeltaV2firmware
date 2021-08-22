/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_PREPARE_H
#define WTGL_SCREEN_PREPARE_H

#include "../WTGL_ScreenBase.h"

class WTGL_Screen_Prepare : public WTGL_ScreenBase
{
public:
	void Init(void);
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);
    
private:

};

#endif
