// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/GameCharacter.h"
#include "Engine/TargetPoint.h"
#include "Core/ArcherPRTData.h"
#include "AI/PRTAIController.h"
#include "Environment/PickupResourcesBase.h"
#include "AICharacter.generated.h"

class UBehaviorTree;
class UBoxComponent;
class UCapsuleComponent;


UCLASS()
class ARCHERPRT_API AAICharacter : public AGameCharacter
{
	GENERATED_BODY()

public:

	AAICharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

#pragma region Animations

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TArray<UAnimMontage*> DeathMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TArray<UAnimMontage*> HitReactionMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TArray<UAnimMontage*> ChargeMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TArray<UAnimMontage*> EvadeMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TArray<UAnimMontage*> WakeUpMontages;

#pragma endregion 

#pragma region Behavior	

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior")
	EAIStartCondition StartCondition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	float TimeToReactionToAiming = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	float ProbabilityEvade = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	float MinDistanceForAttackAfterHitReaction = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
	float MaxDistanceDetection = 1500.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior")
	TArray<ATargetPoint*> TargetsForPatrol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	float SpeedRotation = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	float DistanceForChase = 700.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	float ChaseSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	float WalkSpeed = 100.0f;

	UFUNCTION(BlueprintPure)
	APRTAIController* GetAIControllerRef() { return AIControllerRef; }

	bool CanPerformOnHitReaction = true;

	void RotationOnTarget();

	FVector GetNextPatrolTargetPointLocation();

private:

	int CurrentPatrolIndex = 0;

	APRTAIController* AIControllerRef;


#pragma endregion 

#pragma region Drop Settings

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior|Drop")
	TSubclassOf<APickupResourcesBase> DropCorpse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior|Drop")
	TArray<TSubclassOf<APickupResourcesBase>> DropList;

#pragma endregion 

#pragma region Events For BP

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Behavior")
	void MakeShot();

	UFUNCTION(BlueprintImplementableEvent, Category = "Behavior")
	void AfterReactionToAiming();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Behavior")
	void PerformOnStaggerReaction();

	UFUNCTION(BlueprintImplementableEvent, Category = "Behavior")
	void AfterOnHit(FVector HitDirection, FHitResult HitResult, AActor* Causer, EWeaponType WeaponType, bool Charged);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Behavior")
	void PerformOnHitReaction(FVector HitDirection, FHitResult HitResult, AActor* Causer, EWeaponType WeaponType, bool Charged, bool ForceHeavyHitReactions = false);

	UFUNCTION(BlueprintImplementableEvent, Category = "Behavior")
	void AfterEnemyFound();

#pragma endregion

#pragma region ReactionToAiming

public:

	void ReactionToAiming();

	void StartAccumulateToAiming();

	void FinishAccumulateToAiming();

private:

	FTimerHandle AccumulateToAiminHandleTimer;

#pragma endregion

#pragma region TakeDamage

public:

	UFUNCTION()
	void ToggleActivateHitColliders(bool Activate);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnHit(FVector HitDirection, FHitResult HitResult, AActor* Causer, EWeaponType WeaponType, bool Charged) override;

#pragma endregion

#pragma region MakeDamage

protected:

	virtual void MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle, bool IgnoreBlock, bool MakeStagger) override;

#pragma endregion

#pragma region Death

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	float TimeToDestroyCorpse = 60;


protected:

	virtual void OnDeath() override;

	void DestroyCorpse();

private:

	FTimerHandle DestroyCorpseTimer;



#pragma endregion
};
