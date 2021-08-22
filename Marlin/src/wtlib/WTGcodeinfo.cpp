/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "../MarlinCore.h"
#include "../sd/cardreader.h"
#include "../libs/duration_t.h"
#include "WTGcodeinfo.h"
#include "WTUtilty.h"

WTGcodeinfo gcodeinfo;

const char MSG_NONE [] PROGMEM = {"NONE"};

void WTGcodeinfo::reset(void)
{
	memset(info.machine, 0, 20);
	strcpy_P(info.machine, MSG_NONE);
	memset(info.material, 0, 10);
	strcpy_P(info.material, MSG_NONE);
	memset(info.layerheight, 0, 10);
	strcpy_P(info.layerheight, MSG_NONE);
	memset(info.density, 0, 10);
	strcpy_P(info.density, MSG_NONE);
	memset(info.temp0, 0, 10);
	strcpy_P(info.temp0, MSG_NONE);
	memset(info.totaltime, 0, 10);
	strcpy_P(info.totaltime, MSG_NONE);
	memset(info.filament, 0, 10);
	strcpy_P(info.filament, MSG_NONE);
	info.f_filament = 0;
	info.i_totaltime = 0;
	info.b_image = false;
    info.b_multi = false;
	info.i_line = 0;
    info.x_min = 0;
    info.x_max = 0;
    info.y_min = 0;
    info.y_max = 0;
    info.z_min = 0;
    info.z_max = 0;
}

void WTGcodeinfo::parse(char* filename)
{
	ZERO(info.filename);
	strcpy(info.filename, filename);
	reset();
	card.openFileRead(filename);

	sd_count = 0;
	bool card_eof = card.eof();
	bool sd_comment_mode = false;
	uint16_t linecount = 0;
    uint16_t linemax = NOPIC_MAX_LINE;
    char tempStr[10];

	memset(linebuffer, 0, MAX_COMMENT_LINE_LENGTH);
	while (!card_eof && linecount < linemax)
	{
		const int16_t n = card.get();
		char sd_char = (char)n;
		card_eof = card.eof();

		if (card_eof || n == -1 || sd_char == '\r')
		{	
			linebuffer[sd_count] = '\0';

			if (sd_comment_mode)
			{
				if (startsWith(linebuffer, "MachineType:"))
				{
					getvalue(info.machine);
				}
				else if (startsWith(linebuffer, "FilamentType:"))
				{
					getvalue(info.material);
				}
				else if (startsWith(linebuffer, "Layer height:"))
				{
					getvalue(info.layerheight);
					strcat(info.layerheight, "mm");
				}
				else if (startsWith(linebuffer, "InfillDensity:"))
				{
					getvalue(info.density);
					strcat(info.density, "%");
					SERIAL_ECHO(info.density);
				}
				else if (startsWith(linebuffer, "Extruder0Temperature:"))
				{
					getvalue(info.temp0);
				}
				else if (startsWith(linebuffer, "TIME:"))
				{
					getvalue(info.totaltime);
					info.i_totaltime = atol(info.totaltime) + 120;
					duration_t tempT = info.i_totaltime;
					tempT.toDigital(info.totaltime);
				}
				else if (startsWith(linebuffer, "FilamentUsed:"))
				{
					getvalue(info.filament);
					info.f_filament = atof(info.filament) / (float)335.4 + 1;
					uint16_t tempi = info.f_filament;
					memset(info.filament, 0, 10);
					sprintf_P(info.filament, PSTR("%ig"), tempi);
				}
                else if (startsWith(linebuffer, "MINX:"))
				{
                    ZERO(tempStr);
					getvalue(tempStr);
					info.x_min = atof(tempStr);
				}
                else if (startsWith(linebuffer, "MAXX:"))
				{
                    ZERO(tempStr);
					getvalue(tempStr);
					info.x_max = atof(tempStr);
				}
                else if (startsWith(linebuffer, "MINY:"))
				{
                    ZERO(tempStr);
					getvalue(tempStr);
					info.y_min = atof(tempStr);
				}
                else if (startsWith(linebuffer, "MAXY:"))
				{
                    ZERO(tempStr);
					getvalue(tempStr);
					info.y_max = atof(tempStr);
				}
                else if (startsWith(linebuffer, "MINZ:"))
				{
                    ZERO(tempStr);
					getvalue(tempStr);
					info.z_min = atof(tempStr);
				}
                else if (startsWith(linebuffer, "MAXZ:"))
				{
                    ZERO(tempStr);
					getvalue(tempStr);
					info.z_max = atof(tempStr);
				}
                else if (startsWith(linebuffer, "MultipleExtruderPrint:"))
				{
                    ZERO(tempStr);
					getvalue(tempStr);
					if (startsWith(tempStr, "true"))
                        info.b_multi = true;
				}
			}
			else
			{	
				if (startsWith(linebuffer, "W220"))
				{	
					info.b_image = true;
					info.i_line++;
				}
				else if (startsWith(linebuffer, "W221"))
                {  
                    linemax = PIC_MAX_LINE;
                }
                else if (startsWith(linebuffer, "W222"))
                {  
                    linemax = linecount + NOPIC_MAX_LINE;
                }
			}
			memset(linebuffer, 0, MAX_COMMENT_LINE_LENGTH);

			sd_count = 0; 
			linecount++;
			sd_comment_mode = false;
			
		}
		else if (sd_count >= MAX_COMMENT_LINE_LENGTH - 1) 
		{	
			SERIAL_ECHOLNPGM("Error: comment line too long");
			card.closefile();
			return;
		}
		else 
		{	
			if (sd_char == ';')
				sd_comment_mode = true;
			else if (sd_char != '\n')
				linebuffer[sd_count++] = sd_char;
		}
	}

	card.closefile();
}

void WTGcodeinfo::getvalue(char* data)
{
	uint8_t pos = 0;
	pos = strchr(linebuffer, ':') - linebuffer;

	if (pos > 1 && pos < sd_count)
	{
		uint8_t dp = 0;
		for (uint8_t i = 0; i < sd_count - pos; i++)
		{
			if (linebuffer[pos + i + 1] != 0x20)
				data[dp++] = linebuffer[pos + i + 1];
		}
	}
}
