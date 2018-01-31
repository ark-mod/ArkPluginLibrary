#pragma once
#include "Utils.h"
#include "GiveItemsTemplate.h"
#include <list>
#include <iostream>

namespace ArkLibrary
{
	enum AggressionLevel
	{
		Passive = 0,
		Neutral = 1,
		Aggressive = 2,
		AttackMyTarget = 3
	};

	enum FacingDirection
	{
		Forward = 0,
		Outwards = 1,
		Inwards = 2
	};

	bool SpawnCustomDino(
		unsigned long long steamId,
		std::string bpPath, std::string bpPathSaddle,
		int dinoBaseLevelHealth, int dinoBaseLevelStamina, int dinoBaseLevelOxygen, int dinoBaseLevelFood, int dinoBaseLevelWeight, int dinoBaseLevelMeleeDamage, int dinoBaseLevelMovementSpeed,
		int dinoTamedLevelHealth, int dinoTamedLevelStamina, int dinoTamedLevelOxygen, int dinoTamedLevelFood, int dinoTamedLevelWeight, int dinoTamedLevelMeleeDamage, int dinoTamedLevelMovementSpeed,
		float saddleArmor,
		float imprint,
		std::list<GiveItemDefinition> items,
		float offsetX, float offsetY, float offsetZ = 300.0,
		bool follow = false,
		AggressionLevel aggressionLevel = AggressionLevel::Passive,
		bool ignoreAllWhistles = false,
		bool ignoreAllyLook = false,
		float facingModDegrees = 0.0);
}