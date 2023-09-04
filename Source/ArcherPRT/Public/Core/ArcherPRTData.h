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
	Scrap,
	Crystal,
	BugCorpse,
};

UENUM(BlueprintType, Category = "AIStartCondition")
enum class EAIStartCondition : uint8
{
	Idle,
	Patrol,
	Sleep
};

UENUM(BlueprintType, Category = "AIBehaviorType")
enum class EAIBehaviorType : uint8
{
	Sleep,
	Idle,
	Patrol,
	Range,
	Melee
	
};

UENUM(BlueprintType, Category = "Weapons")
enum class EWeaponType : uint8
{
	None,
	PneumaticGlove,
	PneumaticGun,
};

UENUM(BlueprintType, Category = "Gameplay Ability System")
enum class EAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Fire,
	AltFire,
};

UENUM(BlueprintType, Category = "Collision")
enum class EShapeType : uint8
{
	Box,
	Sphere,
	Capsule,
};