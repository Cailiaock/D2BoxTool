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
	fp = fopen("Log.txt", "a");
	fprintf(fp, "%s\n", _Str);
	fclose(fp);
}