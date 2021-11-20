// Archer Prototype. All rights reserved

#pragma once

#include "AmmoTypeEnum.generated.h"


UENUM(BlueprintType, Category = "Ammo")
enum class EAmmoType : uint8
{
	None,
	WoodArrow,
	RockArrow,
	MetalArrow,
};