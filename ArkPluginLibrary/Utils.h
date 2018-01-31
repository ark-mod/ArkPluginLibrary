#pragma once
#include <windows.h>
#include <sstream>
#include <regex>
#include <cctype>
#include "API/Ark/Ark.h"
#include "Tools.h"
#include "ARK/UE.h"
#include "ARK/Enums.h"
#include "ARK/Actor.h"
#include "json.hpp"
#include "CommandDefinition.h"
#include "FeatureDefinition.h"

namespace ArkLibrary
{
	std::wstring s2ws(const std::string& str);
	std::string ws2s(const std::wstring& str);
	std::string str_tolower(std::string s);
	std::string GetUClassName(UClass* uClass);
	std::string FromWStringToString(const std::wstring &s);
	bool IsPointInsideSphere(FVector point, float sphereX, float sphereY, float sphereZ, float sphereRadius);
	bool IsPointInside2dCircle(FVector point, float circleX, float circleY, float circleRadius);

	std::string GetBlueprintPathStringWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath);
	std::wstring GetBlueprintPathWideStrWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath);
	FString GetBlueprintPathFStringWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath);
	std::string GetBlueprintPathString(const std::string input);
	std::wstring GetBlueprintPathWideStr(const std::string input);
	FString GetBlueprintPathFString(const std::string input);
	std::string GetBlueprintNameString(const std::string input);
	std::wstring GetBlueprintNameWideStr(const std::string input);
	FString GetBlueprintNameFString(const std::string input);
}