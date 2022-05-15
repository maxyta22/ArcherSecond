// Archer Prototype. All rights reserved


#include "AI/Tasks/RandomLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

URandomLocationTask::URandomLocationTask()
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type URandomLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto OwnerAIController = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (!OwnerAIController || !Blackboard) return EBTNodeResult::Failed;

	const auto OwnerAICharacter = OwnerAIController->GetPawn();
	if(!OwnerAICharacter) return EBTNodeResult::Failed;

	const auto NavSys = UNavigationSystemV1::GetCurrent(OwnerAICharacter);
	if(!NavSys) return EBTNodeResult::Failed;

	FNavLocation NavLocation;
	auto Location = OwnerAICharacter->GetActorLocation();
	if (!SelfCenter)
	{
		auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
		if (!CenterActor) return EBTNodeResult::Failed;
		Location = CenterActor->GetActorLocation();
	}

	const auto Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
	if(!Found) return EBTNodeResult::Failed;
	Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, NavLocation.Location);
	return EBTNodeResult::Succeeded;


}
