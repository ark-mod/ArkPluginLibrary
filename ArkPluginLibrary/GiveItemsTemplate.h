#pragma once

#include <list>
#include <string>

struct GiveItemDefinition
{
	std::string blueprint;
	std::list<std::string> blueprints;
	int quantity;
	int count;
	float quality;
};