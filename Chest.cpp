#include "stdafx.h"
#include "Chest.h"

byte table[10][10] = { 0 };

Page* ChestNewPage(Chest* chest)
{
	Page* newPage = NULL;
	ChestSetPageCount(chest, chest->pageCount);
	if (chest->pageCount < chest->pageLength)
	{
		newPage = &chest->page[chest->pageCount];
		PageSetItemCount(newPage, 100);
	}
	chest->pageCount++;
	return newPage;
}

Chest ChestRead(const char * _BagFileName)
{
	//read file
	long lSize = 0;
	char * buffer = ReadAllFile(_BagFileName, "rb", &lSize);

	Chest chest;
	memset(&chest, 0, sizeof(Chest));
	ChestSetPageCount(&chest, 100);

	long iBufferIndex = 0;
	long iBeginIndex = 0;
	Page *page = NULL;
	Item *item = NULL;
	int itemindex = 0;
	chest.itemCount = 0;
	while (iBufferIndex < lSize)
	{
		if (buffer[iBufferIndex] == split_page[0])	//page:0x53, 0x54, 0x00, 0x4A, 0x4D
		{
			//page
			if (iBufferIndex + 5 <= lSize
				&& CharsCompare(buffer + iBufferIndex + 1, split_page + 1, sizeof(split_page)-1))
			{
				if (iBeginIndex == 0)
				{
					//chest head
					chest.headLen = iBufferIndex - iBeginIndex;
					memcpy(chest.head, buffer + iBeginIndex, chest.headLen);
					memcpy(&chest.checkPageCount, chest.head + chest.headLen - 4, 4);

					if (chest.checkPageCount > 0)
					{
						//first page
						page = ChestNewPage(&chest);
						itemindex = 0;
					}
				}
				else
				{
					if (page->fileItemCount > 0)
					{
						PageNewItem(page, buffer + iBeginIndex, iBufferIndex - iBeginIndex);//last item
						chest.itemCount++;
					}

					//new page
					page = ChestNewPage(&chest);
					itemindex = 0;
				}
				iBufferIndex += sizeof(split_page);
				iBeginIndex = iBufferIndex;
				continue;
			}
		}
		else if (buffer[iBufferIndex] == split_item[0])	 //item:0x4A, 0x4D
		{
			if (iBufferIndex + 2 <= lSize
				&& CharsCompare(buffer + iBufferIndex + 1, split_item + 1, sizeof(split_item)-1))
			{
				if (itemindex == 0)
				{
					memcpy(&page->fileItemCount, buffer + iBeginIndex, iBufferIndex - iBeginIndex);
				}
				else
				{
					//check item 
					Item test;
					memcpy(&test, buffer + sizeof(split_item)+iBufferIndex, 1);
					if (test.chk1 != 0 || test.chk2 != 0)
					{
						iBufferIndex += sizeof(split_item);
						continue;
					}

					//item realloc
					PageNewItem(page, buffer + iBeginIndex, iBufferIndex - iBeginIndex);
					chest.itemCount++;
				}
				iBufferIndex += sizeof(split_item);
				iBeginIndex = iBufferIndex;
				itemindex++;
				continue;
			}
		}
		iBufferIndex++;
	}

	if (iBufferIndex == lSize && iBeginIndex != 0)
	{
		if (itemindex == 0)
		{
			//page head
			memcpy(&page->fileItemCount, buffer + iBeginIndex, iBufferIndex - iBeginIndex);
		}
		else
		{
			//item realloc
			PageNewItem(page, buffer + iBeginIndex, iBufferIndex - iBeginIndex);
			chest.itemCount++;
		}
	}

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
	for (int i = 0; i < chest->pageCount; i++)
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

Chest ChestMerge(const char * _ChestFile1, const char * _ChestFile2)
{
	Chest chest1 = ChestRead(_ChestFile1);
	long lSize1 = 0;
	char * buffer1 = ReadAllFile(_ChestFile1, "rb", &lSize1);

	Chest chest2 = ChestRead(_ChestFile2);
	long lSize2 = 0;
	char * buffer2 = ReadAllFile(_ChestFile2, "rb", &lSize2);

	remove("~TmpMerge");
	FILE *fw;
	fw = fopen("~TmpMerge", "wb");
	fwrite(buffer1, sizeof(char), lSize1, fw);
	fwrite(buffer2 + chest2.headLen, sizeof(char), lSize2 - chest2.headLen, fw);
	fclose(fw);

	Chest newChest = ChestRead("~TmpMerge");
	newChest.pageCount = chest1.pageCount + chest2.pageCount;
	ChestSetPageCount(&newChest, newChest.pageCount);

	remove("~TmpMerge");
	FreeChest(&chest1);
	FreeChest(&chest2);
	return newChest;
}

