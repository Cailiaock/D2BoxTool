#include "stdafx.h"
#include "Chest.h"

byte table[10][10] = { 0 };

Chest ChestRead(const char * _BagFileName)
{
	Chest chest;
	memset(&chest, 0, sizeof(Chest));
	//chest.pageLength = 100;
	ChestSetPageCount(&chest, 100);

	//read file
	long lSize;
	char * buffer = ReadAllFile(_BagFileName, "rb", &lSize);

	int split_Page_Begin = 0;
	int split_Page_ReturnLen = 0;
	int split_Page_Index = 0;
	char * pPageSplit = strsplit(buffer, split_page, lSize, sizeof(split_page), &split_Page_Begin, &split_Page_ReturnLen);
	while (pPageSplit != NULL)
	{
		if (split_Page_Index == 0)
		{
			//head
			chest.headLen = split_Page_ReturnLen;
			memcpy(chest.head, pPageSplit, chest.headLen);
			memcpy(&chest.checkPageCount, chest.head + chest.headLen - 4, 4);
		}
		else
		{
			//page realloc
			ChestSetPageCount(&chest, chest.pageCount);

			if (chest.pageCount < chest.pageLength)
			{
				Page *page = &chest.page[chest.pageCount];
				//item
				//page->itemLength = 100;
				PageSetItemCount(page, 100);

				int split_Item_Begin = 0;
				int split_Item_ReturnLen = 0;
				int split_Item_Index = 0;
				char * pItemSplit = strsplit(pPageSplit, split_item, split_Page_ReturnLen, sizeof(split_item), &split_Item_Begin, &split_Item_ReturnLen);
				while (pItemSplit != NULL)
				{
					if (split_Item_Index > 0)
					{
						//item realloc
						PageSetItemCount(page, page->itemCount);

						Item *item = &(page->item[page->itemCount]);
						memset(item, 0, sizeof(Item));
						memcpy(item, pItemSplit, split_Item_ReturnLen);

						char * itemcode = GetItemCode(*item);
						item->len = split_Item_ReturnLen;
						item->invwidth = GetExcelItemInvwidth(itemcode);
						item->invheight = GetExcelItemInvheight(itemcode);
						free(itemcode);
						itemcode = NULL;

						if (item->location != 0)
							page->insertItemCount++;
						page->itemCount++;
					}
					//next split
					split_Item_Index++;
					free(pItemSplit);
					pItemSplit = strsplit(pPageSplit, split_item, split_Page_ReturnLen, sizeof(split_item), &split_Item_Begin, &split_Item_ReturnLen);
				}
				free(pItemSplit);
				pItemSplit = NULL;
			}
			chest.pageCount++;
		}
		//next split
		split_Page_Index++;
		free(pPageSplit);
		pPageSplit = strsplit(buffer, split_page, lSize, sizeof(split_page), &split_Page_Begin, &split_Page_ReturnLen);
	}
	free(pPageSplit);
	pPageSplit = NULL;
	free(buffer);
	buffer = NULL;
	return chest;
}

void ChestWrite(const Chest chest, const char * _BagFileName)
{
	FILE *fw;
	fw = fopen(_BagFileName, "wb");

	//write head
	fwrite(chest.head, sizeof(char), chest.headLen - 4, fw);

	//write pagecount
	fwrite(&chest.pageCount, sizeof(int), 1, fw);

	for (int i = 0; i < chest.pageCount; i++)
	{
		//write page head
		fwrite(split_page, sizeof(char), 5, fw);

		//write page item count
		short pageItemCount = chest.page[i].itemCount - chest.page[i].insertItemCount;
		fwrite(&pageItemCount, sizeof(short), 1, fw);

		//write page items
		for (int j = 0; j < chest.page[i].itemCount; j++)
		{
			fwrite(split_item, sizeof(char), 2, fw);
			fwrite(&chest.page[i].item[j], sizeof(byte), chest.page[i].item[j].len, fw);
		}
	}

	fclose(fw);
}

void FreeChest(Chest *chest)
{
	for (int i = 0; i < chest->pageLength; i++)
	{
		if (chest->page[i].item != NULL)
		{
			free(chest->page[i].item);
			chest->page[i].item = NULL;
		}
	}
	free(chest->page);
	chest->page = NULL;
	chest->checkPageCount = 0;
	chest->pageCount = 0;
	chest->pageLength = 10;
}

