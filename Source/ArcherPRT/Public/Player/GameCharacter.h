// Archer Prototype. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "GameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UInventoryComponent;
class UStatsComponent;
class UWeaponComponent;
class UCustomAction;
class UBoxComponent;

UCLASS(config = Game)
class AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AGameCharacter();

	virtual void BeginPlay() override;

	//Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStatsComponent* StatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UWeaponComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UCustomAction* CustomAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UAbilitySystemComponent* AbilitySystemComponent;


	//Animation

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* HitReaction;


	//Take Damage
	
	UFUNCTION(Server, Reliable)
	void TakeDamage_Server(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "AfterEvents")
		void AfterTakeDamage();

	UFUNCTION(BlueprintImplementableEvent, Category = "AfterEvents")
		void AfterOnDeath();

	//Hit Reaction

	virtual void OnHitReaction();

	//Landed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	//Make Strike

	void MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle);

	void ClearIgnoreActorsDamageStrike();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
		float StrikeDamage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
		TSubclassOf<UDamageType> StrikeDamageType;

	
protected:

	virtual void OnDeath();

	virtual void Landed(const FHitResult& Hit);

private:

	void OnHealChanged(float Health);

	TArray<AActor*> IgnoreActorsDamage;

	
	
	


};