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

	/** called when projectile hits something */
	UFUNCTION(NetMulticast, Reliable)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Description")
		void ShowInfo();
		void ShowInfo_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Description")
		void HideInfo();
		void HideInfo_Implementation();

	UFUNCTION(Server, Reliable)
		void ServerTryTakeProjectile(APlayerCharacter* Pawn);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int DamageProjectile = 0;

		int DamageWeapon;

		


	



	
};

