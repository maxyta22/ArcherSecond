// Archer Prototype. All rights reserved


#include "AI/Tasks/SetWalkSpeedTask.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

USetWalkSpeedTask::USetWalkSpeedTask()
{
	NodeName = "Set Walk Speed";
}

EBTNodeResult::Type USetWalkSpeedTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
	if (!MovementComponent) return EBTNodeResult::Failed;

	MovementComponent->MaxWalkSpeed = Speed;
	
	return EBTNodeResult::Succeeded;
}