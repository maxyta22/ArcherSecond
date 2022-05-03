// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"

class ARCHERPRT_API ArcherPRTData;

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Punch
};

UENUM(BlueprintType, Category = "Ammo")
enum class EAmmoType : uint8
{
	None,
	WoodArrow,
	RockArrow,
	MetalArrow,
};

UENUM(BlueprintType, Category = "Resources")
enum class EResourcesType : uint8
{
	None,
	Wood,
	Rock,
	Grass,
	Metal,
	Food,
};
