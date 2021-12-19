// Copyright Epic Games, Inc. All Rights Reserved.

#include "Projectile/ArcherPRTProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Player/GameCharacter.h"
#include "Components/StatsComponent.h"
#include "Net/UnrealNetwork.h"
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

	// Default Replicates
	ProjectileMovement->SetIsReplicated(true);
	SetReplicates(true);

	
}

void AArcherPRTProjectile::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	if (!GetWorld())  return;

	ProjectileMovement->StopMovementImmediately();

	FHitResult HitResult = Hit;
	

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		const auto Pawn = Cast<AGameCharacter>(OtherActor);
		
		if (Pawn)
		{
			if (HitResult.GetComponent()->ComponentHasTag("Head"))
			{
				Pawn->TakeDamage_Server(DamageWeapon + DamageProjectile, FDamageEvent(), GetInstigatorController(), this);
				Pawn->OnHitReaction();

				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("HEAD SHOT"));
			}
			else 
			{
				Pawn->TakeDamage_Server(0, FDamageEvent(), GetInstigatorController(), this);
			}
			
				
		}
		Destroy();
	}
}