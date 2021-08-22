/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../../MarlinCore.h"
#include "../../gcode/queue.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../module/planner.h"
#include "WTGL_Screen_Filament.h"
#include "../WTGL_Serial.h"
#include "../WTGL_Manager.h"

void WTGL_Screen_Filament::Init()
{
    gserial.LoadScreen(SCREEN_FILAMENT);

	filamentType = 0;
	filaopsts = FSSE_CHOICETYPE;
	holdontime = getcurrenttime();
}

void WTGL_Screen_Filament::Update()
{
	switch (filaopsts)
	{
	case FSSE_CHOICETYPE:
		break;

	case FSSE_HEATTING:
		if (filamentType == 0)
		{
			queue.enqueue_now_P(PSTR("M104 S210"));
			targetTemp = 210;
		}
		else if (filamentType == 1)
		{
			queue.enqueue_now_P(PSTR("M104 S235"));
			targetTemp = 235;
		}
		else if (filamentType == 2)
		{
			queue.enqueue_now_P(PSTR("M104 S240"));
			targetTemp = 240;
		}
        else
		{
			queue.enqueue_now_P(PSTR("M104 S260"));
			targetTemp = 260;
		}
		filaopsts = FSSE_WAITTING;
		wtgl.ShowTemperature();
		gserial.SendByte(REG_FILAMENT_OPERATION, 0);
		break;

	case FSSE_WAITTING:
		if (thermalManager.degHotend(0) > targetTemp)
		{
			queue.enqueue_now_P(PSTR("G92 E0"));

			if (operation == FILAMENT_OPERATION_LOAD)
			{
				queue.enqueue_now_P(PSTR("G1 E100 F200"));
		        gserial.SendByte(REG_FILAMENT_OPERATION, 1);
			}
			else
			{
				queue.enqueue_now_P(PSTR("G1 E5 F100"));
				queue.enqueue_now_P(PSTR("G1 E-40 F300"));
				queue.enqueue_now_P(PSTR("G1 E-400 F900"));
                gserial.SendByte(REG_FILAMENT_OPERATION, 2);
			}

			if (filamentType == 0)
            {
                queue.enqueue_now_P(PSTR("M104 S210"));
                targetTemp = 210;
            }
            else if (filamentType == 1)
            {
                queue.enqueue_now_P(PSTR("M104 S235"));
                targetTemp = 235;
            }
            else if (filamentType == 2)
            {
                queue.enqueue_now_P(PSTR("M104 S240"));
                targetTemp = 260;
            }
            else
            {
                queue.enqueue_now_P(PSTR("M104 S260"));
                targetTemp = 260;
            }
			
			filaopsts = FSSE_LOADING;
		}
		wtgl.ShowTemperature();
		break;

	case FSSE_LOADING:
		if (planner.has_blocks_queued() == false)
		{
			queue.enqueue_now_P(PSTR("G92 E0"));
			filaopsts = FSSE_COMPLETE;
		}
        wtgl.ShowTemperature();
		break;

	case FSSE_COMPLETE:
		filaopsts = FSSE_CHOICETYPE;
		queue.enqueue_now_P(PSTR("M104 S0"));
		Goback();
		break;
	}
}

void WTGL_Screen_Filament::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
    if (addr == VAR_FILAMENT_FEEDIN)
    {
        operation = FILAMENT_OPERATION_LOAD;
    }
    else if (addr == VAR_FILAMENT_RETRACT)
    {
        operation = FILAMENT_OPERATION_UNLOAD;
    }
    else if (addr == VAR_FILAMENT_TYPE && data_length == 1)
    {
        filamentType = data[0];
        filaopsts = FSSE_HEATTING;
		Update();
    }
    else if (addr == VAR_FILAMENT_W1_BACK)
    {
        Goback();
    }
    else if (addr == VAR_FILAMENT_W3_BACK)
    {
        filaopsts = FSSE_COMPLETE;
        queue.enqueue_now_P(PSTR("M104 S0"));
        queue.enqueue_now_P(PSTR("M410"));
        Goback();
    }
}

