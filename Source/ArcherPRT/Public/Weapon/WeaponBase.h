// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/ArcherPRTData.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class AArcherPRTProjectile;
class APlayerCharacter;

UCLASS(ClassGroup = (Custom), blueprinttype, blueprintable, meta = (BlueprintSpawnableComponent))
class ARCHERPRT_API UWeaponBase: public UObject
{
	GENERATED_BODY()
	
public:	
	
	UWeaponBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		FText WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool bMeleeWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "!bMeleeWeapon"), Category = Gameplay)
		TMap<EResourcesType, TSubclassOf<AArcherPRTProjectile>> ProjectileAmmoMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FName MuzzleSocketName = "Muzzle";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool bUseLenghtFireAnimationForFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bUseLenghtFireAnimationForFireRate"), Category = Gameplay)
		float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int Damage = 10;

	





};
