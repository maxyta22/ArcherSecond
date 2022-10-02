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

	//Animation

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation", meta = (Tooltip = "Make Attack after hit reaction"))
		UAnimMontage* CloseAttackAfterHitReaction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* MainAttack;

	//AI

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
		UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior")
		EAIStartCondition StartCondition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
		float TimeToReactionToAiming = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
		float MinDistanceForAttackAfterHitReaction = 300;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
		float MaxDistanceDetection = 1500.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior")
		TArray<ATargetPoint*> TargetsForPatrol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
		float SpeedRotationOnTarget = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
		float DistanceForChase = 700.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
		float ChaseSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
		float WalkSpeed = 100.0f;

	// Drop Settings

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Drop")
		TArray<TSubclassOf<APickupResourcesBase>> DropList;

	UFUNCTION(BlueprintPure)
		APRTAIController* GetAIControollerRef() { return AIControllerRef;}

	UFUNCTION()
		void ToggleHitColliders(bool Activate);

	// Blueprint Implementable Events

	UFUNCTION(BlueprintImplementableEvent, Category = "Behavior")
		void AfterReactionToAiming();

	UFUNCTION(BlueprintImplementableEvent, Category = "Behavior")
		void AfterHitReaction();

	UFUNCTION(BlueprintImplementableEvent, Category = "Behavior")
		void AfterEnemyFound();


	void ReactionToAiming();

	void StartAccumulateToAiming();

	void FinishAccumulateToAiming();

	void RotationOnTarget();

	FVector GetNextPatrolTargetPointLocation();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:

	virtual void OnDeath() override;

	virtual void OnHitReaction() override;


private:

	FTimerHandle AccumulateToAiminHandleTimer;

	int CurrentPatrolIndex = 0;

	APRTAIController* AIControllerRef;

	
};
