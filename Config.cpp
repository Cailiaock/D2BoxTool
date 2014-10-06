#include "stdafx.h"
#include "Config.h"


char CONFIG_BAKPATH[1024] = "c:\\¥Ûœ‰◊”±∏∑›";

char * excel_misc;
long excel_misc_size;

char * excel_weapons;
long excel_weapons_size;

char * excel_armor;
long excel_armor_size;

ExcelItem * ExcelItems;
long ExcelItemsLength;
long ExcelItemsCount;

Config * Configs;
long ConfigsLength;
long ConfigsCount;

ItemType * ItemTypes;
long ItemTypesLength;
long ItemTypesCount;

bool LoadExcel()
{
	ExcelItemsLength = 1000;
	ExcelItemsCount = 0;
	ExcelItems = (ExcelItem *)malloc(sizeof(ExcelItem)*ExcelItemsLength);

	excel_misc = ReadAllFile("excel\\misc.txt", "r", &excel_misc_size);
	if (excel_misc == NULL)
	{
		MessageBox(0, _T("º”‘ÿexcel\\misc.txt ß∞‹"), _T("¥ÌŒÛ"), 0);
		return false;
	}

	excel_weapons = ReadAllFile("excel\\weapons.txt", "r", &excel_weapons_size);
	if (excel_weapons == NULL)
	{
		MessageBox(0, _T("º”‘ÿexcel\\weapons.txt ß∞‹"), _T("¥ÌŒÛ"), 0);
		return false;
	}

	excel_armor = ReadAllFile("excel\\armor.txt", "r", &excel_armor_size);
	if (excel_armor == NULL)
	{
		MessageBox(0, _T("º”‘ÿexcel\\armor.txt ß∞‹"), _T("¥ÌŒÛ"), 0);
		return false;
	}

	char * buf[3] = { excel_misc, excel_weapons, excel_armor };

	long len[3] = { excel_misc_size, excel_weapons_size, excel_armor_size };

	for (int i = 0; i < 3; i++)
	{
		int iName = -1;
		int iCode = -1;
		int iInvwidth = -1;
		int iInvheight = -1;

		char * buffer = buf[i];
		long lSize = len[i];

		int split_Line_Begin = 0;
		int split_Line_ReturnLen = 0;
		int split_Line_Index = 0;
		char * pLineSplit = strsplit(buffer, "\n", lSize, 1, &split_Line_Begin, &split_Line_ReturnLen);
		while (pLineSplit != NULL)
		{
			ExcelItem * nowExcelItem = NULL;
			if (split_Line_Index != 0)
			{
				if (ExcelItemsCount == ExcelItemsLength)
				{
					ExcelItemsLength = ExcelItemsLength << 1;
					ExcelItems = (ExcelItem *)realloc(ExcelItems, sizeof(ExcelItem)* ExcelItemsLength);
				}
				nowExcelItem = &ExcelItems[ExcelItemsCount];
				ExcelItemsCount++;
			}

			int split_Col_Begin = 0;
			int split_Col_ReturnLen = 0;
			int split_Col_Index = 0;
			char * pColSplit = strsplit(pLineSplit, "\t", split_Line_ReturnLen, 1, &split_Col_Begin, &split_Col_ReturnLen);
			while (pColSplit != NULL)
			{
				if (split_Line_Index == 0)
				{
					//head
					if (strncmp(pColSplit, "name", split_Col_ReturnLen) == 0) iName = split_Col_Index;
					else if (strncmp(pColSplit, "code", split_Col_ReturnLen) == 0) iCode = split_Col_Index;
					else if (strncmp(pColSplit, "invwidth", split_Col_ReturnLen) == 0) iInvwidth = split_Col_Index;
					else if (strncmp(pColSplit, "invheight", split_Col_ReturnLen) == 0) iInvheight = split_Col_Index;
				}
				else
				{
					if (split_Col_Index == iName)
						strncpy(nowExcelItem->name, pColSplit, split_Col_ReturnLen);
					else if (split_Col_Index == iCode)
						strncpy(nowExcelItem->code, pColSplit, split_Col_ReturnLen);
					else if (split_Col_Index == iInvwidth)
						nowExcelItem->invwidth = atoi(pColSplit);
					else if (split_Col_Index == iInvheight)
						nowExcelItem->invheight = atoi(pColSplit);
				}

				//next split
				split_Col_Index++;
				free(pColSplit);
				pColSplit = NULL;
				pColSplit = strsplit(pLineSplit, "\t", split_Line_ReturnLen, 1, &split_Col_Begin, &split_Col_ReturnLen);
			}
			free(pColSplit);
			pColSplit = NULL;

			//next split
			split_Line_Index++;
			free(pLineSplit);
			pLineSplit = NULL;
			pLineSplit = strsplit(buffer, "\n", lSize, 1, &split_Line_Begin, &split_Line_ReturnLen);
		}
		free(pLineSplit);
		pLineSplit = NULL;
	}
	return true;
}

