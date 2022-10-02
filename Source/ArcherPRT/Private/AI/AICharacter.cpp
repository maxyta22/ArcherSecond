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
#include "Engine/TargetPoint.h"
#include "TimerManager.h"
#include "Environment/PickupResourcesBase.h"
#include "MathUtils.h"
#include "Kismet/KismetMathLibrary.h"


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
	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	}
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	AIControllerRef = Cast<APRTAIController>(GetController());
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
	if (!GetAIControollerRef()) return;
	if (!GetAIControollerRef()->GetEnemy()) return;
	GetWorld()->GetTimerManager().SetTimer(AccumulateToAiminHandleTimer, this, &AAICharacter::ReactionToAiming, TimeToReactionToAiming);
}

void AAICharacter::FinishAccumulateToAiming()
{
	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().ClearTimer(AccumulateToAiminHandleTimer);
}

void AAICharacter::RotationOnTarget()
{
	if (!GetWorld()) return;
	if (!AIControllerRef) return;
	if (!AIControllerRef->GetBlackboardComponent()) return;
	if (!AIControllerRef->GetBlackboardComponent()->GetValueAsObject("EnemyActor")) return;

	const AActor* Target = Cast<AActor>(AIControllerRef->GetBlackboardComponent()->GetValueAsObject("EnemyActor"));

	if (Target)
	{
		const auto DesiredRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
		const auto XYZRotation = FMath::RInterpTo(GetActorRotation(), DesiredRotation, GetWorld()->GetDeltaSeconds(), SpeedRotationOnTarget);
		const auto ZRotation = FRotator(GetActorRotation().Pitch, XYZRotation.Yaw, GetActorRotation().Roll);
		SetActorRotation(ZRotation);
	}
}

FVector AAICharacter::GetNextPatrolTargetPointLocation()
{
	if (TargetsForPatrol.Num() == 0) return FVector::ZeroVector;

	if (TargetsForPatrol[CurrentPatrolIndex])
	{
		const auto TargetLocation = TargetsForPatrol[CurrentPatrolIndex]->GetActorLocation();

		CurrentPatrolIndex++;

		if (CurrentPatrolIndex >= TargetsForPatrol.Num())
		{
			CurrentPatrolIndex = 0;
		}
		return TargetLocation;
	}

	else
	{
		return FVector::ZeroVector;
	}

	
}

void AAICharacter::ToggleHitColliders(bool Activate)
{
	const auto Components = GetComponents();

	for (UActorComponent* CurrentComponent : Components)
	{	
		const auto Collision = Activate ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;

		if (CurrentComponent->ComponentHasTag("Head"))
		{
			UPrimitiveComponent* CurrentPrimitiveComponent = Cast<UPrimitiveComponent>(CurrentComponent);
			CurrentPrimitiveComponent->SetCollisionEnabled(Collision);
		}

		if (CurrentComponent->ComponentHasTag("Torso"))
		{
			UPrimitiveComponent* CurrentPrimitiveComponent = Cast<UPrimitiveComponent>(CurrentComponent);
			CurrentPrimitiveComponent->SetCollisionEnabled(Collision);
		}	
	}
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
	if (AIController && EventInstigator)
	{
		AIController->SetEnemy(EventInstigator->GetPawn());
	}
		
	return Damage;
}

void AAICharacter::OnDeath()
{
	Super::OnDeath();

}

void AAICharacter::OnHit()
{
	Super::OnHit();

	if (StatsComponent->IsDead()) return;

	AfterOnHit();
	if (CanPerformOnHitReaction)
	{
		PerformOnHitReaction();
	}
	

}