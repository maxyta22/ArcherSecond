// Archer Prototype. All rights reserved


#include "AI/Services/CheckSpeedService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"
#include "Components/PRTAIPerceptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UCheckSpeedService::UCheckSpeedService()
{
	NodeName = "Check Speed";
}

void UCheckSpeedService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	if (Blackboard)
	{
		const auto OwnerAIController = Cast<APRTAIController>(OwnerComp.GetAIOwner());
		if (!OwnerAIController) return;
		const auto OwnerAICharacter = Cast<AAICharacter>(OwnerAIController->GetCharacter());
		if (!OwnerAICharacter) return;
		const auto PerceptionComponent = OwnerAIController->FindComponentByClass<UPRTAIPerceptionComponent>();
		if (!PerceptionComponent) return;
		UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(OwnerAICharacter->GetMovementComponent());
		if (!MovementComponent) return;

		if (OwnerAIController->GetEnemy())
		{
			const float DistToEnemy = OwnerAICharacter->GetDistanceTo(OwnerAIController->GetEnemy());

			MovementComponent->MaxWalkSpeed = DistToEnemy >= OwnerAICharacter->DistanceForChase ? OwnerAICharacter->ChaseSpeed : OwnerAICharacter->WalkSpeed;
			
		}


	}
}