bool LoadItemType()
{
	long lSize = 0;
	ItemTypesLength = 1000;
	ItemTypesCount = 0;
	ItemTypes = (ItemType *)malloc(sizeof(ItemType)*ItemTypesLength);

	char * buffer = ReadAllFile("itemtype.cfg", "r", &lSize);
	if (buffer == NULL)
	{
		MessageBox(0, _T("º”‘ÿitemtype.cfg ß∞‹"), _T("¥ÌŒÛ"), 0);
		return false;
	}

	int split_Line_Begin = 0;
	int split_Line_ReturnLen = 0;
	int split_Line_Index = 0;
	char * pLineSplit = strsplit(buffer, "\n", lSize, 1, &split_Line_Begin, &split_Line_ReturnLen);
	while (pLineSplit != NULL)
	{
		if (*pLineSplit != '#')
		{
			ItemType * nowItemType = NULL;
			if (strstr(pLineSplit, ",") != NULL)
			{
				if (ItemTypesCount == ItemTypesLength)
				{
					ItemTypesLength = ItemTypesLength << 1;
					ItemTypes = (ItemType *)realloc(ItemTypes, sizeof(ItemType)* ItemTypesLength);
				}
				nowItemType = &ItemTypes[ItemTypesCount];
				ItemTypesCount++;
			}

			if (strstr(pLineSplit, ",") != NULL)
			{
				int split_Col_Begin = 0;
				int split_Col_ReturnLen = 0;
				int split_Col_Index = 0;
				char * pColSplit = strsplit(pLineSplit, ",", split_Line_ReturnLen, 1, &split_Col_Begin, &split_Col_ReturnLen);
				while (pColSplit != NULL)
				{
					char * col = (char *)malloc(sizeof(char)* (split_Col_ReturnLen + 1));
					strncpy(col, pColSplit, split_Col_ReturnLen);
					col[split_Col_ReturnLen] = '\0';

					if (split_Col_Index == 0)
						strcpy(nowItemType->type, col);
					else if (split_Col_Index == 1)
						strcpy(nowItemType->code, col);
					free(col);
					col = NULL;

					//next split
					split_Col_Index++;
					free(pColSplit);
					pColSplit = NULL;
					pColSplit = strsplit(pLineSplit, ",", split_Line_ReturnLen, 1, &split_Col_Begin, &split_Col_ReturnLen);
				}
				free(pColSplit);
				pColSplit = NULL;
			}
		}
		//next split
		split_Line_Index++;
		free(pLineSplit);
		pLineSplit = NULL;
		pLineSplit = strsplit(buffer, "\n", lSize, 1, &split_Line_Begin, &split_Line_ReturnLen);
	}
	free(pLineSplit);
	pLineSplit = NULL;
	free(buffer);
	return true;
}

