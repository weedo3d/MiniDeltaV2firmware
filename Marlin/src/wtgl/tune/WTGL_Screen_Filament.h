/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_FILAMENT_H
#define WTGL_SCREEN_FILAMENT_H

#include "../WTGL_ScreenBase.h"
#include "../WTGL_Manager.h"

class WTGL_Screen_Filament : public WTGL_ScreenBase
{
public:
	WTGL_Screen_Filament(void) {};
	void Init(void);
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);

private:
	enum FILAMENT_SCREEN_STATUS_ENUM
	{
		FSSE_CHOICETYPE = 0,
		FSSE_HEATTING = 1,
		FSSE_WAITTING = 2,
		FSSE_LOADING = 3,
		FSSE_COMPLETE = 4
	};

    enum FILAMENT_OPERATION_ENUM
    {
        FILAMENT_OPERATION_LOAD = 0,
        FILAMENT_OPERATION_UNLOAD
    };

	uint8_t filamentType;       // 0 - PLA, 1 - ABS, 2 - TPU, 3 - other
	int targetTemp;

	enum FILAMENT_SCREEN_STATUS_ENUM filaopsts;
    FILAMENT_OPERATION_ENUM operation;
};

#endif

