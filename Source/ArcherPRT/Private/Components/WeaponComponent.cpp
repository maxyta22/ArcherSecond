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
#include "Interfaces/TakeDamageInterface.h"
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

void UWeaponComponent::EquipWeapon(TSubclassOf<UWeaponBase> Weapon) 
{
	FinishFire();
	CurrentEquipWeapon = Weapon;	
}

void UWeaponComponent::TraceAim() 
{
	if (GetWorld() == nullptr) return;
	if (GetOwner() == nullptr) return;
	if (CurrentEquipWeapon == nullptr) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (Owner == nullptr) return;
	
	FVector StartTraceAim = Owner->GetFirstPersonCameraComponent()->GetComponentLocation() + Owner->GetFirstPersonCameraComponent()->GetForwardVector() * 50;
	FVector EndTraceAim = StartTraceAim + Owner->GetFirstPersonCameraComponent()->GetForwardVector() * LenghtAimTrace;
	FHitResult TraceResult;
	FCollisionObjectQueryParams QueryObjectParams;
	QueryObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	QueryObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);

	GetWorld()->LineTraceSingleByObjectType(TraceResult, StartTraceAim, EndTraceAim, QueryObjectParams);

	EndPointAimTraceHitResult = TraceResult;

	if (TraceResult.bBlockingHit)
	{
		EndPointOnAimTrace = TraceResult.Location;
	}
	else
	{
		EndPointOnAimTrace = EndTraceAim;	
	}

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), StartTraceAim, EndTraceAim, FColor::Yellow, false, 0, 0, 0);
		DrawDebugSphere(GetWorld(), EndPointOnAimTrace, 10, 10, FColor::Green, false, 0, 0, 0);
	}


	/////////////////////Check Accumulate To Aiming///////////////////////////////////////
	/////////////////////Return if cant Aiming////////////////////////////////////////////	
	
	switch (CurrentEquipWeapon.GetDefaultObject()->WeaponType)
	{
	case EWeaponType::PneumaticGlove:
		if (!ChargeAttackInProgress())
		{
			FinishAccumulateAimingForCurrentAimingEnemy();
			return;
		}
		break;
	case EWeaponType::PneumaticGun:
		if (!HaveAmmo())
		{
			FinishAccumulateAimingForCurrentAimingEnemy();
			return;
		}
		break;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Reserve(1);
	ObjectTypesArray.Emplace(ECollisionChannel::ECC_Pawn);
	FHitResult TracePawnResult;

	const EDrawDebugTrace::Type DebugTraceType = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartTraceAim, EndTraceAim,
		10, ObjectTypesArray, true, TArray<AActor*>(),
		DebugTraceType, TracePawnResult, true,
		FLinearColor::Green, FLinearColor::Red, 0.0);


	if (TracePawnResult.bBlockingHit && TracePawnResult.GetComponent() && TracePawnResult.GetComponent()->ComponentHasTag("WeakPoint"))
	{
		const auto AimingCharacter = Cast<AAICharacter>(TracePawnResult.GetActor());

		if (AimingCharacter != CurrentAimingEnemy)
		{
			FinishAccumulateAimingForCurrentAimingEnemy();
		}

		CurrentAimingEnemy = AimingCharacter;

		if (CurrentAimingEnemy != nullptr)
		{
			CurrentAimingEnemy->StartAccumulateToAiming();
		}
	}
	else
	{
		FinishAccumulateAimingForCurrentAimingEnemy();
	}

}

void UWeaponComponent::FinishAccumulateAimingForCurrentAimingEnemy() const
{
	if (IsValid(CurrentAimingEnemy))
	{
		CurrentAimingEnemy->FinishAccumulateToAiming();
	}
}

void UWeaponComponent::OnFire()
{
	bPendingOnFire = true;

	if (GetWorld() == nullptr) return;
	if (GetOwner() == nullptr) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	if (!CanFire()) return;
	if (FireInProgress()) return;
	if (ChargeAttackInProgress()) return;
	if (BlockInProgress()) return;
	
	switch (CurrentEquipWeapon.GetDefaultObject()->WeaponType)
	{
		case EWeaponType::PneumaticGlove:
			bChargeAttackInProgress = true;
			break;
		case EWeaponType::PneumaticGun:
			if (HaveAmmo())
			{
				bChargeAttackInProgress = true;
				//GetWorld()->GetTimerManager().SetTimer(AccamulateProjectileTimer, this, &UWeaponComponent::MakeAccamulateProjectile, TimeAccamulateProjectiles, true);
			}
			break;
	}

	Owner->OnFire();
}

