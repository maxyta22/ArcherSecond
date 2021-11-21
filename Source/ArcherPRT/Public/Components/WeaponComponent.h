// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<UWeaponBase> DefaultWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UWeaponBase> CurrentEquipWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		int SelectedUseAmmoIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		bool bDrawDebug;

	UPROPERTY(BlueprintReadWrite, Category = "Internal")
		float SpreadShot;

	UFUNCTION(BlueprintCallable)
		 int GetAmountAmmo();

	

	UFUNCTION(BlueprintPure, Category = "Check")
		bool AimingInProgress() { return bAimingInProgress; };

	void OnAiming();

	void OnFire();

	void SwitchAmmoInCurrentEquipWeapon();

	void TraceAim();

	void EquipWeapon(TSubclassOf<UWeaponBase> Weapon);

	void LoopByAmmo(bool SpendAmmo, bool &HaveAmmo);

	bool CanMakeShot();

	
	
protected:
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	bool bAimingInProgress;

	AAICharacter* CurrentAimingEnemy;


};
