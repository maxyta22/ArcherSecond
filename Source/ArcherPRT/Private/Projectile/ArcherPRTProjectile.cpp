// Copyright Epic Games, Inc. All Rights Reserved.

#include "Projectile/ArcherPRTProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/GameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Environment/InteractObjectBase.h"
#include "Sound/SoundCue.h"

AArcherPRTProjectile::AArcherPRTProjectile() 
{
	CapsuleCollisionForStatic = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollisionForStatic"));
	CapsuleCollisionForStatic->SetupAttachment(RootComponent);
	CapsuleCollisionForStatic->SetCapsuleRadius(20.0f);
	CapsuleCollisionForStatic->SetCapsuleHalfHeight(20.f);
	CapsuleCollisionForStatic->BodyInstance.SetCollisionProfileName("Projectile");
	CapsuleCollisionForStatic->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleCollisionForStatic->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CapsuleCollisionForStatic->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CapsuleCollisionForStatic->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
	CapsuleCollisionForStatic->SetCollisionResponseToChannel(ECC_Check, ECR_Overlap);

	// Set as root component
	RootComponent = CapsuleCollisionForStatic;

	//Set capsule collision
	CapsuleCollisionForPawn = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollisionForPawn"));
	CapsuleCollisionForPawn->SetupAttachment(RootComponent);
	CapsuleCollisionForPawn->SetCapsuleRadius(20.0f);
	CapsuleCollisionForPawn->SetCapsuleHalfHeight(20.f);
	CapsuleCollisionForPawn->BodyInstance.SetCollisionProfileName("Projectile");
	CapsuleCollisionForPawn->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleCollisionForPawn->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	CapsuleCollisionForPawn->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CapsuleCollisionForPawn->SetCollisionResponseToChannel(ECC_Destructible, ECR_Block);
	CapsuleCollisionForPawn->SetCollisionResponseToChannel(ECC_Check, ECR_Overlap);

	// set up a notification for when this component hits something blocking
	CapsuleCollisionForPawn->OnComponentHit.AddDynamic(this, &AArcherPRTProjectile::OnHit);
	CapsuleCollisionForStatic->OnComponentHit.AddDynamic(this, &AArcherPRTProjectile::OnHit);

	// set up a notification for when this component overlap
	CapsuleCollisionForPawn->OnComponentBeginOverlap.AddDynamic(this, &AArcherPRTProjectile::OnOverlap);
	CapsuleCollisionForStatic->OnComponentBeginOverlap.AddDynamic(this, &AArcherPRTProjectile::OnOverlap);

	// Can Return PhysMat
	CapsuleCollisionForPawn->bReturnMaterialOnMove = true;
	CapsuleCollisionForStatic->bReturnMaterialOnMove = true;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CapsuleCollisionForStatic;
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

void AArcherPRTProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	OnImpact(Hit);
}

void AArcherPRTProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnImpact(SweepResult);	
}

void AArcherPRTProjectile::OnImpact_Implementation(const FHitResult& Result)
{

	return;

	if (GetWorld() == nullptr) return;

	//ProjectileMovement->StopMovementImmediately();

	const auto OtherActor = Result.GetActor();
	const auto OtherComp = Result.GetComponent();

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		bool ShouldDestroyed = true;

		const auto Pawn = Cast<AGameCharacter>(OtherActor);
		const auto InteractObject = Cast<AInteractObjectBase>(OtherActor);

		ShouldDestroyed = !FlyThroughPawn;

		if (Pawn != nullptr && Pawn->IsInvulnerable() == false)
		{

			if (Result.GetComponent()->ComponentHasTag("WeakPoint"))
			{
				Pawn->TakeDamage(DamageProjectile, FDamageEvent(), GetInstigatorController(), this);
			}
			else
			{
				Pawn->TakeDamage(0, FDamageEvent(), GetInstigatorController(), this);
			}
		}


		SpawnImpactEffect(Result);

		if (ShouldDestroyed)
		{
			Destroy();
		}
		
	}
}

void AArcherPRTProjectile::SpawnImpactEffect(FHitResult Hit)
{
	if (GetWorld() == nullptr) return;
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

