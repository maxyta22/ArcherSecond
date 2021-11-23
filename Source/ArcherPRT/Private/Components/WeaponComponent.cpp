// Archer Prototype. All rights reserved


#include "Components/WeaponComponent.h"
#include "Weapon/WeaponBase.h"
#include "Player/GameCharacter.h"
#include "Projectile/ArcherPRTProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "AI/AICharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"

UWeaponComponent::UWeaponComponent()

{	
	PrimaryComponentTick.bCanEverTick = true;
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
	
	FVector StartTraceAim = Owner->GetFirstPersonCameraComponent()->GetComponentLocation() + Owner->GetFirstPersonCameraComponent()->GetForwardVector() * 10;
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

void UWeaponComponent::OnFire()
{
	if (!CurrentEquipWeapon) return;
	if (!GetWorld()) return;
	if (!GetOwner()) return;

	UWorld* const World = GetWorld();
	if (!World) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;

	if (CurrentEquipWeapon.GetDefaultObject()->ProjectileAmmoMap.Num() == 0) return;

	//Check Have Ammo
	if (!CanMakeShot()) return;

	//Init Local Var
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	const FVector SpawnLocation = Owner->HandMesh->GetSocketLocation(CurrentEquipWeapon.GetDefaultObject()->MuzzleSocketName);
	const FVector ShootDirection = FMath::VRandCone(Owner->GetFirstPersonCameraComponent()->GetForwardVector(), SpreadShot);
	const FRotator SpawnRotation = ShootDirection.Rotation();


	//SpawnProjectile
	TArray<TSubclassOf<AArcherPRTProjectile>> ValueFromMap;
	CurrentEquipWeapon.GetDefaultObject()->ProjectileAmmoMap.GenerateValueArray(ValueFromMap);
	AArcherPRTProjectile* CurrentProjectile = World->SpawnActor<AArcherPRTProjectile>(ValueFromMap[SelectedUseAmmoIndex], SpawnLocation, SpawnRotation, ActorSpawnParams);
	if (CurrentProjectile)
	{
		CurrentProjectile->DamageWeapon = CurrentEquipWeapon.GetDefaultObject()->Damage;
		CurrentProjectile->Instigator = GetOwner()->GetInstigatorController();
		int AmountAmmo;
		//Spend Ammo
		LoopByAmmo(true, AmountAmmo);
		bAimingInProgress = false;
	}

	// try and play the sound if specified
	if (CurrentEquipWeapon.GetDefaultObject()->FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CurrentEquipWeapon.GetDefaultObject()->FireSound, Owner->GetActorLocation());
	}

	// try and play a firing animation if specified
	if (CurrentEquipWeapon.GetDefaultObject()->FireAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = Owner->GetHandMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(CurrentEquipWeapon.GetDefaultObject()->FireAnimation, 1.f);
		}
	}

}

void UWeaponComponent::LoopByAmmo(bool SpendAmmo, int& AmountAmmo) const
{
	if (!GetOwner()) return;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;

	TArray<EAmmoType> KeysFromMap;
	CurrentEquipWeapon.GetDefaultObject()->ProjectileAmmoMap.GetKeys(KeysFromMap);

	switch (KeysFromMap[SelectedUseAmmoIndex])
	{
	case EAmmoType::WoodArrow:
		
		AmountAmmo = Owner->InventoryComponent->GetWoodArrow();
		
		if (SpendAmmo)
		{
			Owner->InventoryComponent->AddWoodArrow(-1);
		}
		break;

	case EAmmoType::RockArrow:

		AmountAmmo = Owner->InventoryComponent->GetRockArrow();
		
		if (SpendAmmo)
		{
			Owner->InventoryComponent->AddRockArrow(-1);
		}
		break;

	case EAmmoType::MetalArrow:

		AmountAmmo = Owner->InventoryComponent->GetMetalArrow();

		if (SpendAmmo)
		{
			Owner->InventoryComponent->AddMetalArrow(-1);
		}
		break;
	}		
}

bool UWeaponComponent::CanMakeShot() const 
{
	int AmountAmmo;

	LoopByAmmo(false, AmountAmmo);

	return AmountAmmo>0;
}

int UWeaponComponent::GetAmountAmmo() const
{
	if (!GetOwner()) return 0;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return 0;

	int AmountAmmo;

	LoopByAmmo(false, AmountAmmo);
	return AmountAmmo;
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

