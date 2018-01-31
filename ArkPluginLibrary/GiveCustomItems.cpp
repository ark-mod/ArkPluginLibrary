#include "GiveCustomItems.h"
#include "IApiUtils.h"

bool ArkLibrary::GiveCustomItems(
	unsigned long long steamId,
	std::list<GiveItemDefinition> items)
{
	AShooterPlayerController* aShooterPC = ArkApi::GetApiUtils().FindPlayerFromSteamId(steamId);
	if (aShooterPC)
	{
		//give items
		for (std::list<GiveItemDefinition>::iterator it = items.begin(); it != items.end(); ++it)
		{
			std::list<std::string> blueprints;
			if (it->blueprints.size() > 0) blueprints = it->blueprints;
			else blueprints.push_back(it->blueprint);

			for (std::list<std::string>::iterator it2 = blueprints.begin(); it2 != blueprints.end(); ++it2)
			{
				std::wstring itemName = GetBlueprintNameWideStr(*it2);

				UObject* object = Globals::StaticLoadObject(UObject::StaticClass(), nullptr, itemName.c_str(), nullptr, 0, 0, true);
				if (object && object->IsA(UClass::StaticClass()))
				{
					TSubclassOf<UPrimalItem> archetype;
					archetype.uClass = reinterpret_cast<UClass*>(object);

					AShooterCharacter* aShooterCharacter = static_cast<AShooterCharacter*>(aShooterPC->CharacterField()());

					for (int n = 0; n < it->count; n++)
					{
						UPrimalItem* item = UPrimalItem::AddNewItem(archetype, aShooterCharacter->MyInventoryComponentField()(), false, false, it->quality, false, it->quantity, false, 0.0, false, TSubclassOf<UPrimalItem>());
					}
				}
			}
		}

		return true;
	}

	return false;
}