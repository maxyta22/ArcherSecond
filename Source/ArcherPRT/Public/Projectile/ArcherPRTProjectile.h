// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "ArcherPRTProjectile.generated.h"

class USphereComponent;
class UCapsuleComponent;
class UProjectileMovementComponent;
class APlayerCharacter;
class UPhysicalMaterial;
class USoundCue;

UCLASS(config=Game)
class AArcherPRTProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UCapsuleComponent* InteractCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:

	AArcherPRTProjectile();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int DamageProjectile = 0;

	//Effects

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* DefaultCascadeImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		TMap<UPhysicalMaterial*, UParticleSystem*> CascadeImpactEffectsMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UNiagaraSystem* DefaultNiagaraImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		TMap<UPhysicalMaterial*, UNiagaraSystem*> NiagaraImpactEffectsMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		USoundCue* DefaultImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		TMap<UPhysicalMaterial*, USoundCue*> ImpactSoundMap;

	void SpawnHitEffect(FHitResult Hit);

		


	



	
};

