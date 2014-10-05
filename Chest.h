#pragma once

#include "Config.h"
#include "Page.h"
#include "Public.h"

const char split_page[] = { 0x53, 0x54, 0x00, 0x4A, 0x4D };
const char split_item[] = { 0x4A, 0x4D };

typedef struct Chest{
	int checkPageCount;
	int headLen;
	char head[200];

	Page *page;
	int pageCount;
	int pageLength;
}Chest;

Chest ChestRead(const char * _BagFileName);

void ChestWrite(const Chest chest, const char * _BagFileName);

void FreeChest(Chest *chest);

void ChestSort(Chest *chest);

int ChestPutItem(Chest *chest, const Item item, int pageindex);

int ChestPutInsertItem(Chest *chest, const Item item, int pageindex);

void ChestSetPageCount(Chest *chest, int pageindex);

void PageSetItemCount(Page *page, int itemindex);
