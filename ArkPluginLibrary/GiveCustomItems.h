#pragma once
#include "Tools.h"
#include "GiveItemsTemplate.h"
#include <list>
#include <iostream>

bool GiveCustomItems(
	unsigned long long steamId,
	std::list<GiveItemDefinition> items);