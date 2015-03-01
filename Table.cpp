#include "stdafx.h"
#include "Table.h"

long _TableBeginIndex;
long _TableBufferIndex;
int _TableCol;
int _TableRow;

void ResetTable()
{
	_TableBeginIndex = 0;
	_TableBufferIndex = 0;
	_TableCol = 0;
	_TableRow = 0;
}

bool GetTableCell(const char * buf, long lsize, int *iRow, int *iCol, char* cell)
{
	while (_TableBufferIndex < lsize)
	{
		if (buf[_TableBufferIndex] == '\n' || buf[_TableBufferIndex] == '\t')
		{
			//row
			*iRow = _TableRow;
			*iCol = _TableCol;
			memset(cell, 0, 256);
			strncpy(cell, buf + _TableBeginIndex, _TableBufferIndex - _TableBeginIndex);

			if (buf[_TableBufferIndex] == '\n')
			{
				_TableRow++;
				_TableCol = 0;
			}
			else
				_TableCol++;

			_TableBeginIndex = _TableBufferIndex + 1;
			_TableBufferIndex++;
			return true;
		}
		_TableBufferIndex++;
	}
	return false;
}