// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"

class ARCHERPRT_API ArcherPRTData;

UENUM(BlueprintType, Category = "Resources")
enum class EResourcesType : uint8
{
	None,
	Arrow,
	Wood,
	Rock,
	Grass,
	Metal,
	Food,
	Battery,
	Lamp,
	Mine,
};

UENUM(BlueprintType, Category = "AIStartCondition")
enum class EAIStartCondition : uint8
{
	Idle,
	Patrol,
	Sleep
};

UENUM(BlueprintType, Category = "TypeSocketForBugs")
enum class ETypeSocketForBugs : uint8
{
	HeadSocket,
	SpineSocket,
	LeftShoulderSocket,
	LeftArmSocket,
	RightShoulderSocket,
	RightArmSocket,
	LeftLegSocket,
	RightLegSocket,
	RightKneeSocket,
	LeftKneeSocket,
};