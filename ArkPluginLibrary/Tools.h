#pragma once
#include <windows.h>
#include <sstream>
#include <regex>
#include <cctype>
#include "API/Ark.h"

template <typename... Args>
void SendChatMessage(AShooterPlayerController* playerController, const FString& senderName, const wchar_t* msg, Args&&... args)
{
	size_t size = swprintf(nullptr, 0, msg, std::forward<Args>(args)...) + 1;

	wchar_t* buffer = new wchar_t[size];
	_snwprintf_s(buffer, size, _TRUNCATE, msg, std::forward<Args>(args)...);

	FString cmd(buffer);

	FChatMessage* chatMessage = static_cast<FChatMessage*>(malloc(sizeof(FChatMessage)));
	if (chatMessage)
	{
		chatMessage->SenderName = senderName;
		chatMessage->SenderSteamName = L"";
		chatMessage->SenderTribeName = L"";
		chatMessage->SenderId = 0;
		chatMessage->Message = cmd;
		chatMessage->Receiver = L"";
		chatMessage->SenderTeamIndex = 0;
		chatMessage->ReceivedTime = -1;
		chatMessage->SendMode = EChatSendMode::GlobalChat;
		chatMessage->RadioFrequency = 0;
		chatMessage->ChatType = EChatType::GlobalChat;
		chatMessage->SenderIcon = 0;
		chatMessage->UserId = L"";

		void* mem = malloc(sizeof(FChatMessage));
		if (mem)
		{
			FChatMessage* chat = new(mem) FChatMessage(chatMessage);

			playerController->ClientChatMessage(chat);

			chat->~FChatMessage();
			free(mem);
		}

		free(chatMessage);
	}

	delete[] buffer;
}

template <typename... Args>
void SendDirectMessage(AShooterPlayerController* playerController, const wchar_t* msg, Args&&... args)
{
	size_t size = swprintf(nullptr, 0, msg, std::forward<Args>(args)...) + 1;

	wchar_t* buffer = new wchar_t[size];
	_snwprintf_s(buffer, size, _TRUNCATE, msg, std::forward<Args>(args)...);

	FString cmd(buffer);

	FLinearColor msgColor = { 1,1,1,1 };
	playerController->ClientServerChatDirectMessage(&cmd, msgColor, false);

	delete[] buffer;
}

template <typename... Args>
void SendNotification(AShooterPlayerController* playerController, const wchar_t* msg, FLinearColor color, float displayScale, float displayTime, UTexture2D* icon, Args&&... args)
{
	size_t size = swprintf(nullptr, 0, msg, std::forward<Args>(args)...) + 1;

	wchar_t* buffer = new wchar_t[size];
	_snwprintf_s(buffer, size, _TRUNCATE, msg, std::forward<Args>(args)...);

	FString cmd(buffer);

	playerController->ClientServerSOTFNotificationCustom(&cmd, color, displayScale, displayTime, icon, nullptr);

	delete[] buffer;
}

AShooterPlayerController* FindPlayerControllerFromSteamId(unsigned __int64 steamId);
wchar_t* ConvertToWideStr(const std::string& str);
std::string str_tolower(std::string s);
std::string GetCurrentDir();
__int64 GetSteamId(AShooterPlayerController* playerController);
std::string GetUClassName(UClass* uClass);
std::string FromWStringToString(const std::wstring &s);
bool IsPointInsideSphere(FVector point, float sphereX, float sphereY, float sphereZ, float sphereRadius);
bool IsPointInside2dCircle(FVector point, float circleX, float circleY, float circleRadius);

std::string GetBlueprintPathStringWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath);
wchar_t* GetBlueprintPathWideStrWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath);
FString GetBlueprintPathFStringWithOptions(const std::string input, const bool withBlueprintPart, const bool boolWithUnderscoreCPath);
std::string GetBlueprintPathString(const std::string input);
wchar_t* GetBlueprintPathWideStr(const std::string input);
FString GetBlueprintPathFString(const std::string input);
std::string GetBlueprintNameString(const std::string input);
wchar_t* GetBlueprintNameWideStr(const std::string input);
FString GetBlueprintNameFString(const std::string input);