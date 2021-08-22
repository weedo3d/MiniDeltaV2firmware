/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SCREEN_TROUBLESHOOTING_H
#define WTGL_SCREEN_TROUBLESHOOTING_H

#include "../WTGL_ScreenBase.h"

class WTGL_Screen_TroubleShooting : public WTGL_ScreenBase
{
public:
	void Init(void);
	void Update(void);
	void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length);

	void NozzleTempError(void);
	void BedTempError(void);
	void EndStopError(void);

private:
	void Finish(void);
	void ShowEndstop(void);

	uint8_t pageid;
	uint8_t moving;
	int16_t temp_nozzle0;
};

#endif
