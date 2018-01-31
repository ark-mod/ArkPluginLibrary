#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <list>
#include <map>
#include "ArkPluginLibrary.h"

namespace ArkLibrary
{
	std::map<std::string, FeatureDefinition> features;
	std::map<std::string, CommandDefinition> commands;
	std::list<wchar_t*> commandNames;

	void LoadCommandsAndFeatures(nlohmann::json json)
	{
		commands.clear();

		auto commandJson = json.find("Commands");
		if (commandJson != json.end())
		{
			for (auto it = commandJson->begin(); it != commandJson->end(); ++it)
			{
				commands.emplace(it.key(), CreateCommandDefinition(it.value()));
			}
		}

		features.clear();

		auto featureJson = json.find("Features");
		if (featureJson != json.end())
		{
			for (auto it = featureJson->begin(); it != featureJson->end(); ++it)
			{
				features.emplace(it.key(), CreateFeatureDefinition(it.value()));
			}
		}
	}

	void ReloadInternalSettingsForCommandsAndFeatures(nlohmann::json json)
	{
		auto commandJson = json.find("Commands");
		if (commandJson != json.end())
		{
			for (auto it = commandJson->begin(); it != commandJson->end(); ++it)
			{
				auto fit = commands.find(it.key());
				if (fit == commands.end()) continue;

				auto command = CreateCommandDefinition(it.value());
				fit->second.Whitelist = command.Whitelist;
				fit->second.Json = command.Json;
			}
		}

		auto featureJson = json.find("Features");
		if (featureJson != json.end())
		{
			for (auto it = featureJson->begin(); it != featureJson->end(); ++it)
			{
				auto fit = features.find(it.key());
				if (fit == features.end()) continue;

				auto feature = CreateFeatureDefinition(it.value());
				fit->second.Json = feature.Json;
			}
		}
	}

	CommandDefinition CreateCommandDefinition(nlohmann::json command_json)
	{
		std::vector<__int64> whitelist;
		auto whitelistJson = command_json.find("Whitelist");
		if (whitelistJson != command_json.end())
		{
			for (auto itW = whitelistJson->begin(); itW != whitelistJson->end(); ++itW)
			{
				whitelist.push_back(*itW);
			}
		}

		return CommandDefinition(s2ws(command_json.value("Command", "")), command_json.value("Enabled", true), command_json.value("Permissions", false), whitelist, command_json);
	}

	FeatureDefinition CreateFeatureDefinition(nlohmann::json feature_json)
	{
		return FeatureDefinition(feature_json.value("Enabled", false), feature_json);
	}

	CommandDefinition* GetCommand(std::string commandName)
	{
		auto it = commands.find(commandName);
		if (it != commands.end())
		{
			return &(it->second);
		}

		return nullptr;
	}

	void AddCommand(std::string commandName, std::function<void(wchar_t*)> action)
	{
		auto it = commands.find(commandName);
		if (it != commands.end())
		{
			auto command = it->second;
			if (command.Enabled && command.Command.length() > 0)
			{
				auto len = command.Command.length() + 1;
				auto name = new wchar_t[len];
				wcscpy_s(name, len, command.Command.c_str());

				commandNames.push_back(name);
				action(name);
			}
		}
	}

	void RemoveCommand(std::string commandName, std::function<void(wchar_t*)> action)
	{
		auto it = commands.find(commandName);
		if (it != commands.end())
		{
			auto command = it->second;
			if (command.Enabled && command.Command.length() > 0)
			{
				auto len = command.Command.length() + 1;
				auto name = new wchar_t[len];
				wcscpy_s(name, len, command.Command.c_str());

				commandNames.remove_if([&name](wchar_t* str) { return wcscmp(str, name) == 0; });
				action(name);
			}
		}
	}

	FeatureDefinition* GetFeature(std::string featureName)
	{
		auto it = features.find(featureName);
		if (it != features.end())
		{
			return &(it->second);
		}

		return nullptr;
	}

	void AddFeature(std::string featureName, std::function<void(FeatureDefinition)> action)
	{
		auto it = features.find(featureName);
		if (it != features.end())
		{
			auto feature = it->second;
			if (feature.Enabled)
			{
				action(feature);
			}
		}
	}

	void RemoveFeature(std::string featureName, std::function<void(FeatureDefinition)> action)
	{
		auto it = features.find(featureName);
		if (it != features.end())
		{
			auto feature = it->second;
			if (feature.Enabled)
			{
				action(feature);
			}
		}
	}
}