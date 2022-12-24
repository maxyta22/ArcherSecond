// Archer Prototype. All rights reserved


#include "Components/WeaponComponent.h"
#include "Components/CraftComponent.h"
#include "Weapon/WeaponBase.h"
#include "Player/GameCharacter.h"
#include "Projectile/ArcherPRTProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"
#include "AI/AICharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/BuildingComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/PlayerCharacter.h"

UWeaponComponent::UWeaponComponent()

{	
	PrimaryComponentTick.bCanEverTick = true;
	//SetIsReplicated(true);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	EquipWeapon(DefaultWeapon);
	if (CurrentEquipWeapon != NULL)
	{
		AmountAmmoInMagazine = CurrentEquipWeapon.GetDefaultObject()->Magazine;
	}
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	TraceAim();
}

void UWeaponComponent::EquipWeapon(TSubclassOf<UWeaponBase> Weapon) 
{
	FinishFire();
	CurrentEquipWeapon = Weapon;	
}

void UWeaponComponent::TraceAim() 
{
	if (!GetWorld()) return;
	if (!GetOwner()) return;
	if (!CurrentEquipWeapon) return;
	//if (!AimingInProgress()) return;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	
	FVector StartTraceAim = Owner->GetFirstPersonCameraComponent()->GetComponentLocation() + Owner->GetFirstPersonCameraComponent()->GetForwardVector() * 50;
	FVector EndTraceAim = StartTraceAim + Owner->GetFirstPersonCameraComponent()->GetForwardVector() * LenghtAimTrace;
	FHitResult TraceResult;

	GetWorld()->LineTraceSingleByChannel(TraceResult, StartTraceAim, EndTraceAim, ECollisionChannel::ECC_Visibility);

	if (bDrawDebug)
	{
		//DrawDebugFromCamera
		DrawDebugLine(GetWorld(), StartTraceAim, EndTraceAim, FColor::Yellow, false, 0, 0, 0);
	}

	EndPointAimTraceHitResult = TraceResult;
	
	if (TraceResult.bBlockingHit)
	{
		EndPointOnAimTrace = TraceResult.Location;

		const auto Result = Cast<AAICharacter>(TraceResult.GetActor());
		if (Result)
		{
			Result->StartAccumulateToAiming();
			CurrentAimingEnemy = Result;
		}
		else
		{
			if (CurrentAimingEnemy)
			{
				CurrentAimingEnemy->FinishAccumulateToAiming();
			}
		}
	}
	else
	{
		EndPointOnAimTrace = EndTraceAim;

		if (CurrentAimingEnemy)
		{
			CurrentAimingEnemy->FinishAccumulateToAiming();
		}
	}

}

void UWeaponComponent::OnFire()
{
	if (!GetWorld()) return;
	if (!GetOwner()) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	if (!CanFire()) return;
	if (ChargeAttackInProgress()) return;
	
	switch (CurrentEquipWeapon.GetDefaultObject()->WeaponType)
	{
		case EWeaponType::PneumaticGlove:
			break;
		case EWeaponType::PneumaticGun:
			if (HaveAmmo())
			{
				GetWorld()->GetTimerManager().SetTimer(AccamulateProjectileTimer, this, &UWeaponComponent::MakeAccamulateProjectile, TimeAccamulateProjectiles, true);
			}
			break;
	}
	
	bChargeAttackInProgress = true;

	Owner->OnFire();
}

void UWeaponComponent::TryFire()
{

	if (!GetWorld()) return;
	if (!GetOwner()) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	if (!CanFire()) return;
	if (!ChargeAttackInProgress()) return;

	if (CurrentEquipWeapon.GetDefaultObject()->WeaponType == EWeaponType::PneumaticGun && !HaveAmmo()) return;

	Owner->TryFire();


}

void UWeaponComponent::FinishFire()
{
	UWorld* const World = GetWorld();
	if (!World) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	GetWorld()->GetTimerManager().ClearTimer(AccamulateProjectileTimer);
	GetWorld()->GetTimerManager().ClearTimer(ReloadWeaponInProgressTimer);
	bChargeAttackInProgress = false;
	bReloadWeaponInProgress = false;
	bBlockInProgress = false;
	bWeaponCharged = false;
	CountAccamulateProjectile = 1;
	SpreadShot = 0.0f;
}

void UWeaponComponent::OnAltFire()
{
	if (!GetWorld()) return;
	if (!GetOwner()) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;

	bBlockInProgress = true;
}

void UWeaponComponent::FinishAltFire()
{
	bBlockInProgress = false;
}

bool UWeaponComponent::CanFire() const
{
	if (!GetWorld()) return false;
	if (!GetOwner()) return false;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return false;
	if (bReloadWeaponInProgress) return false;
	if (!CurrentEquipWeapon) return false;
	if (Owner->BuildingComponent->BuildingModeActivated()) return false;
	if (Owner->CraftComponent->CraftInProgress()) return false;
	return true;
}

bool UWeaponComponent::CanReloadWeapon() const
{
	if (!GetWorld()) return false;
	if (!GetOwner()) return false;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return false;
	if (bChargeAttackInProgress) return false;
	if (bReloadWeaponInProgress) return false;
	if (!CurrentEquipWeapon) return false;
	if (Owner->BuildingComponent->BuildingModeActivated()) return false;
	if (Owner->CraftComponent->CraftInProgress()) return false;
	if (Owner->InventoryComponent->GetValueResources(EResourcesType::Arrow) <= 0) return false;
	if (AmountAmmoInMagazine == CurrentEquipWeapon.GetDefaultObject()->Magazine) return false;
	return true;
}

