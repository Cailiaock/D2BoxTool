#pragma once

#include "Public.h"

typedef struct Config{
	char type[30];
	int quality;
	int page;
}Config;

typedef struct ItemType{
	char type[30];
	char code[3];
}ItemType;

typedef struct ExcelItem{
	char name[100];
	char code[3];
	int invwidth;
	int invheight;
}ExcelItem;

extern char * excel_misc;
extern long excel_misc_size;

extern char * excel_weapons;
extern long excel_weapons_size;

extern char * excel_armor;
extern long excel_armor_size;

extern ExcelItem * ExcelItems;
extern long ExcelItemsLength;
extern long ExcelItemsCount;

extern Config * Configs;
extern long ConfigsLength;
extern long ConfigsCount;

extern ItemType * ItemTypes;
extern long ItemTypesLength;
extern long ItemTypesCount;

bool LoadExcel();
bool LoadItemType();
bool LoadConfig();

int GetExcelItemInvwidth(const char * _Code);
int GetExcelItemInvheight(const char * _Code);

void GetItemType(char * _Dest, const char * _Code);