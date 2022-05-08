// Archer Prototype. All rights reserved

#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StatsComponent.h"
#include "Components/CustomAction.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TimerManager.h"


AAICharacter::AAICharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = APRTAIController::StaticClass();

	//Setup Capsule Collision
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	
	
	// Setup Smooth Rotation
	bUseControllerRotationYaw = true;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Accumulate To Aiming 

void AAICharacter::StartAccumulateToAiming()
{
	if (!GetWorld()) return;
	if (GetWorld()->GetTimerManager().IsTimerActive(AccumulateToAiminHandleTimer)) return;
	GetWorld()->GetTimerManager().SetTimer(AccumulateToAiminHandleTimer, this, &AAICharacter::ReactionToAiming, TimeToReactionToAiming);
}

void AAICharacter::FinishAccumulateToAiming()
{
	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().ClearTimer(AccumulateToAiminHandleTimer);
}

void AAICharacter::ReactionToAiming()
{
	AfterReactionToAiming();
}

// Take Damage

float AAICharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	const auto AIController = Cast<APRTAIController>(Controller);
	if (AIController)
	{
		AIController->SetEnemy(EventInstigator->GetPawn());
	}
		
	return Damage;
}

void AAICharacter::OnDeath()
{
	Super::OnDeath();

	const auto AIController = Cast<AAIController>(Controller);
	if (AIController && AIController->BrainComponent)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool("LockBehavior", true);
		AIController->BrainComponent->Cleanup();
	}

	if (DeathAnimMontage)
	{
		TArray<UAnimMontage*> EmptyArr;
		CustomAction->TryPerformPlayAnimMontage_Server(DeathAnimMontage, true);
	}

	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}

void AAICharacter::OnHitReaction()
{
	Super::OnHitReaction();

	if (StatsComponent->IsDead()) return;

	const auto AIController = Cast<APRTAIController>(Controller);
	if (!AIController) return;

	AfterHitReaction();

	/*
	
	// If No Enemy
	if (!AIController->GetEnemy())
	{
		if (!HitReaction) return;
		CustomAction->TryPerformPlayAnimMontage_Server(HitReaction, true);
		return;
	}

	// If Have Close Attack After Hit Reaction
	if ((CloseAttackAfterHitReaction) && (GetDistanceTo(AIController->GetEnemy()) <= MinDistanceForAttackAfterHitReaction))
	{
		CustomAction->TryPerformPlayAnimMontage_Server(CloseAttackAfterHitReaction, true);
		return;
	}

	if (!HitReaction) return;

	CustomAction->TryPerformPlayAnimMontage_Server(HitReaction, true);
	
	
	*/
	
}