#include "stdafx.h"
#include "Page.h"

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

void PageNewItem(Page *page, const void * _Item, size_t _Size)
{
	PageSetItemCount(page, page->itemCount);

	Item *item = &(page->item[page->itemCount]);
	memset(item, 0, sizeof(Item));
	memcpy(item, _Item, _Size);

	char * itemcode = GetItemCode(*item);
	item->len = _Size;
	item->invwidth = GetExcelItemInvwidth(itemcode);
	item->invheight = GetExcelItemInvheight(itemcode);
	free(itemcode);
	itemcode = NULL;

	GetItemCol(*item);

	if (item->location != 0)
		page->insertItemCount++;
	page->itemCount++;
}