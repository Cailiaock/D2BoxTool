#include "stdafx.h"
#include "Box.h"

int BagClean(const char * _BagFileName)
{
	int result = 0;

	Chest chest = ChestRead(_BagFileName);
	remove("~BoxToolTmp");

	if (chest.itemCount > 0)
	{
		ChestWrite(chest, "~BoxToolTmp");

		if (FileCompare(_BagFileName, "~BoxToolTmp"))
		{
			ChestSort(&chest);
			ChestWrite(chest, _BagFileName);
		}
		else
		{
			result = 1;
			MessageBox(0, _T("文件解析失败，无法整理！"), _T("提示"), 0);
		}
		remove("~BoxToolTmp");
	}
	else
	{
		result = 1;
		MessageBox(0, _T("大箱子无物品，不需要整理！"), _T("提示"), 0);
	}

	//free
	FreeChest(&chest);

	return result;
}