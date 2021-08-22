/**
* Copyright (C) 2021 WEEDO3D Perron
*/


#ifndef __PACKET_H
#define __PACKET_H

#define START_BYTE 0xEB
#define END_BYTE 0x0A
#define MAX_PACKET_PAYLOAD 255
#define OUT_PACKET_PAYLOAD 96
#define LCD_SERIAL	MSerial2
#define RECEIVE_TIMEOUT     1000

#include <stdio.h>

typedef enum {
	PS_START  = 0,
	PS_LEN = 1,
	PS_PAYLOAD = 2,
	PS_CRC = 3,
	PS_END = 4
} PacketStatusEnum;

typedef enum {
	RC_EXIST = 0xEB,
	RC_OK = 0x80,
	RC_CRC_MISMATCH = 0x81,
	RC_BUFFER_OVERFLOW = 0x82,
	RC_CMD_UNSUPPORTED = 0x83,
	RC_CMD_ExecFail = 0x84,
	RC_PACKET_ERROR = 0x85,
	RC_PACKET_NOEND = 0x86
} ResponseCode;

class Packet
{
	protected:
		uint8_t length = 0; 
		uint8_t crc; 
		uint16_t addr;

		ResponseCode rcode; 
		PacketStatusEnum state = PS_START;
		uint8_t i;
	
		void error(ResponseCode error_code_in);
	
	public:
		uint8_t payload[MAX_PACKET_PAYLOAD];

		void appendByte(uint8_t data);

		void Reset(void);

		char Get_Error(void);

		PacketStatusEnum State(void) { return state; };

};

class OutPacket : public Packet
{
public:
	void payload_append_int(uint16_t data);
	void payload_append_int32(uint32_t data);
	void payload_append_float(float data);
	void payload_append_buffer(uint8_t* buffer);
	void payload_append_string(const char* str);

	void Send(void);

private:
	uint8_t buffer[OUT_PACKET_PAYLOAD];
};

class InPacket : public Packet
{
	public:
		void Receiver(uint8_t data);
        bool isTimeout(void);
		
	private:
		volatile uint8_t expected_length;
        uint32_t timeout;
};


#endif

