#pragma once
#include "Utils.h"

#define DECLARE_COMMAND(name) const std::string CommandName_##name## = #name;
#define DECLARE_FEATURE(name) const std::string FeatureName_##name## = #name;

#define DECLARE_HOOK_INLINE(name, returnType, ...) typedef returnType(__fastcall * name ## _Func)(__VA_ARGS__); \
inline name ## _Func name ## _original; \
returnType __fastcall Hook_ ## name(__VA_ARGS__)

namespace ArkLibrary
{
	void LoadCommandsAndFeatures(nlohmann::json json);
	void ReloadInternalSettingsForCommandsAndFeatures(nlohmann::json json);

	CommandDefinition CreateCommandDefinition(nlohmann::json command_json);
	FeatureDefinition CreateFeatureDefinition(nlohmann::json feature_json);

	CommandDefinition* GetCommand(std::string commandName);
	FeatureDefinition* GetFeature(std::string featureName);

	void AddCommand(std::string commandName, std::function<void(wchar_t*)> action);
	void AddFeature(std::string featureName, std::function<void(FeatureDefinition)> action);
	
	void RemoveCommand(std::string commandName, std::function<void(wchar_t*)> action);
	void RemoveFeature(std::string featureName, std::function<void(FeatureDefinition)> action);
}