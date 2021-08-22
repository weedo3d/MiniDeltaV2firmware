/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WTGL_SERIAL_H
#define WTGL_SERIAL_H

#include "WTGL_ADDR.h"

class WTGL_Serial
{
public:
	WTGL_Serial();

	static void Init(unsigned long baud);
	static void Process();
	static void SendString(uint16_t addr, const char* str);
	static void SendInt16(uint16_t addr, uint16_t data);
	static void SendInt32(uint16_t addr, uint32_t data);
    static void SendByte(uint16_t addr, uint8_t data);
    static void SendCmd(uint16_t addr);
    static void SendPosition(uint16_t addr, float x, float y, float z);
    static void SendFloat(uint16_t addr, float data);

	static void LoadScreen(uint8_t screenID);

private:

};

extern WTGL_Serial gserial;

#endif