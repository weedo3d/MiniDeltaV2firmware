/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_PRINTING_H
#define WTGL_SCREEN_PRINTING_H

#include "../WTGL_ScreenBase.h"

class WTGL_Screen_Printing : public WTGL_ScreenBase
{
public:
	void Init(void);
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);

private:
	void calc_local_printing_time(void);

	int lasttemp_nozzle0;
	int lasttemp_bed;
	int16_t temp_nozzle0;
	int16_t temp_bed;
	uint8_t percent_current;
	int percent_last;
    bool pausing;
};


#endif