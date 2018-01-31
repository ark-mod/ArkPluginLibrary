#pragma once
#include "Utils.h"
#include "GiveItemsTemplate.h"
#include <list>
#include <iostream>

namespace ArkLibrary
{
	bool GiveCustomItems(
		unsigned long long steamId,
		std::list<GiveItemDefinition> items);
}