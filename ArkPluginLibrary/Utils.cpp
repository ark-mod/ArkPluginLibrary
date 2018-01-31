#include "Utils.h"
#include <codecvt>

namespace ArkLibrary
{
	std::wstring s2ws(const std::string& str)
	{
		auto slength = (int)str.length() + 1;
		auto len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
		std::wstring r(len, L'\0');
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, &r[0], len);
		r.resize(r.length() - 1);

		return r;
	}

	std::string ws2s(const std::wstring& str)
	{
		auto slength = (int)str.length() + 1;
		auto len = WideCharToMultiByte(CP_ACP, 0, str.c_str(), slength, 0, 0, 0, 0);
		std::string r(len, '\0');
		WideCharToMultiByte(CP_ACP, 0, str.c_str(), slength, &r[0], len, 0, 0);
		r.resize(r.length() - 1);

		return r;
	}
}

std::string ArkLibrary::FromWStringToString(const std::wstring &s)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;
	std::string u8str = conv1.to_bytes(s);
	return u8str;
}

std::string ArkLibrary::str_tolower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
	return s;
}

std::string ArkLibrary::GetUClassName(UClass* uClass) {
	if (uClass == nullptr) return "[null]";

	FString* className = new FString();
	FName bp = uClass->NameField()();
	bp.ToString(className);
	std::string result = className->ToString();
	delete className;

	return result;
}

bool ArkLibrary::IsPointInsideSphere(FVector point, float sphereX, float sphereY, float sphereZ, float sphereRadius)
{
	long double x = point.X - sphereX;
	long double y = point.Y - sphereY;
	long double z = point.Z - sphereZ;

	long double distancesq = x * x + y * y + z * z;
	return distancesq < sphereRadius * sphereRadius;
}

bool ArkLibrary::IsPointInside2dCircle(FVector point, float circleX, float circleY, float circleRadius)
{
	long double x = point.X - circleX;
	long double y = point.Y - circleY;

	long double distancesq = x * x + y * y;
	return distancesq < circleRadius * circleRadius;
}

// Converts blueprint path strings between different formats with a set of options.
/*
/Game/PrimalEarth/Dinos/Rex/Rex_Character_BP.Rex_Character_BP
/Game/PrimalEarth/Dinos/Rex/Rex_Character_BP.Rex_Character_BP_C
Blueprint'/Game/PrimalEarth/Dinos/Rex/Rex_Character_BP.Rex_Character_BP_C'
Blueprint'/Game/PrimalEarth/Dinos/Rex/Rex_Character_BP.Rex_Character_BP'
*/
std::string ArkLibrary::GetBlueprintPathStringWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath)
{
	const std::regex e("^(Blueprint')?(.+?)(_C)?(')?$");
	std::cmatch cm;
	if (std::regex_match(input.c_str(), cm, e, std::regex_constants::match_default) && cm.size() >= 5 && cm[2].matched)
	{
		std::stringstream ss;
		if (withBlueprintPart) ss << "Blueprint'";
		ss << cm[2].str();
		if (boolWithUnderscoreCPath) ss << "_C";
		if (withBlueprintPart) ss << "'";
		return ss.str();
	}

	return {};
}

std::wstring ArkLibrary::GetBlueprintPathWideStrWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath)
{
	std::string str = GetBlueprintPathStringWithOptions(input, withBlueprintPart, boolWithUnderscoreCPath);
	if (str.empty()) return {};

	return ArkApi::Tools::ConvertToWideStr(str);
}

FString ArkLibrary::GetBlueprintPathFStringWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath)
{
	std::string str = GetBlueprintPathStringWithOptions(input, withBlueprintPart, boolWithUnderscoreCPath);
	if (str.empty()) return {};

	return FString(ArkApi::Tools::ConvertToWideStr(str).c_str());
}

std::string ArkLibrary::GetBlueprintPathString(const std::string input)
{
	return GetBlueprintPathStringWithOptions(input, true, false);
}

std::wstring ArkLibrary::GetBlueprintPathWideStr(const std::string input)
{
	return GetBlueprintPathWideStrWithOptions(input, true, false);
}

FString ArkLibrary::GetBlueprintPathFString(const std::string input)
{
	return GetBlueprintPathFStringWithOptions(input, true, false);
}

std::string ArkLibrary::GetBlueprintNameString(const std::string input)
{
	return GetBlueprintPathStringWithOptions(input, false, true);
}

std::wstring ArkLibrary::GetBlueprintNameWideStr(const std::string input)
{
	return GetBlueprintPathWideStrWithOptions(input, false, true);
}

FString ArkLibrary::GetBlueprintNameFString(const std::string input)
{
	return GetBlueprintPathFStringWithOptions(input, false, true);
}