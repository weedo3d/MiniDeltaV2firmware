/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../MarlinCore.h"
#include "WTGL_Serial.h"
#include "../wtlib/WTPacket.h"
#include "../core/serial.h"

InPacket inp;       
OutPacket outp;    

void WTGL_Serial::SendString(uint16_t addr, const char* str)
{
	outp.Reset();
    outp.payload_append_int(addr);
    outp.payload_append_string(str);
    outp.Send();
}

void WTGL_Serial::SendInt16(uint16_t addr, uint16_t data)
{
    outp.Reset();
    outp.payload_append_int(addr);
    outp.payload_append_int(data);
    outp.Send();
}

void WTGL_Serial::SendInt32(uint16_t addr, uint32_t data)
{
    outp.Reset();
    outp.payload_append_int(addr);
    outp.payload_append_int32(data);
    outp.Send();
}

void WTGL_Serial::SendByte(uint16_t addr, uint8_t data)
{
    outp.Reset();
    outp.payload_append_int(addr);
    outp.appendByte(data);
    outp.Send();
}

void WTGL_Serial::SendCmd(uint16_t addr)
{
    outp.Reset();
    outp.payload_append_int(addr);
    outp.Send();
}

void WTGL_Serial::SendPosition(uint16_t addr, float x, float y, float z)
{
    outp.Reset();
	outp.payload_append_int(addr);
	outp.payload_append_float(x);
	outp.payload_append_float(y);
	outp.payload_append_float(z);
	outp.Send();
}

void WTGL_Serial::SendFloat(uint16_t addr, float data)
{
    outp.Reset();
	outp.payload_append_int(addr);
	outp.payload_append_float(data);
	outp.Send();
}


WTGL_Serial::WTGL_Serial()
{

}

void WTGL_Serial::Init(unsigned long baud)
{
#ifndef DMA_RX_SUPPORT
	LCD_SERIAL.begin(WTGL_BAUDRATE);
    uint32_t serial_connect_timeout = millis() + 1000UL;
    while (!LCD_SERIAL && PENDING(millis(), serial_connect_timeout)) { /*nada*/ }
#endif
}

void WTGL_Serial::Process()
{
	uint8_t b;

    if (inp.State() != PS_START && inp.isTimeout())
        inp.Reset();

	while (LCD_SERIAL.available() > 0)
	{
		b = (uint8_t)LCD_SERIAL.read();
		inp.Receiver(b);
	}
}

void WTGL_Serial::LoadScreen(uint8_t screenID)
{
	SendByte(REG_SCREEN_ID, screenID);
    // serial_echopair_PGM(PSTR("Load screen "), screenID);
    SERIAL_EOL();
}


