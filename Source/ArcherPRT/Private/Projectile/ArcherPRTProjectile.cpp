// Copyright Epic Games, Inc. All Rights Reserved.

#include "Projectile/ArcherPRTProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Player/GameCharacter.h"
#include "Components/StatsComponent.h"
#include "Kismet/GameplayStatics.h"

AArcherPRTProjectile::AArcherPRTProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AArcherPRTProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

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
}

void AArcherPRTProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld())  return;

	ProjectileMovement->StopMovementImmediately();

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		const auto Pawn = Cast<AGameCharacter>(OtherActor);
		if (Pawn)
		{
			Pawn->TakeDamage(DamageWeapon+DamageProjectile, FDamageEvent(), Instigator, this);
			Pawn->OnHitReaction();	
		}
		Destroy();
	}
}