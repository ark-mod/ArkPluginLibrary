#include "Tools.h"
#include <codecvt>

AShooterPlayerController* FindPlayerControllerFromSteamId(unsigned __int64 steamId)
{
	AShooterPlayerController* result = nullptr;

	auto playerControllers = ArkApi::GetApiUtils().GetWorld()->GetPlayerControllerListField();
	for (uint32_t i = 0; i < playerControllers.Num(); ++i)
	{
		auto playerController = playerControllers[i];

		APlayerState* playerState = playerController->GetPlayerStateField();
		__int64 currentSteamId = playerState->GetUniqueIdField()->UniqueNetId->GetUniqueNetIdField();

		if (currentSteamId == steamId)
		{
			AShooterPlayerController* aShooterPC = static_cast<AShooterPlayerController*>(playerController.Get());

			result = aShooterPC;
			break;
		}
	}

	return result;
}

wchar_t* ConvertToWideStr(const std::string& str)
{
	size_t newsize = str.size() + 1;

	wchar_t* wcstring = new wchar_t[newsize];

	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str.c_str(), _TRUNCATE);

	return wcstring;
}

std::string FromWStringToString(const std::wstring &s)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;
	std::string u8str = conv1.to_bytes(s);
	return u8str;
}

std::string str_tolower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
	return s;
}

std::string GetCurrentDir()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");

	return std::string(buffer).substr(0, pos);
}

__int64 GetSteamId(AShooterPlayerController* playerController)
{
	__int64 steamId = 0;

	APlayerState* playerState = playerController->GetPlayerStateField();
	if (playerState)
	{
		steamId = playerState->GetUniqueIdField()->UniqueNetId->GetUniqueNetIdField();
	}

	return steamId;
}

std::string GetUClassName(UClass* uClass) {
	if (uClass == nullptr) return "[null]";

	FString* className = new FString();
	FName bp = uClass->GetNameField();
	bp.ToString(className);
	std::string result = className->ToString();
	delete className;

	return result;
}

bool IsPointInsideSphere(FVector point, float sphereX, float sphereY, float sphereZ, float sphereRadius)
{
	long double x = point.X - sphereX;
	long double y = point.Y - sphereY;
	long double z = point.Z - sphereZ;

	long double distancesq = x * x + y * y + z * z;
	return distancesq < sphereRadius * sphereRadius;
}

bool IsPointInside2dCircle(FVector point, float circleX, float circleY, float circleRadius)
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
std::string GetBlueprintPathStringWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath)
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

wchar_t* GetBlueprintPathWideStrWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath)
{
	std::string str = GetBlueprintPathStringWithOptions(input, withBlueprintPart, boolWithUnderscoreCPath);
	if (str.empty()) return {};

	return ConvertToWideStr(str);
}

FString GetBlueprintPathFStringWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath)
{
	std::string str = GetBlueprintPathStringWithOptions(input, withBlueprintPart, boolWithUnderscoreCPath);
	if (str.empty()) return {};

	//todo: does FString release this wchar_t*?
	return FString(ConvertToWideStr(str));
}

std::string GetBlueprintPathString(const std::string input)
{
	return GetBlueprintPathStringWithOptions(input, true, false);
}

wchar_t* GetBlueprintPathWideStr(const std::string input)
{
	return GetBlueprintPathWideStrWithOptions(input, true, false);
}

FString GetBlueprintPathFString(const std::string input)
{
	return GetBlueprintPathFStringWithOptions(input, true, false);
}

std::string GetBlueprintNameString(const std::string input)
{
	return GetBlueprintPathStringWithOptions(input, false, true);
}

wchar_t* GetBlueprintNameWideStr(const std::string input)
{
	return GetBlueprintPathWideStrWithOptions(input, false, true);
}

FString GetBlueprintNameFString(const std::string input)
{
	return GetBlueprintPathFStringWithOptions(input, false, true);
}