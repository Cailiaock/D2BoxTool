#pragma once

#include "Item.h"

typedef struct Page
{
	Item * item;
	int itemLength;
	int itemCount;
	int insertItemCount;
}Page;