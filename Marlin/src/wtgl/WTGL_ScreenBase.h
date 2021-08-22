/**
* Copyright (C) 2021 WEEDO3D Perron
*/


#ifndef WTGL_SCREEN_BASE_H
#define WTGL_SCREEN_BASE_H

#include "../MarlinCore.h"
#include "../../Configuration.h"

#define UNICODE_FILENAME_LENGTH	54

typedef struct
{
	char buffer[UNICODE_FILENAME_LENGTH];
    uint8_t len;
}wt_fileinfo_t;

class WTGL_ScreenBase
{
	public:
		WTGL_ScreenBase(void) { parent = NULL;}
		void SetParent(WTGL_ScreenBase* p) { parent = p; }
		void Goback(void);
		 
		virtual void Init(void) {};
		virtual void Update(void) {};
		virtual void KeyProcess(uint16_t addr, uint8_t *data, uint8_t data_length) {};
		virtual void ShowFilename(const char* fname) {};

		uint32_t updaterate = 500;
		uint32_t holdontime;

		bool activated = false;

	protected:
		WTGL_ScreenBase* parent = NULL;
};

#endif
