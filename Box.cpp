#include "stdafx.h"
#include "Box.h"

int BagClean(const char * _BagFileName)
{
	int result = 0;
	
	Chest chest = ChestRead(_BagFileName);

	ChestWrite(chest, "~BoxToolTmp");

	char sCmd[300] = "FC ~BoxToolTmp ";
	strcat(sCmd, _BagFileName);
	if (system(sCmd) == 0)
	{
		ChestSort(&chest);
		ChestWrite(chest, _BagFileName);
	}
	else
	{
		result = 1;
		MessageBox(0, _T("�ļ�����ʧ�ܣ��޷�����"), _T("��ʾ"), 0);
	}
	remove("~BoxToolTmp");

	//free
	FreeChest(&chest);

	return result;
}