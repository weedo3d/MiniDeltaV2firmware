/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "WTGL_Screen_MotorOff.h"
#include "../WTGL_Serial.h"
#include "../../MarlinCore.h"
#include "../../module/stepper.h"

void WTGL_Screen_MotorOff::Init()
{
    ShowMotorState();
    gserial.LoadScreen(SCREEN_MOTOR);
	holdontime = getcurrenttime();
}

void WTGL_Screen_MotorOff::Update()
{
	ShowMotorState();
}

void WTGL_Screen_MotorOff::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
	if (addr == VAR_MOTOR_BACK)
    {
        Goback();
    }
    else if (addr == VAR_MOTOR_X)
    {
        DISABLE_AXIS_X();
    }
    else if (addr == VAR_MOTOR_Y)
    {
        DISABLE_AXIS_Y();
    }
    else if (addr == VAR_MOTOR_Z)
    {
        DISABLE_AXIS_Z();
    }
    else if (addr == VAR_MOTOR_E)
    {
        disable_e_steppers();
    }
    else if (addr == VAR_MOTOR_ALL)
    {
        disable_all_steppers();
    }

    ShowMotorState();
}

void WTGL_Screen_MotorOff::ShowMotorState(void)
{
    uint8_t x1, y1, z1, e1, state;

    x1 = !X_ENABLE_READ();
    y1 = !Y_ENABLE_READ();
    z1 = !Z_ENABLE_READ();
    e1 = !E0_ENABLE_READ();

    state = (x1 << 7) + (y1 << 5) + (z1 << 3) + (e1 << 1);

    gserial.SendByte(REG_MOTOR_STATE, state);
}

