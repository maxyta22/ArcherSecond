// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "WeaponComponent.generated.h"


class AWeaponBase;
class UWeaponBase;
class APlayerCharacter;
class AAICharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCHERPRT_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UWeaponComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
		bool bDrawDebug;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<UWeaponBase> DefaultWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UWeaponBase> CurrentEquipWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TArray<TSubclassOf<UWeaponBase>> AvailableWeapons;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		int SelectedUseAmmoIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShotGun")
		int MaxAccamulateProjectiles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShotGun")
		float TimeAccamulateProjectiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotGun")
		float SpreadShotGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glove")
		bool bGloveAttackCharged;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
		float LenghtAimTrace = 1000.0f;

	UFUNCTION(BlueprintCallable)
		int GetAmountAmmoInMagazine() const;

	UFUNCTION(BlueprintCallable)
		int GetAmountAmmo() const;

	UFUNCTION(BlueprintCallable)
		int GetMaxAmmo() const;

	UFUNCTION(BlueprintCallable)
		int GetCurrentAccamulateProjectiles() const { return CountAccamulateProjectile; };

	UFUNCTION(BlueprintPure, Category = "Check")
		bool AimingInProgress() const { return bAimingInProgress; };

	UFUNCTION(BlueprintPure, Category = "Check")
		bool BlockInProgress() const { return bBlockInProgress; };

	UFUNCTION(BlueprintPure, Category = "Check")
		bool FireInProgress() const { return bFireInProgress; };

	UFUNCTION(BlueprintPure, Category = "Check")
		bool ReloadInProgress() const { return bReloadWeaponInProgress; };

	UFUNCTION(BlueprintPure, Category = "Aim")
		FVector GetEndPointOnAimTrace() const { return EndPointOnAimTrace; }

	UFUNCTION(BlueprintPure, Category = "Aim")
		FHitResult GetEndPointOnAimTraceHitResult() const { return EndPointAimTraceHitResult; }

	//Fire

	void OnFire();

	void OnAltFire();

	void FinishAltFire();

	void MakeShot();

	void TryFire();

	UFUNCTION(BlueprintCallable)
	void FinishFire();

	UFUNCTION(BlueprintPure)
	bool HaveAmmo() { return AmountAmmoInMagazine > 0; };

	bool CanFire() const;

	//Reload Weapon

	void TryReloadWeapon();

	void PerformReloadWeapon();

	void FinishReloadWeapon();

	bool CanReloadWeapon() const;

	void TraceAim();

	void SwitchAmmoInCurrentEquipWeapon();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(TSubclassOf<UWeaponBase> Weapon);

protected:
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:


	void MakeAccamulateProjectile();

	int AmountAmmoInMagazine;

	float SpreadShot;

	bool bAimingInProgress;

	bool bFireInProgress;

	bool bBlockInProgress;

	bool bReloadWeaponInProgress;

	int CountAccamulateProjectile = 1;

	FVector EndPointOnAimTrace;

	FHitResult EndPointAimTraceHitResult;

	FTimerHandle FireInProgressTimer;

	FTimerHandle ReloadWeaponInProgressTimer;

	FTimerHandle AccamulateProjectileTimer;

	AAICharacter* CurrentAimingEnemy;


};