void UWeaponComponent::TryFire()
{
	bPendingOnFire = false;

	if (GetWorld() == nullptr) return;
	if (GetOwner() == nullptr) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	if (!CanFire()) return;
	if (!ChargeAttackInProgress()) return;
	if (FireInProgress()) return;
	if (CurrentEquipWeapon.GetDefaultObject()->WeaponType == EWeaponType::PneumaticGun && !HaveAmmo()) return;

	
	bFireInProgress = true;
	Owner->TryFire();


}

void UWeaponComponent::FinishFire(bool ForceFinishFire)
{
	if (GetWorld() == nullptr) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	bReloadWeaponInProgress = false;
	bFireInProgress = false;
	bChargeAttackInProgress = false;
	bBlockInProgress = false;
	bWeaponCharged = false;
	SpreadShot = 0.0f;

	if (ForceFinishFire)
	{
		bPendingOnFire = false;
		bPendingOnAltFire = false;
	}

	if (bPendingOnFire)
	{
		OnFire();
		return;
	}
					
	if (bPendingOnAltFire)
	{
		OnAltFire();
	}
}

void UWeaponComponent::OnAltFire()
{

	bPendingOnAltFire = true;

	if (!GetWorld()) return;
	if (!GetOwner()) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	if (ChargeAttackInProgress()) return;
	if (FireInProgress()) return;
	
	if (GetWeaponDurability(EWeaponType::PneumaticGlove) == 0)
	{
		Owner->FailTryUseBlock();
		bPendingOnAltFire = false;
		return;
	}

	bBlockInProgress = true;
}

void UWeaponComponent::FinishAltFire()
{
	bPendingOnAltFire = false;
	bBlockInProgress = false;
	

	if (bPendingOnFire)
	{
		OnFire();
	}
}

bool UWeaponComponent::CanFire() const
{
	if (GetWorld() == nullptr) return false;
	if (GetOwner() == nullptr) return false;
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
	if (GetWorld() == nullptr) return false;
	if (GetOwner() == nullptr) return false;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (Owner == nullptr) return false;
	if (bChargeAttackInProgress) return false;
	if (bReloadWeaponInProgress) return false;
	if (!CurrentEquipWeapon) return false;
	if (Owner->BuildingComponent->BuildingModeActivated()) return false;
	if (Owner->CraftComponent->CraftInProgress()) return false;
	if (Owner->InventoryComponent->GetValueResources(EResourcesType::Arrow) <= 0) return false;
	if (AmmoInMagazine == CurrentEquipWeapon.GetDefaultObject()->Magazine) return false;
	return true;
}

