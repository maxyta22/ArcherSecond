// Archer Prototype. All rights reserved


#include "Components/WeaponComponent.h"
#include "Components/CraftComponent.h"
#include "Weapon/WeaponBase.h"
#include "Player/GameCharacter.h"
#include "Projectile/ArcherPRTProjectile.h"
#include "Kismet/GameplayStatics.h"
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
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	TraceAim();
}

void UWeaponComponent::FinishFire()
{
	UWorld* const World = GetWorld();
	if (!World) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	bFireInProgress = false;
}

void UWeaponComponent::EquipWeapon(TSubclassOf<UWeaponBase> Weapon) 
{
	CurrentEquipWeapon = Weapon;	
}

void UWeaponComponent::TraceAim() 
{
	if (!GetWorld()) return;
	if (!GetOwner()) return;
	if (!CurrentEquipWeapon) return;
	if (!AimingInProgress()) return;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	
	FVector StartTraceAim = Owner->GetFirstPersonCameraComponent()->GetComponentLocation() + Owner->GetFirstPersonCameraComponent()->GetForwardVector() * 50;
	FVector EndTraceAim = StartTraceAim + Owner->GetFirstPersonCameraComponent()->GetForwardVector() * 1000;
	FHitResult TraceResult;

	GetWorld()->LineTraceSingleByChannel(TraceResult, StartTraceAim, EndTraceAim, ECollisionChannel::ECC_Visibility);

	if (bDrawDebug)
	{
		//DrawDebugFromCamera
		DrawDebugLine(GetWorld(), StartTraceAim, EndTraceAim, FColor::Yellow, false, 0, 0, 0);
	}

	
	if (TraceResult.bBlockingHit)
	{
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
		if (CurrentAimingEnemy)
		{
			CurrentAimingEnemy->FinishAccumulateToAiming();
		}
	}

}

void UWeaponComponent::OnAiming()
{
	//Check Have Ammo
	if (!CanMakeShot()) return;
	bAimingInProgress = true;
}

void UWeaponComponent::OffAiming()
{
	bAimingInProgress = false;
}

void UWeaponComponent::OnFire()
{
	if (!GetWorld()) return;
	if (!GetOwner()) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	
	if (!CanFire()) return;

	bFireInProgress = true;

	Owner->PlayAnimMontage(CurrentEquipWeapon.GetDefaultObject()->FireAnimation);

	if (CurrentEquipWeapon.GetDefaultObject()->FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CurrentEquipWeapon.GetDefaultObject()->FireSound, Owner->GetActorLocation());
	}

	GetWorld()->GetTimerManager().SetTimer(FireInProgressTimer, this, &UWeaponComponent::FinishFire, CurrentEquipWeapon.GetDefaultObject()->FireAnimation->SequenceLength, false);

}

void UWeaponComponent::MakeShot()
{
	if (!CanMakeShot()) return;

	UWorld* const World = GetWorld();
	if (!World) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;

	//Init Local Var
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	const FVector SpawnLocation = Owner->GetMesh()->GetSocketLocation(CurrentEquipWeapon.GetDefaultObject()->MuzzleSocketName);
	//const FVector ShootDirection = FMath::VRandCone(Owner->GetFirstPersonCameraComponent()->GetForwardVector(), SpreadShot);
	//const FVector ShootDirection = FMath::VRandCone(Owner->GetControlRotation().Vector(), SpreadShot);
	const FVector ShootDirection = Owner->GetControlRotation().Vector();
	const FRotator SpawnRotation = ShootDirection.Rotation();


	//SpawnProjectile
	TArray<TSubclassOf<AArcherPRTProjectile>> ValueFromMap;
	CurrentEquipWeapon.GetDefaultObject()->ProjectileAmmoMap.GenerateValueArray(ValueFromMap);
	//AArcherPRTProjectile* CurrentProjectile = World->SpawnActor<AArcherPRTProjectile>(ValueFromMap[SelectedUseAmmoIndex], SpawnLocation, SpawnRotation, ActorSpawnParams);
	AArcherPRTProjectile* CurrentProjectile = World->SpawnActorDeferred<AArcherPRTProjectile>(ValueFromMap[SelectedUseAmmoIndex], FTransform(SpawnRotation, SpawnLocation));
	if (CurrentProjectile)
	{
		CurrentProjectile->DamageWeapon = CurrentEquipWeapon.GetDefaultObject()->Damage;
		CurrentProjectile->SetInstigator(Owner);
		
		CurrentProjectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));

		//Spend Ammo
		int AmountAmmo;
		int MaxAmmo;

		LoopByAmmo(true, AmountAmmo, MaxAmmo);
		
	}

	SuccessMakeShot();
}

void UWeaponComponent::SuccessMakeShot()
{
}

void UWeaponComponent::LoopByAmmo(bool SpendAmmo, int& AmountAmmo, int& MaxAmmo) const
{
	if (!GetOwner()) return;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;

	TArray<EResourcesType> KeysFromMap;

	if (CurrentEquipWeapon.GetDefaultObject()->ProjectileAmmoMap.Num() == 0) return;

	CurrentEquipWeapon.GetDefaultObject()->ProjectileAmmoMap.GetKeys(KeysFromMap);

	switch (KeysFromMap[SelectedUseAmmoIndex])
	{
	case EResourcesType::Arrow:
		
		AmountAmmo = Owner->InventoryComponent->GetValueResources(EResourcesType::Arrow);
		MaxAmmo = Owner->InventoryComponent->GetMaxResources(EResourcesType::Arrow);
		
		if (SpendAmmo)
		{
			Owner->InventoryComponent->AddResources(EResourcesType::Arrow, -1);
		}
		break;
	}		
}

bool UWeaponComponent::CanMakeShot() const 
{
	int AmountAmmo;
	int MaxAmmo;

	LoopByAmmo(false, AmountAmmo, MaxAmmo);

	return AmountAmmo>0;
}

bool UWeaponComponent::CanFire() const
{
	if (!GetWorld()) return false;
	if (!GetOwner()) return false;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return false;
	if (bFireInProgress) return false;
	if (!CurrentEquipWeapon) return false;
	if (Owner->BuildingComponent->BuildingModeActivated()) return false;
	if (Owner->CraftComponent->CraftInProgress()) return false;
	return true;
}

int UWeaponComponent::GetAmountAmmo() const
{
	if (!GetOwner()) return 0;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return 0;

	int AmountAmmo;
	int MaxAmmo;

	LoopByAmmo(false, AmountAmmo, MaxAmmo);
	return AmountAmmo;
}

int UWeaponComponent::GetMaxAmmo() const
{
	if (!GetOwner()) return 0;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return 0;

	int AmountAmmo;
	int MaxAmmo;

	LoopByAmmo(false, AmountAmmo, MaxAmmo);
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


