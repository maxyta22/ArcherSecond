// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ArcherPRTData.generated.h"

class UWeaponBase;

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

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator PlayerRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EResourcesType, int> Resources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EResourcesType, int> MaxResources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UWeaponBase>> AvaliableWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmmoInMagazine;


};

USTRUCT(BlueprintType)
struct FSettingsSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseSensitivity;

};

USTRUCT(BlueprintType)
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FHitResult DamagePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AController* DamageInstigator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* DamageCauser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType DamageWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DamageCharged;

};

UCLASS()
class ARCHERPRT_API UArcherPRTData : public UObject
{
	GENERATED_BODY()

};