void UWeaponComponent::MakeShot()
{
	if (GetWorld() == nullptr) return;
	if (!HaveAmmo()) return;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!IsValid(Owner)) 
		return;

	AmmoInMagazine--;
	bChargeAttackInProgress = false;


	//For Pawn
	FVector StartTrace = Owner->GetFirstPersonCameraComponent()->GetComponentLocation() + Owner->GetFirstPersonCameraComponent()->GetForwardVector() * 50;
	FVector EndTrace = StartTrace + Owner->GetFirstPersonCameraComponent()->GetForwardVector() * LenghtShotTrace;
	FVector TraceDirection = Owner->GetFirstPersonCameraComponent()->GetForwardVector();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());
	TArray<FHitResult> OutHits;
	//For Static
	FHitResult WorldStaticTraceResult;
	FCollisionObjectQueryParams QueryObjectParams;
	QueryObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
  
	const auto LDrawDebug = DrawShotDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	GetWorld()->LineTraceSingleByObjectType(WorldStaticTraceResult, StartTrace, EndTrace, QueryObjectParams);

	UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartTrace,
		EndTrace, ShotCapsuleRadius, ShotCapsuleHalfHeight, ObjectTypes, true, IgnoreActors, LDrawDebug, OutHits, true);

	FDamageData damageData;
	damageData.DamageDirection = TraceDirection;
	damageData.DamageGameplayEffect = CurrentEquipWeapon.GetDefaultObject()->WeaponGameplayEffect;
	damageData.DamageWeaponType = CurrentEquipWeapon.GetDefaultObject()->WeaponType;
	damageData.DamageInstigator = GetOwner()->GetInstigatorController();
	damageData.DamageCauser = GetOwner();
	damageData.DamageCharged = bWeaponCharged;

	for (const FHitResult & HitResult : OutHits)
	{
		damageData.DamagePoint = HitResult;

		FVector LStaticLocation = WorldStaticTraceResult.bBlockingHit ? WorldStaticTraceResult.Location : WorldStaticTraceResult.TraceEnd;

		if (FVector::Dist(StartTrace, HitResult.Location) > FVector::Dist(StartTrace, LStaticLocation))
			continue;

		AGameCharacter* LCharacter = Cast<AGameCharacter>(HitResult.GetActor());

		if (IsValid(LCharacter))
			{
			if (!LCharacter->IsInvulnerable() && HitResult.GetComponent()->ComponentHasTag("WeakPoint") && !IgnoreActors.Contains(LCharacter))
			{
				LCharacter->ImplementTakeDamage(damageData);
				IgnoreActors.Add(LCharacter);
				Owner->AddSuccessDamageCount();
			}
			else
			{
				LCharacter->TakeDamage(0, FDamageEvent(), nullptr, GetOwner());
			}
			Owner->DamageActors.AddUnique(LCharacter);
			}

		const auto LInteractObject = Cast<AInteractObjectBase>(HitResult.GetActor());

		if (LInteractObject)
			{
				LInteractObject->AfterShotHit(HitResult, GetOwner());
			}		

		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UTakeDamageInterface::StaticClass()))
		{
			ITakeDamageInterface::Execute_TakeDamageInteface(HitResult.GetActor(), damageData);
		}

	}

	if (Owner->CheckMissSuccess())
	{
		Owner->MakeMiss();
	}

	Owner->ClearTempInternalActors();

	
	//SpawnProjectile
	const FVector SpawnLocation = Owner->GetMesh()->GetSocketLocation(CurrentEquipWeapon.GetDefaultObject()->MuzzleSocketName);
	FVector AimDirection = UKismetMathLibrary::GetDirectionUnitVector(SpawnLocation, EndPointOnAimTrace);
	const FVector ShootDirection = FMath::VRandCone(AimDirection, SpreadShot);
	const FRotator SpawnRotation = ShootDirection.Rotation();

	const auto SpawnedProjectile =  bWeaponCharged  ? CurrentEquipWeapon.GetDefaultObject()->ChargedProjectile : CurrentEquipWeapon.GetDefaultObject()->Projectile;
	AArcherPRTProjectile* CurrentProjectile = GetWorld()->SpawnActorDeferred<AArcherPRTProjectile>(SpawnedProjectile, FTransform(SpawnRotation, SpawnLocation));

	if (CurrentProjectile)
		{
			//CurrentProjectile->DamageProjectile = CurrentProjectile->DamageProjectile + bWeaponCharged ? CurrentEquipWeapon.GetDefaultObject()->ChargeDamage : CurrentEquipWeapon.GetDefaultObject()->Damage;
			CurrentProjectile->SetInstigator(Owner);
			CurrentProjectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
		
		}
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

void UWeaponComponent::TryReloadWeapon()
{
	if (CanReloadWeapon())
	{
		FinishFire(true);
		PerformReloadWeapon();
	}
	

}

void UWeaponComponent::PerformReloadWeapon()
{
	if (GetWorld() == nullptr) return;
	if (GetOwner() == nullptr) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (Owner == nullptr) return;
	bReloadWeaponInProgress = true;
	Owner->OnReloadWeapon();

}

void UWeaponComponent::FinishReloadWeapon()
{
	bReloadWeaponInProgress = false;

	if (!GetOwner()) return;

	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;

	int NeedAmmo = CurrentEquipWeapon.GetDefaultObject()->Magazine - AmmoInMagazine;

	if (Owner->InventoryComponent->GetValueResources(EResourcesType::Arrow) < CurrentEquipWeapon.GetDefaultObject()->Magazine - AmmoInMagazine)
	{
		NeedAmmo = Owner->InventoryComponent->GetValueResources(EResourcesType::Arrow);
	}
	
	Owner->InventoryComponent->AddResources(EResourcesType::Arrow, -NeedAmmo);

	SetAmmoInMagazine(GetAmmoInMagazine() + NeedAmmo); 

}

float UWeaponComponent::GetWeaponDurability(EWeaponType WeaponType) const
{
	switch (WeaponType)
	{
	case EWeaponType::None:
		return 0.0f;
		break;
	case EWeaponType::PneumaticGlove:
		return GloveDurability;
		break;
	case EWeaponType::PneumaticGun:
		return GunDurability;
		break;
	}
	return 0.0f;
}

void UWeaponComponent::AddWeaponDurability(EWeaponType WeaponType, float Value) 
{
	switch (WeaponType)
	{
	case EWeaponType::None:
		return;
		break;
	case EWeaponType::PneumaticGlove:
		GloveDurability = FMath::Clamp(GloveDurability + Value, 0, 100);
		return;
		break;
	case EWeaponType::PneumaticGun:
		GunDurability = FMath::Clamp(GunDurability + Value, 0, 100);
		return;
		break;
	}
}