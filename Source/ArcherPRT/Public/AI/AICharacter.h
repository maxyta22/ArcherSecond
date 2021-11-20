// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/GameCharacter.h"
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

	//Colliders

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Colliders")
		//UCapsuleComponent* HeadCollider;

	//AI

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
		UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
		float TimeToReactionToAiming = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
		float MinDistanceForAttackAfterHitReaction = 300;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation", meta = (Tooltip = "Make Attack after hit reaction"))
		UAnimMontage* CloseAttackAfterHitReaction;

	UFUNCTION(BlueprintImplementableEvent, Category = "Behavior")
		void AfterReactionToAiming();

	UFUNCTION(BlueprintImplementableEvent, Category = "Behavior")
		void AfterCustomAction();

	void ReactionToAiming();

	void StartAccumulateToAiming();

	void FinishAccumulateToAiming();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:

	virtual void OnDeath() override;

	virtual void OnHitReaction() override;


private:

	FTimerHandle AccumulateToAiminHandleTimer;


	



	
};
