/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../../MarlinCore.h"
#include "../../gcode/queue.h"
#include "../../module/motion.h"
#include "../../module/endstops.h"
#include "WTGL_Screen_Jog.h"
#include "../WTGL_Serial.h"
#include "../WTGL_Manager.h"

void WTGL_Screen_Jog::Init()
{
	gserial.LoadScreen(SCREEN_JOG);
	holdontime = getcurrenttime();

    endstops.enable(true); 
}

void WTGL_Screen_Jog::Update()
{
	lpos = current_position.asLogical();
    gserial.SendPosition(REG_POSITION, lpos.x, lpos.y, lpos.z);
}

void WTGL_Screen_Jog::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
    if (addr == VAR_JOG_BACK)
    {
        queue.enqueue_now_P(PSTR("G90"));
        Goback();
    }
    else if (addr == VAR_JOG_HOME)
    {
        queue.enqueue_now_P(PSTR("G28"));
    }
    else if (addr == VAR_JOG_STOP)
    {
        queue.enqueue_now_P(PSTR("M410"));
    }

    if (queue.length >= BUFSIZE - 2) return;

    if (addr == VAR_JOG_X3)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 X-1"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_X2)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 X-5"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_X1)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 X-10"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_X4)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 X1"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_X5)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 X5"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_X6)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 X10"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Y3)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Y-1"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Y2)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Y-5"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Y1)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Y-10"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Y4)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Y1"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Y5)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Y5"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Y6)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Y10"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Z4)
    {	
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Z-0.1"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Z5)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Z-1"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Z6)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Z-10"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Z3)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Z0.1"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Z2)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Z1"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }
    else if (addr == VAR_JOG_Z1)
    {
        // queue.enqueue_now_P(PSTR("M211 S0"));
        queue.enqueue_now_P(PSTR("G91"));
        queue.enqueue_now_P(PSTR("G1 Z10"));
        // queue.enqueue_now_P(PSTR("M211 S1"));
    }

}

void WTGL_Screen_Jog::ShowHelp(void)
{

}