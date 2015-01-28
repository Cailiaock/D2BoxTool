#include "stdafx.h"
#include "Box.h"

int BagClean(const char * _BagFileName)
{
	int result = 0;

	Chest chest = ChestRead(_BagFileName);
	remove("~BoxToolTmp");
	ChestWrite(chest, "~BoxToolTmp");

	/*Chest chest2 = ChestRead2(_BagFileName);
	remove("~BoxToolTmp2");
	ChestWrite(chest2, "~BoxToolTmp2");*/

	/*char sCmd[300] = "FC ~BoxToolTmp ";
	strcat(sCmd, _BagFileName);*/

	if (FileCompare(_BagFileName, "~BoxToolTmp"))
	{
		/*ChestSort(&chest);
		ChestWrite(chest, _BagFileName);*/
	}
	else
	{
		result = 1;
		MessageBox(0, _T("文件解析失败，无法整理！"), _T("提示"), 0);
	}
	//remove("~BoxToolTmp");

	//free
	FreeChest(&chest);

	return result;
}