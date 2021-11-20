// Archer Prototype. All rights reserved

#pragma once

#include "ResourcesTypeEnum.generated.h"


UENUM(BlueprintType, Category="Resources")
enum class EResourcesType : uint8
{
	None,
	Wood,
	Rock,
	Grass,
	Metal,
	Food,
};
