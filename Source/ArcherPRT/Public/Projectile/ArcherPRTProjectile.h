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

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UCapsuleComponent* CapsuleCollisionForPawn;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UCapsuleComponent* CapsuleCollisionForStatic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:

	AArcherPRTProjectile();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
		void OnImpact(const FHitResult& Result);

	UFUNCTION(BlueprintCallable, Category = "Effects")
		void SpawnImpactEffect(FHitResult Hit);
	 

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int DamageProjectile = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool FlyThroughPawn;

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

	

		


	



	
};

