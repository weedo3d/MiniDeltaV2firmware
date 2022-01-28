/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTCMD_H
#define WTCMD_H

#include "stdint.h"

#define OCTOPRINT_IDLE		0
#define OCTOPRINT_PAUSED	1
#define OCTOPRINT_PRINTING	2
#define OCTOPRINT_LOST		3

#define HOST_WIIBUILDER	0
#define HOST_SPARK		1

enum WT_STATUS : char 
{
	WS_IDLE = 0, 
	WS_PRINTING = 1,
	WS_PAUSE = 2,
	WS_ABORT = 3,
	WS_FINISH = 4
};

enum WT_MAIN_ACTION : char
{
	WMA_IDLE = 0,
	WMA_PAUSE = 1,
	WMA_RESUME = 2
};

extern WT_STATUS wt_machineStatus;

extern WT_MAIN_ACTION wt_mainloop_action;

#ifdef AUTOPOWEROFF
void setup_autopoweroffpin();
#endif

void WTCMD_Process();

void wt_sdcard_stop();

void wt_sdcard_pause();

void wt_sdcard_resume();

void wt_restart();

void GetMachineStatus();

void wt_loopaction(void);

void wt_load_sd(void);

void wt_unload_sd(void);

void wt_send_queue_length(void);

void wt_move_axis(const uint8_t axis, const float distance, const float fr_mm_s);

#endif

