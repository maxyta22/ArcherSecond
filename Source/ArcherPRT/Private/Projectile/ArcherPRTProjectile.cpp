// Copyright Epic Games, Inc. All Rights Reserved.

#include "Projectile/ArcherPRTProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/GameCharacter.h"
#include "Components/StatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"

AArcherPRTProjectile::AArcherPRTProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentHit.AddDynamic(this, &AArcherPRTProjectile::OnHit);
	// Can Return PhysMat
	CollisionComp->bReturnMaterialOnMove = true;

	// Set as root component
	RootComponent = CollisionComp;

	InteractCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(RootComponent);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	
	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	// Default Replicates
	ProjectileMovement->SetIsReplicated(true);
	SetReplicates(true);

}

void AArcherPRTProjectile::TryTakeProjectile(APlayerCharacter* Pawn)
{
	if (Pawn->InventoryComponent->CheckCanTakeAmmo(EAmmoType::WoodArrow))
	{
		Pawn->InventoryComponent->AddAmmo(EAmmoType::WoodArrow, 1);
		Destroy();
	}
	
}

void AArcherPRTProjectile::SpawnHitEffect(FHitResult Hit)
{
	auto ImpactNiagaraEffect = DefaultNiagaraImpactEffect;
	auto ImpactCascadeEffect = DefaultCascadeImpactEffect;
	auto ImpactSound = DefaultImpactSound;

	if (Hit.PhysMaterial.IsValid())
	{
		const auto PhysMat = Hit.PhysMaterial.Get();

		if (NiagaraImpactEffectsMap.Contains(PhysMat))
		{
			ImpactNiagaraEffect = NiagaraImpactEffectsMap[PhysMat];
		}

		if (CascadeImpactEffectsMap.Contains(PhysMat))
		{
			ImpactCascadeEffect = CascadeImpactEffectsMap[PhysMat];
		}

		if (ImpactSoundMap.Contains(PhysMat))
		{
			ImpactSound = ImpactSoundMap[PhysMat];
		}
		
	}

	if (ImpactCascadeEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactCascadeEffect, GetActorLocation(), GetActorRotation());
	}

	if (ImpactNiagaraEffect)
	{
		UNiagaraComponent* SpawnNiagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactNiagaraEffect, GetActorLocation(), GetActorRotation());
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	}
	
}

void AArcherPRTProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	if (!GetWorld())  return;

	ProjectileMovement->StopMovementImmediately();

	SpawnHitEffect(Hit);

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		const auto Pawn = Cast<AGameCharacter>(OtherActor);
		
		if (Pawn)
		{
			if (Hit.GetComponent()->ComponentHasTag("Head"))
			{
				Pawn->TakeDamage(DamageWeapon + DamageProjectile, FDamageEvent(), GetInstigatorController(), this);
				Pawn->OnHitReaction();

				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("HEAD SHOT"));
			}
			else 
			{
				Pawn->TakeDamage(0, FDamageEvent(), GetInstigatorController(), this);
			}	
			Destroy();		
		}
		//Destroy();
	}
}