void UWeaponComponent::MakeShot()
{
	if (!HaveAmmo()) return;

	UWorld* const World = GetWorld();
	if (!World) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;


	for (size_t i = 0; i < CountAccamulateProjectile; i++)
	{
		const FVector SpawnLocation = Owner->GetMesh()->GetSocketLocation(CurrentEquipWeapon.GetDefaultObject()->MuzzleSocketName);
		const FVector AimDirection = UKismetMathLibrary::GetDirectionUnitVector(SpawnLocation, EndPointOnAimTrace);
		const FVector ShootDirection = FMath::VRandCone(AimDirection, SpreadShot);
		const FRotator SpawnRotation = ShootDirection.Rotation();

		//SpawnProjectile
		TArray<TSubclassOf<AArcherPRTProjectile>> ValueFromMap;
		CurrentEquipWeapon.GetDefaultObject()->ProjectileAmmoMap.GenerateValueArray(ValueFromMap);
		AArcherPRTProjectile* CurrentProjectile = World->SpawnActorDeferred<AArcherPRTProjectile>(ValueFromMap[SelectedUseAmmoIndex], FTransform(SpawnRotation, SpawnLocation));

		if (CurrentProjectile)
		{
			CurrentProjectile->DamageWeapon = CurrentEquipWeapon.GetDefaultObject()->Damage;
			CurrentProjectile->SetInstigator(Owner);
			CurrentProjectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));

			AmountAmmoInMagazine--;
		}

	}
	
}

int UWeaponComponent::GetAmountAmmoInMagazine() const
{
	if (!GetOwner()) return 0;

	return AmountAmmoInMagazine;

}

int UWeaponComponent::GetAmountAmmo() const
{
	if (!GetOwner()) return 0;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return 0;

	const auto AmountAmmo = Owner->InventoryComponent->GetValueResources(EResourcesType::Arrow);
	const auto MaxAmmo = Owner->InventoryComponent->GetMaxResources(EResourcesType::Arrow);

	return AmountAmmo;
}

int UWeaponComponent::GetMaxAmmo() const
{
	if (!GetOwner()) return 0;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return 0;

	const auto AmountAmmo = Owner->InventoryComponent->GetValueResources(EResourcesType::Arrow);
	const auto MaxAmmo = Owner->InventoryComponent->GetMaxResources(EResourcesType::Arrow);
	return MaxAmmo;
}

void UWeaponComponent::SwitchAmmoInCurrentEquipWeapon()
{
	if (!CurrentEquipWeapon) return;
	if (SelectedUseAmmoIndex == CurrentEquipWeapon.GetDefaultObject()->ProjectileAmmoMap.Num()-1)
	{
		SelectedUseAmmoIndex = 0;
	}
	else
		SelectedUseAmmoIndex = FMath::Clamp(SelectedUseAmmoIndex + 1, 0, CurrentEquipWeapon.GetDefaultObject()->ProjectileAmmoMap.Num()-1);

}

void UWeaponComponent::MakeAccamulateProjectile()
{
	SpreadShot = SpreadShotGun;

	if (CountAccamulateProjectile < AmountAmmoInMagazine)
	{
		CountAccamulateProjectile = FMath::Clamp(CountAccamulateProjectile + 1, 1, MaxAccamulateProjectiles);
	}

}

void UWeaponComponent::TryReloadWeapon()
{
	if (CanReloadWeapon())
	{
		PerformReloadWeapon();
	}
	

}

void UWeaponComponent::PerformReloadWeapon()
{
	if (!GetWorld()) return;
	if (!GetOwner()) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;


	if (CurrentEquipWeapon.GetDefaultObject()->ReloadingAnimation)
	{
		Owner->OnReloadWeapon();
		bReloadWeaponInProgress = true;
		Owner->PlayAnimMontage(CurrentEquipWeapon.GetDefaultObject()->ReloadingAnimation);
		const auto TimeReload = CurrentEquipWeapon.GetDefaultObject()->ReloadingAnimation->SequenceLength;
		GetWorld()->GetTimerManager().SetTimer(ReloadWeaponInProgressTimer, this, &UWeaponComponent::FinishReloadWeapon, TimeReload, false);
	}

}

void UWeaponComponent::FinishReloadWeapon()
{

	if (!GetOwner()) return;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;

	bReloadWeaponInProgress = false;

	int NeedAmmo = CurrentEquipWeapon.GetDefaultObject()->Magazine - AmountAmmoInMagazine;

	if (Owner->InventoryComponent->GetValueResources(EResourcesType::Arrow) < CurrentEquipWeapon.GetDefaultObject()->Magazine - AmountAmmoInMagazine)
	{
		NeedAmmo = Owner->InventoryComponent->GetValueResources(EResourcesType::Arrow);
	}
	

	Owner->InventoryComponent->AddResources(EResourcesType::Arrow, -NeedAmmo);

	AmountAmmoInMagazine = AmountAmmoInMagazine+NeedAmmo;

	
}

