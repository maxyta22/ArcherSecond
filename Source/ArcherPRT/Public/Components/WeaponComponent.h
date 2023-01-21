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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Internal")
		bool bWeaponCharged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glove")
		float GloveAttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glove")
		float GloveAttackRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
		float LenghtAimTrace = 1000.0f;

	UFUNCTION(BlueprintCallable)
		int GetAmountAmmoInMagazine() const;

	UFUNCTION(BlueprintCallable)
		int GetAmountAmmo() const;

	UFUNCTION(BlueprintCallable)
		int GetMaxAmmo() const;

	UFUNCTION(BlueprintPure, Category = "Check")
		bool AimingInProgress() const { return bAimingInProgress; };

	UFUNCTION(BlueprintPure, Category = "Check")
		bool BlockInProgress() const { return bBlockInProgress; };

	UFUNCTION(BlueprintPure, Category = "Check")
		bool ChargeAttackInProgress() const { return bChargeAttackInProgress; };

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

	int AmountAmmoInMagazine;

	float SpreadShot;

	bool bAimingInProgress;

	bool bChargeAttackInProgress;

	bool bFireInProgress;

	bool bBlockInProgress;

	bool bPendingOnAltFire;

	bool bPendingOnFire;

	bool bReloadWeaponInProgress;

	FVector EndPointOnAimTrace;

	FHitResult EndPointAimTraceHitResult;

	FTimerHandle FireInProgressTimer;

	FTimerHandle ReloadWeaponInProgressTimer;

	AAICharacter* CurrentAimingEnemy;

};
