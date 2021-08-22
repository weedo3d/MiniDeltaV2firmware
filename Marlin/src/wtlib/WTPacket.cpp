/**
* Copyright (C) 2021 WEEDO3D Perron
*/


#include "CRC8.h"
#include "WTPacket.h"
#include "../wtgl/WTGL_Manager.h"

/// Append a byte and update the CRC
 void Packet::appendByte(uint8_t data)
{
	if (length < MAX_PACKET_PAYLOAD) 
	{
		payload[length] = data;
		length++;
	}
	else 
		error(RC_BUFFER_OVERFLOW);
}

void Packet::error(ResponseCode error_code_in)
{
	Reset();
	rcode = error_code_in;
}


char Packet::Get_Error(void)
{
	return rcode;
}

/// Reset this packet to an empty state
void Packet::Reset()
{
	crc = 0;
	length = 0;

	memset((char *)payload, 0 , MAX_PACKET_PAYLOAD);
	rcode = RC_OK;
	state = PS_START;
}

void InPacket::Receiver(uint8_t b)
{
	if (state == PS_START) 
	{
		if (b == START_BYTE) 	
		{
			state = PS_LEN;	
            timeout = getcurrenttime() + RECEIVE_TIMEOUT;	
		} 
	} 
	else if (state == PS_LEN) 
	{
		if (b <= MAX_PACKET_PAYLOAD) 	
		{
			expected_length = b;
			if (expected_length == 0)
				error(RC_PACKET_ERROR);
			else
				state = PS_PAYLOAD;
		} 
		else 
		{
			error(RC_BUFFER_OVERFLOW);
		}
	} 
	else if (state == PS_PAYLOAD)
	{
		appendByte(b);
		if (length >= expected_length) 
		{
			state = PS_CRC;
		}
	} 
	else if (state == PS_CRC) 
	{
		crc = CRC8(payload, 0, length);
		if (crc == b) 
			state = PS_END;
		else 
			error(RC_CRC_MISMATCH);		
	}
	else if (state == PS_END)
	{
		if (b == END_BYTE)
		{
            if (length >= 2)
            {
                addr = (payload[0] << 8) + payload[1];
                wtgl.PayloadProcess(addr, &payload[2], length - 2);
                Reset();
            }
            else
            {
                error(RC_PACKET_ERROR);
            }
            
		}
		else
		{
			error(RC_PACKET_NOEND);
		}
	}
}

void OutPacket::Send(void)
{
	buffer[0] = START_BYTE;
	buffer[1] = length;
	for (i = 0; i < length; i++)
		buffer[i + 2] = payload[i];
	crc = CRC8(payload, 0, length);
	buffer[length + 2] = crc;
	buffer[length + 3] = END_BYTE;

    LCD_SERIAL.write(buffer, length + 4);

    safe_delay(2);
}

void OutPacket::payload_append_int(uint16_t data)
{
	if (length < MAX_PACKET_PAYLOAD)
	{
		payload[length] = data >> 8;
		payload[length + 1] = data & 0xFF;
		length += 2;
	}
    else 
		error(RC_BUFFER_OVERFLOW);
}

void OutPacket::payload_append_int32(uint32_t data)
{
	if (length < MAX_PACKET_PAYLOAD)
	{
		payload[length] = (data & 0xFF000000) >> 24;
		payload[length + 1] = (data & 0xFF0000) >> 16;
		payload[length + 2] = (data & 0xFF00) >> 8;
		payload[length + 3] = data & 0xFF;
		length += 4;
	}
    else 
		error(RC_BUFFER_OVERFLOW);
}

void OutPacket::payload_append_float(float data)
{
	if (length < MAX_PACKET_PAYLOAD)
	{
		uint8_t a[4], i;

		uint8_t *pdata = (uint8_t*)(&data);
		for (i = 0; i < 4; i++)
			a[i] = *pdata++;

		appendByte(a[0]);
		appendByte(a[1]);
		appendByte(a[2]);
		appendByte(a[3]);
	}
    else 
		error(RC_BUFFER_OVERFLOW);
}

void OutPacket::payload_append_buffer(uint8_t* buffer)
{
	uint8_t* p = buffer;

	while (*p != 0 && length < MAX_PACKET_PAYLOAD)
	{
		payload[length] = *p;
		length++;
		p++;
	}
}

void OutPacket::payload_append_string(const char* str)
{
	payload_append_buffer((uint8_t*) str);
}

bool InPacket::isTimeout(void)
{
    if (getcurrenttime() > timeout)
        return true;
    else
        return false;
}