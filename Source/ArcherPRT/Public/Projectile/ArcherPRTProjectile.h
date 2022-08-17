// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "ArcherPRTProjectile.generated.h"

class USphereComponent;
class UCapsuleComponent;
class UProjectileMovementComponent;
class APlayerCharacter;

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

	void TryTakeProjectile(APlayerCharacter* Pawn);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Description")
		void ShowInfo();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Description")
		void HideInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int DamageProjectile = 0;

		int DamageWeapon;

		


	



	
};

