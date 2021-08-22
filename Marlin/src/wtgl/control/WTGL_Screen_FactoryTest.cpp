/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../../MarlinCore.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../module/planner.h"
#include "../../module/endstops.h"
#include "../../wtlib/WTCMD.h"
#include "WTGL_Screen_FactoryTest.h"
#include "../WTGL_Serial.h"
#include "../WTGL_Manager.h"

#define STRING_ON	"on "
#define STRING_OFF	"off"

void WTGL_Screen_FactoryTest::Init()
{
	gserial.LoadScreen(SCREEN_TEST);

	ShowEndstop();
	holdontime = getcurrenttime();
	updaterate = 200;
    endstops.enable(true); 
}

void WTGL_Screen_FactoryTest::Update()
{
	ShowEndstop();
}

void WTGL_Screen_FactoryTest::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
    if (addr == VAR_TEST_BACK)
    {
        Goback();
    }
    else if (addr == VAR_TEST_X1)
    {	
        wt_move_axis(A_AXIS, -5, get_homing_bump_feedrate(A_AXIS) * 2);
    }
    else if (addr == VAR_TEST_X2)
    {	
        wt_move_axis(A_AXIS, 5, get_homing_bump_feedrate(A_AXIS) * 2);
    }
    else if (addr == VAR_TEST_Y1)
    {	
        wt_move_axis(B_AXIS, -5, get_homing_bump_feedrate(B_AXIS) * 2);
    }
    else if (addr == VAR_TEST_Y2)
    {	
        wt_move_axis(B_AXIS, 5, get_homing_bump_feedrate(B_AXIS) * 2);
    }
    else if (addr == VAR_TEST_Z1)
    {	
        wt_move_axis(C_AXIS, -5, get_homing_bump_feedrate(C_AXIS) * 2);
    }
    else if (addr == VAR_TEST_Z2)
    {	
        wt_move_axis(C_AXIS, 5, get_homing_bump_feedrate(C_AXIS) * 2);
    }
    else if (addr == VAR_TEST_E1)
    {	
        queue.enqueue_now_P(PSTR("M302 P1"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 E1"));
    }
    else if (addr == VAR_TEST_E2)
    {	
        queue.enqueue_now_P(PSTR("M302 P1"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 E-1"));
    }
}

void WTGL_Screen_FactoryTest::ShowEndstop(void)
{
    uint8_t xmax, ymax, zmax, zmin, estate = 0;

    xmax = READ(X_MAX_PIN);
    ymax = READ(Y_MAX_PIN);
    zmax = READ(Z_MAX_PIN);
    zmin = !READ(Z_MIN_PIN);

    estate = (xmax << 6) + (ymax << 4) + (zmax << 2) + (zmin << 3);

    gserial.SendByte(REG_ENDSTOP_STATE, estate);

}