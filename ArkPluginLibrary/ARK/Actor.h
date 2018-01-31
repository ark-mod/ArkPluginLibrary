#pragma once
#include "API/Ark/Ark.h"

namespace ArkExtensions
{
	struct APlayerController : ::AController
	{
		BitFieldValue<bool, unsigned __int32> bIsAdminField() { return { this, "APlayerController.bIsAdmin" }; }
	};

	struct AShooterPlayerController : APlayerController, ::APlayerController
	{
		BitFieldValue<bool, unsigned __int32> bIsAdminPauseUIEnabledField() { return { this, "AShooterPlayerController.bIsAdminPauseUIEnabled" }; }
	};

	struct UPrimalCharacterStatusComponent
	{
		BitFieldValue<bool, unsigned __int32> bInitializedBaseLevelMaxStatusValuesField() { return { this, "UPrimalCharacterStatusComponent.bInitializedBaseLevelMaxStatusValues" }; }
	};

	struct APrimalDinoCharacter : ::APrimalCharacter
	{
		BitFieldValue<bool, unsigned __int32> bIgnoreAllWhistlesField() { return { this, "APrimalDinoCharacter.bIgnoreAllWhistles" }; }
		BitFieldValue<bool, unsigned __int32> bIgnoreAllyLookField() { return { this, "APrimalDinoCharacter.bIgnoreAllyLook" }; }
	};

	struct ADroppedItem : ::AActor
	{
		static UClass* StaticClass() { return NativeCall<UClass *>(nullptr, "ADroppedItem.StaticClass"); }
	};
}