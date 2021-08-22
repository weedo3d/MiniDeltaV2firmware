/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#pragma once

#ifndef __WT_GCODE_INFO_H
#define __WT_GCODE_INFO_H

#define MAX_COMMENT_LINE_LENGTH		100
#define PIC_MAX_LINE			300
#define NOPIC_MAX_LINE			30


typedef struct 
{
	char machine[20];	
	char material[10];		
	char layerheight[10];	
	char density[10];		
	char temp0[10];			
	char totaltime[10];		
	char filament[10];		
	float f_filament;	
	uint32_t i_totaltime;	
	char filename[27];		
	bool b_image;			
    bool b_multi;         
	uint8_t i_line;			
    float x_min, x_max, y_min, y_max, z_min, z_max;
} wt_gcode_info_t;

class WTGcodeinfo
{
public:
	WTGcodeinfo() {};

	void parse(char* filename);

	wt_gcode_info_t info;


private:
	char linebuffer[MAX_COMMENT_LINE_LENGTH];
	uint16_t sd_count;

	void reset(void);
	void getvalue(char* data);
};

extern WTGcodeinfo gcodeinfo;

#endif