bool LoadConfig()
{
	long lSize = 0;
	ConfigsLength = 1000;
	ConfigsCount = 0;
	Configs = (Config *)malloc(sizeof(Config)*ConfigsLength);

	char * buffer = ReadAllFile("config.cfg", "r", &lSize);
	if (buffer == NULL)
	{
		MessageBox(0, _T("º”‘ÿconfig.cfg ß∞‹"), _T("¥ÌŒÛ"), 0);
		return false;
	}

	int split_Line_Begin = 0;
	int split_Line_ReturnLen = 0;
	int split_Line_Index = 0;
	char * pLineSplit = strsplit(buffer, "\n", lSize, 1, &split_Line_Begin, &split_Line_ReturnLen);
	while (pLineSplit != NULL)
	{
		if (*pLineSplit != '#')
		{
			bool bBackPath = false;
			Config * nowConfig = NULL;
			if (strstr(pLineSplit, ",") != NULL)
			{
				if (ConfigsCount == ConfigsLength)
				{
					ConfigsLength = ConfigsLength << 1;
					Configs = (Config *)realloc(Configs, sizeof(Config)* ConfigsLength);
				}
				nowConfig = &Configs[ConfigsCount];
				ConfigsCount++;
			}

			if (strstr(pLineSplit, ",") != NULL)
			{
				int split_Col_Begin = 0;
				int split_Col_ReturnLen = 0;
				int split_Col_Index = 0;
				char * pColSplit = strsplit(pLineSplit, ",", split_Line_ReturnLen, 1, &split_Col_Begin, &split_Col_ReturnLen);
				while (pColSplit != NULL)
				{
					char * col = (char *)malloc(sizeof(char)* (split_Col_ReturnLen + 1));
					strncpy(col, pColSplit, split_Col_ReturnLen);
					col[split_Col_ReturnLen] = '\0';

					if (split_Col_Index == 0)
					{
						if (strcmp(col, "bakpath") == 0)
							bBackPath = true;
						else
							strcpy(nowConfig->type, col);
					}
					else if (split_Col_Index == 1)
					{
						if (bBackPath)
						{
							strcpy(CONFIG_BAKPATH, col);
							ConfigsCount--;
						}
						else
							nowConfig->quality = atoi(col);
					}
					else if (split_Col_Index == 2 && !bBackPath)
					{
						nowConfig->page = atoi(col);
						nowConfig->page--;
						if (nowConfig->page < 0)
						{
							nowConfig->page = 0;
						}
					}
					free(col);
					col = NULL;

					//next split
					split_Col_Index++;
					free(pColSplit);
					pColSplit = NULL;
					pColSplit = strsplit(pLineSplit, ",", split_Line_ReturnLen, 1, &split_Col_Begin, &split_Col_ReturnLen);
				}
				free(pColSplit);
				pColSplit = NULL;
			}
		}
		//next split
		split_Line_Index++;
		free(pLineSplit);
		pLineSplit = NULL;
		pLineSplit = strsplit(buffer, "\n", lSize, 1, &split_Line_Begin, &split_Line_ReturnLen);
	}
	free(pLineSplit);
	pLineSplit = NULL;
	free(buffer);

	return true;
}

int GetExcelItemInvwidth(const char * _Code)
{
	for (int i = 0; i < ExcelItemsCount; i++)
	{
		if (strncmp(ExcelItems[i].code, _Code, 3) == 0)
		{
			return ExcelItems[i].invwidth;
		}
	}
	return 2;
}

int GetExcelItemInvheight(const char * _Code)
{
	for (int i = 0; i < ExcelItemsCount; i++)
	{
		if (strncmp(ExcelItems[i].code, _Code, 3) == 0)
		{
			return ExcelItems[i].invheight;
		}
	}
	return 4;
}

void GetItemType(char * _Dest, const char * _Code)
{
	for (int i = 0; i < ItemTypesCount; i++)
	{
		if (strncmp(ItemTypes[i].code, _Code, 3) == 0)
		{
			strcpy(_Dest, ItemTypes[i].type);
			return;
		}
	}
}