void ChestSort(Chest *chest)
{
	int allitemlength = 1000;
	int allitemcount = 0;
	Item * allitems = (Item *)malloc(sizeof(Item)*allitemlength);
	for (int i = 0; i < chest->pageCount; i++)
	{
		for (int j = 0; j < chest->page[i].itemCount; j++)
		{
			//realloc
			if (allitemcount == allitemlength)
			{
				allitemlength = allitemlength << 1;
				allitems = (Item *)realloc(allitems, sizeof(Item)*allitemlength);
			}
			allitems[allitemcount] = chest->page[i].item[j];
			allitems[allitemcount].putted = false;
			allitemcount++;
		}
	}

	FreeChest(chest);

	for (int i = 0; i < ConfigsCount; i++)
	{
		for (int k = 0; k < ItemTypesCount; k++)
		{
			if (strcmp(ItemTypes[k].type, Configs[i].type) == 0)
			{
				for (int j = 0; j < allitemcount; j++)
				{
					if (!allitems[j].putted && allitems[j].location == 0)
					{
						char * code = GetItemCode(allitems[j]);

						if (strncmp(code, ItemTypes[k].code, 3) == 0
							&& (
							Configs[i].quality == 0 || Configs[i].quality == GetItemQuality(allitems[j])
							))
						{
							int iOldPage = ChestPutItem(chest, allitems[j], Configs[i].page);
							allitems[j].putted = true;

							int next = j + 1;
							while (next < allitemcount && allitems[next].location != 0 && !allitems[next].putted)
							{
								ChestPutInsertItem(chest, allitems[next], iOldPage);
								allitems[next].putted = true;
								next++;
							}
						}
						free(code);
					}
				}
			}
		}
	}

	int iOldPage;
	for (int i = 0; i < allitemcount; i++)
	{
		if (!allitems[i].putted)
		{
			if (allitems[i].location == 0)
			{
				iOldPage = ChestPutItem(chest, allitems[i], 0);
			}
			else
			{
				ChestPutInsertItem(chest, allitems[i], iOldPage);
			}
			allitems[i].putted = true;
		}
	}
	free(allitems);

	//delete empty page
	if (chest->page != NULL)
	{
		chest->pageCount = chest->pageLength;
		while (chest->page[chest->pageCount - 1].itemCount == 0)
		{
			free(chest->page[chest->pageCount - 1].item);
			chest->pageCount--;
		}
	}
}

int ChestPutItem(Chest *chest, const Item item, int pageindex)
{
	ChestSetPageCount(chest, pageindex);

	Page *page = &chest->page[pageindex];

	memset(table, 0, sizeof(table));

	byte col = 0;
	byte row = 0;
	for (int i = 0; i < page->itemCount; i++)
	{
		if (page->item[i].location != 0) continue;
		col = GetItemCol(page->item[i]);
		row = GetItemRow(page->item[i]);

		for (int x = col; x < col + page->item[i].invwidth; x++)
		{
			for (int y = row; y < row + page->item[i].invheight; y++)
			{
				table[x][y] = 1;
			}
		}
	}

	Item newItem = item;
	for (int y = 0; y < 11 - item.invheight; y++)
	{
		for (int x = 0; x < 11 - item.invwidth; x++)
		{
			bool test = false;
			for (int i = 0; i < newItem.invwidth; i++)
			{
				for (int j = 0; j < newItem.invheight; j++)
				{
					if (table[x + i][y + j] == 1)
					{
						test = true;
						break;
					}
				}
				if (test) break;
			}

			if (test)
				continue;
			else
			{
				//put item
				SetItemCol(&newItem, x);
				SetItemRow(&newItem, y);

				if (page->itemLength == 0)
				{
					//page->itemLength = 100;
					PageSetItemCount(page, 100);
				}

				//item realloc 
				PageSetItemCount(page, page->itemCount);

				page->item[page->itemCount] = newItem;
				page->itemCount++;


				return pageindex;
			}
		}
	}
	return ChestPutItem(chest, item, pageindex + 1);
}

int ChestPutInsertItem(Chest *chest, const Item item, int pageindex)
{
	Page *page = &chest->page[pageindex];
	//item realloc 
	PageSetItemCount(page, page->itemCount);
	page->item[page->itemCount] = item;
	page->itemCount++;
	page->insertItemCount++;
	return pageindex;
}

void ChestSetPageCount(Chest *chest, int pageindex)
{
	if (chest->pageLength == 0)
		chest->pageLength = pageindex;

	if (chest->page == NULL)
	{
		chest->page = (Page *)malloc(sizeof(Page)*chest->pageLength);
		memset(chest->page, 0, sizeof(Page)* chest->pageLength);
	}

	while (pageindex >= chest->pageLength)
	{
		chest->pageLength = chest->pageLength << 1;
		chest->page = (Page *)realloc(chest->page, sizeof(Page)*chest->pageLength);
		memset(chest->page + (chest->pageLength >> 1), 0, sizeof(Page)* (chest->pageLength >> 1));
	}
}

void PageSetItemCount(Page *page, int itemindex)
{
	if (page->itemLength == 0)
		page->itemLength = itemindex;

	if (page->item == NULL)
	{
		page->item = (Item *)malloc(sizeof(Item)*page->itemLength);
		memset(page->item, 0, sizeof(Item)* page->itemLength);
	}

	while (itemindex >= page->itemLength)
	{
		page->itemLength = page->itemLength << 1;
		page->item = (Item *)realloc(page->item, sizeof(Item)*page->itemLength);
		memset(page->item + (page->itemLength >> 1), 0, sizeof(Page)* (page->itemLength >> 1));
	}
}