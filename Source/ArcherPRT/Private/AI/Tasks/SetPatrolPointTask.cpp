// Archer Prototype. All rights reserved


#include "AI/Tasks/SetPatrolPointTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/AICharacter.h"
#include "NavigationSystem.h"

USetPatrolPointTask::USetPatrolPointTask()
{
	NodeName = "SetPatrol PointTask";
}

EBTNodeResult::Type USetPatrolPointTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto OwnerAIController = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (!OwnerAIController || !Blackboard) return EBTNodeResult::Failed;

	const auto OwnerAICharacter = Cast<AAICharacter>(OwnerAIController->GetPawn());
	if (!OwnerAICharacter) return EBTNodeResult::Failed;

	const auto NavSys = UNavigationSystemV1::GetCurrent(OwnerAICharacter);
	if (!NavSys) return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, OwnerAICharacter->GetNextPatrolTargetPointLocation());
	return EBTNodeResult::Succeeded;

}
