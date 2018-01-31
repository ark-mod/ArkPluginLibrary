#include "SpawnCustomDino.h"
#include "IApiUtils.h"

bool ArkLibrary::SpawnCustomDino(
	unsigned long long steamId,
	std::string bpPath, std::string bpPathSaddle,
	int dinoBaseLevelHealth, int dinoBaseLevelStamina, int dinoBaseLevelOxygen, int dinoBaseLevelFood, int dinoBaseLevelWeight, int dinoBaseLevelMeleeDamage, int dinoBaseLevelMovementSpeed,
	int dinoTamedLevelHealth, int dinoTamedLevelStamina, int dinoTamedLevelOxygen, int dinoTamedLevelFood, int dinoTamedLevelWeight, int dinoTamedLevelMeleeDamage, int dinoTamedLevelMovementSpeed,
	float saddleArmor,
	float imprint,
	std::list<GiveItemDefinition> items,
	float offsetX, float offsetY, float offsetZ, 
	bool follow, 
	AggressionLevel aggressionLevel, 
	bool ignoreAllWhistles, 
	bool ignoreAllyLook,
	float facingModDegrees)
{
	if (bpPath.empty()) return false;

	AShooterPlayerController* aShooterPC = ArkApi::GetApiUtils().FindPlayerFromSteamId(steamId);
	if (aShooterPC)
	{
		//todo: does FString release the underlying wchar_t*?
		FString bpPathFString = GetBlueprintPathFString(bpPath);

		//todo: raycast position (?)
		//UWorld* world = Ark::GetWorld();
		//FVector pos = aShooterPC->GetDefaultActorLocationField();
		//FHitResult hit;
		//Globals::RaycastSingle(world, &hit, pos, )

		AActor* actor = aShooterPC->SpawnActor(&bpPathFString, offsetX, offsetY, offsetZ, true);
		if (actor && actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass()))
		{
			APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(actor);
			auto dinoEx = static_cast<ArkExtensions::APrimalDinoCharacter*>(actor);

			if (facingModDegrees > 0.0)
			{
				FRotator rot = dino->RootComponentField()()->RelativeRotationField()();
				rot.Yaw += facingModDegrees;
				dino->SetActorRotation(rot); //todo: had to change from &rot with v2 (does it work?)
			}

			dino->AbsoluteBaseLevelField() = 1; //temp set to 1 in order to avoid random level assignment

			APlayerState* playerState = aShooterPC->PlayerStateField()();
			AShooterPlayerState* shooterPlayerState = static_cast<AShooterPlayerState*>(aShooterPC->PlayerStateField()());

			dino->TamingTeamIDField() = aShooterPC->TargetingTeamField()();

			FString* playerName = new FString();
			shooterPlayerState->GetPlayerName(playerName);
			//NOTE: Getting the player name from playerState->GetPlayerNameField() causes crash

			FString tamer = dino->TamerStringField()();
			tamer = *playerName;

			//todo: memory leak? playerName not deleted 

			shooterPlayerState->SetTribeTamingDinoSettings(dino);

			/*__int32 flag = GetNativeField<__int32>(dino, "APrimalDinoCharacter", "_bf128c");
			SetNativeField(dino, "APrimalDinoCharacter", "_bf128c", flag | 0x80000);*/
			/*__int32 flag = *reinterpret_cast<__int32*>(reinterpret_cast<DWORD64>(dino) + static_cast<DWORD64>(0x128c));
			*reinterpret_cast<__int32*>(reinterpret_cast<DWORD64>(dino) + static_cast<DWORD64>(0x128c)) = flag | 0x80000;*/

			dino->TameDino(aShooterPC, false, 0); //todo: new arguments in v2 (ignoreMaxTameLimit and overrideTamingTeamId) (does it work?)

			dino->BeginPlay();

			//option: follow
			if (!follow) dino->TamedFollowTargetField() = TWeakObjectPtr<AActor>();

			//option: aggression level
			dino->TamedAggressionLevelField() = static_cast<int>(aggressionLevel);

			//option: ignore all whistles
			if (ignoreAllWhistles) dinoEx->bIgnoreAllWhistlesField() = true;

			//option: ignore ally look
			if (ignoreAllyLook) dinoEx->bIgnoreAllyLookField() = true;

			UPrimalCharacterStatusComponent* status = dino->MyCharacterStatusComponentField()();
			auto statusEx = reinterpret_cast<ArkExtensions::UPrimalCharacterStatusComponent*>(status);
			if (status)
			{
				// 0: health
				// 1: stamina
				// 2: torpor
				// 3: oxygen
				// 4: food
				// 5: water
				// 6: temperature
				// 7: weight
				// 8: melee damage
				// 9: movement speed
				// 10: fortitude
				// 11: crafting speed

				char* stats = status->NumberOfLevelUpPointsAppliedField()();
				stats[0] = dinoBaseLevelHealth;
				stats[1] = dinoBaseLevelStamina;
				stats[3] = dinoBaseLevelOxygen;
				stats[4] = dinoBaseLevelFood;
				stats[7] = dinoBaseLevelWeight;
				stats[8] = dinoBaseLevelMeleeDamage;
				stats[9] = dinoBaseLevelMovementSpeed;

				status->BaseCharacterLevelField() = 1
					+ dinoBaseLevelHealth 
					+ dinoBaseLevelStamina 
					+ dinoBaseLevelOxygen 
					+ dinoBaseLevelFood 
					+ dinoBaseLevelWeight 
					+ dinoBaseLevelMeleeDamage 
					+ dinoBaseLevelMovementSpeed;

				status->ExtraCharacterLevelField() = dinoTamedLevelHealth
					+ dinoTamedLevelStamina 
					+ dinoTamedLevelOxygen 
					+ dinoTamedLevelFood 
					+ dinoTamedLevelWeight 
					+ dinoTamedLevelMeleeDamage 
					+ dinoTamedLevelMovementSpeed;

				
				//NOTE: better to set the actual xp because this call gets scaled by configuration settings for different xp types (generic by default is 2x)
				//dino->GetMyCharacterStatusComponentField()->AddExperience(status->GetExperienceRequiredForPreviousLevelUp(), false, EXPType::XP_GENERIC);
				
				status->ExperiencePointsField() = status->GetExperienceRequiredForPreviousLevelUp();

				char* statsTamed = status->NumberOfLevelUpPointsAppliedTamedField()();
				statsTamed[0] = dinoTamedLevelHealth;
				statsTamed[1] = dinoTamedLevelStamina;
				statsTamed[3] = dinoTamedLevelOxygen;
				statsTamed[4] = dinoTamedLevelFood;
				statsTamed[7] = dinoTamedLevelWeight;
				statsTamed[8] = dinoTamedLevelMeleeDamage;
				statsTamed[9] = dinoTamedLevelMovementSpeed;

				//status->SetNumberOfLevelUpPointsAppliedTamedField(stats);

				statusEx->bInitializedBaseLevelMaxStatusValuesField() = true;
			}

			//getting the saddle from the dino class is not working
			/*TSubclassOf<UPrimalItem> saddleItemClass = dino->GetSaddleItemClassField();

			std::cout << (saddleItemClass.uClass == NULL ? "saddleItemClass NULL" : "saddleItemClass not null") << "\n";

			if (saddleItemClass.uClass)
			{
			UPrimalItem* saddle = static_cast<UPrimalItem*>(saddleItemClass.uClass->GetDefaultObject(true));
			FString* bpPath = new FString();
			saddle->GetPathName(NULL, bpPath);

			std::cout << bpPath->ToString() << "\n";

			delete bpPath;
			}*/

			//add saddle
			if (!bpPathSaddle.empty())
			{
				std::wstring bpPathSaddleWStr = GetBlueprintNameWideStr(bpPathSaddle);

				UObject* object = Globals::StaticLoadObject(UObject::StaticClass(), nullptr, bpPathSaddleWStr.c_str(), nullptr, 0, 0, true);

				if (object && object->IsA(UClass::StaticClass()))
				{
					TSubclassOf<UPrimalItem> archetype;
					archetype.uClass = reinterpret_cast<UClass*>(object);

					AShooterCharacter* aShooterCharacter = static_cast<AShooterCharacter*>(aShooterPC->CharacterField()());

					UPrimalItem* saddle = UPrimalItem::AddNewItem(archetype, dino->MyInventoryComponentField()(), true, false, 0.0, false, 0, false, 0.0, false, TSubclassOf<UPrimalItem>());
					if (saddleArmor >= 0)
					{
						unsigned short* statValues = saddle->ItemStatValuesField()();
						if (statValues != nullptr) statValues[1] = (unsigned short)std::floor(((saddleArmor - 25.0) / 5.0) * 1000.0);

						saddle->UpdatedItem();
					}
				}

				//give items
				for (std::list<GiveItemDefinition>::iterator it = items.begin(); it != items.end(); ++it)
				{
					std::wstring itemBlueprint = GetBlueprintNameWideStr(it->blueprint);
					UObject* object = Globals::StaticLoadObject(UObject::StaticClass(), nullptr, itemBlueprint.c_str(), nullptr, 0, 0, true);
					if (object)
					{
						TSubclassOf<UPrimalItem> archetype;
						archetype.uClass = reinterpret_cast<UClass*>(object);

						AShooterCharacter* aShooterCharacter = static_cast<AShooterCharacter*>(aShooterPC->CharacterField()());

						for (int n = 0; n < it->count; n++)
						{
							UPrimalItem* item = UPrimalItem::AddNewItem(archetype, dino->MyInventoryComponentField()(), false, false, 0.0, false, it->quantity, false, 0.0, false, TSubclassOf<UPrimalItem>());
						}
					}
				}
			}

			if (imprint > 0.0)
			{
				long long playerId = aShooterPC->LinkedPlayerIDField()();
				FString* playerName2 = new FString();
				aShooterPC->GetPlayerCharacterName(playerName2);

				dino->UpdateImprintingDetails(playerName2, playerId);
				//todo: memory leak? playerName2 not deleted
				dino->UpdateImprintingQuality(imprint > 1.0 ? 1.0 : imprint);
			}
			else
			{
				dino->UpdateImprintingQuality(0.0);
			}

			if (status)
			{
				/*v75->MaxStatusValues[0] = *(float *)&a2 * v75->MaxStatusValues[0];
				v76 = v3->MyCharacterStatusComponent;
				*(_OWORD *)&a2 = LODWORD(v76->MaxStatusValues[0]);
				if (*(float *)&a2 < v76->CurrentStatusValues[0])
				LODWORD(v76->CurrentStatusValues[0]) = (_DWORD)a2;*/

				float* maxStatsValues = status->MaxStatusValuesField()();
				float wildRandomScale = dino->WildRandomScaleField()();
				float* currentStatValues = status->CurrentStatusValuesField()();
				currentStatValues[0] = maxStatsValues[0]; //health (calculated as maxStatsValues[0] * wildRandomScale; in server code)
				currentStatValues[1] = maxStatsValues[1]; //stamina
				currentStatValues[3] = maxStatsValues[3]; //oxygen
				currentStatValues[4] = maxStatsValues[4]; //food

				/*v5->MaxStatusValues[2] = (float)((float)(*((float *)&v29 + 2) * v5->TheMaxTorporIncreasePerBaseLevel)
				* (float)(v5->BaseCharacterLevel - 1))
				+ v5->OriginalMaxTorpor;*/
			}

			/*v202 = v3->MyInventoryComponent;
			if (v202)
			{
			if (v3->TargetingTeam >= 50000)
			{
			v203 = UPrimalInventoryComponent::GetEquippedItemOfType(v202, DinoSaddle);*/

			return true;
		}
	}

	return false;
}