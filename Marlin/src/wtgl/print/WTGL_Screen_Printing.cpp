/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../../MarlinCore.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../module/planner.h"
#include "../../module/motion.h"
#include "../../libs/duration_t.h"
#include "../../module/printcounter.h"
#include "WTGL_Screen_Printing.h"
#include "../WTGL_Serial.h"
#include "../WTGL_Manager.h"
#include "../../wtlib/WTCMD.h"
#include "../../wtlib/WTGcodeinfo.h"
#include "../../wtlib/WTUtilty.h"

void WTGL_Screen_Printing::Init()
{
    gserial.LoadScreen(SCREEN_PRINT);

	lasttemp_nozzle0 = -100;
	lasttemp_bed = -100;
	percent_last = -1;

	Update();

    holdontime = getcurrenttime();
    pausing = false;
}

void WTGL_Screen_Printing::Update()
{
	temp_nozzle0 = thermalManager.degHotend(0);
	temp_bed = thermalManager.degBed();

	if (lasttemp_nozzle0 != temp_nozzle0)
	{
		if (temp_nozzle0 >= 0)
			gserial.SendInt16(REG_NOZZLE0_CURRENT, temp_nozzle0);
		else
			gserial.SendInt16(REG_NOZZLE0_CURRENT, 0);
	}

    if (lasttemp_bed != temp_bed)
	{
		if (temp_bed >= 0)
			gserial.SendInt16(REG_BED_CURRENT, temp_bed);
		else
			gserial.SendInt16(REG_BED_CURRENT, 0);
	}

	lasttemp_nozzle0 = temp_nozzle0;
	lasttemp_bed = temp_bed;

	percent_current = card.percentDone();

	if (percent_current != percent_last)
	{
		gserial.SendByte(REG_PRINTING_PERCENT, percent_current);

		percent_last = percent_current;
	}

	duration_t elapsed = print_job_timer.duration();
    gserial.SendInt32(REG_PRINTING_ELAPSE, elapsed.second());

	if (percent_current == 0 && gcodeinfo.info.i_totaltime > 0)
	{
        gserial.SendInt32(REG_PRINTING_REMAIN, gcodeinfo.info.i_totaltime);
	}
	else
	{
		calc_local_printing_time();
        gserial.SendInt32(REG_PRINTING_REMAIN, wtgl.jobinfo.printTimeLeft);
	}

    if (pausing)
    {
        wt_send_queue_length();
        if (queue.length == 0)
            pausing = false;
    }
}

void WTGL_Screen_Printing::KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
    if (addr == VAR_PRINT_CANCEL)
    {   
        wt_sdcard_stop();
    }
    else if (addr == VAR_PRINT_PAUSE)
    {
        if (card.flag.sdprinting)
        {   
            wt_send_queue_length();
            wt_sdcard_pause();
            pausing = true;
        }
        else
        {   
            wt_sdcard_resume();
        }
    }
	else if (addr == VAR_PRINT_SETTING)
    {   
        gserial.SendInt16(REG_NOZZLE0_TARGET, thermalManager.degTargetHotend(0));
        gserial.SendInt16(REG_BED_TARGET, thermalManager.degTargetBed());
        gserial.SendInt16(REG_PRINTING_SPEED, feedrate_percentage);  
    }
    else if (addr == VAR_SPEED_SETTING_DATA && data_length == 2)
    {   
        feedrate_percentage = str2uint16(data);
    }
    else if (addr == VAR_TEMP_SETTING_NOZZLE && data_length == 2)
    {   
        thermalManager.setTargetHotend(str2uint16(data), 0);
    }
    else if (addr == VAR_TEMP_SETTING_BED && data_length == 2)
    {   
        thermalManager.setTargetBed(str2uint16(data));
    }
}

void WTGL_Screen_Printing::calc_local_printing_time(void)
{
	wtgl.jobinfo.printTime = print_job_timer.duration();

	float p = (float) wtgl.jobinfo.filepos;
	p /= wtgl.jobinfo.filesize;
	if (p < 0) p = 0;
	if (p > 1) p = 1;
	wtgl.jobinfo.completion = p;

	long est_total;
	if (gcodeinfo.info.i_totaltime > 0)
	{
		if (p > 0)
			est_total = (long)((float) wtgl.jobinfo.printTime / p);
		else
			est_total = gcodeinfo.info.i_totaltime;

		long est_last = est_total - wtgl.jobinfo.printTime;
		long slicing_last = gcodeinfo.info.i_totaltime - wtgl.jobinfo.printTime;
		if (slicing_last < 0) slicing_last = 0;

		wtgl.jobinfo.printTimeLeft = (long)((float)est_last * p + (float)slicing_last * (1 - p));
	}
	else
	{
		if (p > 0)
			est_total = (long)((float)wtgl.jobinfo.printTime / p);
		else
			est_total = 0;

		wtgl.jobinfo.printTimeLeft = est_total - wtgl.jobinfo.printTime;
		if (wtgl.jobinfo.printTimeLeft < 0) wtgl.jobinfo.printTimeLeft = 0;
	}
}