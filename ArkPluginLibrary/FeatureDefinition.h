#pragma once
#include <string>
#include <vector>
#include "json.hpp"

namespace ArkLibrary
{
	struct FeatureDefinition
	{
		FeatureDefinition() {}

		FeatureDefinition(bool enabled, nlohmann::json json) : Enabled(enabled), Json(json)
		{
		}

		bool Enabled;
		nlohmann::json Json;
	};
}