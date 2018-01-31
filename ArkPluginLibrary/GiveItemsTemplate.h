#pragma once

#include <list>
#include <string>

namespace ArkLibrary
{
	struct GiveItemDefinition
	{
		std::string blueprint;
		std::list<std::string> blueprints;
		int quantity;
		int count;
		float quality;
	};
}