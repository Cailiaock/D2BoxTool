#include "stdafx.h"
#include "Public.h"

char * strsplit(const char * _Str, const char * _Split, const int _StrLen, const int _SplitLen, int * _BeginIndex, int * _ReturnLen)
{
	for (int i = *_BeginIndex; i < _StrLen; i++)
	{
		int iBegin = *_BeginIndex;
		int iEnd = -1;
		if (i < _StrLen - _SplitLen)
		{
			bool bCheck = true;
			for (int j = 0; j < _SplitLen; j++)
			{
				if (_Split[j] != _Str[i + j])
				{
					bCheck = false;
					break;
				}

			}
			if (bCheck)
			{
				iEnd = i - 1;
				*_BeginIndex = i = i + _SplitLen;
			}
		}
		else
		{
			iEnd = _StrLen - 1;
			*_BeginIndex = _StrLen;
		}

		if (iEnd != -1)
		{
			char * getStr = NULL;
			*_ReturnLen = iEnd - iBegin + 1;
			getStr = (char*)malloc(sizeof(char)*(*_ReturnLen));
			memcpy(getStr, _Str + iBegin, *_ReturnLen);
			return getStr;
		}
	}
	return NULL;
}

bool CharsCompare(const char * _Buffer1, const char * _Buffer2, long _Size)
{
	for (long i = 0; i < _Size; i++)
	{
		if (_Buffer1[i] != _Buffer2[i]) return false;
	}
	return true;
}

bool FileCompare(const char * _FileName1, const char * _FileName2)
{
	long lSize1, lSize2;
	bool bSame = true;
	char * buffer1 = ReadAllFile(_FileName1, "rb", &lSize1);
	char * buffer2 = ReadAllFile(_FileName2, "rb", &lSize2);
	if (lSize1 == lSize2)
		bSame = CharsCompare(buffer1, buffer2, lSize1);
	else
		bSame = false;
	free(buffer1);
	free(buffer2);
	buffer1 = NULL;
	buffer2 = NULL;

	return bSame;
}

char * ReadAllFile(const char * _FileName, const char * _Mode, long * _Size)
{
	FILE *fp;
	char * buffer;
	size_t result;
	fp = fopen(_FileName, _Mode);
	if (fp == NULL) return NULL;
	fseek(fp, 0, SEEK_END);
	*_Size = ftell(fp);
	rewind(fp);
	buffer = (char*)malloc(sizeof(char)* *_Size);
	result = fread(buffer, 1, *_Size, fp);
	fclose(fp);
	return buffer;
}

void WriteLog(const char * _Str)
{
	FILE *fp;
	fp = fopen("BoxLog.txt", "a");
	fprintf(fp, "%s\n", _Str);
	fclose(fp);
}