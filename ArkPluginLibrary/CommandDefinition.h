#pragma once
#include <string>
#include <vector>

namespace ArkLibrary
{
	struct CommandDefinition
	{
		CommandDefinition() {}

		CommandDefinition(std::wstring command, bool enabled, bool permissions, std::vector<__int64> whitelist, nlohmann::json configSection) : Command(command), Enabled(enabled), Permissions(permissions), Whitelist(whitelist), Json(configSection)
		{
		}

		std::wstring Command;
		bool Enabled;
		bool Permissions;
		std::vector<__int64> Whitelist;
		nlohmann::json Json;
	};
}