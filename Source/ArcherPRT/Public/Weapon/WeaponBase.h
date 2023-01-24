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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		FText WeaponName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		bool bMeleeWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "!bMeleeWeapon"), Category = Gameplay)
		TSubclassOf<AArcherPRTProjectile> Projectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "!bMeleeWeapon"), Category = Gameplay)
		TSubclassOf<AArcherPRTProjectile> ChargedProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* ChargedFireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* ReloadingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* EmptyAmmoAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* ChargeAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* FullChargedAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FName MuzzleSocketName = "Muzzle";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool bUseLenghtFireAnimationForFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bUseLenghtFireAnimationForFireRate"), Category = Gameplay)
		float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int Magazine = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float ChargeDamage = 30;

	





};
