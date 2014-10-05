#include "stdafx.h"
#include "Item.h"

byte GetItemCol(Item item)
{
	return item.col;
}

byte GetItemRow(Item item)
{
	return item.row1 + (item.row2 << 3);
}

char * GetItemCode(Item item)
{
	char * itemcode = (char*)malloc(sizeof(char)* 4);
	itemcode[0] = item.itemcode1 + (item.itemcode2 << 4);
	itemcode[1] = item.itemcode3 + (item.itemcode4 << 4);
	itemcode[2] = item.itemcode5 + (item.itemcode6 << 4);
	itemcode[3] = item.itemcode7 + (item.itemcode8 << 4);
	return itemcode;
}

byte GetItemQuality(Item item)
{
	return item.quality1 + (item.quality2 << 2);
}

void SetItemCol(Item *item, byte col)
{
	(*item).col = col;
}

void SetItemRow(Item *item, byte row)
{
	(*item).row1 = row & 0x7;
	(*item).row2 = (row & 0xf) >> 3;
}

void SetItemCode(Item *item, char code[4])
{
	(*item).itemcode1 = code[0] & 0xf;
	(*item).itemcode2 = code[0] >> 4;
	(*item).itemcode3 = code[1] & 0xf;
	(*item).itemcode4 = code[1] >> 4;
	(*item).itemcode5 = code[2] & 0xf;
	(*item).itemcode6 = code[2] >> 4;
	(*item).itemcode7 = code[3] & 0xf;
	(*item).itemcode8 = code[3] >> 4;
}

void SetItemQuality(Item *item, byte quality)
{
	(*item).quality1 = quality & 0x3;
	(*item).quality2 = (quality & 0xf) >> 2;
}