#pragma once

/*
QuestItem 16, 1
Location 58, 3
Col 65, 4
Row 69, 4
ItemCode 76, 32
Quality = 150, 4
*/
typedef struct Item
{
	//byte data1[2];  -- 4a 4d 
	byte questitem : 1;	//16 1
byte: 7;				//unknown

	byte data2[4];

byte: 2;				//unknown
	byte location : 3;	//58 3
byte: 3;				//unknown

byte: 1;				//unknown
	byte col : 4;		//65 4
	byte row1 : 3;		//69 4

	byte row2 : 1;
byte: 3;				//unknown
	byte itemcode1 : 4;

	byte itemcode2 : 4;
	byte itemcode3 : 4;

	byte itemcode4 : 4;
	byte itemcode5 : 4;

	byte itemcode6 : 4;
	byte itemcode7 : 4;

	byte itemcode8 : 4;
byte: 4;				//unknown

	byte data3[4];

byte:6;					//unknown
	byte quality1 : 2;

	byte quality2 : 2;
byte:6;					//unknown

	byte data[300];		//应付修改变态的Item

	/*
	========================================
	*/
	int len;			//item buffer length
	int invwidth;
	int invheight;
	bool putted;
}Item;

byte GetItemCol(Item item);

byte GetItemRow(Item item);

char * GetItemCode(Item item);

byte GetItemQuality(Item item);

void SetItemCol(Item *item, byte col);

void SetItemRow(Item *item, byte row);

void SetItemCode(Item *item, char code[4]);

void SetItemQuality(Item *item, byte quality);