/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_MOTOR_OFF_H
#define WTGL_SCREEN_MOTOR_OFF_H

#include "../WTGL_ScreenBase.h"

//
class WTGL_Screen_MotorOff : public WTGL_ScreenBase
{
public:
	void Init();
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);
	
private:
    void ShowMotorState(void);
    
};

#endif
