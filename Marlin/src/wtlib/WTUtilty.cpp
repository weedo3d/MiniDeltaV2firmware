/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "WTUtilty.h"

#define V_ARR_LEN 20

bool startsWith(const char *str1, const char *str2)
{
	if (strlen(str1) < strlen(str2)) return false;
	int len = strlen(str2);
	char *p1 = (char*)str1;
	char *p2 = (char*)str2;
	for (int i = 0; i < len; i++)
	{
		if (*p1 != *p2) return false;
		p1++;
		p2++;
	}
	return true;
}


uint16_t hexStringConvert(const char *hexStr, uint8_t *pBinArray)
{
	if (NULL == hexStr || NULL == pBinArray)
	{
		return 0;
	}

	uint16_t uiKeySize = strlen(hexStr) / 2;
	uint16_t i = 0;
	char cTempor = 0;

	while (i < uiKeySize)
	{
		if (*hexStr >= '0' && *hexStr <= '9')
		{
			cTempor = *hexStr - 48;
		}
		else
		{
			cTempor = 0xa + (*hexStr - 'a');
		}

		pBinArray[i] = cTempor;
		hexStr++;

		if (*hexStr >= '0' && *hexStr <= '9')
		{
			cTempor = *hexStr - 48;
		}
		else
		{
			cTempor = 0xa + (*hexStr - 'a');
		}

		pBinArray[i] = (pBinArray[i] << 4) | cTempor;
		hexStr++;
		i++;
	}

	return uiKeySize;
}

uint8_t extractFront(const char* source, char* ident, char* valString)
{
	int len1 = strlen(source);
	int len2 = strlen(ident);

	if (len1 < len2) return 0;

	int start;
	int i = 0;
	const char* sp;
	sp = source;
	do
	{
		start = strstr(sp, ident) - source;
		sp++;
		i++;
	} while ((start > 0) && (start < len1) && (i < len1) && (source[start - 1] != 0x20));

	if (start >= 0 && start < len1)
	{
		sp = source;
		sp += len2 + start;
		i = len2 + start;

		while (i < len1 && *sp != 0x20)
		{
			sp++;
			i++;
		}
		if (i > len2)		
		{
			sp = source;
			sp += len2 + start;
			memcpy(valString, sp, i - len2 - start);
			valString[i - len2 - start] = 0;
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

uint16_t Utf8ToUnicode(const char* pInput, char* pOutput)
{
	uint16_t outputSize = 0; 
 
	while (*pInput)
	{
		if (*pInput > 0x00 && *pInput <= 0x7F) 
		{
			*pOutput = 0; 
			pOutput++;
			*pOutput = *pInput;
		}
		else if (((*pInput) & 0xE0) == 0xC0) 
		{
			char high = *pInput;
			pInput++;
			char low = *pInput;
			if ((low & 0xC0) != 0x80)  
			{
				return -1; 
			}

			*pOutput = (high >> 2) & 0x07;
			pOutput++;
			*pOutput = (high << 6) + (low & 0x3F);
		}
		else if (((*pInput) & 0xF0) == 0xE0) 
		{
			char high = *pInput;
			pInput++;
			char middle = *pInput;
			pInput++;
			char low = *pInput;
			if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
			{
				return -1;
			}
			
			*pOutput = (high << 4) + ((middle >> 2) & 0x0F);
			pOutput++;
			*pOutput = (middle << 6) + (low & 0x3F);
		}
		else 
		{
			return -1;
		}
		pInput ++;
		pOutput ++;
		outputSize += 2;
	}

	*pOutput = 0;
	 pOutput++;
	*pOutput = 0;
	return outputSize;
}


void divPoint(char *v, int *v_arr)
{
	int i = 0;
	char *result = NULL;
	result = strtok(v, ".");
	while (result != NULL)
	{
		v_arr[i++] = atoi(result);
		result = strtok(NULL, ".");
	}
}


bool hasNewVersion(int *oldVer, int *newVer)
{
	int i = 0;
	for (i = 0; i <= V_ARR_LEN - 1; i++)
	{
		if (oldVer[i] > newVer[i])
			return false;
		else if (newVer[i] > oldVer[i])
			return true;
	}
	return false;
}

bool VersionCompare(char* oldVersion, char* newVersion)
{
	int oldVer[V_ARR_LEN] = { 0 };
	int newVer[V_ARR_LEN] = { 0 };

	divPoint(oldVersion, oldVer);
	divPoint(newVersion, newVer);

	return hasNewVersion(oldVer, newVer);
}

uint32_t str2uint32(const uint8_t* str)
{
    uint32_t ret = (str[0] << 24) + (str[1] << 16) + (str[2] << 8) + str[3];
    return ret;
}

uint16_t str2uint16(const uint8_t* str)
{
    uint16_t ret = (str[0] << 8) + str[1];
    return ret;
}

float str2float(const uint8_t* str)
{
    float ret = 0;
    uint8_t *pdata = (uint8_t*)(&ret);

    memcpy(pdata, str, 4);

    return ret;
}


