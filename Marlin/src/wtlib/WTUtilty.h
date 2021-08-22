/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#ifndef WT_UTILTY_H
#define WT_UTILTY_H

#include <stdbool.h>

bool startsWith(const char *str1, const char *str2);

uint16_t hexStringConvert(const char *hexStr, uint8_t *pBinArray);

uint8_t extractFront(const char* source, char* ident, char* valString);

uint16_t Utf8ToUnicode(const char* pInput, char* pOutput);

bool VersionCompare(char* oldVersion, char* newVersion);

uint32_t str2uint32(const uint8_t* str);

uint16_t str2uint16(const uint8_t* str);

float str2float(const uint8_t* str);

#endif