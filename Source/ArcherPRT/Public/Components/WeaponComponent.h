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


	UFUNCTION(BlueprintCallable)
		int GetAmountAmmo() const;

	UFUNCTION(BlueprintCallable)
		int GetMaxAmmo() const;

	UFUNCTION(BlueprintCallable)
		int GetCurrentAccamulateProjectiles() const { return CountAccamulateProjectile; };

	UFUNCTION(BlueprintPure, Category = "Check")
		bool AimingInProgress() const { return bAimingInProgress; };

	//Aiming

	UFUNCTION()
		void OnAiming();

	UFUNCTION()
		void OffAiming();

	//Fire

	
	void OnFire();

	void OnAltFire();

	void FinishAltFire();

	void MakeShot();

	void SwitchAmmoInCurrentEquipWeapon();

	void TraceAim();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(TSubclassOf<UWeaponBase> Weapon);

	void LoopByAmmo(bool SpendAmmo, int &AmountAmmo, int &MaxAmmo) const;

	bool CanMakeShot() const;

	bool CanFire() const;

	
protected:
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void FinishFire();

	void MakeAccamulateProjectile();

	float SpreadShot;

	bool bAimingInProgress;

	bool bFireInProgress;

	int CountAccamulateProjectile = 1;

	FVector EndPointOnAimTrace;

	FTimerHandle FireInProgressTimer;

	FTimerHandle AccamulateProjectileTimer;

	AAICharacter* CurrentAimingEnemy;


};
